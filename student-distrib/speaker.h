#ifndef _SPEAKER_H
#define _SPEAKER_H

#include "keyboard.h"
#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "systemcall.h"
#include "page.h"
#include "terminal.h"
#include "PIT.h"

#define ONE_SEC 512

#define PIT 1193180

// note to freq
#define C4  262
#define Cs4  277
#define D4  293
#define Ds4  311
#define E4  329
#define F4  349
#define Fs4  369
#define G4  390
#define Gs4  415
#define A4  440
#define As4  466
#define B4  493
#define C5  523
#define Cs5  554
#define D5  587
#define Ds5  622
#define E5  659
#define F5  698
#define Fs5  740
#define G5  783
#define Gs5  830
#define A5  880
#define As5  932
#define B5  987

/*
void play_sound(uint32_t nFrequence) ;
description: play square wave sound with nFrequence, non-stop
input: the frequency of playing sound
return val: -
*/
void play_sound(uint32_t nFrequence) ;

/*
void nosound() ;
description: shut down the PC sok
input: -
return val: -
*/
void nosound();



/*
timer_add()
description: called  by pit with 512 hz, add the local counter in speaker for sound duration
input: -
return val: -
*/
void timer_add();

/*
void timer_wait(uint32_t time);
description: sleep for this much of time in second
input: time for waiting
return val: -
*/
void timer_wait(uint32_t time);

/*
void windows_start();
description: a piece of music when booting
input: -
return val: -
*/
void windows_start();

/*
void switch_sound();
description: a piece of music when halting
input: -
return val: -
*/
void switch_sound();

/*
void show_boot_pic() ;
description: called at the bootimg stage, give a open frame
input: -
return val: -
*/
void show_boot_pic();

#endif 
