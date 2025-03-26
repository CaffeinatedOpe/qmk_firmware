// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

typedef enum {
	TD_NONE,
	TD_UNKNOWN,
	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,
	TD_DOUBLE_TAP
} td_state_t;

typedef struct {
	bool is_press_action;
	td_state_t state;
} td_tap_t;

enum {
	SUPER_SHIFT,
	BK_ESC,
	RESET
};

td_state_t cur_dance(tap_dance_state_t *state) {
	if (state->count == 1) {
			if (!state->pressed) return TD_SINGLE_TAP;
			else return TD_SINGLE_HOLD;
	} else if (state->count == 2) return TD_DOUBLE_TAP;
	else return TD_UNKNOWN;
}

static td_tap_t ss_tap_state = {
	.is_press_action = true,
	.state = TD_NONE
};

void ss_finished(tap_dance_state_t *state, void *user_data) {
	ss_tap_state.state = cur_dance(state);
	switch (ss_tap_state.state) {
			case TD_SINGLE_TAP:
					tap_code(KC_SPC);
					break;
			case TD_SINGLE_HOLD:
					register_code(KC_LSFT);
					break;
			case TD_DOUBLE_TAP:
					tap_code(KC_ENTER);
					break;
			default:
					break;
	}
}

void ss_reset(tap_dance_state_t *state, void *user_data) {
	if (ss_tap_state.state == TD_SINGLE_HOLD) {
			unregister_code(KC_LSFT);
	}
	ss_tap_state.state = TD_NONE;
}

void safe_reset(tap_dance_state_t *state, void *user_data) {
	if (state->count >= 3) {
		reset_keyboard();
		reset_tap_dance(state);
	}
}

tap_dance_action_t tap_dance_actions[] = {
	[SUPER_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ss_finished, ss_reset),
	[BK_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_BSPC, KC_ESC),
	[RESET] = ACTION_TAP_DANCE_FN(safe_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(
//,-----------------------------------------------------.                                 ,-----------------------------------------------------.
		 KC_GRV,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                      KC_J,    KC_L,    KC_U,    KC_Y,   KC_SCLN,  KC_LBRC,
//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
		KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                                      KC_M,    KC_N,    KC_E,    KC_I, KC_O, KC_RBRC,
//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
		KC_ESC,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                                      KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH,  KC_QUOT,
//|--------+--------+--------+--------+--------+--------+--------|               |--------+--------+--------+--------+--------+--------+--------|
					LGUI_T(KC_TAB), LCTL_T(KC_HOME), TD(SUPER_SHIFT),          	LCTL_T(KC_BSPC),   LALT_T(KC_END), TO(1)
																		//`--------------------------'               `--------------------------'

),
[1] = LAYOUT_split_3x6_3(
	//,-----------------------------------------------------.                                 ,-----------------------------------------------------.
	KC_NO,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_NO,                                      KC_NO,    KC_7,    KC_8,    KC_9,   KC_NO,  KC_MINUS,
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
	KC_NO,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_NO,                                      KC_NO,    KC_4,    KC_5,    KC_6, KC_NO, KC_EQUAL,
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
	KC_NO,    KC_F9,    KC_F10,    KC_F11,    KC_F12,    KC_NO,                                      KC_0,    KC_1, KC_2,  KC_3, KC_NO,  KC_BSLS,
	//|--------+--------+--------+--------+--------+--------+--------|               |--------+--------+--------+--------+--------+--------+--------|
										TO(0), LCTL_T(KC_HOME), TD(SUPER_SHIFT),          KC_BSPC,   LALT_T(KC_END), TO(2)
	//`--------------------------'               `--------------------------'
),
[2] = LAYOUT_split_3x6_3(
	//,-----------------------------------------------------.                                 ,-----------------------------------------------------.
	KC_NO,    KC_NO,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_VOLU,                                      KC_PGUP,    KC_NO,    KC_UP,    KC_NO,   KC_NO,  TD(RESET),
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
	KC_NO,    KC_NO,   KC_LALT,    KC_LGUI,    KC_LCTL,    KC_MUTE,                                      KC_DEL,    KC_LEFT,    KC_DOWN,    KC_RIGHT, KC_NO, KC_NO,
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
	KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_VOLD,                                      KC_PGDN,    KC_NO, KC_NO,  KC_NO, KC_NO,  KC_NO,
	//|--------+--------+--------+--------+--------+--------+--------|               |--------+--------+--------+--------+--------+--------+--------|
										TO(0), LCTL_T(KC_HOME), TD(SUPER_SHIFT),          KC_BSPC,   LALT_T(KC_END), KC_NO
	//`--------------------------'               `--------------------------'
),
[3] = LAYOUT_split_3x6_3(
	//,-----------------------------------------------------.                                 ,-----------------------------------------------------.
			 KC_GRV,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                      KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
			KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                      KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_RBRC,
	//|--------+--------+--------+--------+--------+--------|                                 |--------+--------+--------+--------+--------+--------|
			KC_TAB,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                      KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_QUOT,
	//|--------+--------+--------+--------+--------+--------+--------|               |--------+--------+--------+--------+--------+--------+--------|
										TO(0), LCTL_T(KC_HOME), TD(SUPER_SHIFT),          KC_BSPC,   LALT_T(KC_END), TO(3)
	//`--------------------------'               `--------------------------'
	
	)
};