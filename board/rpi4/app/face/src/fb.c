#include "fb.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

int fb_open(Framebuffer *fb, const char *dev){
    fb->fd = open(dev, O_RDWR);
    if(fb->fd < 0) {perror("open"); return -1; }
    
    fb->buf = mmap(NULL, FB_WIDTH * FB_HEIGHT * 4,
               PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
    if(fb->buf == MAP_FAILED){perror("mmap"); close(fb->fd); return -1; }
    return 0; 
}

void fb_close(Framebuffer *fb){
    munmap(fb->buf, FB_WIDTH * FB_HEIGHT * 4);
    close(fb->fd);
}

void fb_clear(Framebuffer *fb, uint32_t color){
    for(int i = 0; i < FB_WIDTH * FB_HEIGHT; i++){
        fb->buf[i] = color;
    }
}