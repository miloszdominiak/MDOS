#include <graphics.h>
#include <stdlib.h>

uint32_t* screen_buffer;
uint32_t* back_buffer;
size_t screen_width;
size_t screen_height;

void graphics_initialize(uint32_t* buffer, size_t width, size_t height)
{
    screen_buffer = buffer;
    back_buffer = malloc(width * height * 4);
    screen_width = width;
    screen_height = height;
}

void update_rect(size_t start_x, size_t start_y, size_t width, size_t height)
{
    size_t index = start_y * screen_width + start_x;

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            screen_buffer[index] = back_buffer[index];
            ++index;
        }
        index += screen_width - width;
    }
}

void draw_rect(size_t start_x, size_t start_y, size_t width, size_t height, uint32_t color)
{
    size_t index = start_y * screen_width + start_x;

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            back_buffer[index] = color;
            ++index;
        }
        index += screen_width - width;
    }
}

void button(size_t start_x, size_t start_y, size_t width, size_t height)
{
    size_t index = (start_y - 1) * screen_width + start_x - 1;
    for(size_t x = 0; x < width + 2; x++)
        back_buffer[index + x] = 0xFFFFFF;

    for(size_t y = 0; y < height + 2; y++)
        back_buffer[index + y * screen_width] = 0xFFFFFF;

    index = (start_y + height) * screen_width + start_x;

    for(size_t x = 0; x < width + 1; x++)
        back_buffer[index + x] = 0x808080;
    
    index += screen_width - 1;

    for(size_t x = 0; x < width + 3; x++)
        back_buffer[index + x] = 0x000000;

    index = start_y * screen_width + start_x + width;

    for(size_t y = 0; y < height + 1; y++)
        back_buffer[index + y * screen_width] = 0x808080;

    index += (1 - screen_width);

    for(size_t y = 0; y < height + 3; y++)
        back_buffer[index + y * screen_width] = 0x000000;

    index = start_y * screen_width + start_x;

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {
            back_buffer[index] = 0xc0c0c0;
            ++index;
        }
        index += screen_width - width;
    }
}