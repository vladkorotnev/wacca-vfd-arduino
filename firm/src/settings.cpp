#include <settings.h>
#include <Arduino.h>
#include <EEPROMWearLevel.h>

#define SETTINGS_VERSION 0
#define SETTINGS_MAGIC 0x3939

void cfg_begin() {
    EEPROMwl.begin(SETTINGS_VERSION, cfg_index::MAX_INVALID);
    if(!cfg_valid()) {
        cfg_write(cfg_index::COIN_COUNT, 0);
        cfg_write(cfg_index::CFG_READY, SETTINGS_MAGIC);
    }
}

int32_t cfg_read(cfg_index_t idx) {
    int32_t tmp = -1;
    EEPROMwl.get(idx, tmp);
    return tmp;
} 

void cfg_write(cfg_index_t idx, int32_t value) {
    EEPROMwl.put(idx, value);
}

bool cfg_valid() {
    return cfg_read(cfg_index::CFG_READY) == SETTINGS_MAGIC;
}