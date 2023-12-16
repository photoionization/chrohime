// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/c/chrohime.h"

#include <stdio.h>

chrohime_view_t CreateSection() {
  chrohime_view_t view = chrohime_view_create();
  chrohime_border_t border = chrohime_border_create_rounded_rect(
      sk_color_create_argb(50, 0, 0, 0), 1, 4);
  chrohime_view_set_border(view, border);
  chrohime_view_set_number_style(view, u"padding", 5);
  chrohime_view_set_number_style(view, u"margin", 5);
  chrohime_object_unref((chrohime_object_t)border);
  return view;
}

void OnButtonClick(chrohime_button_t button, void* data) {
  fprintf(stderr, "OnButtonClick\n");
}

void CreateMaterialButtonExample(chrohime_view_t view) {
  chrohime_background_t background = chrohime_background_create_rounded_rect(
      sk_color_create_rgb(255, 255, 255), 1, 4);
  chrohime_view_set_background(view, background);
  chrohime_object_unref((chrohime_object_t)background);

  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_number_style(view, u"gap", 5);

  chrohime_material_button_t button1 = chrohime_material_button_create();
  chrohime_button_on_click_connect(
      (chrohime_button_t)button1, OnButtonClick, NULL);
  chrohime_label_button_set_title((chrohime_label_button_t)button1, u"Button");
  chrohime_view_add_child_view(view, (chrohime_view_t)button1);
  chrohime_object_unref((chrohime_object_t)button1);

  chrohime_material_button_t button2 = chrohime_material_button_create();
  chrohime_button_on_click_connect(
      (chrohime_button_t)button2, OnButtonClick, NULL);
  chrohime_label_button_make_default((chrohime_label_button_t)button2, true);
  chrohime_label_button_set_title((chrohime_label_button_t)button2, u"Default");
  chrohime_view_add_child_view(view, (chrohime_view_t)button2);
  chrohime_object_unref((chrohime_object_t)button2);

  chrohime_material_button_t button3 = chrohime_material_button_create();
  chrohime_label_button_set_title(
      (chrohime_label_button_t)button3, u"Disabled");
  chrohime_view_set_enabled((chrohime_view_t)button3, false);
  chrohime_view_add_child_view(view, (chrohime_view_t)button3);
  chrohime_object_unref((chrohime_object_t)button3);

  chrohime_material_button_t button4 = chrohime_material_button_create();
  chrohime_material_button_set_style(button4,
                                     CHROHIME_MATERIAL_BUTTON_STYLE_TONAL);
  chrohime_label_button_set_title((chrohime_label_button_t)button4, u"Tonal");
  chrohime_view_add_child_view(view, (chrohime_view_t)button4);
  chrohime_object_unref((chrohime_object_t)button4);

  chrohime_material_button_t button5 = chrohime_material_button_create();
  chrohime_material_button_set_style(button5,
                                     CHROHIME_MATERIAL_BUTTON_STYLE_TEXT);
  chrohime_label_button_set_title((chrohime_label_button_t)button5, u"Text");
  chrohime_view_add_child_view(view, (chrohime_view_t)button5);
  chrohime_object_unref((chrohime_object_t)button5);

  chrohime_view_set_number_style(
      view, u"min-width", chrohime_view_get_preferred_size(view).width);
}

void CreateCheckboxExample(chrohime_view_t view) {
  chrohime_view_set_style(view, u"flex-direction", u"row");
  chrohime_view_set_style(view, u"justify-content", u"center");
  chrohime_view_set_number_style(view, u"gap", 5);

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
  chrohime_view_set_number_style(view, u"gap", 5);
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
  chrohime_view_set_number_style(view, u"flex", 1);

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

  chrohime_view_set_number_style((chrohime_view_t)scroll_view, u"flex", 1);
  chrohime_view_add_child_view(view, (chrohime_view_t)scroll_view);
  chrohime_object_unref((chrohime_object_t)scroll_view);
}

void OnReady(void* data) {
  chrohime_window_options_t options;
  chrohime_window_options_init(&options);
  chrohime_window_t window = chrohime_window_create(&options);
  chrohime_window_set_title(window, u"👸🏾");
  chrohime_window_center_to(window, (chrohime_size_t){400, 400});
  chrohime_window_on_close_connect(
      window, (chrohime_window_on_close_callback)chrohime_object_unref, NULL);
  chrohime_view_t content_view = chrohime_window_get_content_view(window);

  typedef void (*CreateExample)(chrohime_view_t view);
  CreateExample examples[] = {
      CreateMaterialButtonExample,
      CreateCheckboxExample,
      CreateRadioButtonExample,
      CreateToggleButtonExample,
      CreateTableViewExample,
  };
  for (size_t i = 0; i < sizeof(examples) / sizeof(CreateExample); ++i) {
    chrohime_view_t section = CreateSection();
    examples[i](section);
    chrohime_view_add_child_view(content_view, section);
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
