// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/c/chrohime.h"

#include <stdio.h>

chrohime_view_t CreateTableViewExample() {
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
  return (chrohime_view_t)scroll_view;
}

void OnReady(void* data) {
  chrohime_window_options_t options;
  chrohime_window_options_init(&options);
  chrohime_window_t window = chrohime_window_create(&options);
  chrohime_window_set_title(window, u"👸🏾");
  chrohime_window_center_to(window, (chrohime_size_t){400, 300});
  chrohime_window_on_close_connect(
      window, (chrohime_window_on_close_callback)chrohime_object_unref, NULL);
  chrohime_view_t content_view = chrohime_window_get_content_view(window);

  chrohime_view_t table = CreateTableViewExample();
  chrohime_view_set_number_style(table, u"flex", 1);
  chrohime_view_add_child_view(content_view, table);
  chrohime_object_unref((chrohime_object_t)table);

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
