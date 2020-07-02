#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUM,
    _MV
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  BACKLIT,
  RGBRST,
  NUM,
  MOVE,
  CAD,
  NIGHT,
  JEEZE,
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

bool oled_enabled=true;

//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)
#define QC_CAD LCTL(LALT(KC_DEL))
#define QC_CSFT MT(MOD_RSFT,KC_CAPS)
#define QC_NPQT LT(_NUM,KC_QUOT)
#define RAISER LM(_RAISE,MOD_RSFT)
#define QC_DKL C(G(KC_LEFT))
#define QC_DKR C(G(KC_RGHT))
#if HELIX_ROWS == 4

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT( \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
      KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, QC_NPQT, \
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, QC_CSFT, \
      MO(_MV), KC_LCTL, KC_LALT, KC_NO,  KC_LGUI, RAISER,   KC_SPC,   KC_ENT, RAISE,   KC_NO,   MO(_MV), KC_RALT, QC_DKL, QC_DKR \
      ),
 
  [_RAISE] = LAYOUT( \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
      _______,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
      _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,                    KC_F12,  _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
      ), 
 
  [_NUM] = LAYOUT( \
      _______, _______, _______, _______, _______, _______,                   _______, KC_P7, KC_P8,   KC_P9, KC_PSLS, _______, \
      _______, _______, _______, _______, _______, _______,                   _______, KC_P4, KC_P5,   KC_P6, KC_PAST, _______, \
      _______, _______, _______, _______, _______, _______,                   _______, KC_P1, KC_P2,   KC_P3, KC_PMNS, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P0, KC_P0, KC_PDOT, KC_PPLS, _______ \
      ),

  [_MV] =  LAYOUT( \
      NIGHT,   RGB_HUI, RGB_SAI, RGB_VAI, _______, RESET,                      _______, KC_PGUP, KC_UP,   KC_HOME,  KC_INS, _______, \
      KC_DEL,  RGB_HUD, RGB_SAD, RGB_VAD, _______, RGB_TOG,                    _______, KC_LEFT, _______, KC_RGHT, _______, _______,  \
      _______, RGB_M_P, RGB_M_R, RGB_M_SN, RGB_M_G, _______,                   _______, KC_PGDOWN, KC_DOWN, KC_END, KC_APP, _______, \
      _______, RGB_M_B, RGB_M_SW, RGB_M_K, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
      )
};

#else
#error "undefined keymaps"
#endif



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case MOVE:
      if (record->event.pressed) {
        layer_on(_MV);
      } else {
        layer_off(_MV);
      }
      return false;
      break;
    case NIGHT:
     if (record->event.pressed) {
         rgblight_toggle();
         oled_enabled = !oled_enabled;
      }
      return false;
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    #ifdef RGBLIGHT_ENABLE
    #endif

}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_master) {
    return OLED_ROTATION_270;
  } else {
    return OLED_ROTATION_270;
  }
}
//logo swapped to only use what it needs
void render_helix_logo(void) {
  static const char PROGMEM helix_logo[4][6] = {
    {0x84, 0x85, 0x86, 0x87, 0x88,0},
    {0xa4, 0xa5, 0xa6, 0xa7, 0xa8,0},
    {0xc4, 0xc5, 0xc6, 0xc7, 0xc8,0} };
  
    oled_write_P(helix_logo[0], false);
    oled_write_P(helix_logo[1], false);
    oled_write_P(helix_logo[2], false);
    oled_write_P(PSTR("     "), false);
    oled_write_P(PSTR("M.A.D"), false);
}


void render_status(void) {
  uint8_t modifiers = get_mods();
  uint8_t one_shot = get_oneshot_mods();

  oled_write_P(PSTR("Layer"), false);
  switch (biton32(layer_state)) {
    case 0:
      oled_write_P(PSTR("Base "), false);
      break;
    case _RAISE:
      oled_write_P( (modifiers & MOD_MASK_SHIFT || one_shot & MOD_MASK_SHIFT) ? PSTR("Lower") : PSTR("Raise"), false);
      break;
    case _LOWER:
      oled_write_P(PSTR("Lower"), false);
      break;
    case _ADJUST:
      oled_write_P(PSTR("Adjst"), false);
      break;
    case _NUM:
      oled_write_P(PSTR("Num  "), false);
      break;
    case _MV:
      oled_write_P(PSTR("Move "), false);
      break;
    default:
      oled_write_P(PSTR("Unkn "), false);
      break;
  }



  oled_write_P(PSTR("Mods:"), false);
  oled_write_P( (modifiers & MOD_MASK_CTRL  || one_shot & MOD_MASK_CTRL ) ? PSTR(" CTL ") : PSTR("     "), false);
  oled_write_P( (modifiers & MOD_MASK_GUI   || one_shot & MOD_MASK_GUI  ) ? PSTR(" GUI ") : PSTR("     "), false);
  oled_write_P( (modifiers & MOD_MASK_ALT   || one_shot & MOD_MASK_ALT  ) ? PSTR(" ALT ") : PSTR("     "), false);
  oled_write_P( (modifiers & MOD_MASK_SHIFT || one_shot & MOD_MASK_SHIFT) ? PSTR(" SFT ") : PSTR("     "), false);


  oled_write_P(PSTR("BTMGK"), false);
  static const char PROGMEM mode_logo[4][6] = {
    {0x84, 0x85, 0x86, 0x87, 0x88,0},
    {0xa4, 0xa5, 0xa6, 0xa7, 0xa8,0},
    {0xc4, 0xc5, 0xc6, 0xc7, 0xc8,0} };

    oled_write_P(mode_logo[0], false);
    oled_write_P(mode_logo[1], false);
    oled_write_P(mode_logo[2], false);


  

  uint8_t led_usb_state = host_keyboard_leds();
  oled_write_P(PSTR("Lock:"), false);
  oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK)    ? PSTR(" NUM ") : PSTR("     "), false);
  oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK)   ? PSTR(" CAPS") : PSTR("     "), false);
  oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR(" SCRL") : PSTR("     "), false);
}

void oled_task_user(void) {
  if (!oled_enabled) {
    oled_off();
    return;
  } else{
    oled_on();
  if(is_master) {
    render_status();     // Renders the current keyboard state (layer, lock, caps, scroll, etc)
  } else {
    render_helix_logo();
    oled_scroll_left();  // Turns on scrolling
  }
}}

#endif
#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif
/*  drashna's suggested oled turnoff switch
bool oled_enabled;

void oled_task_user(void) {
  if (!oled_enabled) {
    oled_off();
    return;
  }
  // normal stuff
}
*/