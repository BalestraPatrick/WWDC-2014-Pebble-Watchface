/*

 WWDC 2014 Pebble Watchface
 Developed by Patrick Balestra
 Part of the code for the countdown by Kernel Sanders: https://github.com/kernel-sanders/Time-Until-Pebble
 
*/

#include <pebble.h>

#define COUNTDOWN 1401728400 // WWDC Date in unix format (GMT+0 timezone)

Window *window;
TextLayer *text_time_layer;
TextLayer *text_date_layer;
TextLayer *text_countdown_layer;

BitmapLayer *logo_image_layer;
BitmapLayer *text_image_layer;
BitmapLayer *tim_cook_image_layer;

GBitmap *logo_image;
GBitmap *text_image;
GBitmap *tim_cook_image;

GFont dolce_vita_font;

int current_timezone_offset = 0;

// Functions

void in_received_handler(DictionaryIterator *received, void *context) {
    Tuple *offsetTuple = dict_find(received, 1);
    current_timezone_offset = offsetTuple->value->uint8;
}

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
	static char time_text[] = "00:00";
    
	strftime(time_text, sizeof(time_text), "%R", tick_time);
	text_layer_set_text(text_time_layer, time_text);
	
	unsigned int unix_time;
	unix_time = ((-current_timezone_offset)*3600) + /* time zone offset */
    + tick_time->tm_sec /* start with seconds */
    + tick_time->tm_min*60 /* add minutes */
    + tick_time->tm_hour*3600 /* add hours */
    + tick_time->tm_yday*86400 /* add days */
    + (tick_time->tm_year-70)*31536000 /* add years since 1970 */
    + ((tick_time->tm_year-69)/4)*86400 /* add a day after leap years, starting in 1973 */
    - ((tick_time->tm_year-1)/100)*86400 /* remove a leap day every 100 years, starting in 2001 */
    + ((tick_time->tm_year+299)/400)*86400; /* add a leap day back every 400 years, starting in 2001*/
	
	int days;
	int hours;
	int minutes;
    int seconds;
    
    static char countdownString[] = "XXX : XX : XX : XX";
	
    int timeRemaining = COUNTDOWN - (int)unix_time;
	days = (timeRemaining/86400);
	hours = (timeRemaining/3600) - days*24;
	minutes = (timeRemaining/60) - days*1440 - hours*60;
	seconds = (timeRemaining) - days*86400 - hours*3600 - minutes*60;
    
    if (days <= 0 && hours <= 0 && minutes <= 0) {
        snprintf(countdownString, sizeof(countdownString), "%d", seconds);
    } else if (days <= 0 && hours <= 0) {
        snprintf(countdownString, sizeof(countdownString), "%d %d", minutes, seconds);
    } else if (days <= 0) {
        snprintf(countdownString, sizeof(countdownString), "%d %d %d", hours, minutes, seconds);
    } else {
        snprintf(countdownString, sizeof(countdownString), "%d %d %d %d", days, hours, minutes, seconds);
    }

    text_layer_set_text(text_countdown_layer, countdownString);
    
    // Guess this feature *__*
    if (days <= 0 && hours <= 0 && minutes <= 0 && seconds <= 0) {
        layer_set_hidden(bitmap_layer_get_layer(logo_image_layer), true);
        layer_set_hidden(bitmap_layer_get_layer(text_image_layer), true);
        layer_set_hidden(bitmap_layer_get_layer(tim_cook_image_layer), false);
    }

}

void handle_init(void) {
	window = window_create();
	window_stack_push(window, true);
	window_set_background_color(window, GColorBlack);
    Layer *window_layer = window_get_root_layer(window);
    
    // Set up AppMessage to receive timezone offset
    app_message_register_inbox_received(in_received_handler);
    app_message_open(64, 64);
    
    dolce_vita_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GEOSANS_LIGHT_26));
	
	// Current Time
	text_time_layer = text_layer_create(GRect(0, 114, 144, 43));
	text_layer_set_text_color(text_time_layer, GColorWhite);
	text_layer_set_background_color(text_time_layer, GColorClear);
	text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
	
    // Apple Logo
    logo_image = gbitmap_create_with_resource(RESOURCE_ID_WWDC14);
    logo_image_layer = bitmap_layer_create(GRect(0, 0, 144, 60));
    bitmap_layer_set_bitmap(logo_image_layer, logo_image);
    layer_add_child(window_layer, bitmap_layer_get_layer(logo_image_layer));
    
    // WWDC14 Text
    text_image = gbitmap_create_with_resource(RESOURCE_ID_WWDC14TEXT);
    text_image_layer = bitmap_layer_create(GRect(0, 60, 144, 30));
    bitmap_layer_set_bitmap(text_image_layer, text_image);
    layer_add_child(window_layer, bitmap_layer_get_layer(text_image_layer));
    
    // Countdown
	text_countdown_layer = text_layer_create(GRect(0, 88, 144, 33));
	text_layer_set_text_color(text_countdown_layer, GColorWhite);
	text_layer_set_background_color(text_countdown_layer, GColorBlack);
	text_layer_set_font(text_countdown_layer, dolce_vita_font);
	text_layer_set_text_alignment(text_countdown_layer, GTextAlignmentCenter);
    text_layer_set_background_color(text_countdown_layer, GColorClear);
	layer_add_child(window_layer, text_layer_get_layer(text_countdown_layer));
    
    tim_cook_image = gbitmap_create_with_resource(RESOURCE_ID_TIMCOOK);
    tim_cook_image_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    bitmap_layer_set_bitmap(tim_cook_image_layer, tim_cook_image);
    layer_add_child(window_layer, bitmap_layer_get_layer(tim_cook_image_layer));
	layer_set_hidden(bitmap_layer_get_layer(tim_cook_image_layer), true);
    
	tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
    
}

void handle_deinit(void) {
	text_layer_destroy(text_time_layer);
	text_layer_destroy(text_date_layer);
	text_layer_destroy(text_countdown_layer);
    bitmap_layer_destroy(text_image_layer);
    bitmap_layer_destroy(logo_image_layer);
	window_destroy(window);
}

int main(void) {
    handle_init();
    app_event_loop();
    handle_deinit();
}