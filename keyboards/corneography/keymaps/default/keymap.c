// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <qp.h>

static painter_device_t display;
void keyboard_post_init_kb(void) {
	debug_enable = true;
	display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
	qp_init(display, QP_ROTATION_0);   // Initialise the display
	qp_circle(display, 64, 16, 8, 255, 255, 255, true);
	qp_flush(display);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
	if (record->event.pressed) {
		qp_circle(display, 64, 16, 8, 255, 255, 255, true);
		qp_flush(display);
	}
	return true;
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
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                               KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                            KC_LGUI, KC_BSPC, KC_SPC,           KC_SPC,  KC_ENT,  KC_RALT
    )
};
