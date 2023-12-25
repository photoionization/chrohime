// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime/main_menu_builder.h"

#include <utility>

#include "base/check_op.h"
#include "base/mac/mac_util.h"
#include "base/strings/sys_string_conversions.h"
#include "ui/base/accelerators/platform_accelerator_cocoa.h"
#include "ui/base/l10n/l10n_util_mac.h"
#include "ui/strings/grit/ui_strings.h"

namespace hime {

namespace {

// Helper class that builds NSMenuItems from data. Instances of this class
// should not outlive an autorelease pool scope as it does not retain any
// Objective-C members.
//
// This builder follows a fluent-interface pattern where the setters are
// not prefixed with the typical "set_" and they return a reference to this
// for easier method chaining.
class MenuItemBuilder {
 public:
  explicit MenuItemBuilder(int string_id = 0) : string_id_(string_id) {}
  explicit MenuItemBuilder(std::u16string title) : title_(std::move(title)) {}
  ~MenuItemBuilder() = default;

  // Converts the item to a separator. Only tag() is also applicable.
  MenuItemBuilder& is_separator() {
    DCHECK_EQ(string_id_, 0);
    is_separator_ = true;
    return *this;
  }

  MenuItemBuilder& target(id target) {
    target_ = target;
    return *this;
  }

  MenuItemBuilder& action(SEL action) {
    DCHECK(!action_);
    action_ = action;
    return *this;
  }

  MenuItemBuilder& tag(int tag) {
    tag_ = tag;
    return *this;
  }

  // Wires up the menu item to the CommandDispatcher based on an
  // IDC_ command code.
  MenuItemBuilder& command_id(int command_id) {
    return tag(command_id).action(@selector(commandDispatch:));
  }

  // Specifies the string to substitute for the $1 found in the string for
  // |string_id_|.
  MenuItemBuilder& string_format_1(const std::u16string& arg) {
    string_arg1_ = arg;
    return *this;
  }

  MenuItemBuilder& submenu(std::vector<MenuItemBuilder> items) {
    submenu_ = std::move(items);
    return *this;
  }

  // Registers a custom key equivalent. Normally the key equivalent is looked
  // up via AcceleratorsCocoa based on the command_id(). If one is not present,
  // the one specified here is used instead.
  MenuItemBuilder& key_equivalent(NSString* key_equivalent,
                                  NSEventModifierFlags flags) {
    DCHECK((flags & NSEventModifierFlagShift) == 0)
        << "The shift modifier flag should be directly applied to the key "
           "equivalent.";
    key_equivalent_ = key_equivalent;
    key_equivalent_flags_ = flags;
    return *this;
  }

  // Marks the item as an alternate keyboard equivalent menu item.
  MenuItemBuilder& is_alternate() {
    is_alternate_ = true;
    return *this;
  }

  // Excludes this item from the menu if |condition| is true.
  MenuItemBuilder& remove_if(bool condition) {
    is_removed_ |= condition;
    return *this;
  }

  // Hide this item from the menu if |condition| is true.
  MenuItemBuilder& set_hidden(bool condition) {
    is_hidden_ |= condition;
    return *this;
  }

  // Builds a NSMenuItem instance from the properties set on the Builder.
  NSMenuItem* Build() const;

 private:
  bool is_separator_ = false;

  int string_id_ = 0;
  std::u16string title_;
  std::u16string string_arg1_;

  int tag_ = 0;

  id __strong target_ = nil;
  SEL action_ = nil;

  NSString* __strong key_equivalent_ = @"";
  NSEventModifierFlags key_equivalent_flags_ = 0;

  bool is_alternate_ = false;

  bool is_removed_ = false;

  std::optional<std::vector<MenuItemBuilder>> submenu_;

  bool is_hidden_ = false;
};

NSMenuItem* MenuItemBuilder::Build() const {
  if (is_removed_)
    return nil;

  if (is_separator_) {
    NSMenuItem* item = [NSMenuItem separatorItem];
    if (tag_) {
      [item setTag:tag_];
    }
    [item setHidden:is_hidden_];
    return item;
  }

  NSString* title;
  if (string_id_ == 0) {
    title = base::SysUTF16ToNSString(title_);
  } else {
    if (!string_arg1_.empty())
      title = l10n_util::GetNSStringFWithFixup(string_id_, string_arg1_);
    else
      title = l10n_util::GetNSStringWithFixup(string_id_);
  }

  SEL action = !submenu_.has_value() ? action_ : nil;

  NSMenuItem* item = [[NSMenuItem alloc] initWithTitle:title
                                                action:action
                                         keyEquivalent:key_equivalent_];
  item.target = target_;
  item.tag = tag_;
  item.keyEquivalentModifierMask = key_equivalent_flags_;
  item.alternate = is_alternate_;
  item.hidden = is_hidden_;

  if (submenu_.has_value()) {
    NSMenu* menu = [[NSMenu alloc] initWithTitle:title];
    for (const auto& subitem : submenu_.value()) {
      NSMenuItem* ns_subitem = subitem.Build();
      if (ns_subitem)
        [menu addItem:ns_subitem];
    }
    [item setSubmenu:menu];
  }

  return item;
}

using Item = MenuItemBuilder;

NSMenuItem* BuildAppMenu(NSApplication* nsapp,
                         id app_delegate,
                         const std::u16string& product_name) {
  NSMenuItem* item =
      Item(u"Chrohime")
          .submenu({
            Item(u"About " + product_name)
                .target(app_delegate)
                .action(@selector(orderFrontStandardAboutPanel:)),
            Item().is_separator(),
            Item(u"Services").tag(-1).submenu({}),
            Item().is_separator(),
            Item(u"Hide " + product_name)
                .action(@selector(hide:))
                .key_equivalent(@"h", NSEventModifierFlagCommand),
            Item(u"Hide Others")
                .action(@selector(hideOtherApplications:))
                .key_equivalent(@"h", NSEventModifierFlagCommand |
                                      NSEventModifierFlagOption),
            Item(u"Show All")
                .action(@selector(unhideAllApplications:)),
            Item().is_separator(),
            // AppKit inserts "Quit and Keep Windows" as an alternate item
            // automatically by using the -terminate: action.
            Item(u"Quit " + product_name)
                .target(nsapp)
                .action(@selector(terminate:))
                .key_equivalent(@"q", NSEventModifierFlagCommand),
          })
          .Build();

  NSMenuItem* services_item = [item.submenu itemWithTag:-1];
  [services_item setTag:0];
  nsapp.servicesMenu = services_item.submenu;

  return item;
}

NSMenuItem* BuildEditMenu(NSApplication* nsapp,
                          id app_delegate,
                          const std::u16string& product_name) {
  NSMenuItem* item =
      Item(u"Edit")
          .submenu({
            Item(u"Undo")
                .action(@selector(undo:))
                .key_equivalent(@"z", NSEventModifierFlagCommand),
            Item(u"Redo")
                .action(@selector(redo:))
                .key_equivalent(@"Z", NSEventModifierFlagCommand),
            Item().is_separator(),
            Item(u"Cut")
                .action(@selector(cut:))
                .key_equivalent(@"x", NSEventModifierFlagCommand),
            Item(u"Copy")
                .action(@selector(copy:))
                .key_equivalent(@"c", NSEventModifierFlagCommand),
            Item(u"Paste")
                .action(@selector(paste:))
                .key_equivalent(@"v", NSEventModifierFlagCommand),
            Item(u"Paste and Match Style")
                .action(@selector(pasteAndMatchStyle:))
                .key_equivalent(@"V", NSEventModifierFlagCommand),
            Item(u"Paste and Match Style")
                .action(@selector(pasteAndMatchStyle:))
                .is_alternate()
                .key_equivalent(@"V", NSEventModifierFlagCommand |
                                      NSEventModifierFlagOption),
            Item(u"Delete")
                .action(@selector(delete:)),
            Item(u"Select All")
                .action(@selector(selectAll:))
                .key_equivalent(@"a", NSEventModifierFlagCommand),
            Item().is_separator(),
            Item(u"Spelling and Grammar")
                .submenu({
                  Item(u"Show Spelling and Grammar")
                      .action(@selector(showGuessPanel:))
                      .key_equivalent(@":", NSEventModifierFlagCommand),
                  Item(u"Check Document Now")
                      .action(@selector(checkSpelling:))
                      .key_equivalent(@";", NSEventModifierFlagCommand),
                  Item(u"Check Spelling While Typing")
                      .action(@selector(toggleContinuousSpellChecking:)),
                  Item(u"Check Grammar With Spelling")
                      .action(@selector(toggleGrammarChecking:)),
                }),
            Item(u"Substitutions").submenu({
              Item(u"Show Substitutions")
                  .action(@selector(orderFrontSubstitutionsPanel:)),
                  Item().is_separator(),
                  Item(u"Smart Quotes")
                      .action(@selector(toggleAutomaticQuoteSubstitution:)),
                  Item(u"Smart Dashes")
                      .action(@selector(toggleAutomaticDashSubstitution:)),
                  Item(u"Text Replacement")
                      .action(@selector(toggleAutomaticTextReplacement:)),
            }),
            Item(u"Transformations").submenu({
              Item(u"Make Uppercase")
                  .action(@selector(uppercaseWord:)),
              Item(u"Make Lowercase")
                  .action(@selector(lowercaseWord:)),
              Item(u"Capitalize")
                  .action(@selector(capitalizeWord:)),
            }),
            Item(IDS_SPEECH_MAC).submenu({
              Item(IDS_SPEECH_START_SPEAKING_MAC)
                  .action(@selector(startSpeaking:)),
              Item(IDS_SPEECH_STOP_SPEAKING_MAC)
                  .action(@selector(stopSpeaking:)),
            }),
            // The "Start Dictation..." and "Emoji & Symbols" items are
            // inserted by AppKit.
          })
          .Build();
  return item;
}

NSMenuItem* BuildViewMenu(NSApplication* nsapp,
                          id app_delegate,
                          const std::u16string& product_name) {
  NSMenuItem* item =
      Item(u"View")
          .submenu({
            Item(u"Enter Full Screen")
                .action(@selector(toggleFullScreen:))
                .is_alternate()
                .key_equivalent(@"f", NSEventModifierFlagCommand |
                                      NSEventModifierFlagControl),
            Item().is_separator(),
          })
          .Build();
  return item;
}

NSMenuItem* BuildWindowMenu(NSApplication* nsapp,
                            id app_delegate,
                            const std::u16string& product_name) {
  NSMenuItem* item =
      Item(u"Window")
          .submenu({
            Item(u"Minimize")
                .action(@selector(performMiniaturize:))
                .key_equivalent(@"m", NSEventModifierFlagCommand),
            Item(u"Zoom")
                .action(@selector(performZoom:)),
            // AppKit inserts "Close All" as an alternate item automatically
            // by using the -performClose: action.
            Item(u"Close Window")
                .action(@selector(performClose:)),
            Item().is_separator(),
          })
          .Build();
  [nsapp setWindowsMenu:[item submenu]];
  return item;
}

}  // namespace

void BuildMainMenu(NSApplication* nsapp,
                   id<NSApplicationDelegate> app_delegate,
                   const std::u16string& product_name) {
  NSMenu* main_menu = [[NSMenu alloc] initWithTitle:@""];
  for (auto* builder : {
           &BuildAppMenu,
           &BuildEditMenu,
           &BuildViewMenu,
           &BuildWindowMenu,
       }) {
    auto item = builder(nsapp, app_delegate, product_name);
    if (item)
      [main_menu addItem:item];
  }
  [nsapp setMainMenu:main_menu];
}

}  // namespace hime
