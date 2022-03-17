#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>
#include <util/string.h>


static uint8_t stack[8000];


void* get_tag(struct stivale2_struct* stivale2_struct, uint64_t id) {
    struct stivale2_tag* curTag = (void*)stivale2_struct->tags;

    while (1) {
        if (!(curTag)) {
            // End.
            return NULL;
        }

        if (curTag->identifier == id) {
            return curTag;
        }

        curTag = (void*)curTag->next;
    }
}


static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },

    .flags = 0
};


static struct stivale2_header_tag_framebuffer lfb_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },

    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};


__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (uintptr_t)&lfb_tag
};


static void(*kwrite_origin)(const char* const STR, size_t len);


void kwrite(const char* const STR) {
    kwrite_origin(STR, strlen(STR));
}


void _start(struct stivale2_struct* ss, uint64_t id) {
    struct stivale2_struct_tag_terminal* term_tag = get_tag(ss, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (!(term_tag)) {
        __asm__ __volatile__("cli; hlt");
    }

    void(*termwrite_origin)(const char* str, size_t size) = (void*)term_tag->term_write;
    kwrite_origin = termwrite_origin;

    kwrite("This is it for now!");

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
