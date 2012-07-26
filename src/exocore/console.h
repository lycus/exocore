#ifndef EXOCORE_H_CONSOLE
#define EXOCORE_H_CONSOLE

#include "common.h"

typedef enum console_color : ui8
{
    CONSOLE_COLOR_BLACK = 0,
    CONSOLE_COLOR_BLUE = 1,
    CONSOLE_COLOR_GREEN = 2,
    CONSOLE_COLOR_CYAN = 3,
    CONSOLE_COLOR_RED = 4,
    CONSOLE_COLOR_MAGENTA = 5,
    CONSOLE_COLOR_YELLOW = 6,
    CONSOLE_COLOR_LIGHT_GREY = 7,
    CONSOLE_COLOR_GREY = 8,
    CONSOLE_COLOR_LIGHT_BLUE = 9,
    CONSOLE_COLOR_LIGHT_GREEN = 10,
    CONSOLE_COLOR_LIGHT_CYAN = 11,
    CONSOLE_COLOR_LIGHT_RED = 12,
    CONSOLE_COLOR_LIGHT_MAGENTA = 13,
    CONSOLE_COLOR_LIGHT_YELLOW = 14,
    CONSOLE_COLOR_WHITE = 15,
} console_color_t;

void console_print_char(const console_color_t fg, const console_color_t bg, const char character);
attr(nonnull) void console_print(const console_color_t fg, const console_color_t bg, const char* const message);

void console_clear(void);

void console_scroll_display(const ui8 lines);

#define _PRINT(MSG, CLR) console_print(CLR, CONSOLE_COLOR_GREY, MSG);

#define INFO(MSG) _PRINT(MSG, CONSOLE_COLOR_WHITE)
#define SUCCESS(MSG) _PRINT(MSG, CONSOLE_COLOR_LIGHT_GREEN)
#define WARNING(MSG) _PRINT(MSG, CONSOLE_COLOR_LIGHT_YELLOW)
#define ERROR(MSG) _PRINT(MSG, CONSOLE_COLOR_LIGHT_RED)

#ifdef EXOCORE_DEBUG
#    define DEBUG(MSG) INFO(MSG)
#else
#    define DEBUG(MSG)
#endif

#define CLEAR() console_clear()

#endif
