#include <futaba.h>
#include <Arduino.h>

//
// Thanks to
//   OBIWAN
// for help with this protocol
//

#ifndef FTB_PORT
#define FTB_PORT Serial1
#endif

#define LEFT_HI(x) (((x) & 0x100) >> 8)
#define LEFT_LO(x) ((x) & 0xFF)

#define FTB_PORT_WRITE_LEFT(x) {FTB_PORT.write(LEFT_HI(x)); FTB_PORT.write(LEFT_LO(x));}

void ftb_init() {
    FTB_PORT.begin(115200);
}

void ftb_reset() {
    FTB_PORT.write("\x1B\x0B"); // reset
}

void ftb_clear() {
    FTB_PORT.write("\x1B\x0C");
}

void ftb_power(bool on) {
    FTB_PORT.write("\x1B\x21"); // on
    FTB_PORT.write(on ? 0x1 : 0x0);
}

void ftb_brightness(ftb_bright_t brightness) {
    FTB_PORT.write("\x1b\x20");
    FTB_PORT.write((char) brightness);
}

void ftb_canvas_shift(uint16_t left) {
    FTB_PORT.write("\x1B\x22"); 
    FTB_PORT_WRITE_LEFT(left);
}

void ftb_cursor(uint16_t left, uint8_t top) {
    FTB_PORT.write("\x1B\x30"); // cursor set
    FTB_PORT_WRITE_LEFT(left);
    FTB_PORT.write(top);
}

void ftb_language(ftb_lang_t lang) {
    FTB_PORT.write("\x1B\x32");
    FTB_PORT.write((char) lang);
} 

void ftb_font_size(ftb_font_size_t font) {
    FTB_PORT.write("\x1B\x33");
    FTB_PORT.write((char) font);
}

void ftb_scroll_box_make(uint16_t left, uint8_t top, uint16_t width, uint8_t height) {
    FTB_PORT.write("\x1B\x40");
    FTB_PORT_WRITE_LEFT(left);
    FTB_PORT.write(top);
    FTB_PORT_WRITE_LEFT(width);
    FTB_PORT.write(height);
}

void ftb_scroll_speed(uint8_t divisor) {
    FTB_PORT.write("\x1B\x41");
    FTB_PORT.write((char) divisor);
}

void ftb_scroll_text(const char * text) {
    size_t len = strlen(text);
    if(len > 255) return;
    
    FTB_PORT.write("\x1B\x50");
    FTB_PORT.write(len);
    FTB_PORT.write(text);
}

void ftb_scroll_start() {
    FTB_PORT.write("\x1B\x51");
}

void ftb_write(const char * text) {
    FTB_PORT.write(text);
}

void ftb_draw_image(img_data_t * image, uint16_t left, uint8_t top) {
    FTB_PORT.write("\x1B\x2E"); // bmp out
    FTB_PORT_WRITE_LEFT(left);
    FTB_PORT.write(top);
    FTB_PORT_WRITE_LEFT(image->width_pixels - 1);
    FTB_PORT.write(image->height_strides - 1);

    for(unsigned int i = 0; i < (image->width_pixels) * (image->height_strides); i++) {
        byte c;
        if(image->is_progmem) {
            c = pgm_read_byte(&image->data[i]);
        } else {
            c = image->data[i];
        }

        if(image->inverse) {
            c = ~c;
        }

        FTB_PORT.write(c);
    }
}
