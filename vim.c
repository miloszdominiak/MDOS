#include <vim.h>
#include <terminal.h>
#include <stdio.h>

size_t cursor_row = 0, cursor_column = 0;

extern char getc();

#define CTRL_KEY(a) ((a) & 0x1f)

int process_key()
{
    char c = getc();
    switch(c)
    {
    case CTRL_KEY('q'):
        return 0;

    case 'h':
        cursor_column--;
        break;

    case 'l':
        cursor_column++;
        break;

    case 'j':
        cursor_row++;
        break;

    case 'k':
        cursor_row--;
        break;

    }
    return 1;
}

void draw_rows()
{
    for(size_t i = 0; i < terminal_rows; i++)
    {
        if(i == terminal_rows / 3)
        {
            terminal_row = i;
            terminal_column = 0;
            printf("Vim editor - version 0.0.1");
        }
        else
            put_character_at('~', terminal_color, i, 0);
    }
    terminal_row = cursor_row;
    terminal_column = cursor_column;
}

void refresh_screen()
{
    terminal_clear();
    draw_rows();
}

void vim_main()
{
    terminal_echo = false;

    while(1)
    {
        refresh_screen();
        if(!process_key())
            break;
    }
        put_character_at(' ', terminal_color, 0, 0);
    terminal_row = terminal_rows - 1;
    terminal_column = 0;
    terminal_echo = true;
}