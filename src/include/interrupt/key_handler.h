#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include <stdint.h>

#define EXT_MODE 1
#define LSHIFT_DOWN 2
#define RSHIFT_DOWN 4
#define LCTRL_DOWN 8
#define RCTRL_DOWN 16

#define KEYUP_BIT 128

#define MSG_MAX 1023

void key_handler(uint32_t int_num);

#endif
