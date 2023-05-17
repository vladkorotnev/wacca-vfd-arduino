#ifndef FUTABA_H_
#define FUTABA_H_
#include <rsrc.h>

void ftb_init();
void ftb_reset();
void ftb_clear();
void ftb_power(bool on);

typedef enum {
    BRIGHT_0 = 0,
    BRIGHT_25 = 1,
    BRIGHT_50 = 2,
    BRIGHT_75 = 3,
    BRIGHT_100 = 4
} ftb_bright_t;

void ftb_brightness(ftb_bright_t);

void ftb_canvas_shift(uint16_t left);
void ftb_draw_image(img_data_t * image, uint16_t left, uint8_t top);

void ftb_cursor(uint16_t left, uint8_t top);

typedef enum {
    SIMP_CHINESE,
    TRAD_CHINESE,
    JAPANESE,
    KOREAN
} ftb_lang_t;
void ftb_language(ftb_lang_t);

typedef enum {
    FONT_16_16,
    FONT_6_8
} ftb_font_size_t;
void ftb_font_size(ftb_font_size_t);

void ftb_scroll_box_make(uint16_t left, uint8_t top, uint16_t width, uint8_t height);
void ftb_scroll_speed(uint8_t divisor);
void ftb_scroll_text(const char * text);
void ftb_scroll_start();

void ftb_write(const char * text);
#endif