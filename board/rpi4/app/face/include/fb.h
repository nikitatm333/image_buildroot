#ifndef FB_H
#define FB_H

#include <stdint.h>

#define FB_WIDTH  160
#define FB_HEIGHT 128

typedef struct {
    int fd;
    uint32_t *buf;
} Framebuffer;

int  fb_open(Framebuffer *fb, const char *dev);
void fb_close(Framebuffer *fb);
void fb_clear(Framebuffer *fb, uint32_t color);

#endif