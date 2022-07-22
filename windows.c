#include <windows.h>
#include <stdlib.h>
#include <graphics.h>

struct window_struct* window_create(unsigned int x, unsigned int y, 
                   unsigned int width, unsigned height)
{
        struct window_struct *window = malloc(sizeof(struct window_struct));

        window->x = x;
        window->y = y;
        window->width = width;
        window->height = height;

        return window;
}

void window_draw(struct window_struct *window)
{
        uint32_t color = ((rand() % 256) << 16) | 
                         ((rand() % 256) << 8)  | 
                         ((rand() % 256));

        draw_rect(window->x, window->y, 
                  window->width, window->height, color);
        

}