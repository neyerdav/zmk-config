/* Custom nice!view battery screen for ZMK v0.3.0. */
#include <string.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <lvgl.h>
#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/display/status_screen.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/split/central.h>

#define SIZE 68
static lv_obj_t *canvas;
static lv_color_t pixels[SIZE * SIZE];
static lv_color_t rotated[SIZE * SIZE];

struct battery_state {
    uint8_t left;
    uint8_t right;
    bool right_known;
};

static struct battery_state get_state(const zmk_event_t *event) {
    static bool right_known;
    const struct zmk_peripheral_battery_state_changed *peripheral =
        event ? as_zmk_peripheral_battery_state_changed(event) : NULL;
    uint8_t right = 0;
    zmk_split_central_get_peripheral_battery_level(0, &right);
    if ((peripheral && peripheral->source == 0) || right > 0) {
        right_known = true;
    }
    return (struct battery_state){.left = zmk_battery_state_of_charge(),
                                  .right = right, .right_known = right_known};
}

static void update_screen(struct battery_state state) {
    char text[32];
    if (state.right_known) {
        snprintf(text, sizeof(text), "L %u%%\nR %u%%", state.left, state.right);
    } else {
        snprintf(text, sizeof(text), "L %u%%\nR --", state.left);
    }

    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);
    lv_draw_label_dsc_t label;
    lv_draw_label_dsc_init(&label);
    label.color = lv_color_white();
    label.font = &lv_font_montserrat_16;
    label.align = LV_TEXT_ALIGN_CENTER;
    lv_canvas_draw_text(canvas, 0, 12, SIZE, &label, text);

    /* nice!view is mounted sideways; match the upstream screen's rotation. */
    memcpy(rotated, pixels, sizeof(rotated));
    lv_img_dsc_t image = {0};
    image.data = (void *)rotated;
    image.header.cf = LV_IMG_CF_TRUE_COLOR;
    image.header.w = SIZE;
    image.header.h = SIZE;
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);
    lv_canvas_transform(canvas, &image, 900, LV_IMG_ZOOM_NONE, -1, 0, SIZE / 2, SIZE / 2, true);
}

ZMK_DISPLAY_WIDGET_LISTENER(both_batteries, struct battery_state, update_screen, get_state)
ZMK_SUBSCRIPTION(both_batteries, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(both_batteries, zmk_peripheral_battery_state_changed);

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    canvas = lv_canvas_create(screen);
    lv_obj_align(canvas, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_canvas_set_buffer(canvas, pixels, SIZE, SIZE, LV_IMG_CF_TRUE_COLOR);
    both_batteries_init();
    return screen;
}
