#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fs-uae.h"
#include <fs/emu.h>
#include <fs/emu/actions.h>
#include <uae/uae.h>

#define ALL -1
#define MOD FS_ML_KEY_MOD_SPECIAL

static fs_emu_key_translation g_default_keymap[] = {
    { FS_ML_KEY_ESCAPE,       ALL, INPUTEVENT_KEY_ESC },

    { FS_ML_KEY_F1,           ALL, INPUTEVENT_KEY_F1 },
    { FS_ML_KEY_F2,           ALL, INPUTEVENT_KEY_F2 },
    { FS_ML_KEY_F3,           ALL, INPUTEVENT_KEY_F3 },
    { FS_ML_KEY_F4,           ALL, INPUTEVENT_KEY_F4 },
    { FS_ML_KEY_F5,           ALL, INPUTEVENT_KEY_F5 },
    { FS_ML_KEY_F6,           ALL, INPUTEVENT_KEY_F6 },
    { FS_ML_KEY_F7,           ALL, INPUTEVENT_KEY_F7 },
    { FS_ML_KEY_F8,           ALL, INPUTEVENT_KEY_F8 },
    { FS_ML_KEY_F9,           ALL, INPUTEVENT_KEY_F9 },
    { FS_ML_KEY_F10,          ALL, INPUTEVENT_KEY_F10 },

    { FS_ML_KEY_1,            ALL, INPUTEVENT_KEY_1 },
    { FS_ML_KEY_2,            ALL, INPUTEVENT_KEY_2 },
    { FS_ML_KEY_3,            ALL, INPUTEVENT_KEY_3 },
    { FS_ML_KEY_4,            ALL, INPUTEVENT_KEY_4 },
    { FS_ML_KEY_5,            ALL, INPUTEVENT_KEY_5 },
    { FS_ML_KEY_6,            ALL, INPUTEVENT_KEY_6 },
    { FS_ML_KEY_7,            ALL, INPUTEVENT_KEY_7 },
    { FS_ML_KEY_8,            ALL, INPUTEVENT_KEY_8 },
    { FS_ML_KEY_9,            ALL, INPUTEVENT_KEY_9 },
    { FS_ML_KEY_0,            ALL, INPUTEVENT_KEY_0 },

    { FS_ML_KEY_TAB,          ALL, INPUTEVENT_KEY_TAB },

    { FS_ML_KEY_A,            ALL, INPUTEVENT_KEY_A },
    { FS_ML_KEY_B,            ALL, INPUTEVENT_KEY_B },
    { FS_ML_KEY_C,            ALL, INPUTEVENT_KEY_C },
    { FS_ML_KEY_D,            ALL, INPUTEVENT_KEY_D },
    { FS_ML_KEY_E,            ALL, INPUTEVENT_KEY_E },
    { FS_ML_KEY_F,            ALL, INPUTEVENT_KEY_F },
    { FS_ML_KEY_G,            ALL, INPUTEVENT_KEY_G },
    { FS_ML_KEY_H,            ALL, INPUTEVENT_KEY_H },
    { FS_ML_KEY_I,            ALL, INPUTEVENT_KEY_I },
    { FS_ML_KEY_J,            ALL, INPUTEVENT_KEY_J },
    { FS_ML_KEY_K,            ALL, INPUTEVENT_KEY_K },
    { FS_ML_KEY_L,            ALL, INPUTEVENT_KEY_L },
    { FS_ML_KEY_M,            ALL, INPUTEVENT_KEY_M },
    { FS_ML_KEY_N,            ALL, INPUTEVENT_KEY_N },
    { FS_ML_KEY_O,            ALL, INPUTEVENT_KEY_O },
    { FS_ML_KEY_P,            ALL, INPUTEVENT_KEY_P },
    { FS_ML_KEY_Q,            ALL, INPUTEVENT_KEY_Q },
    { FS_ML_KEY_R,            ALL, INPUTEVENT_KEY_R },
    { FS_ML_KEY_S,            ALL, INPUTEVENT_KEY_S },
    { FS_ML_KEY_T,            ALL, INPUTEVENT_KEY_T },
    { FS_ML_KEY_U,            ALL, INPUTEVENT_KEY_U },
    { FS_ML_KEY_W,            ALL, INPUTEVENT_KEY_W },
    { FS_ML_KEY_V,            ALL, INPUTEVENT_KEY_V },
    { FS_ML_KEY_X,            ALL, INPUTEVENT_KEY_X },
    { FS_ML_KEY_Y,            ALL, INPUTEVENT_KEY_Y },
    { FS_ML_KEY_Z,            ALL, INPUTEVENT_KEY_Z },

    /* FIXME:  ID_FLAG_TOGGLE */
    { FS_ML_KEY_CAPSLOCK,     ALL, INPUTEVENT_KEY_CAPS_LOCK},

    { FS_ML_KEY_KP1,          ALL, INPUTEVENT_KEY_NP_1 },
    { FS_ML_KEY_KP2,          ALL, INPUTEVENT_KEY_NP_2 },
    { FS_ML_KEY_KP3,          ALL, INPUTEVENT_KEY_NP_3 },
    { FS_ML_KEY_KP4,          ALL, INPUTEVENT_KEY_NP_4 },
    { FS_ML_KEY_KP5,          ALL, INPUTEVENT_KEY_NP_5 },
    { FS_ML_KEY_KP6,          ALL, INPUTEVENT_KEY_NP_6 },
    { FS_ML_KEY_KP7,          ALL, INPUTEVENT_KEY_NP_7 },
    { FS_ML_KEY_KP8,          ALL, INPUTEVENT_KEY_NP_8 },
    { FS_ML_KEY_KP9,          ALL, INPUTEVENT_KEY_NP_9 },
    { FS_ML_KEY_KP0,          ALL, INPUTEVENT_KEY_NP_0 },
    { FS_ML_KEY_KP_PERIOD,    ALL, INPUTEVENT_KEY_NP_PERIOD },
    { FS_ML_KEY_KP_PLUS,      ALL, INPUTEVENT_KEY_NP_ADD },
    { FS_ML_KEY_KP_MINUS,     ALL, INPUTEVENT_KEY_NP_SUB },
    { FS_ML_KEY_KP_MULTIPLY,  ALL, INPUTEVENT_KEY_NP_MUL },
    { FS_ML_KEY_KP_DIVIDE,    ALL, INPUTEVENT_KEY_NP_DIV },
    { FS_ML_KEY_KP_ENTER,     ALL, INPUTEVENT_KEY_ENTER },

    { FS_ML_KEY_MINUS,        ALL, INPUTEVENT_KEY_SUB },
    { FS_ML_KEY_EQUALS,       ALL, INPUTEVENT_KEY_EQUALS },
    { FS_ML_KEY_BACKSPACE,    ALL, INPUTEVENT_KEY_BACKSPACE },
    { FS_ML_KEY_RETURN,       ALL, INPUTEVENT_KEY_RETURN },
    { FS_ML_KEY_SPACE,        ALL, INPUTEVENT_KEY_SPACE },

    { FS_ML_KEY_LSHIFT,       ALL, INPUTEVENT_KEY_SHIFT_LEFT },
    { FS_ML_KEY_LCTRL,        ALL, INPUTEVENT_KEY_CTRL },
    /* FIXME */
    { FS_ML_KEY_LSUPER,       ALL, INPUTEVENT_KEY_AMIGA_LEFT },
#if 0
    { FS_ML_KEY_LMETA,        ALL, INPUTEVENT_KEY_AMIGA_LEFT },
#endif
    { FS_ML_KEY_LALT,         ALL, INPUTEVENT_KEY_ALT_LEFT },
    { FS_ML_KEY_RALT,         ALL, INPUTEVENT_KEY_ALT_RIGHT },
    /* FIXME */
    { FS_ML_KEY_RSUPER,       ALL, INPUTEVENT_KEY_AMIGA_RIGHT },
#if 0
    { FS_ML_KEY_RMETA,        ALL, INPUTEVENT_KEY_AMIGA_RIGHT },
#endif
    { FS_ML_KEY_MENU,         ALL, INPUTEVENT_KEY_AMIGA_RIGHT },
#if 0
    { FS_ML_KEY_RCTRL,        ALL, INPUTEVENT_KEY_CTRL_RIGHT },
#endif
    { FS_ML_KEY_RSHIFT,       ALL, INPUTEVENT_KEY_SHIFT_RIGHT },

    { FS_ML_KEY_UP,           ALL, INPUTEVENT_KEY_CURSOR_UP },
    { FS_ML_KEY_DOWN,         ALL, INPUTEVENT_KEY_CURSOR_DOWN },
    { FS_ML_KEY_LEFT,         ALL, INPUTEVENT_KEY_CURSOR_LEFT },
    { FS_ML_KEY_RIGHT,        ALL, INPUTEVENT_KEY_CURSOR_RIGHT },

#if 1
    /* 0x2B is the number (hash) key on some international keyboards. */
    { FS_ML_KEY_INSERT,       ALL, INPUTEVENT_KEY_2B },
#else
    { FS_ML_KEY_INSERT,       ALL, INPUTEVENT_KEY_BACKSLASH },
#endif

    { FS_ML_KEY_HOME,         ALL, INPUTEVENT_KEY_NP_LPAREN },
    { FS_ML_KEY_PAGEUP,       ALL, INPUTEVENT_KEY_NP_RPAREN },

    { FS_ML_KEY_DELETE,       ALL, INPUTEVENT_KEY_DEL },
    { FS_ML_KEY_END,          ALL, INPUTEVENT_KEY_HELP },
    { FS_ML_KEY_PAGEDOWN,     ALL, INPUTEVENT_KEY_AMIGA_RIGHT },

    { FS_ML_KEY_LEFTBRACKET,  ALL, INPUTEVENT_KEY_LEFTBRACKET },
    { FS_ML_KEY_RIGHTBRACKET, ALL, INPUTEVENT_KEY_RIGHTBRACKET },
    { FS_ML_KEY_SEMICOLON,    ALL, INPUTEVENT_KEY_SEMICOLON },
    { FS_ML_KEY_QUOTE,        ALL, INPUTEVENT_KEY_SINGLEQUOTE },
    { FS_ML_KEY_BACKQUOTE,    ALL, INPUTEVENT_KEY_BACKQUOTE },

#if 1
    { FS_ML_KEY_BACKSLASH,    ALL, INPUTEVENT_KEY_BACKSLASH },
#else
    { FS_ML_KEY_BACKSLASH,    ALL, INPUTEVENT_KEY_2B },
#endif

    { FS_ML_KEY_COMMA,        ALL, INPUTEVENT_KEY_COMMA },
    { FS_ML_KEY_PERIOD,       ALL, INPUTEVENT_KEY_PERIOD },
    { FS_ML_KEY_SLASH,        ALL, INPUTEVENT_KEY_DIV },

    /* 0x30 is the key to the left of Z. */
    { FS_ML_KEY_LESS,         ALL, INPUTEVENT_KEY_30 },

    // FIXME: international key??
    //{ DIK_OEM_102, ALL, INPUTEVENT_KEY_30 },

    // FIXME:

    //{ INPUTEVENT_KEY_30, ALL, INPUTEVENT_KEY_30 },
    //{ INPUTEVENT_SPC_MASTER_VOLUME_DOWN, ALL, INPUTEVENT_SPC_MASTER_VOLUME_DOWN },
    //{ INPUTEVENT_SPC_MASTER_VOLUME_UP, ALL, INPUTEVENT_SPC_MASTER_VOLUME_UP },
    //{ INPUTEVENT_SPC_MASTER_VOLUME_MUTE, ALL, INPUTEVENT_SPC_MASTER_VOLUME_MUTE },
    //{ INPUTEVENT_KEY_70, ALL, INPUTEVENT_KEY_70 },
    //{ INPUTEVENT_KEY_71, ALL, INPUTEVENT_KEY_71 },
    //    { DIK_SYSRQ, ALL, INPUTEVENT_KEY_6E },
    //    { DIK_F12, ALL, INPUTEVENT_KEY_6F },
    //{ INPUTEVENT_KEY_47, ALL, INPUTEVENT_KEY_47 },
    //    { DIK_PRIOR, ALL, INPUTEVENT_KEY_48 },
    //{ INPUTEVENT_SPC_FREEZEBUTTON, ALL, INPUTEVENT_SPC_FREEZEBUTTON },
    //{ INPUTEVENT_KEY_49, ALL, INPUTEVENT_KEY_49 },
    //{ INPUTEVENT_KEY_4B, ALL, INPUTEVENT_KEY_4B },

    //{ INPUTEVENT_KEY_CDTV_STOP, ALL, INPUTEVENT_KEY_CDTV_STOP },
    //{ INPUTEVENT_KEY_CDTV_PLAYPAUSE, ALL, INPUTEVENT_KEY_CDTV_PLAYPAUSE },
    //{ INPUTEVENT_KEY_CDTV_PREV, ALL, INPUTEVENT_KEY_CDTV_PREV },
    //{ INPUTEVENT_KEY_CDTV_NEXT, ALL, INPUTEVENT_KEY_CDTV_NEXT },

    // { FS_ML_KEY_F, FS_ML_KEY_MOD_LALT, FS_EMU_ACTION_FULLSCREEN },

    { FS_ML_KEY_K,            MOD, FS_EMU_ACTION_FULL_KEYBOARD },
    { FS_ML_KEY_F,            MOD, FS_EMU_ACTION_FULLSCREEN },
    { FS_ML_KEY_RETURN,       MOD, FS_EMU_ACTION_FULLSCREEN },
    { FS_ML_KEY_G,            MOD, FS_EMU_ACTION_GRAB_INPUT },
    { FS_ML_KEY_F12,           -1, FS_EMU_ACTION_MENU_ALT },
    { FS_ML_KEY_P,            MOD, FS_EMU_ACTION_PAUSE },
    { FS_ML_KEY_PAUSE,         -1, FS_EMU_ACTION_PAUSE },
    { FS_ML_KEY_F4,           MOD, FS_EMU_ACTION_QUIT },
    { FS_ML_KEY_Q,            MOD, FS_EMU_ACTION_QUIT },
    { FS_ML_KEY_PRINT,         -1, FS_EMU_ACTION_SCREENSHOT },
    { FS_ML_KEY_S,            MOD, FS_EMU_ACTION_SCREENSHOT },
    { FS_ML_KEY_X,            MOD, FS_EMU_ACTION_SPECIAL },
    { FS_ML_KEY_TAB,          MOD, FS_EMU_ACTION_SWITCH_WINDOW },
    { FS_ML_KEY_COMMA,        MOD, FS_EMU_ACTION_VOLUME_DOWN },
    { FS_ML_KEY_VOLUMEDOWN,    -1, FS_EMU_ACTION_VOLUME_DOWN },
    { FS_ML_KEY_M,            MOD, FS_EMU_ACTION_VOLUME_MUTE },
    { FS_ML_KEY_MUTE,          -1, FS_EMU_ACTION_VOLUME_MUTE },
    { FS_ML_KEY_PERIOD,       MOD, FS_EMU_ACTION_VOLUME_UP },
    { FS_ML_KEY_VOLUMEUP,      -1, FS_EMU_ACTION_VOLUME_UP },
    { FS_ML_KEY_W,            MOD, FS_EMU_ACTION_WARP },
    { FS_ML_KEY_F11,           -1, FS_EMU_ACTION_ZOOM },
    { FS_ML_KEY_Z,            MOD, FS_EMU_ACTION_ZOOM },
    { FS_ML_KEY_B,            MOD, FS_EMU_ACTION_ZOOM_BORDER },

    { FS_ML_KEY_D,            MOD, FS_UAE_ACTION_ENTER_DEBUGGER },

    { FS_ML_KEY_A,            MOD, INPUTEVENT_SPC_FREEZEBUTTON },
    { FS_ML_KEY_T,            MOD, INPUTEVENT_SPC_HARDRESET },
    { FS_ML_KEY_R,            MOD, INPUTEVENT_SPC_SOFTRESET },
    { FS_ML_KEY_AUDIOPREV,     -1, INPUTEVENT_SPC_DISKSWAPPER_PREV },
    { FS_ML_KEY_AUDIONEXT,     -1, INPUTEVENT_SPC_DISKSWAPPER_NEXT },
    { FS_ML_KEY_AUDIOPLAY,     -1, INPUTEVENT_SPC_DISKSWAPPER_INSERT0 },
    { FS_ML_KEY_N,            MOD, INPUTEVENT_UAE_MUTE_FLOPPY_SOUNDS },

    { FS_ML_KEY_UP,           MOD, INPUTEVENT_KEY_CURSOR_UP },
    { FS_ML_KEY_DOWN,         MOD, INPUTEVENT_KEY_CURSOR_DOWN },
    { FS_ML_KEY_LEFT,         MOD, INPUTEVENT_KEY_CURSOR_LEFT },
    { FS_ML_KEY_RIGHT,        MOD, INPUTEVENT_KEY_CURSOR_RIGHT },

    { 0, 0, 0 } /* Sentinel */
};

void fs_uae_map_keyboard(void)
{
    fs_emu_set_keyboard_translation(g_default_keymap);
}
