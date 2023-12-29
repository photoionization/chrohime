// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime.h"

#include <stdio.h>
#include <string.h>

#define PADDING 10
#define GITHUB_REPO u"https://github.com/photoionization/chrohime"

static bool running_test = false;

hime_view_t CreateSection() {
  hime_view_t view = hime_view_create();
  hime_border_t border = hime_border_create_rounded_rect(
      sk_color_create_argb(50, 0, 0, 0), 1, 4);
  hime_view_set_border(view, border);
  hime_view_set_style_number(view, u"padding", PADDING);
  hime_object_unref((hime_object_t)border);
  return view;
}

void CreateMiscExample(hime_view_t view) {
  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_link_t link = hime_link_create();
  hime_label_set_text((hime_label_t)link, GITHUB_REPO);
  hime_view_add_child_view(view, (hime_view_t)link);
  hime_object_unref((hime_object_t)link);

  hime_badge_t badge = hime_badge_create();
  hime_badge_set_text(badge, u"GitHub Repo");
  hime_view_add_child_view(view, (hime_view_t)badge);
  hime_object_unref((hime_object_t)badge);
}

void OnSliderChange(hime_slider_t slider, float new_value, float old_value,
                    void* data) {
  hime_progress_bar_t bar = (hime_progress_bar_t)data;
  hime_progress_bar_set_value(bar, new_value);
}

void CreateSliderExample(hime_view_t view) {
  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");
  hime_view_set_style(view, u"align-items", u"center");
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_slider_t slider = hime_slider_create();
  hime_slider_set_value(slider, 0.5);
  hime_view_set_style_number((hime_view_t)slider, u"flex", 1);
  hime_view_set_accessible_name((hime_view_t)slider, u"slider");
  hime_view_add_child_view(view, (hime_view_t)slider);
  hime_object_unref((hime_object_t)slider);

  hime_progress_bar_t bar = hime_progress_bar_create();
  hime_progress_bar_set_value(bar, 0.5);
  hime_view_set_style_number((hime_view_t)bar, u"flex", 1);
  hime_view_set_accessible_name((hime_view_t)bar, u"progress bar");
  hime_view_add_child_view(view, (hime_view_t)bar);
  hime_object_unref((hime_object_t)bar);

  hime_slider_on_change_connect(slider, OnSliderChange, bar);

  hime_progress_bar_t indeterminate = hime_progress_bar_create();
  hime_progress_bar_set_indeterminate(indeterminate, true);
  hime_view_set_style_number((hime_view_t)indeterminate, u"flex", 1);
  hime_view_set_accessible_name((hime_view_t)indeterminate, u"indeterminate");
  hime_view_add_child_view(view, (hime_view_t)indeterminate);
  hime_object_unref((hime_object_t)indeterminate);
}

void CreateLabelExample(hime_view_t view) {
  hime_label_t label = hime_label_create();
  hime_label_set_multi_line(label, true);
  hime_label_set_selectable(label, true);
  hime_label_set_text(label, u"This is a multi-line label...\n"
                             u"...which supports text selection.");

  hime_view_add_child_view(view, (hime_view_t)label);
  hime_object_unref((hime_object_t)label);
}

void OnPickerChange(hime_picker_t picker, void* data) {
  hime_combobox_set_text((hime_combobox_t)data,
                         hime_picker_get_selected_item(picker));
}

void CreateComboboxExample(hime_view_t view) {
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_simple_combobox_model_t model1 =
      hime_simple_combobox_model_create();
  hime_simple_combobox_model_t model2 =
      hime_simple_combobox_model_create();
  const char16_t* list[] = {
      u"Kurohime", u"Obasute", u"Matsumoto", u"Omachi", u"Azumino", u"Nagano",
  };
  for (size_t i = 0; i < sizeof(list) / sizeof(const char16_t*); ++i) {
    hime_simple_combobox_model_add_item(model1, list[i]);
    hime_simple_combobox_model_add_item(model2, list[i]);
  }

  hime_picker_t picker = hime_picker_create();
  hime_picker_set_model(picker, (hime_combobox_model_t)model1);
  hime_object_unref((hime_object_t)model1);
  hime_view_add_child_view(view, (hime_view_t)picker);
  hime_view_set_accessible_name((hime_view_t)picker, u"Picker");
  hime_object_unref((hime_object_t)picker);

  hime_combobox_t combobox = hime_combobox_create();
  hime_combobox_set_model(combobox, (hime_combobox_model_t)model2);
  hime_combobox_set_placeholder_text(combobox, u"Editable Combobox");
  hime_object_unref((hime_object_t)model2);
  hime_view_add_child_view(view, (hime_view_t)combobox);
  hime_view_set_accessible_name((hime_view_t)combobox, u"Combobox");
  hime_object_unref((hime_object_t)combobox);

  hime_picker_on_change_connect(picker, OnPickerChange, combobox);
}

void CreateTextfieldExample(hime_view_t view) {
  hime_view_set_style_number(view, u"flex", 1);
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_textfield_t text = hime_textfield_create();
  hime_view_set_accessible_name((hime_view_t)text, u"Textfield");
  hime_view_add_child_view(view, (hime_view_t)text);
  hime_object_unref((hime_object_t)text);

  hime_textfield_t password = hime_textfield_create();
  hime_textfield_set_type(password, kHimeTextfieldTypePassword);
  hime_textfield_set_placeholder_text(password, u"Password");
  hime_view_add_child_view(view, (hime_view_t)password);
  hime_object_unref((hime_object_t)password);

  hime_textfield_t invalid = hime_textfield_create();
  hime_textfield_set_invalid(invalid, true);
  hime_textfield_set_placeholder_text(invalid, u"Invalid Input");
  hime_view_add_child_view(view, (hime_view_t)invalid);
  hime_object_unref((hime_object_t)invalid);

  hime_textfield_t readonly = hime_textfield_create();
  hime_textfield_set_readonly(readonly, true);
  hime_textfield_set_placeholder_text(readonly, u"Readonly Textfield");
  hime_view_add_child_view(view, (hime_view_t)readonly);
  hime_object_unref((hime_object_t)readonly);

  hime_textarea_t textarea = hime_textarea_create();
  hime_textfield_set_text(
      (hime_textfield_t)textarea,
      u"In Chromium's views, the textarea stays,\n"
      u"Where words and dreams dance in endless arrays.\n"
      u"From UI's embrace, a canvas serene,\n"
      u"Where thoughts ignite and stories convene.\n"
      u"Lines unfold, gracefully and refined,\n"
      u"In the multi-line textarea, tales intertwined.");
  hime_view_set_accessible_name((hime_view_t)textarea, u"Textarea");
  hime_view_set_style_number((hime_view_t)textarea, u"flex", 1);
  hime_view_add_child_view(view, (hime_view_t)textarea);
  hime_object_unref((hime_object_t)textarea);
}

void OnButtonClick(hime_button_t button, void* data) {
  fprintf(stderr, "OnButtonClick\n");
}

void CreateMaterialButtonExample(hime_view_t view) {
  // Use the preferred background color of views to make buttons look better.
  hime_background_t background =
      hime_background_create_themed_rounded_rect(
          kHimeColorIdDialogBackground, 1, 4);
  hime_view_set_background(view, background);
  hime_object_unref((hime_object_t)background);

  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_material_button_t button1 = hime_material_button_create();
  hime_button_on_click_connect((hime_button_t)button1, OnButtonClick, NULL);
  hime_label_button_set_title((hime_label_button_t)button1, u"Button");
  hime_view_add_child_view(view, (hime_view_t)button1);
  hime_object_unref((hime_object_t)button1);

  hime_material_button_t button3 = hime_material_button_create();
  hime_label_button_set_title((hime_label_button_t)button3, u"Disabled");
  hime_view_set_enabled((hime_view_t)button3, false);
  hime_view_add_child_view(view, (hime_view_t)button3);
  hime_object_unref((hime_object_t)button3);

  hime_material_button_t button4 = hime_material_button_create();
  hime_material_button_set_style(button4, kHimeMaterialButtonStyleText);
  hime_label_button_set_title((hime_label_button_t)button4, u"Text");
  hime_view_add_child_view(view, (hime_view_t)button4);
  hime_object_unref((hime_object_t)button4);

  hime_material_button_t button5 = hime_material_button_create();
  hime_material_button_set_style(button5, kHimeMaterialButtonStyleTonal);
  hime_label_button_set_title((hime_label_button_t)button5, u"Tonal");
  hime_view_add_child_view(view, (hime_view_t)button5);
  hime_object_unref((hime_object_t)button5);

  hime_view_set_style_number(
      view, u"min-width", (float)hime_view_get_preferred_size(view).width);
}

void CreateCheckboxExample(hime_view_t view) {
  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");
  hime_view_set_style_number(view, u"gap", PADDING);

  hime_checkbox_t checkbox1 = hime_checkbox_create();
  hime_label_button_set_title((hime_label_button_t)checkbox1, u"Checkbox");
  hime_view_add_child_view(view, (hime_view_t)checkbox1);
  hime_object_unref((hime_object_t)checkbox1);

  hime_checkbox_t checkbox2 = hime_checkbox_create();
  hime_label_button_set_title((hime_label_button_t)checkbox2, u"Disabled");
  hime_view_set_enabled((hime_view_t)checkbox2, false);
  hime_view_add_child_view(view, (hime_view_t)checkbox2);
  hime_object_unref((hime_object_t)checkbox2);

  hime_checkbox_t checkbox3 = hime_checkbox_create();
  hime_label_button_set_title((hime_label_button_t)checkbox3,
                              u"Disabled checked");
  hime_checkbox_set_checked(checkbox3, true);
  hime_view_set_enabled((hime_view_t)checkbox3, false);
  hime_view_add_child_view(view, (hime_view_t)checkbox3);
  hime_object_unref((hime_object_t)checkbox3);
}

void CreateRadioButtonExample(hime_view_t view) {
  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");
  hime_view_set_style_number(view, u"gap", PADDING);
  for (int i = 0; i < 3; ++i ) {
    char16_t title[] = u"Checkbox ?";
    title[9] = u'1' + i;
    hime_radio_button_t radio =
        hime_radio_button_create_with_title_and_group(title, 1);
    if (i == 0)
      hime_checkbox_set_checked((hime_checkbox_t)radio, true);
    hime_view_add_child_view(view, (hime_view_t)radio);
    hime_object_unref((hime_object_t)radio);
  }
}

void CreateToggleButtonExample(hime_view_t view) {
  hime_view_set_style(view, u"flex-direction", u"row");
  hime_view_set_style(view, u"justify-content", u"center");

  hime_toggle_button_t button1 = hime_toggle_button_create();
  hime_view_set_accessible_name((hime_view_t)button1, u"button1");
  hime_view_add_child_view(view, (hime_view_t)button1);
  hime_object_unref((hime_object_t)button1);

  hime_toggle_button_t button2 = hime_toggle_button_create();
  hime_toggle_button_set_on_off(button2, true);
  hime_view_set_accessible_name((hime_view_t)button2, u"button2");
  hime_view_add_child_view(view, (hime_view_t)button2);
  hime_object_unref((hime_object_t)button2);

  hime_toggle_button_t button3 = hime_toggle_button_create();
  hime_view_set_enabled((hime_view_t)button3, false);
  hime_view_set_accessible_name((hime_view_t)button3, u"button3");
  hime_view_add_child_view(view, (hime_view_t)button3);
  hime_object_unref((hime_object_t)button3);
}

#if defined(CHROHIME_WITH_CONTENT)
hime_view_t CreateWebViewPage() {
  hime_web_view_t webview = hime_web_view_create();
  hime_web_view_load_initial_url(webview, GITHUB_REPO);
  return (hime_view_t)webview;
}
#endif

hime_view_t CreateTableViewPage() {
  hime_simple_table_model_t model = hime_simple_table_model_create();
  const char16_t* rows[5][4] = {
      { u"Orange", u"Orange", u"South America", u"$5" },
      { u"Apple", u"Green", u"Canada", u"$3" },
      { u"Blueberries", u"Blue", u"Mexico", u"$10.30" },
      { u"Strawberries", u"Red", u"California", u"$7" },
      { u"Cantaloupe", u"Orange", u"South America", u"$5" },
  };
  for (size_t i = 0; i < 5; ++ i) {
    hime_simple_table_model_add_row(model, rows[i], 4);
  }

  hime_table_view_t table = hime_table_view_create();
  hime_table_view_column_t columns[4];
  for (size_t i = 0; i < 4; ++i) {
    hime_table_view_column_init(&columns[i]);
    columns[i].id = (int)i;
  }
  columns[0].title = u"Fruit";
  columns[0].percent = 1;
  columns[1].title = u"Color";
  columns[2].title = u"Origin";
  columns[3].title = u"Price";
  hime_table_view_set_columns(table, columns, 4);
  hime_table_view_set_model(table, (hime_table_model_t)model);
  hime_object_unref((hime_object_t)model);

  hime_scroll_view_t scroll_view = hime_scroll_view_create_with_table(table);
  hime_object_unref((hime_object_t)table);
  return (hime_view_t)scroll_view;
}

void OnDraw(hime_view_t view, hime_painter_t painter, void* data) {
  hime_rect_t bounds = hime_view_get_bounds(view);
  hime_vector2df_t offset = {
    (bounds.width - 150.f) / 2.f,
    (bounds.height - 150.f) / 2.f,
  };
  hime_painter_translate(painter, offset);
  hime_paint_t paint;
  hime_paint_init(&paint);
  paint.color = sk_color_create_rgb(0xD4, 0x6A, 0x6A);
  hime_painter_draw_path(painter, (hime_path_t)data, &paint);
}

hime_view_t CreatePainterPage() {
  hime_path_t path = hime_path_create();
#define P(x, y) (hime_pointf_t){x, y}
  hime_path_move_to(path, P(75, 40));
  hime_path_cubic_to(path, P(75, 37), P(70, 25), P(50, 25));
  hime_path_cubic_to(path, P(20, 25), P(20, 62.5f), P(20, 62.5f));
  hime_path_cubic_to(path, P(20, 80), P(40, 102), P(75, 120));
  hime_path_cubic_to(path, P(110, 102), P(130, 80), P(130, 62.5f));
  hime_path_cubic_to(path, P(130, 62.5f), P(130, 25), P(100, 25));
  hime_path_cubic_to(path, P(85, 25), P(75, 37), P(75, 40));
#undef P

  hime_view_t view = hime_view_create();
  hime_view_on_draw_connect_closure(view, OnDraw, path,
                                    (hime_free_callback)hime_object_unref);
  return view;
}

void CreateTabViewExample(hime_view_t view) {
  hime_view_set_style_number(view, u"flex", 1);

  hime_tab_view_t tab = hime_tab_view_create();
  hime_view_t page1 = CreateTableViewPage();
  hime_tab_view_add_page(tab, u"TableView", page1);
  hime_object_unref((hime_object_t)page1);

  hime_view_t page2 = CreatePainterPage();
  hime_tab_view_add_page(tab, u"Painter", page2);
  hime_object_unref((hime_object_t)page2);

#if defined(CHROHIME_WITH_CONTENT)
  hime_view_t page3 = CreateWebViewPage();
  hime_tab_view_add_page_at(tab, u"WebView", page3, 0);
  hime_object_unref((hime_object_t)page3);
#endif

  hime_view_set_style_number((hime_view_t)tab, u"flex", 1);
  hime_view_add_child_view(view, (hime_view_t)tab);
  hime_object_unref((hime_object_t)tab);
}

hime_view_t CreateColumn(hime_view_t content_view) {
  hime_view_t view = hime_view_create();
  hime_view_set_style_number(view, u"gap", PADDING);
  hime_view_set_style_number(view, u"flex", 1);
  hime_view_add_child_view(content_view, view);
  hime_object_unref((hime_object_t)view);
  return view;
}

void OnWindowClose(hime_window_t window, void* data) {
  hime_object_unref((hime_object_t)window);
  hime_lifetime_quit(hime_lifetime_get_current());
}

void OnReady(void* data) {
  hime_window_options_t options;
  hime_window_options_init(&options);
  hime_window_t window = hime_window_create(&options);
  hime_window_set_title(window, u"👸🏾");
  hime_window_center_to(window, (hime_size_t){800, 600});
  hime_window_on_close_connect(
      window, (hime_window_on_close_callback)OnWindowClose, NULL);

  hime_view_t content_view = hime_window_get_content_view(window);
  hime_view_set_style(content_view, u"flex-direction", u"row");
  hime_view_set_style_number(content_view, u"padding", PADDING);
  hime_view_set_style_number(content_view, u"gap", PADDING);

  hime_view_t column1 = CreateColumn(content_view);
  hime_view_t column2 = CreateColumn(content_view);

  typedef void (*CreateExample)(hime_view_t view);
  CreateExample examples[] = {
      CreateMiscExample,
      CreateSliderExample,
      CreateLabelExample,
      CreateComboboxExample,
      CreateTextfieldExample,
      CreateMaterialButtonExample,
      CreateCheckboxExample,
      CreateRadioButtonExample,
      CreateToggleButtonExample,
      CreateTabViewExample,
  };
  for (size_t i = 0; i < sizeof(examples) / sizeof(CreateExample); ++i) {
    hime_view_t section = CreateSection();
    examples[i](section);
    hime_view_add_child_view(i < 5 ? column1 : column2, section);
    hime_object_unref((hime_object_t)section);
  }

  hime_window_activate(window);

  if (running_test) {
    hime_window_close(window);
  }
}

int main(int argc, const char** argv) {
  running_test = argc == 2 && strcmp(argv[1], "--test") == 0;

#if defined(_WIN32)
  hime_lifetime_t lifetime = hime_lifetime_create();
#else
  hime_lifetime_t lifetime = hime_lifetime_create(argc, argv);
#endif
  hime_lifetime_on_ready_connect(lifetime, OnReady, NULL);

  hime_state_t state = hime_state_create();

  int code = hime_lifetime_run_main(lifetime);

  hime_state_destroy(state);
  hime_lifetime_destroy(lifetime);
  return code;
}
