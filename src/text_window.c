#include <pebble.h>
#include "text_window.h"
#include <stdio.h>
#include <string.h>
#include "rtl_faker.h"

static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_text_layer;

static GFont hebrew_font;
static char* s_scroll_text;
static char* reversed_str;
// Setup the scroll layer on window load
// We do this here in order to be able to get the max used text size
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  reversed_str = fake_rtl(s_scroll_text);

  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  hebrew_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TAAMEY_FRANK_20));


  // Initialize the scroll layer
  s_scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);

  // Initialize the text layer
  s_text_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(s_text_layer, reversed_str);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", reversed_str);

  //   text_layer_set_font(s_text_layer, hebrew_font);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentRight);

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(s_text_layer);
  text_layer_set_size(s_text_layer, GSize(bounds.size.w, max_size.h));
  scroll_layer_set_content_size(s_scroll_layer, GSize(bounds.size.w, max_size.h + 4));

  // Add the layers for display
  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));


  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "window loaded");
}

static void window_unload(Window *window) {
  free(reversed_str);
  free(s_scroll_text);
  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
  fonts_unload_custom_font(hebrew_font);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window unloaded");
}

void text_window_push(const char* text){
  s_scroll_text = malloc(strlen(text)+1);
  strncpy(s_scroll_text, text, strlen(text));
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}