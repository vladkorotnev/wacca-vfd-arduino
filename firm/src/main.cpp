#include <Arduino.h>
#include <futaba.h>
#include <rsrc.h>
#include <settings.h>
#include <coin.h>

extern "C" {
  void setup();
  void loop();
}

volatile uint16_t time_coin_remain = 0;
volatile uint32_t coin_amount = 0;
uint32_t coin_disp_amount = 0;
volatile uint16_t coin_combo = 0;

void coin_callback(uint32_t count, uint32_t amount) {
  time_coin_remain = 5000;
  coin_combo++;
  coin_amount = amount;
}

bool task_coin() {
  if(time_coin_remain == 0) return false;

  time_coin_remain--;

  if(coin_disp_amount != coin_amount) {
    ftb_canvas_shift(0);
    char buf[21] = { 0 };
    ftb_cursor(0, 0);
    if(coin_combo < 2) {
      ftb_write("** SAVE SOME CASH **");
    } else {
      ftb_write("** ");
      sprintf(buf, "%u COMBO", coin_combo);
      ftb_write(buf);
      ftb_write("      **");
      memset(buf, 0, 21);
    }
    ftb_cursor(0, 2);
    sprintf(buf, "TOTAL: %lu \\", coin_amount);
    ftb_write(buf);
    coin_disp_amount = coin_amount;
  }

  if(time_coin_remain == 0)  {
    coin_combo = 0;
    ftb_cursor(0, 0);
    ftb_write("                    ");
    ftb_cursor(0, 2);
    ftb_write("                    ");
  }

  return true;
}

void setup() {
  cfg_begin();
  coin_begin();

#ifdef STARTUP_DELAY
  delay(STARTUP_DELAY);
#endif

  ftb_init();

  ftb_reset();
  ftb_power(true);
  ftb_brightness(BRIGHT_50);

  ftb_canvas_shift(0);
  ftb_cursor(0, 0);
  ftb_font_size(FONT_16_16);

  bool coin_did_reset = coin_reset_if_needed();
  if(coin_did_reset) {
    ftb_cursor(0, 0);
    ftb_write("** COIN COUNT CLR **");
    delay(2000);
    ftb_cursor(0, 0);
    ftb_write("                    ");
  }  

  coin_set_callback(coin_callback);
}

typedef enum {
  LOGO_IN,
  LOGO_STAY,
  LOGO_OUT,
  TANOC_IN,
  TANOC_STAY,
  TANOC_OUT,
  AMT_IN,
  AMT_STAY,
  AMT_OUT,
} idle_phase;

idle_phase now_phase = LOGO_IN;
uint32_t logo_left = 0;
uint32_t logo_time = 0;

void task_idle() {
  switch(now_phase) {
    case LOGO_IN:
    case TANOC_IN:
      if(logo_left == 0) {
        ftb_cursor(0, 0);
        ftb_draw_image(now_phase == LOGO_IN ? &IMG_WOCAO : &IMG_TANOC, 160, 0);
      } else {
        ftb_canvas_shift(logo_left);
      }
      logo_left++;
      if(logo_left == 161) {
        now_phase = (now_phase == LOGO_IN ? LOGO_STAY : TANOC_STAY);
        logo_time = 1000;
      }
      break;

    case AMT_IN:
      if(logo_left == 0) {
          char buf1[21] = {0};
          char buf2[21] = {0};
          sprintf(buf1, "%lu coins", coin_get());
          sprintf(buf2, "Total: %lu \\", coin_get_amt());
          ftb_cursor(160, 0);
          ftb_write("                    ");
          ftb_cursor(160, 0);
          ftb_write(buf1);
          ftb_cursor(160, 2);
          ftb_write("                    ");
          ftb_cursor(160, 2);
          ftb_write(buf2);
      } else {
        ftb_canvas_shift(logo_left);
      }
      logo_left++;
      if(logo_left == 161) {
        now_phase = AMT_STAY;
        logo_time = 1000;
      }
      break;

    case LOGO_STAY:
    case TANOC_STAY:
    case AMT_STAY:
      logo_time--;
      if(logo_time == 0) {
        switch(now_phase) {
          case LOGO_STAY: now_phase = LOGO_OUT; break;
          case TANOC_STAY: now_phase = TANOC_OUT; break;
          case AMT_STAY: now_phase = AMT_OUT; break;
        }
      }
      break;
    
    case LOGO_OUT:
    case TANOC_OUT:
    case AMT_OUT:
      ftb_canvas_shift(logo_left);
      logo_left++;
      if(logo_left == 160*2 + 1) {
        switch(now_phase) {
          case LOGO_OUT: now_phase = TANOC_IN; break;
          case TANOC_OUT: now_phase = AMT_IN; break;
          case AMT_OUT: now_phase = LOGO_IN; break;
        }
        logo_left = 0;
      }
      break;
  }
}

unsigned long last_millis = 0;
void loop() {
  unsigned long now_time = millis();
  if(now_time - last_millis > 1) {
    last_millis = now_time;
    if(task_coin()) {
      now_phase = LOGO_IN;
      logo_left = 0;
      coin_save_if_needed();
      return;
    }

    task_idle();
  }
}
