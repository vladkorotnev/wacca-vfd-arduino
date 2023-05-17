#ifndef RSRC_H_
#define RSRC_H_
#include <Arduino.h>

typedef struct {
    bool is_progmem;
    const unsigned char * data;
    uint16_t width_pixels;
    uint8_t height_strides;
    bool inverse;
} img_data_t;

size_t img_offset_at(img_data_t * image, uint16_t left, uint8_t top);
img_data_t img_invert(img_data_t * image);

extern img_data_t IMG_WOCAO;
extern img_data_t IMG_TANOC;

#endif