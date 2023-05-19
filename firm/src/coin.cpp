#include <coin.h>
#include <settings.h>
#include <Arduino.h>

#define COIN_PIN 2
#define COIN_DENOMINATION 100

static volatile int32_t coin_count_cache = 0;
static volatile bool coin_need_save = false;
static void (*callback)(uint32_t, uint32_t) = nullptr;

void coin_call_callback() {
    if(callback != nullptr && coin_count_cache > 0) {
        callback(coin_count_cache, coin_count_cache * COIN_DENOMINATION);
    }
}

void coin_ISR() {
    static unsigned long debounce_timer = 0;
    unsigned long now_timer = millis();
    if(now_timer - debounce_timer > 200) {
        coin_count_cache++;
        coin_need_save = true;
        coin_call_callback();
        debounce_timer = now_timer;
    }
}

void coin_begin() {
    callback = nullptr;
    pinMode(COIN_PIN, INPUT_PULLUP);
    if(cfg_valid()) {
        coin_count_cache = cfg_read(cfg_index::COIN_COUNT);
    }
}

bool coin_reset_if_needed() {
    bool rslt = false;
    if(digitalRead(COIN_PIN) == LOW) {
        int millis_start = millis();
        while(digitalRead(COIN_PIN) == LOW) delay(1);
        if(millis() - millis_start > 2000) {
            cfg_write(cfg_index::COIN_COUNT, 0);
            rslt = true;
        }
    }

    return rslt;
}

bool haveInterrupt = false;
void coin_set_callback(void (*fun_ptr)(uint32_t, uint32_t)) {
    if(!haveInterrupt) {
        attachInterrupt(digitalPinToInterrupt(COIN_PIN), coin_ISR, LOW);
        haveInterrupt = true;
    }
    callback = fun_ptr;
}


void coin_save_if_needed() {
    if(coin_need_save) {
        cli();
        cfg_write(cfg_index_t::COIN_COUNT, coin_count_cache);
        coin_need_save = false;
        sei();
    }
}

uint32_t coin_get() {
    return coin_count_cache;
}

uint32_t coin_get_amt() {
    return coin_get() * COIN_DENOMINATION;
}