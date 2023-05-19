#ifndef COIN_H_
#define COIN_H_
#include <stdint.h>

void coin_begin();
void coin_set_callback(void (*fun_ptr)(uint32_t, uint32_t));
void coin_save_if_needed();
uint32_t coin_get();
uint32_t coin_get_amt();
bool coin_reset_if_needed();

#endif