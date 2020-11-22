/**
 * @file hwSPI.c
 * @brief Funktionsdatei
 */ 

#include "hwSPI.h"

volatile uint8_t *_spiPORT, *_spiDDR;
uint8_t _sckBIT, _mosiBIT, _ssBIT;

// XXXX'X|Clockdivider|DATAMODE|Bitorder
uint8_t _spi_settings_changed;

void hwSPI_init()
{
	*_spiDDR |= (1<<_sckBIT);
	*_spiDDR |= (1<<_mosiBIT);
	*_spiDDR |= (1<<_ssBIT);
	
	*_spiPORT &=~ (1<<_sckBIT);
	*_spiPORT &=~ (1<<_mosiBIT);
	*_spiPORT &=~ (1<<_ssBIT);
	
	SPCR = ((1<<MSTR) | (1<<SPE));
}

void hwSPI_configurePins(volatile uint8_t *spiPort, volatile uint8_t *spiDDR,
					uint8_t sckBIT, uint8_t mosiBIT, uint8_t ssBIT)
{
	_spiPORT = spiPort;
	_spiDDR = spiDDR;
	_sckBIT = sckBIT;
	_mosiBIT = mosiBIT;
	_ssBIT = ssBIT;
}

void hwSPI_close()
{
	SPCR &=~ (1<<SPE);
}

void hwSPI_setBitOrder(uint8_t bitOrder)
{
	if (bitOrder==hwSPI_LSBFIRST)
	{
		if (!(SPCR & (1<<DORD)))	_spi_settings_changed |= (1<<0);
		SPCR |= (1<<DORD);
	}
	else
	{
		if (SPCR & (1<<DORD))		_spi_settings_changed |= (1<<0);
		SPCR &=~ (1<<DORD);
	}
}

void hwSPI_setDataMode(uint8_t mode)
{
	if (SPCR != ((SPCR & ~hwSPI_MODE_MASK) | mode))	_spi_settings_changed |= (1<<1);
	SPCR = (SPCR & ~hwSPI_MODE_MASK) | mode;
}

void hwSPI_setClockDivider(uint8_t rate)
{
	int x = (SPCR & ~hwSPI_CLOCK_MASK) | (rate & hwSPI_CLOCK_MASK);
	x = (x & ~hwSPI_2XCLOCK_MASK) | ((rate >> 2) & hwSPI_2XCLOCK_MASK);
	
	if (SPCR != x)	_spi_settings_changed |= (1<<2);
	
	SPCR = (SPCR & ~hwSPI_CLOCK_MASK) | (rate & hwSPI_CLOCK_MASK);
	SPSR = (SPSR & ~hwSPI_2XCLOCK_MASK) | ((rate >> 2) & hwSPI_2XCLOCK_MASK);
}

uint8_t hwSPI_getChangedSettings()
{
	uint8_t _changes = _spi_settings_changed;
	_spi_settings_changed = 0;
	return (_changes);
}

uint8_t hwSPI_transfer(uint8_t _data)
{
	SPDR = _data;
	while(!(SPSR & (1<<SPIF)))
	{}
	return SPDR;
}