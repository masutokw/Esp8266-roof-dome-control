#include "config.h"
#ifdef IR_CONTROL
#ifndef IR_CONTROL_H_INCLUDED
#define IR_CONTROL_H_INCLUDED

void ir_init(void);
void ir_read(void);
uint32 get_IR_lcode(uint32_t bcode);
#define EAST 0x8
#define WEST  90
#define OPEN 24
#define CLOSE 82
#define OK  28
#define B_1 106
#define B_2 107
#define B_3 108
#define B_4 109
#define B_5 110
#define B_6 111
#define B_7 112
#define B_8 113
#define B_9 114
#define B_0 115
#define GO_TO 116
#define CLEAR 13

#endif
#endif
