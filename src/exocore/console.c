#include "exocore/console.h"

// Number of columns on the console screen.
#define CONSOLE_COLUMNS 80

// Number of lines on the console screen.
#define CONSOLE_LINES 24

// The default foreground color.
#define CONSOLE_FOREGROUND CONSOLE_COLOR_WHITE

// The default background color.
#define CONSOLE_BACKGROUND CONSOLE_COLOR_GREY

#if EXOCORE_IS_32_BIT
#    define CONSOLE_VIDEO_ADDRESS ((volatile ui8*)0xC00b8000)
#else
#    define CONSOLE_VIDEO_ADDRESS ((volatile ui8*)0xffff8000000b8000)
#endif

static ui8 console_position_x;
static ui8 console_position_y;

void console_print_char(const console_color_t fg, const console_color_t bg, const char character)
{
    if (character == '\t')
        console_position_x += 4;
    else if (character != '\n')
    {
        volatile ui8* const location = CONSOLE_VIDEO_ADDRESS + (console_position_x + console_position_y * CONSOLE_COLUMNS) * 2;

        *location = (ui8)character;
        *(location + 1) = (fg & 0x0f) | (ui8)(bg << 4);

        console_position_x++;
    }

    if (character == '\n' || console_position_x >= CONSOLE_COLUMNS)
    {
        console_position_x = 0;
        console_position_y++;

        if (console_position_y >= CONSOLE_LINES)
            console_scroll_display(1);
    }
}

void console_print(const console_color_t fg, const console_color_t bg, const char* const message)
{
    ASSERT(message);

    for (const char* ptr = message; *ptr; ptr++)
        console_print_char(fg, bg, *ptr);
}

void console_clear(void)
{
    for (ui16 i = 0; i < CONSOLE_COLUMNS * CONSOLE_LINES; i++)
    {
        volatile ui8* const location = CONSOLE_VIDEO_ADDRESS + i * 2;

        *location = 0x00;
        *(location + 1) = (CONSOLE_FOREGROUND & 0x0f) | (ui8)(CONSOLE_BACKGROUND << 4);
    }

    console_position_x = 0;
    console_position_y = 0;
}

void console_scroll_display(const ui8 lines)
{
    if (lines == CONSOLE_LINES)
    {
        console_clear();
        return;
    }

    ui8 current_y = 0;
    ui16 offset1 = 0;
    ui16 offset2 = lines * CONSOLE_COLUMNS;

    for (; current_y <= CONSOLE_LINES - lines; current_y++)
    {
        for (ui8 current_x = 0; current_x < CONSOLE_COLUMNS; current_x++)
        {
            volatile ui8* const src_location = CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2;
            volatile ui8* const dst_location = CONSOLE_VIDEO_ADDRESS + (current_x + offset1 + offset2) * 2;

            *src_location = *dst_location;
            *(src_location + 1) = *(dst_location + 1);
        }

        offset1 += CONSOLE_COLUMNS;
    }

    for (; current_y <= CONSOLE_LINES; current_y++)
    {
        for (ui8 current_x = 0; current_x < CONSOLE_COLUMNS; current_x++)
        {
            volatile ui8* const location = CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2;

            *location = 0x00;
            *(location + 1) = (CONSOLE_FOREGROUND & 0x0f) | (ui8)(CONSOLE_BACKGROUND << 4);
        }
    }

    if (lines > console_position_y)
        console_position_y = 0;
    else
        console_position_y -= lines;
}
