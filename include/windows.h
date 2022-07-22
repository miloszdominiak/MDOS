#ifndef WINDOWS_H
#define WINDOWS_H

struct window_struct {
    int x;
    int y;
    int width;
    int height;
};

struct window_struct* window_create(unsigned int x, unsigned int y, 
                                    unsigned int width, unsigned height);
void window_draw(struct window_struct *window);

#endif