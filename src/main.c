#include <pebble.h>

static Window *s_window;

static TextLayer *s_hour_layer;
static TextLayer *s_second_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_timeof_day_layer;
static TextLayer *s_year_layer;
static TextLayer *s_weekday_layer;
static TextLayer *s_month_layer;

static BitmapLayer *s_bitmap_layer;
static GBitmap *s_background_bitmap;

static GFont s_time_font;
static GFont s_second_font;


static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char minute_buffer[] = "00:00";
  static char second_buffer[] = "00";
  static char s_timeof_day[] = "AM";
  static char s_year[9];
  static char s_month[16];
  static char s_weekday[11];
  
  if(clock_is_24h_style() == true){
    strftime(minute_buffer, sizeof("00:00"), "%H:%M", tick_time);
    snprintf(s_timeof_day, sizeof("24"), "24");
  }
  else{
    strftime(minute_buffer, sizeof("00:00"), "%I:%M", tick_time);
    strftime(s_timeof_day, sizeof(s_timeof_day), "%p", tick_time);
  }
  strftime(s_year, sizeof(s_year), "%Y.%j", tick_time);
  strftime(s_month, sizeof(s_month), "%B %e", tick_time);
  strftime(s_weekday, sizeof(s_weekday), "%A", tick_time);
  strftime(second_buffer, sizeof("00"), "%S", tick_time);
  
  text_layer_set_text(s_timeof_day_layer, s_timeof_day);
  text_layer_set_text(s_hour_layer, minute_buffer);
  text_layer_set_text(s_second_layer, second_buffer);
  text_layer_set_text(s_year_layer, s_year);
  text_layer_set_text(s_month_layer, s_month);
  text_layer_set_text(s_weekday_layer, s_weekday);
}

static void battery_handler(BatteryChargeState charge_state){
  static char s_battery_percent[16];
  
  if (charge_state.is_charging){
    snprintf(s_battery_percent, sizeof(s_battery_percent), "----");
  }
  else{
    snprintf(s_battery_percent, sizeof(s_battery_percent), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(s_battery_layer, s_battery_percent);
}

static void main_window_load(Window *window){
  //bitmap background
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BRADWATCH_BACK);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  
  //fonts loader
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_SEVEN_38));
  s_second_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_SEVEN_14));
  
  //text layers
  s_hour_layer = text_layer_create(GRect(28, 47, 85, 41));
  s_second_layer = text_layer_create(GRect(61, 91, 23, 14));
  s_battery_layer = text_layer_create(GRect(15, 3, 31, 15));
  s_timeof_day_layer = text_layer_create(GRect(115, 3, 15, 15));
  s_year_layer = text_layer_create(GRect(10, 143, 124, 18));
  s_month_layer = text_layer_create(GRect(10, 122, 124, 18));
  s_weekday_layer = text_layer_create(GRect(48, 3, 65, 15));
  
  //text layer colors
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_second_layer, GColorBlack);
  text_layer_set_background_color(s_second_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorBlack);
  
  //text layer fonts
  text_layer_set_font(s_hour_layer, s_time_font);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  text_layer_set_font(s_second_layer, s_second_font);
  text_layer_set_text_alignment(s_second_layer, GTextAlignmentCenter);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
  text_layer_set_font(s_timeof_day_layer, s_second_font);
  text_layer_set_text_alignment(s_timeof_day_layer, GTextAlignmentCenter);
  text_layer_set_font(s_year_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_year_layer, GTextAlignmentCenter);
  text_layer_set_font(s_month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_month_layer, GTextAlignmentCenter);
  text_layer_set_font(s_weekday_layer,s_second_font);
  text_layer_set_text_alignment(s_weekday_layer, GTextAlignmentCenter);
  
  //text layer is child of main window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_second_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timeof_day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_year_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weekday_layer));
}

static void main_window_unload(Window *window){
  //fonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_second_font);
  
  //bitmaps
  gbitmap_destroy(s_background_bitmap);
  
  //bitmap layers
  bitmap_layer_destroy(s_bitmap_layer);
  
  //text layers
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_second_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_timeof_day_layer);
  text_layer_destroy(s_year_layer);
  text_layer_destroy(s_month_layer);
  text_layer_destroy(s_weekday_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  //what to do every second
  update_time();
}

static void init() {
  s_window = window_create();
  
  
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = main_window_load, //when loaded, call:
    .unload = main_window_unload //when unloading, call:
  });
  
  window_stack_push(s_window, true);
  update_time(); //to stop "flickering"
  battery_handler(battery_state_service_peek()); //every time you start watch, find battery %
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); //every second, call tick_handler()
  battery_state_service_subscribe(battery_handler); //every time battery changes, call battery_handler()
}

static void deinit() {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
