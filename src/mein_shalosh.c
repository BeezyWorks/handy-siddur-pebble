#include <pebble.h>
#include "mein_shalosh.h"
#include <stdio.h>
#include <string.h>

static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_text_layer;

static GFont hebrew_font;
//char s_scroll_text[] = "ברוך אתה יהוה אלהינו מלך העולם הזן את העולם כלו בטובו בחן בחסד וברחמים , הוא נתן לחם לכל בשר כי לעולם חסדו ובטובו הגדול תמיד לא חסר לנו ואל יחסר לנו מזון לעולם ועד בעבור שמו הגדול כי הוא אל זן ומפרנס לכל ומטיב לכל ומכין מזון לכל בריותיו אשר ברא ברוך אתה יי הזן את הכל";
char s_scroll_text[]="לכה תא ןזה יי התא ךורב ארב רשא ויתוירב לכל ןוזמ ןיכמו לכל ביטמו לכל סנרפמו ןז לא אוה יכ לודגה ומש רובעב דעו םלועל ןוזמ ונל רסחי לאו ונל רסח אל דימת לודגה ובוטבו ודסח םלועל יכ רשב לכל םחל ןתנ אוה , םימחרבו דסחב ןחב ובוטב ולכ םלועה תא ןזה םלועה ךלמ וניהלא הוהי התא ךורב";

static void reverse(char s[])
{
    int length = strlen(s) ;
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Setup the scroll layer on window load
// We do this here in order to be able to get the max used text size
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
//    APP_LOG(APP_LOG_LEVEL_DEBUG, s_scroll_text);
//   reverse(s_scroll_text);
//     APP_LOG(APP_LOG_LEVEL_DEBUG, s_scroll_text);
  
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
  text_layer_set_text(s_text_layer, s_scroll_text);

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(s_text_layer, hebrew_font);
// text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  
  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(s_text_layer);
  text_layer_set_size(s_text_layer, max_size);
  scroll_layer_set_content_size(s_scroll_layer, GSize(bounds.size.w, max_size.h + 4));

  // Add the layers for display
  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));


  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
  fonts_unload_custom_font(hebrew_font);
}

void mein_shalosh_window_push(){
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}