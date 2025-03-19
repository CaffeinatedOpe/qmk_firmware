#include QMK_KEYBOARD_H
#include <qp.h>
#include "images/cube.qgf.h"
#include "fonts/pixellari.qff.h"

static painter_font_handle_t font;
static painter_device_t display;
static painter_image_handle_t cube;
static deferred_token cube_anim;

static void check_oled_timeout(void);
bool is_oled_timeout = false;
int val;

static const char* messages[10][4] = {
	{"YOU'LL FIND MY", "SMOKING BODY", "HUNG IN WIRES", "OVERHEAD"},
	{"CLOCKWORK", "WIND UP", "ALL", "ALONE"},
	{"DIAL", "NUMBER", "WATCH YOUR", "TONE"},
	{"MORE YOU", "STOP THE", "MORE I", "GO"},
	{"DO YOUR", "BEST, I'LL", "DO ONE", "BETTER"},
	{"BRUISES ON", "MY NECK", "JUST A DOLL", "OF FLESH"},
	{"I'M AT YOUR", "BARK AND COLLAR", "JUST FORGET I'M" "SOMEONE'S DAUGHTER"},
	{"CLOCKWORK", "WIND UP", "ALL", "ALONE"},
	{"CLOCKWORK", "WIND UP", "ALL", "ALONE"},
	{"CLOCKWORK", "WIND UP", "ALL", "ALONE"},
	
};

void start_cube(void) {
	if (cube != NULL) {
		cube_anim = qp_animate(display, 0, 0, cube);
	}
}

void load_lyric(void) {
	int index = timer_read() % 10;
	qp_clear(display);
	if (is_keyboard_left()) {
		qp_drawtext(display, 0, 0, font, messages[index][0]);
		qp_drawtext(display, 0, 16, font, messages[index][1]);
	}
	else {
		qp_drawtext(display, 0, 0, font, messages[index][2]);
		qp_drawtext(display, 0, 16, font, messages[index][3]);
	}
}

void stop_cube(void) {
	qp_stop_animation(cube_anim);
	load_lyric();
}

void check_oled_timeout(void) {
	if (!is_oled_timeout && last_input_activity_elapsed() > 30000)
	{
		start_cube();
		is_oled_timeout = true;
	}
	if (is_oled_timeout && last_input_activity_elapsed() < 30000)
	{
		is_oled_timeout = false;
		stop_cube();
	}
}

void housekeeping_task_kb(void) {
	check_oled_timeout();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
			case LT(0, KC_X):
					if (!record->tap.count && record->event.pressed) {
							tap_code16(C(KC_X));
							return false;
					}
					return true;
				}
	return true;
}

void keyboard_post_init_kb(void) {
	display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
	cube = qp_load_image_mem(gfx_cube);
	if (is_keyboard_left()) {
		qp_init(display, QP_ROTATION_0);
		font = qp_load_font_mem(font_pixellari);
		qp_drawtext(display, 0, 0, font, messages[0][0]);
		qp_drawtext(display, 0, 16, font, messages[0][1]);
	}
	else {
		qp_init(display, QP_ROTATION_180);
		font = qp_load_font_mem(font_pixellari);
		qp_drawtext(display, 0, 0, font, messages[0][2]);
		qp_drawtext(display, 0, 16, font, messages[0][3]);
	}


	//z2 = qp_load_image_mem(gfx_zerotwo);
	//if (z2 != NULL) {
	//	z2anim = qp_animate(display, 0, 0, z2);
	//}
	qp_flush(display);
}