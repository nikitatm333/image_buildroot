#ifndef DRAW_H
#define DRAW_H

#include "fb.h"
#include <stdint.h>

#define COLOR_BLACK  0xFF000000
#define COLOR_WHITE  0xFFFFFFFF

void draw_pixel(Framebuffer *fb, int x, int y, uint32_t color);
void draw_rect(Framebuffer *fb, int x, int y, int w, int h, uint32_t color);
void draw_line(Framebuffer *fb, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_circle(Framebuffer *fb, int cx, int cy, int r, uint32_t color);

#endif