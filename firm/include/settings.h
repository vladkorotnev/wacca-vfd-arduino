#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <stdint.h>

void cfg_begin();

typedef enum cfg_index {
    CFG_READY = 0,
    COIN_COUNT = 1,
    MAX_INVALID
} cfg_index_t;

bool cfg_valid();
int32_t cfg_read(cfg_index_t);
void cfg_write(cfg_index_t, int32_t value);

#endif