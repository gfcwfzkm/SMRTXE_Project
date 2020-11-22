/*
 * mx25L1005c.h
 * EEPROM Library for the on-board external EEPROM.
 * http://igorx.irk.ru/datasheet/vGgQWpIrKZCxPECFzu8G.pdf
 * Created: 08.10.2018 10:57:58
 *  Author: gfcwfzkm
 */ 


#ifndef MX25L1005C_H_
#define MX25L1005C_H_

#include <avr/io.h>
#include "sxe_pinout.h"
#include "hwSPI.h"

#define MX25_SETCS()		EEPROMPORT &=~ (1<<EEPROMCS)
#define MX25_CLRCS()		EEPROMPORT |= (1<<EEPROMCS)
#define MX25_SPIWRITE(x)	hwSPI_transfer(x)
#define MX25_SPIREAD()		hwSPI_transfer(0)

uint8_t mx25_init();

void mx25_writeEnable(uint8_t _enable);

uint16_t mx25_getIdentification();

uint8_t mx25_getStatus();

void mx25_setStatus(uint8_t _status);

void mx25_eraseSector(uint32_t _address);

void mx25_eraseBlock(uint32_t _address);

void mx25_eraseChip();

void mx25_sleepMode(uint8_t powerdown);

void mx25_writePage(uint32_t _address, uint8_t _page[256]);

void mx25_readData(uint32_t _address, uint8_t *_buffer, uint32_t _len);


#endif /* MX25L1005C_H_ */