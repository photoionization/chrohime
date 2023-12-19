// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/c/chrohime.h"

#include <stdio.h>

#define PADDING 10

chrohime_view_t CreateSection() {
  chrohime_view_t view = chrohime_view_create();
  chrohime_border_t border = chrohime_border_create_rounded_rect(
      sk_color_create_argb(50, 0, 0, 0), 1, 4);
  chrohime_view_set_border(view, border);
  chrohime_view_set_style_number(view, u"padding", PADDING);
  chrohime_object_unref((chrohime_object_t)border);
  return view;
}

void CreateMiscExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_link_t link = chrohime_link_create();
  chrohime_label_set_text((chrohime_label_t)link,
                          u"https://github.com/photoionization/chrohime");
  chrohime_view_add_child_view(view, (chrohime_view_t)link);
  chrohime_object_unref((chrohime_object_t)link);

  chrohime_badge_t badge = chrohime_badge_create();
  chrohime_badge_set_text(badge, u"GitHub Repo");
  chrohime_view_add_child_view(view, (chrohime_view_t)badge);
  chrohime_object_unref((chrohime_object_t)badge);
}

void OnSliderChange(chrohime_slider_t slider, float new_value, float old_value,
                    void* data) {
  chrohime_progress_bar_t bar = (chrohime_progress_bar_t)data;
  chrohime_progress_bar_set_value(bar, new_value);
}

void CreateSliderExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_style(view, u"align-items", u"center");
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_slider_t slider = chrohime_slider_create();
  chrohime_slider_set_value(slider, 0.5);
  chrohime_view_set_style_number((chrohime_view_t)slider, u"flex", 1);
  chrohime_view_set_accessible_name((chrohime_view_t)slider, u"slider");
  chrohime_view_add_child_view(view, (chrohime_view_t)slider);
  chrohime_object_unref((chrohime_object_t)slider);

  chrohime_progress_bar_t bar = chrohime_progress_bar_create();
  chrohime_progress_bar_set_value(bar, 0.5);
  chrohime_view_set_style_number((chrohime_view_t)bar, u"flex", 1);
  chrohime_view_set_accessible_name((chrohime_view_t)bar, u"progress bar");
  chrohime_view_add_child_view(view, (chrohime_view_t)bar);
  chrohime_object_unref((chrohime_object_t)bar);

  chrohime_slider_on_change_connect(slider, OnSliderChange, bar);

  chrohime_progress_bar_t indeterminate = chrohime_progress_bar_create();
  chrohime_progress_bar_set_indeterminate(indeterminate, true);
  chrohime_view_set_style_number((chrohime_view_t)indeterminate, u"flex", 1);
  chrohime_view_set_accessible_name(
      (chrohime_view_t)indeterminate, u"indeterminate");
  chrohime_view_add_child_view(view, (chrohime_view_t)indeterminate);
  chrohime_object_unref((chrohime_object_t)indeterminate);
}

void CreateLabelExample(chrohime_view_t view) {
  chrohime_label_t label = chrohime_label_create();
  chrohime_label_set_multi_line(label, true);
  chrohime_label_set_selectable(label, true);
  chrohime_label_set_text(label, u"This is a multi-line label...\n"
                                 u"...which supports text selection.");

  chrohime_view_add_child_view(view, (chrohime_view_t)label);
  chrohime_object_unref((chrohime_object_t)label);
}

void OnPickerChange(chrohime_picker_t picker, void* data) {
  chrohime_combobox_set_text(
      (chrohime_combobox_t)data, chrohime_picker_get_selected_item(picker));
}

void CreateComboboxExample(chrohime_view_t view) {
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_simple_combobox_model_t model1 =
      chrohime_simple_combobox_model_create();
  chrohime_simple_combobox_model_t model2 =
      chrohime_simple_combobox_model_create();
  const char16_t* list[] = {
      u"Kurohime", u"Obasute", u"Matsumoto", u"Omachi", u"Azumino", u"Nagano",
  };
  for (size_t i = 0; i < sizeof(list) / sizeof(const char16_t*); ++i) {
    chrohime_simple_combobox_model_add_item(model1, list[i]);
    chrohime_simple_combobox_model_add_item(model2, list[i]);
  }

  chrohime_picker_t picker = chrohime_picker_create();
  chrohime_picker_set_model(picker, (chrohime_combobox_model_t)model1);
  chrohime_object_unref((chrohime_object_t)model1);
  chrohime_view_add_child_view(view, (chrohime_view_t)picker);
  chrohime_view_set_accessible_name((chrohime_view_t)picker, u"Picker");
  chrohime_object_unref((chrohime_object_t)picker);

  chrohime_combobox_t combobox = chrohime_combobox_create();
  chrohime_combobox_set_model(combobox, (chrohime_combobox_model_t)model2);
  chrohime_object_unref((chrohime_object_t)model2);
  chrohime_view_add_child_view(view, (chrohime_view_t)combobox);
  chrohime_view_set_accessible_name((chrohime_view_t)combobox, u"Combobox");
  chrohime_object_unref((chrohime_object_t)combobox);

  chrohime_picker_on_change_connect(picker, OnPickerChange, combobox);
}

void CreateTextfieldExample(chrohime_view_t view) {
  chrohime_view_set_style_number(view, u"flex", 1);
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_textfield_t text = chrohime_textfield_create();
  chrohime_view_set_accessible_name((chrohime_view_t)text, u"Textfield");
  chrohime_view_add_child_view(view, (chrohime_view_t)text);
  chrohime_object_unref((chrohime_object_t)text);

  chrohime_textfield_t password = chrohime_textfield_create();
  chrohime_textfield_set_type(password, kChrohimeTextfieldTypePassword);
  chrohime_textfield_set_placeholder_text(password, u"Password");
  chrohime_view_add_child_view(view, (chrohime_view_t)password);
  chrohime_object_unref((chrohime_object_t)password);

  chrohime_textfield_t invalid = chrohime_textfield_create();
  chrohime_textfield_set_invalid(invalid, true);
  chrohime_textfield_set_placeholder_text(invalid, u"Invalid Input");
  chrohime_view_add_child_view(view, (chrohime_view_t)invalid);
  chrohime_object_unref((chrohime_object_t)invalid);

  chrohime_textfield_t readonly = chrohime_textfield_create();
  chrohime_textfield_set_readonly(readonly, true);
  chrohime_textfield_set_placeholder_text(readonly, u"Readonly Textfield");
  chrohime_view_add_child_view(view, (chrohime_view_t)readonly);
  chrohime_object_unref((chrohime_object_t)readonly);

  chrohime_textarea_t textarea = chrohime_textarea_create();
  chrohime_textfield_set_text(
      (chrohime_textfield_t)textarea,
      u"In Chromium's views, the textarea stays,\n"
      u"Where words and dreams dance in endless arrays.\n"
      u"From UI's embrace, a canvas serene,\n"
      u"Where thoughts ignite and stories convene.\n"
      u"Lines unfold, gracefully and refined,\n"
      u"In the multi-line textarea, tales intertwined.");
  chrohime_view_set_accessible_name((chrohime_view_t)textarea, u"Textarea");

  chrohime_scroll_view_t scroll_view = chrohime_scroll_view_create();
  chrohime_view_set_style_number((chrohime_view_t)scroll_view, u"flex", 1);
  chrohime_scroll_view_set_content_view(scroll_view, (chrohime_view_t)textarea);
  chrohime_object_unref((chrohime_object_t)textarea);

  chrohime_view_add_child_view(view, (chrohime_view_t)scroll_view);
  chrohime_object_unref((chrohime_object_t)scroll_view);
}

void OnButtonClick(chrohime_button_t button, void* data) {
  fprintf(stderr, "OnButtonClick\n");
}

void CreateMaterialButtonExample(chrohime_view_t view) {
  // Use the preferred background color of views to make buttons look better.
  chrohime_background_t background =
      chrohime_background_create_themed_rounded_rect(
          kChrohimeColorIdDialogBackground, 1, 4);
  chrohime_view_set_background(view, background);
  chrohime_object_unref((chrohime_object_t)background);

  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_material_button_t button1 = chrohime_material_button_create();
  chrohime_button_on_click_connect(
      (chrohime_button_t)button1, OnButtonClick, NULL);
  chrohime_label_button_set_title((chrohime_label_button_t)button1, u"Button");
  chrohime_view_add_child_view(view, (chrohime_view_t)button1);
  chrohime_object_unref((chrohime_object_t)button1);

  chrohime_material_button_t button3 = chrohime_material_button_create();
  chrohime_label_button_set_title(
      (chrohime_label_button_t)button3, u"Disabled");
  chrohime_view_set_enabled((chrohime_view_t)button3, false);
  chrohime_view_add_child_view(view, (chrohime_view_t)button3);
  chrohime_object_unref((chrohime_object_t)button3);

  chrohime_material_button_t button4 = chrohime_material_button_create();
  chrohime_material_button_set_style(button4,
                                     kChrohimeMaterialButtonStyleTonal);
  chrohime_label_button_set_title((chrohime_label_button_t)button4, u"Tonal");
  chrohime_view_add_child_view(view, (chrohime_view_t)button4);
  chrohime_object_unref((chrohime_object_t)button4);

  chrohime_material_button_t button5 = chrohime_material_button_create();
  chrohime_material_button_set_style(button5,
                                     kChrohimeMaterialButtonStyleText);
  chrohime_label_button_set_title((chrohime_label_button_t)button5, u"Text");
  chrohime_view_add_child_view(view, (chrohime_view_t)button5);
  chrohime_object_unref((chrohime_object_t)button5);

  chrohime_view_set_style_number(
      view, u"min-width", chrohime_view_get_preferred_size(view).width);
}

void CreateCheckboxExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_style_number(view, u"gap", PADDING);

  chrohime_checkbox_t checkbox1 = chrohime_checkbox_create();
  chrohime_label_button_set_title(
      (chrohime_label_button_t)checkbox1, u"Checkbox");
  chrohime_view_add_child_view(view, (chrohime_view_t)checkbox1);
  chrohime_object_unref((chrohime_object_t)checkbox1);

  chrohime_checkbox_t checkbox2 = chrohime_checkbox_create();
  chrohime_label_button_set_title(
      (chrohime_label_button_t)checkbox2, u"Disabled");
  chrohime_view_set_enabled((chrohime_view_t)checkbox2, false);
  chrohime_view_add_child_view(view, (chrohime_view_t)checkbox2);
  chrohime_object_unref((chrohime_object_t)checkbox2);

  chrohime_checkbox_t checkbox3 = chrohime_checkbox_create();
  chrohime_label_button_set_title(
      (chrohime_label_button_t)checkbox3, u"Disabled checked");
  chrohime_checkbox_set_checked(checkbox3, true);
  chrohime_view_set_enabled((chrohime_view_t)checkbox3, false);
  chrohime_view_add_child_view(view, (chrohime_view_t)checkbox3);
  chrohime_object_unref((chrohime_object_t)checkbox3);
}

void CreateRadioButtonExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_style_number(view, u"gap", PADDING);
  for (int i = 0; i < 3; ++i ) {
    char16_t title[] = u"Checkbox ?";
    title[9] = u'1' + i;
    chrohime_radio_button_t radio =
        chrohime_radio_button_create_with_title_and_group(title, 1);
    if (i == 0)
      chrohime_checkbox_set_checked((chrohime_checkbox_t)radio, true);
    chrohime_view_add_child_view(view, (chrohime_view_t)radio);
    chrohime_object_unref((chrohime_object_t)radio);
  }
}

void CreateToggleButtonExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");

  chrohime_toggle_button_t button1 = chrohime_toggle_button_create();
  chrohime_view_set_accessible_name((chrohime_view_t)button1, u"button1");
  chrohime_view_add_child_view(view, (chrohime_view_t)button1);
  chrohime_object_unref((chrohime_object_t)button1);

  chrohime_toggle_button_t button2 = chrohime_toggle_button_create();
  chrohime_toggle_button_set_on_off(button2, true);
  chrohime_view_set_accessible_name((chrohime_view_t)button2, u"button2");
  chrohime_view_add_child_view(view, (chrohime_view_t)button2);
  chrohime_object_unref((chrohime_object_t)button2);

  chrohime_toggle_button_t button3 = chrohime_toggle_button_create();
  chrohime_view_set_enabled((chrohime_view_t)button3, false);
  chrohime_view_set_accessible_name((chrohime_view_t)button3, u"button3");
  chrohime_view_add_child_view(view, (chrohime_view_t)button3);
  chrohime_object_unref((chrohime_object_t)button3);
}

void CreateTableViewExample(chrohime_view_t view) {
  chrohime_view_set_style_number(view, u"flex", 1);

  chrohime_simple_table_model_t model = chrohime_simple_table_model_create();
  const char16_t* rows[5][4] = {
      { u"Orange", u"Orange", u"South America", u"$5" },
      { u"Apple", u"Green", u"Canada", u"$3" },
      { u"Blueberries", u"Blue", u"Mexico", u"$10.30" },
      { u"Strawberries", u"Red", u"California", u"$7" },
      { u"Cantaloupe", u"Orange", u"South America", u"$5" },
  };
  for (size_t i = 0; i < 5; ++ i) {
    chrohime_simple_table_model_add_row(model, rows[i], 4);
  }

  chrohime_table_view_t table = chrohime_table_view_create();
  chrohime_table_view_column_t columns[4];
  for (size_t i = 0; i < 4; ++i) {
    chrohime_table_view_column_init(&columns[i]);
    columns[i].id = i;
  }
  columns[0].title = u"Fruit";
  columns[0].percent = 1;
  columns[1].title = u"Color";
  columns[2].title = u"Origin";
  columns[3].title = u"Price";
  chrohime_table_view_set_columns(table, columns, 4);
  chrohime_table_view_set_model(table, (chrohime_table_model_t)model);
  chrohime_object_unref((chrohime_object_t)model);

  chrohime_scroll_view_t scroll_view =
      chrohime_scroll_view_create_with_table(table);
  chrohime_object_unref((chrohime_object_t)table);

  chrohime_view_set_style_number((chrohime_view_t)scroll_view, u"flex", 1);
  chrohime_view_add_child_view(view, (chrohime_view_t)scroll_view);
  chrohime_object_unref((chrohime_object_t)scroll_view);
}

chrohime_view_t CreateColumn(chrohime_view_t content_view) {
  chrohime_view_t view = chrohime_view_create();
  chrohime_view_set_style_number(view, u"gap", PADDING);
  chrohime_view_set_style_number(view, u"flex", 1);
  chrohime_view_add_child_view(content_view, view);
  chrohime_object_unref((chrohime_object_t)view);
  return view;
}

void OnReady(void* data) {
  chrohime_window_options_t options;
  chrohime_window_options_init(&options);
  chrohime_window_t window = chrohime_window_create(&options);
  chrohime_window_set_title(window, u"👸🏾");
  chrohime_window_center_to(window, (chrohime_size_t){800, 600});
  chrohime_window_on_close_connect(
      window, (chrohime_window_on_close_callback)chrohime_object_unref, NULL);

  chrohime_view_t content_view = chrohime_window_get_content_view(window);
  chrohime_view_set_style(content_view, u"flex-direction", u"row");
  chrohime_view_set_style_number(content_view, u"padding", PADDING);
  chrohime_view_set_style_number(content_view, u"gap", PADDING);

  chrohime_view_t column1 = CreateColumn(content_view);
  chrohime_view_t column2 = CreateColumn(content_view);

  typedef void (*CreateExample)(chrohime_view_t view);
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
      CreateTableViewExample,
  };
  for (size_t i = 0; i < sizeof(examples) / sizeof(CreateExample); ++i) {
    chrohime_view_t section = CreateSection();
    examples[i](section);
    chrohime_view_add_child_view(i < 5 ? column1 : column2, section);
    chrohime_object_unref((chrohime_object_t)section);
  }

  chrohime_window_activate(window);
}

int main(int argc, const char** argv) {
#if defined(_WIN32)
  chrohime_lifetime_t lifetime = chrohime_lifetime_create();
#else
  chrohime_lifetime_t lifetime = chrohime_lifetime_create(argc, argv);
#endif

  chrohime_state_create();
  chrohime_lifetime_on_ready_connect(lifetime, OnReady, NULL);
  return chrohime_lifetime_run_main(lifetime);
}
