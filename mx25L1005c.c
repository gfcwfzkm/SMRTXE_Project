/*
 * mx25L1005c.c
 * Datasheet: http://igorx.irk.ru/datasheet/vGgQWpIrKZCxPECFzu8G.pdf
 * Created: 08.10.2018 10:57:40
 *  Author: gfcwfzkm
 */ 

#include "mx25L1005c.h"

// Page 6 of the datasheet, Table 2
enum commandList {
	WREN = 0x06,
	WRDI = 0x04,
	RDID = 0x9F,
	RDSR = 0x05,	// bit3: BP1, bit2: BP0, bit1: WEL, bit0: 
	WRSR = 0x01,
	READ = 0x03,	// followed by 3 address bytes
	FREAD = 0x0B,	// followed by 3 address bytes and a dummy byte
	SEr = 0x20,		// followed by 3 address bytes
	BE = 0xD8,		// followed by 3 address bytes
	CE = 0x60,
	PP = 0x02,		// followed by 3 address bytes
	DP = 0xB9,		
	RDP = 0xAB,
	RES = 0xAB,		// followed by 3 dummy bytes
	REMS = 0x90		// followed by 2 dummy bytes and 0x00 or 0x01 
};

#define MANUFACTURER_ID	0xC2
#define WIP				0x01
#define WEL				0x02

uint8_t mx25_init()
{
	uint8_t manID;
	manID = (uint8_t)mx25_getIdentification();
	if (manID!=MANUFACTURER_ID)
	{
		return 1;
	}
	mx25_writeEnable(1);
	return 0;
}

void mx25_writeEnable(uint8_t _enable)
{
	MX25_SETCS();
	if (_enable)
	{
		MX25_SPIWRITE(WREN);
	}
	else
	{
		MX25_SPIWRITE(WRDI);
	}
	MX25_CLRCS();
}

uint16_t mx25_getIdentification()
{
	uint32_t _ID;
	MX25_SETCS();
	MX25_SPIWRITE(RDID);
	_ID = (uint32_t)MX25_SPIREAD();
	_ID |= ((uint32_t)MX25_SPIREAD() << 8);
	MX25_CLRCS();
	return _ID;
}

uint8_t mx25_getStatus()
{
	uint8_t _status;
	
	MX25_SETCS();
	MX25_SPIWRITE(RDSR);
	_status = MX25_SPIREAD();
	MX25_CLRCS();
	
	return _status;
}

void mx25_setStatus(uint8_t _status)
{
	mx25_writeEnable(1);
	MX25_SETCS();
	MX25_SPIWRITE(WRSR);
	MX25_SPIWRITE(_status);
	MX25_CLRCS();
}

void mx25_eraseSector(uint32_t _address)
{	
	while(mx25_getStatus() & WIP)
	{}
	MX25_SETCS();
	MX25_SPIWRITE(SEr);
	MX25_SPIWRITE((uint8_t)(_address >> 16));
	MX25_SPIWRITE((uint8_t)(_address >> 8));
	MX25_SPIWRITE((uint8_t)_address);
	MX25_CLRCS();
}

void mx25_eraseBlock(uint32_t _address)
{
	while(mx25_getStatus() & WIP)
	{}
	MX25_SETCS();
	MX25_SPIWRITE(BE);
	MX25_SPIWRITE((uint8_t)(_address >> 16));
	MX25_SPIWRITE((uint8_t)(_address >> 8));
	MX25_SPIWRITE((uint8_t)_address);
	MX25_CLRCS();
}

void mx25_eraseChip()
{
	while(mx25_getStatus() & WIP)
	{}
	MX25_SETCS();
	MX25_SPIWRITE(BE);
	MX25_CLRCS();
}

void mx25_sleepMode(uint8_t _powerdown)
{
	MX25_SETCS();
	if (_powerdown)
	{
		MX25_SPIWRITE(DP);
	}
	else
	{
		MX25_SPIWRITE(RDP);
	}
	MX25_CLRCS();
}

void mx25_writePage(uint32_t _address, uint8_t _page[256])
{
	while(mx25_getStatus() & WIP)
	{}
	MX25_SETCS();
	MX25_SPIWRITE(PP);
	MX25_SPIWRITE((uint8_t)(_address >> 16));
	MX25_SPIWRITE((uint8_t)(_address >> 8));
	MX25_SPIWRITE((uint8_t)_address);
	for (uint16_t i = 0; i < 256; i++)
	{
		MX25_SPIWRITE(_page[i]);
	}
	MX25_CLRCS();
}

void mx25_readData(uint32_t _address, uint8_t *_buffer, uint32_t _len)
{
	MX25_SETCS();
	MX25_SPIWRITE(READ);
	MX25_SPIWRITE((uint8_t)(_address >> 16));
	MX25_SPIWRITE((uint8_t)(_address >> 8));
	MX25_SPIWRITE((uint8_t)_address);
	for (uint16_t i = 0; i < _len; i++)
	{
		_buffer[i] = MX25_SPIREAD();
	}
	MX25_CLRCS();
}