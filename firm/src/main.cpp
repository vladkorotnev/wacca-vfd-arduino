#include <Arduino.h>
#include <futaba.h>
#include <rsrc.h>

extern "C" {
  void setup();
  void loop();
}


void setup() {
  ftb_init();

  delay(1000);

  ftb_reset();
  ftb_power(true);
  ftb_brightness(BRIGHT_25);

  ftb_canvas_shift(0);
  ftb_cursor(0, 0);
  ftb_font_size(FONT_16_16);
}

void scroll_from_to(uint16_t from, uint16_t to, uint16_t w) {
  for(uint16_t x = from; x < to; x++) {
    ftb_canvas_shift(x);
    delay(w);
  }
}

void loop() {
  img_data_t tmp;
  ftb_canvas_shift(0);
  tmp = img_invert(&IMG_WOCAO);
  ftb_draw_image(&IMG_WOCAO, 160, 0);
  scroll_from_to(0, 160, 10);
  delay(1000);
  for(int i = 0; i < 4; i++) {
   ftb_cursor(0, 0);
    ftb_draw_image(&tmp, 160, 0);
    delay(250);
   ftb_cursor(0, 0);
    ftb_draw_image(&IMG_WOCAO, 160, 0);
    delay(250);
  }
  scroll_from_to(160, 160 * 2, 10);

  ftb_cursor(160 * 3, 0);
  ftb_write("                    ");

  ftb_canvas_shift(0);
  tmp = img_invert(&IMG_TANOC);
  ftb_cursor(0, 0);

  ftb_draw_image(&IMG_TANOC, 160, 0);
  scroll_from_to(0, 160, 10);
  delay(1000);
  for(int i = 0; i < 4; i++) {
  ftb_cursor(0, 0);
    ftb_draw_image(&tmp, 160, 0);
    delay(250);
  ftb_cursor(0, 0);
    ftb_draw_image(&IMG_TANOC, 160, 0);
    delay(250);
  }
  scroll_from_to(160, 160 * 2, 10);

  ftb_cursor(160 * 3, 0);
  ftb_write("** ALL SAS OELUTZ **");

  scroll_from_to(160*2, 160 * 3, 10);

  ftb_scroll_box_make(0, 2, 160, 2);
  ftb_scroll_speed(2);
  ftb_scroll_text("Are you ready to RAVE to the music of HARDCORE TANO-C?? ENJOY the SUPER COOL mega Game WACCA, sunovabitch!!  ");
  ftb_scroll_start();

  delay(10000);

  ftb_scroll_text("");
}
