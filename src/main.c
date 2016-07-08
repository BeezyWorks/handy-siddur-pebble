#include "pebble.h"
#include "luach.h"
#include "text_window.h"
#include "hebrew_texts.h"

#define NUM_CELLS 5
#define MENU_CELL_HEIGHT 48

static Window *s_main_window;
static MenuLayer *s_menu_layer;



static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_CELLS;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
    menu_cell_basic_draw(ctx, cell_layer, "ןוזמה תכרב", NULL, NULL);
    break;
    case 1:
    menu_cell_basic_draw(ctx, cell_layer, "שלוש ןיעמ", NULL, NULL);
    break;
    case 2:
    menu_cell_basic_draw(ctx, cell_layer, "ךרדה תליפת", NULL, NULL);
    break;
    case 3:
    menu_cell_basic_draw(ctx, cell_layer, "הטימה לע ש''ק", NULL, NULL);
    break;
    case 4:
    menu_cell_basic_draw(ctx, cell_layer, "חול", NULL, NULL);
    break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return MENU_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
    text_window_push(bentching_text);
    break;
    case 1:
    text_window_push(mein_shalosh);
    break;
    case 2:
    text_window_push(tefilas_haderech);
    break;
    case 4:
    luach_window_push();
    break;
  }
}

static void window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows_callback,
    .draw_row = draw_row_callback,
    .get_cell_height = get_cell_height_callback,
    .select_click = select_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(){
  init();
  app_event_loop();
  deinit();
}