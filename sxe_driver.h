/*
 * sxe_driver.h
 *
 * Created: 20.09.2018 07:33:16
 *  Author: gfcwfzkm
 */ 



#ifndef SXE_DRIVER_H_
#define SXE_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "sxe_pinout.h"
#include "hwSPI.h"
#include "ST7586.h"
#include "glcd_library.h"
#include "mx25L1005c.h"

#define KEY_NU		0x01
#define KEY_BCKSPC	0x08
#define KEY_ENTER	0x0A
#define KEY_UP		0x10
#define KEY_DOWN	0x11
#define KEY_LEFT	0x12
#define KEY_RIGHT	0x13
#define KEY_DIV		0x14
#define KEY_SQRT	0x15
#define KEY_EXP		0x16
#define KEY_PI		0x17
#define KEY_THETA	0x18
#define KEY_EQSMALLER	0x19
#define KEY_EQBIGGER	0x1A
#define KEY_SYM		0x1B
#define KEY_SHT		0x1C
#define KEY_MENU	0x1D
#define KEY_SPACE	0x20
#define KEY_APOSTROPH	0x27
#define KEY_DELETE	0x7F
#define KEY_S0		0x80
#define KEY_S1		0x81
#define KEY_S2		0x82
#define KEY_S3		0x83
#define KEY_S4		0x84
#define KEY_S5		0x85
#define KEY_S6		0x86
#define KEY_S7		0x87
#define KEY_S8		0x88
#define KEY_S9		0x89

struct keyboardHandler{
	uint8_t keys[10];		// [10] Rows, each row containing 6 columns
	uint8_t keysPressed[10];
	uint8_t keysRelease[10];
	uint8_t shift:1;
	uint8_t sym:1;
};

void sxe_initHW();

void sxe_scanKeyboard();

uint8_t sxe_getPressedKey();

uint8_t sxe_getReleasedKey();

uint8_t sxe_isKeyMapPressed(uint8_t row, uint8_t col);

uint8_t sxe_isKeyPressed(uint8_t pkey, uint8_t ignoreShiftSym);

#endif /* SXE_DRIVER_H_ */