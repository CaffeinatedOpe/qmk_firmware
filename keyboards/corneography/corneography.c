#include QMK_KEYBOARD_H
#include "quantum.h"
#include <qp.h>
#include "images/cube.qgf.h"
#include "fonts/pixellari.qff.h"
#include "transactions.h"


static painter_font_handle_t font;
static painter_device_t display;
static painter_image_handle_t cube;
static deferred_token cube_anim;

static void check_oled_timeout(void);
bool is_oled_timeout = false;
int screen_state;
int new_screen;

static const char* messages[10][4] = {
	{"YOU'LL FIND MY", "SMOKING BODY", "HUNG IN WIRES", "OVERHEAD"},
	{"CLOCKWORK", "WIND UP", "ALL", "ALONE"},
	{"DIAL", "NUMBER", "WATCH YOUR", "TONE"},
	{"MORE YOU", "STOP THE", "MORE I", "GO"},
	{"DO YOUR", "BEST, I'LL", "DO ONE", "BETTER"},
	{"BRUISES ON", "MY NECK", "JUST A DOLL", "OF FLESH"},
	{"I'M AT", "YOUR", "BARK AND", "COLLAR"},
	{"JUST", "FORGET I'M", "SOMEONE'S", "DAUGHTER"},
	{"FOLLOW THE", "WHITE RABBIT", "FOLLOW THE", "WHITE RABBIT"},
	{"DON'T BE", "BITTEN TWICE", "DON'T BE", "BITTEN TWICE"},	
};

typedef struct _master_to_slave_t {
	int m2s_data;
} master_to_slave_t;

typedef struct _slave_to_master_t {
	int s2m_data;
} slave_to_master_t;

void load_lyric(int index) {
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

void random_lyric(void) {
	qp_clear(display);
    int index = timer_read() % 10;
    if (is_keyboard_master()) {
        load_lyric(index);
        screen_state          = index;
        master_to_slave_t m2s = {index};
        slave_to_master_t s2m = {42};
        transaction_rpc_exec(KEYBOARD_SYNC_RANDINT, sizeof(m2s), &m2s, sizeof(s2m), &s2m);
    }
}

void start_cube(void) {
	if (cube != NULL) {
		cube_anim = qp_animate(display, 0, 0, cube);
	}
	screen_state = -1;
}

void stop_cube(void) {
    qp_stop_animation(cube_anim);
    screen_state = 0;
    random_lyric();
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


void kb_sync_randint_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
	const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
	new_screen = m2s->m2s_data;
}

void housekeeping_task_kb(void) {
	check_oled_timeout();
	if(!is_keyboard_master() && (new_screen  != screen_state) && (screen_state != -1)) { 
		screen_state = new_screen;
		load_lyric(screen_state);
	}
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

layer_state_t layer_state_set_kb(layer_state_t state) {
	random_lyric();
	return state;
}

void keyboard_post_init_kb(void) {
	transaction_register_rpc(KEYBOARD_SYNC_RANDINT, kb_sync_randint_slave_handler);
	
	display = qp_sh1106_make_i2c_device(128, 32, 0x3c);
	cube = qp_load_image_mem(gfx_cube);
	if (is_keyboard_left()) {
		qp_init(display, QP_ROTATION_0);
		font = qp_load_font_mem(font_pixellari);
		load_lyric(0);
	}
	else {
		qp_init(display, QP_ROTATION_180);
		font = qp_load_font_mem(font_pixellari);
		load_lyric(0);
	}
	qp_flush(display);
}