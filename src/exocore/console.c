#include "console.h"

// Number of columns on the console screen.
#define CONSOLE_COLUMNS 80

// Number of lines on the console screen.
#define CONSOLE_LINES 24

#ifdef EXOCORE_IS_32_BIT
#    define CONSOLE_VIDEO_ADDRESS ((volatile ui8*)0x000b8000)
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
        volatile ui8* location = CONSOLE_VIDEO_ADDRESS + (console_position_x + console_position_y * CONSOLE_COLUMNS) * 2;

        *location = character & 0xff;
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
    for (const char* ptr = message; *ptr; ptr++)
        console_print_char(fg, bg, *ptr);
}

void console_clear(void)
{
    for (ui16 i = 0; i < CONSOLE_COLUMNS * CONSOLE_LINES; i++)
        *(CONSOLE_VIDEO_ADDRESS + i) = 0;

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
    ui8 offset1 = 0;
    ui8 offset2 = lines * CONSOLE_COLUMNS;

    for (; current_y <= CONSOLE_LINES - lines; current_y++)
    {
        for (ui8 current_x = 0; current_x < CONSOLE_COLUMNS; current_x++)
        {
            *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2) = *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1 + offset2) * 2);
            *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2 + 1) = *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1 + offset2) * 2 + 1);
        }

        offset1 += CONSOLE_COLUMNS;
    }

    for (; current_y <= CONSOLE_LINES; current_y++)
    {
        for (ui8 current_x = 0; current_x < CONSOLE_COLUMNS; current_x++)
        {
            *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2) = 0x00;
            *(CONSOLE_VIDEO_ADDRESS + (current_x + offset1) * 2 + 1) = 0x00;
        }
    }

    if (lines > console_position_y)
        console_position_y = 0;
    else
        console_position_y -= lines;
}
