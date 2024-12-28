﻿#pragma once
// copy from glfw3.h
/* Printable keys */

struct KeyCode
{
    static constexpr unsigned int KEY_SPACE         = 32;
    static constexpr unsigned int KEY_APOSTROPHE    = 39; /* ' */
    static constexpr unsigned int KEY_COMMA         = 44; /* , */
    static constexpr unsigned int KEY_MINUS         = 45; /* - */
    static constexpr unsigned int KEY_PERIOD        = 46; /* . */
    static constexpr unsigned int KEY_SLASH         = 47; /* / */
    static constexpr unsigned int KEY_0             = 48;
    static constexpr unsigned int KEY_1             = 49;
    static constexpr unsigned int KEY_2             = 50;
    static constexpr unsigned int KEY_3             = 51;
    static constexpr unsigned int KEY_4             = 52;
    static constexpr unsigned int KEY_5             = 53;
    static constexpr unsigned int KEY_6             = 54;
    static constexpr unsigned int KEY_7             = 55;
    static constexpr unsigned int KEY_8             = 56;
    static constexpr unsigned int KEY_9             = 57;
    static constexpr unsigned int KEY_SEMICOLON     = 59; /* ; */
    static constexpr unsigned int KEY_EQUAL         = 61; /* = */
    static constexpr unsigned int KEY_A             = 65;
    static constexpr unsigned int KEY_B             = 66;
    static constexpr unsigned int KEY_C             = 67;
    static constexpr unsigned int KEY_D             = 68;
    static constexpr unsigned int KEY_E             = 69;
    static constexpr unsigned int KEY_F             = 70;
    static constexpr unsigned int KEY_G             = 71;
    static constexpr unsigned int KEY_H             = 72;
    static constexpr unsigned int KEY_I             = 73;
    static constexpr unsigned int KEY_J             = 74;
    static constexpr unsigned int KEY_K             = 75;
    static constexpr unsigned int KEY_L             = 76;
    static constexpr unsigned int KEY_M             = 77;
    static constexpr unsigned int KEY_N             = 78;
    static constexpr unsigned int KEY_O             = 79;
    static constexpr unsigned int KEY_P             = 80;
    static constexpr unsigned int KEY_Q             = 81;
    static constexpr unsigned int KEY_R             = 82;
    static constexpr unsigned int KEY_S             = 83;
    static constexpr unsigned int KEY_T             = 84;
    static constexpr unsigned int KEY_U             = 85;
    static constexpr unsigned int KEY_V             = 86;
    static constexpr unsigned int KEY_W             = 87;
    static constexpr unsigned int KEY_X             = 88;
    static constexpr unsigned int KEY_Y             = 89;
    static constexpr unsigned int KEY_Z             = 90;
    static constexpr unsigned int KEY_LEFT_BRACKET  = 91;  /* [ */
    static constexpr unsigned int KEY_BACKSLASH     = 92;  /* \ */
    static constexpr unsigned int KEY_RIGHT_BRACKET = 93;  /* ] */
    static constexpr unsigned int KEY_GRAVE_ACCENT  = 96;  /* ` */
    static constexpr unsigned int KEY_WORLD_1       = 161; /* non-US #1 */
    static constexpr unsigned int KEY_WORLD_2       = 162; /* non-US #2 */

/* Function keys */
#ifdef USE_GLFW
    static constexpr unsigned int KEY_ESCAPE = 256;
#else
    static constexpr unsigned int KEY_ESCAPE = 27;
#endif

    static constexpr unsigned int KEY_ENTER         = 257;
    static constexpr unsigned int KEY_TAB           = 258;
    static constexpr unsigned int KEY_BACKSPACE     = 259;
    static constexpr unsigned int KEY_INSERT        = 260;
    static constexpr unsigned int KEY_DELETE        = 261;
    static constexpr unsigned int KEY_RIGHT         = 262;
    static constexpr unsigned int KEY_LEFT          = 263;
    static constexpr unsigned int KEY_DOWN          = 264;
    static constexpr unsigned int KEY_UP            = 265;
    static constexpr unsigned int KEY_PAGE_UP       = 266;
    static constexpr unsigned int KEY_PAGE_DOWN     = 267;
    static constexpr unsigned int KEY_HOME          = 268;
    static constexpr unsigned int KEY_END           = 269;
    static constexpr unsigned int KEY_CAPS_LOCK     = 280;
    static constexpr unsigned int KEY_SCROLL_LOCK   = 281;
    static constexpr unsigned int KEY_NUM_LOCK      = 282;
    static constexpr unsigned int KEY_PRINT_SCREEN  = 283;
    static constexpr unsigned int KEY_PAUSE         = 284;
    static constexpr unsigned int KEY_F1            = 290;
    static constexpr unsigned int KEY_F2            = 291;
    static constexpr unsigned int KEY_F3            = 292;
    static constexpr unsigned int KEY_F4            = 293;
    static constexpr unsigned int KEY_F5            = 294;
    static constexpr unsigned int KEY_F6            = 295;
    static constexpr unsigned int KEY_F7            = 296;
    static constexpr unsigned int KEY_F8            = 297;
    static constexpr unsigned int KEY_F9            = 298;
    static constexpr unsigned int KEY_F10           = 299;
    static constexpr unsigned int KEY_F11           = 300;
    static constexpr unsigned int KEY_F12           = 301;
    static constexpr unsigned int KEY_F13           = 302;
    static constexpr unsigned int KEY_F14           = 303;
    static constexpr unsigned int KEY_F15           = 304;
    static constexpr unsigned int KEY_F16           = 305;
    static constexpr unsigned int KEY_F17           = 306;
    static constexpr unsigned int KEY_F18           = 307;
    static constexpr unsigned int KEY_F19           = 308;
    static constexpr unsigned int KEY_F20           = 309;
    static constexpr unsigned int KEY_F21           = 310;
    static constexpr unsigned int KEY_F22           = 311;
    static constexpr unsigned int KEY_F23           = 312;
    static constexpr unsigned int KEY_F24           = 313;
    static constexpr unsigned int KEY_F25           = 314;
    static constexpr unsigned int KEY_KP_0          = 320;
    static constexpr unsigned int KEY_KP_1          = 321;
    static constexpr unsigned int KEY_KP_2          = 322;
    static constexpr unsigned int KEY_KP_3          = 323;
    static constexpr unsigned int KEY_KP_4          = 324;
    static constexpr unsigned int KEY_KP_5          = 325;
    static constexpr unsigned int KEY_KP_6          = 326;
    static constexpr unsigned int KEY_KP_7          = 327;
    static constexpr unsigned int KEY_KP_8          = 328;
    static constexpr unsigned int KEY_KP_9          = 329;
    static constexpr unsigned int KEY_KP_DECIMAL    = 330;
    static constexpr unsigned int KEY_KP_DIVIDE     = 331;
    static constexpr unsigned int KEY_KP_MULTIPLY   = 332;
    static constexpr unsigned int KEY_KP_SUBTRACT   = 333;
    static constexpr unsigned int KEY_KP_ADD        = 334;
    static constexpr unsigned int KEY_KP_ENTER      = 335;
    static constexpr unsigned int KEY_KP_EQUAL      = 336;
    static constexpr unsigned int KEY_LEFT_SHIFT    = 340;
    static constexpr unsigned int KEY_LEFT_CONTROL  = 341;
    static constexpr unsigned int KEY_LEFT_ALT      = 342;
    static constexpr unsigned int KEY_LEFT_SUPER    = 343;
    static constexpr unsigned int KEY_RIGHT_SHIFT   = 344;
    static constexpr unsigned int KEY_RIGHT_CONTROL = 345;
    static constexpr unsigned int KEY_RIGHT_ALT     = 346;
    static constexpr unsigned int KEY_RIGHT_SUPER   = 347;
    static constexpr unsigned int KEY_MENU          = 348;

    static constexpr unsigned int KEY_LAST            = KeyCode::KEY_MENU;
    static constexpr unsigned int MOUSE_BUTTON_1      = 0;
    static constexpr unsigned int MOUSE_BUTTON_2      = 1;
    static constexpr unsigned int MOUSE_BUTTON_3      = 2;
    static constexpr unsigned int MOUSE_BUTTON_4      = 3;
    static constexpr unsigned int MOUSE_BUTTON_5      = 4;
    static constexpr unsigned int MOUSE_BUTTON_6      = 5;
    static constexpr unsigned int MOUSE_BUTTON_7      = 6;
    static constexpr unsigned int MOUSE_BUTTON_8      = 7;
    static constexpr unsigned int MOUSE_BUTTON_LAST   = KeyCode::MOUSE_BUTTON_8;
    static constexpr unsigned int MOUSE_BUTTON_LEFT   = KeyCode::MOUSE_BUTTON_1;
    static constexpr unsigned int MOUSE_BUTTON_RIGHT  = KeyCode::MOUSE_BUTTON_2;
    static constexpr unsigned int MOUSE_BUTTON_MIDDLE = KeyCode::MOUSE_BUTTON_3;
};