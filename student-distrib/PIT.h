#ifndef _PIT_H
#define _PIT_H
#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "lib.h"
#include "page.h"
#include "systemcall.h"
#include "types.h"
#include "x86_desc.h"
#include "terminal.h"
#include "speaker.h"

#define PIT_COMMAND 0x43
#define PIT_DATA 0x40
#define MODE3 0x37
// set pit frequency for scheduler to 512 hz
#define FREQUENCY 1193180 / 512
#define PIT_IRQ_NUM 0
#define TERMINAL_SIZE 3

void PIT_init();
void scheduler();
void PIT_interrupt_handler();

#endif 
