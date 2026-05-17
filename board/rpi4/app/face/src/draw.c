#include "draw.h"
#include <stdlib.h>

#define FB_WIDTH  160
#define FB_HEIGHT 128

void draw_pixel(Framebuffer *fb, int x, int y, uint32_t color) {
    if (x < 0 || x >= FB_WIDTH || y < 0 || y >= FB_HEIGHT) return;
    fb->buf[y * FB_WIDTH + x] = color;
}

void draw_rect(Framebuffer *fb, int x, int y, int w, int h, uint32_t color) {
    int x_end = x + w;
    int y_end = y + h;
    if (x_end > FB_WIDTH)  x_end = FB_WIDTH;
    if (y_end > FB_HEIGHT) y_end = FB_HEIGHT;
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    for (int j = y; j < y_end; j++)
        for (int i = x; i < x_end; i++)
            draw_pixel(fb, i, j, color);
}

void draw_line(Framebuffer *fb, int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        draw_pixel(fb, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void draw_filled_circle(Framebuffer *fb, int cx, int cy, int r, uint32_t color) {
    for (int yy = -r; yy <= r; yy++) {
        for (int xx = -r; xx <= r; xx++) {
            if (xx*xx + yy*yy <= r*r)
                draw_pixel(fb, cx + xx, cy + yy, color);
        }
    }
}