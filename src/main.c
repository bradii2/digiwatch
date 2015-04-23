#include <pebble.h>

static Window *s_window;
static TextLayer *s_hour_layer;
static TextLayer *s_second_layer;
static TextLayer *s_battery_layer;
static BitmapLayer *s_bitmap_layer;
static GBitmap *s_background_bitmap;
static GFont s_time_font;

static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char minute_buffer[] = "00:00";
  static char second_buffer[] = "00";
  
  if(clock_is_24h_style() == true){
    strftime(minute_buffer, sizeof(minute_buffer), "%H:%M", tick_time);
  }
  else{
    strftime(minute_buffer, sizeof(minute_buffer), "%I:%M", tick_time);
  }
  strftime(second_buffer, sizeof(second_buffer), "%S", tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void main_window_load(Window *window){
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BRADWATCH_BACK);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_SEVEN_24));
  
  s_hour_layer = text_layer_create(GRect(34, 47, 76, 21));
  s_second_layer = text_layer_create(GRect(34, 57, 76, 21));
  s_battery_layer = text_layer_create(GRect(3, 5, 17, 4));
  
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_second_layer, GColorBlack);
  text_layer_set_background_color(s_second_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorBlack);
  
  text_layer_set_font(s_hour_layer, s_time_font);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  text_layer_set_font(s_second_layer, s_time_font);
  text_layer_set_text_alignment(s_second_layer, GTextAlignmentCenter);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_second_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
}

static void main_window_unload(Window *window){
  fonts_unload_custom_font(s_time_font);
  
  gbitmap_destroy(s_background_bitmap);
  
  bitmap_layer_destroy(s_bitmap_layer);
  
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_second_layer);
  text_layer_destroy(s_battery_layer);
}

static void init() {
  s_window = window_create();

  window_set_window_handlers(s_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  text_layer_destroy(s_hour_layer);
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
