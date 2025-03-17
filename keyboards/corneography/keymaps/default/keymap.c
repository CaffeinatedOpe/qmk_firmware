// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <qp.h>
#include "images/zerotwo.qgf.h"
#include "images/token.qgf.h"
#include "images/PUSH_UR_T3MPRR.qgf.h"
#include "fonts/pixellari.qff.h"

static painter_image_handle_t image;
static painter_font_handle_t font;
static painter_device_t display;

/*
static uint32_t key_timer;
static void refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool is_rgb_timeout = false;         // store if RGB has timed out or not in a boolean

void refresh_rgb(void) {
	key_timer = timer_read32(); // store time of last refresh
	if (is_oled_timeout)
	{
		is_rgb_timeout = false;
		rgblight_wakeup();
	}
}
void check_rgb_timeout(void) {
	if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT) // check if RGB has already timeout and if enough time has passed
	{
		rgblight_suspend();
		is_rgb_timeout = true;
	}
}

void housekeeping_task_user(void) {
	check_oled_timeout();
}

void post_process_record_user(uint16_t keycode, keyrecord_t* record) {
	if (record->event.pressed)
		refresh_oled();
}*/

void keyboard_post_init_kb(void) {
	display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
	if (is_keyboard_left()) {
		qp_init(display, QP_ROTATION_0);
		font = qp_load_font_mem(font_pixellari);
		static const char* line0 = "HUNG IN WIRES";
		qp_drawtext(display, 0, 0, font, line0);
		static const char* line1 = "OVERHEAD";
		qp_drawtext(display, 0, 16, font, line1);
	}
	else {
		qp_init(display, QP_ROTATION_90);
		image = qp_load_image_mem(gfx_token);
		if (image != NULL) {
			qp_drawimage(display, 0, 0, image);
		}
	}


	//z2 = qp_load_image_mem(gfx_zerotwo);
	//if (z2 != NULL) {
	//	z2anim = qp_animate(display, 0, 0, z2);
	//}
	qp_flush(display);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/*
	 * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
	 * │Tab│ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │Bsp│
	 * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
	 * │Ctl│ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │ ' │
	 * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
	 * │Sft│ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │Sft│
	 * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
	 *               ┌───┐                   ┌───┐
	 *               │GUI├───┐           ┌───┤Alt│
	 *               └───┤Bsp├───┐   ┌───┤Ent├───┘
	 *                   └───┤   │   │   ├───┘
	 *                       └───┘   └───┘
	 */
	[0] = LAYOUT_split_3x6_3(
		//,-----------------------------------------------------.                    ,-----------------------------------------------------.
				 KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
		//|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
				KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
		//|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
				KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
		//|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
																						KC_LGUI,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
																				//`--------------------------'  `--------------------------'
	)
};
