#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct {
    void* base_addr;
    uint16_t width;
    uint16_t height;
} framebuffer_t;


void framebuffer_init(framebuffer_t* lfb);


#endif
