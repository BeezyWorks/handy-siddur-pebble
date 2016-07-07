#include <pebble.h>
#include "luach.h"

# define NUM_ZMANIM 11

static Window *s_main_window;
static MenuLayer *s_menu_layer;

char zman_times[NUM_ZMANIM][6];

const char *zman_names[NUM_ZMANIM] = {"רחשה תולע",
                                      "ריכישמ",
                                      "המחה ץנה",
                                      "א''רג עמש", 
                                      "א''רג הליפת",
                                      "תוצח",
                                      "הלודג החנמ", 
                                      "המחה תעיקש", 
                                      "םיבככה תאצ",
                                      "ת''ר תאצ",
                                      "הלילה תוצח"};


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  static Tuple* tuples[NUM_ZMANIM];
  tuples[0] = dict_find(iterator, MESSAGE_KEY_ALOS);
  tuples[1] = dict_find(iterator, MESSAGE_KEY_MISHEYAKIR);
  tuples[2] = dict_find(iterator, MESSAGE_KEY_NEITZ);
  tuples[3] = dict_find(iterator, MESSAGE_KEY_SHMA_GRA);
  tuples[4] = dict_find(iterator, MESSAGE_KEY_TEFILA_GRA);
  tuples[5] = dict_find(iterator, MESSAGE_KEY_CHATZOS);
  tuples[6] = dict_find(iterator, MESSAGE_KEY_MINCHA_GEDOLA);
  tuples[7] = dict_find(iterator, MESSAGE_KEY_SHKIA);
  tuples[8] = dict_find(iterator, MESSAGE_KEY_TZAIS);
  tuples[9] = dict_find(iterator, MESSAGE_KEY_TZAIS_RT);
  tuples[10] = dict_find(iterator, MESSAGE_KEY_CHATZOS_LAILA);

  for(int i = 0 ; i<NUM_ZMANIM ; i++){
    char *data = tuples[i]->value->cstring;
    strcpy(zman_times[i], data);
  }
  
  window_stack_push(s_main_window, true);
}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_ZMANIM;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
 int row = cell_index -> row;
  menu_cell_basic_draw(ctx, cell_layer, zman_names[row], zman_times[row], NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return 48;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {

}

static void window_load(Window *window) {
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

void luach_window_push() { 
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
  }

  app_message_register_inbox_received(inbox_received_callback);
  // Open AppMessage
  const int inbox_size = 124;
  const int outbox_size = 32;
  app_message_open(inbox_size, outbox_size);
  
  // Declare the dictionary's iterator
DictionaryIterator *out_iter;

// Prepare the outbox buffer for this message
AppMessageResult result = app_message_outbox_begin(&out_iter);
if(result == APP_MSG_OK) {
  // Add an item to ask for weather data
  int value = 0;
  dict_write_int(out_iter, MESSAGE_KEY_ALOS, &value, sizeof(int), true);

  // Send this message
  result = app_message_outbox_send();
  if(result != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
  }
} else {
  // The outbox cannot be used right now
  APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
}

}