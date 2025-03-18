// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

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

static const char* messages[1][4] = {
	{
		"YOU'LL FIND MY", "SMOKING BODY", "HUNG IN WIRES", "OVERHEAD"
	}
};

void start_cube(void) {
	if (cube != NULL) {
		cube_anim = qp_animate(display, 0, 0, cube);
	}
}

void stop_cube(void) {
	qp_stop_animation(cube_anim);
	if (is_keyboard_left()) {
		qp_clear(display);
		qp_drawtext(display, 0, 0, font, messages[0][0]);
		qp_drawtext(display, 0, 16, font, messages[0][1]);
	}
	else {
		qp_clear(display);
		qp_drawtext(display, 0, 0, font, messages[0][2]);
		qp_drawtext(display, 0, 16, font, messages[0][3]);
	}
}

void check_oled_timeout(void) {
	if (!is_oled_timeout && last_input_activity_elapsed() > 000)
	{
		start_cube();
		is_oled_timeout = true;
	}
	if (last_input_activity_elapsed() < 30000)
	{
		is_oled_timeout = false;
		stop_cube();
	}
}

void housekeeping_task_user(void) {
	check_oled_timeout();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
			case LT(0,KC_X):
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(
//,-----------------------------------------------------.                                 ,-----------------------------------------------------.
		 KC_GRV,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                      KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,
//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
		KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                      KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_RBRC,
//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
		KC_TAB,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                      KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_QUOT,
//|--------+--------+--------+--------+--------+--------+--------|               |--------+--------+--------+--------+--------+--------+--------|
																		KC_LGUI,   KC_TAB, LSFT_T(KC_SPC),          KC_BSPC,   MO(2), KC_RALT
																		//`--------------------------'               `--------------------------'

)
};