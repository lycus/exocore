#ifndef EXOCORE_H_CONSOLE
#define EXOCORE_H_CONSOLE

#include "common.h"

typedef enum console_color
{
    CONSOLE_COLOR_BLACK,
    CONSOLE_COLOR_BLUE,
    CONSOLE_COLOR_GREEN,
    CONSOLE_COLOR_CYAN,
    CONSOLE_COLOR_RED,
    CONSOLE_COLOR_MAGENTA,
    CONSOLE_COLOR_YELLOW,
    CONSOLE_COLOR_LIGHT_GREY,
    CONSOLE_COLOR_GREY,
    CONSOLE_COLOR_LIGHT_BLUE,
    CONSOLE_COLOR_LIGHT_GREEN,
    CONSOLE_COLOR_LIGHT_CYAN,
    CONSOLE_COLOR_LIGHT_RED,
    CONSOLE_COLOR_LIGHT_MAGENTA,
    CONSOLE_COLOR_LIGHT_YELLOW,
    CONSOLE_COLOR_WHITE,
} console_color_t;

void console_print_char(const console_color_t fg, const console_color_t bg, const char character);
attr(nonnull) void console_print(const console_color_t fg, const console_color_t bg, const char* const message);

void console_clear();

void console_scroll_display(const ui8 lines);

#endif
