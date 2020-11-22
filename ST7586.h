/*
 * Graustufen-LCD Controller (SMART Response XE Projekt)
 * 
 *
 * ST7586.h
 *
 * Created: 10.09.2018 10:59:17
 *  Author: gfcwfzkm
 */ 


#ifndef ST7586_H_
#define ST7586_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "sxe_pinout.h"
#include "hwSPI.h"

#define ST7586_SERIAL		// 4-Line serial interface
//#define ST7586_PARALLEL	// 6800 Series Interface (unimplemented)

#define ST7586_DISPLAYSIZE_X	384
#define ST7586_DISPLAYSIZE_Y	136

#define ST7586_CSLOW()		DISPPORT_CE &=~ (1<<DISPPIN_CE)
#define ST7586_CSHIGH()		DISPPORT_CE |= (1<<DISPPIN_CE)
#define ST7586_CMDMODE()	DISPPORT_A0 &=~ (1<<DISPPIN_A0)
#define ST7586_DATAMODE()	DISPPORT_A0 |= (1<<DISPPIN_A0)
#define ST7586_SETRST()		DISPPORT_RST &=~ (1<<DISPPIN_RST)
#define ST7586_CLRRST()		DISPPORT_RST |= (1<<DISPPIN_RST)
#define ST7586_SERIALW(x)	hwSPI_transfer(x)

#define ST7586_COLOR_BLACK		3
#define ST7586_COLOR_DARKGRAY	2
#define ST7586_COLOR_LIGHTGRAY	1
#define ST7586_COLOR_WHITE		0

// All display instructions. A0 = 0 to send the instruction, 1 to send the data/arguments.
enum ST7586_INSTRUCTION_TABLE {
	NOP=0x00,				// No Operation
	RESET=0x01,				// Software Reset
	POWERSAVE=0x10,			// Set Power Save mode (D0=0: Sleep in mode, D0=1: Sleep out mode)
	PARTIALMODE=0x12,		// Set partial mode (D0=0: Partial mode on, D0=1: Partial mode off)
	INVERSEDISPLAY=0x20,	// Set inverse display mode (D0=0: Normal display, D0=1: Inverse display)
	ALLPIXELONOFF=0x22,		// Set all pixel on mode (D0=0: All pixel off mode, D0=1: All pixel on mode)
	DISPLAYONOFF=0x28,		// Set LCD Display (D0=0: Display off, D0=1: Display on)
	SETCOLUMNADDRESS=0x2A,	// Set column address. Starting column address: 0x00=<XS<=0x7F,Ending Column address: XS=<XE<=0x7F
	SETROWADDRESS=0x2B,		// Set row address. Starting row address: 0x00=<YS=<0x9F,Ending row address: YS=<YE=<0x9F
	WRITEDISPLAYDATA=0x2C,	// Write display data to DDRAM
	READDISPLAYDATA=0x2E,	// Read display data from DDRAM (unavailable in serial interface)
	PARTIALDISPLAYAREA=0x30,// Set partial area. Partial disp. addr start: 0x00=<PTS<=0x9F, Partial disp addr end: 0x00=<PTE=<0x9F. Display Area: 64 =< Duty =< 160
	SCROLLAREA=0x33,		// Set scroll area. Top area: TA=0x00~0xA0, Scrolling Area: SA=0x00~0xA0, Bottom Area: BA=0x00~0xA0, TA+SA+BA = 160
	DISPLAYCONTROL=0x36,	// Set scan direction of COM and SEG: MY=0: COM0->COM159, MY=1: COM159->COM0, MX=0: SEG0->SEG383, MX=1: SEG383->SEG0
	STARTLINE=0x37,			// Set display start line. S=0x00~0x9F
	DISPLAYMODE=0x38,		// Set display mode. (D0=0: Gray mode, D0=1: Monochrome mode)
	ENABLEDDRAMINTERFACE=0x3A, // Enable DDRAM interface, followed by a defined argument
	ENABLEDDRAMINTERFACE_FIXARG=0x02,
	DISPLAYDUTY=0xB0,		// Set display duty DT=0x03~0x9F
	FIRSTOUTPUTCOM=0xB1,	// Set first output COM. FC=0x00~0x9F
	FOSCDIVIDER=0xB3,		// Set FOSC dividing ratio
	PARTIALDISPLAY=0xB4,	// Set partial display mode, needs a fixed argument
	PARTIALDISPLAY_FIXARG=0xA0,
	NLINEINVERSION=0xB5,	// Set N-Line inversion
	READMODIFYWRITE=0xB8,	// Read modify write control (D0=0: Enable read modify write, D0=1: Disable read modify write)
	SETVOP=0xC0,			// Set Vop
	VOPINCREASE=0xC1,		// Vop inrease one step
	VOPDECREASE=0xC2,		// Vop decrease one step
	BIASSYSTEM=0xC3,		// Set BIAS system
	BOOSTERLEVEL=0xC4,		// Set booster level
	VOPOFFSET=0xC7,			// Set Vop offset
	ANALOGCONTROL=0xD0,		// Enable analog circuit, requires fixed argument
	ANALOGCONTROL_FIXARG=0x1D,
	AUTOREADCONTROL=0xD7,	// Auto read control. On the Argument: D4=0: Enable auto read, D4=1: Disable auto read
	AUTOREADCONTROL_ARG=0x0F,
	OTPWRRDCONTROL=0xE0,	// OTP WR/RD control
	OTPCONTROLOUT=0xE1,		// OTP control out
	OTPWRITE=0xE2,			// OTP programming procedure
	OTPREAD=0xE3,			// OTP up-load procedure
	OTPSELECTIONCONTROL=0xE4,// OTP selection control. On the argument: D6=0: disable OTP, D6=1: Enable OTP
	OTPSELECTIONCONTROL_ARG=0x19,
	OPTPROGSETTING=0xE5,	// OTP programming setting, needs a fixed argument
	OPTPROGSETTING_FIXARG=0x0F,
	FRAMERATEGRAYMODE=0xF0,	// Frame rate setting in different temperature range (Gray scale mode)
	FRAMERATEMONOMODE=0xF1,	// Frame rate setting in different temperature range (Monochrome mode)
	TEMPERATURERANGE=0xF2,	// Temperature range setting
	TEMPGRADIENTCOMPENSATION=0xF4 // Set temperature gradient compensation coefficient
};

void ST7586_init(char monochrome);

void ST7586_data(uint8_t data[], uint8_t dataSize);

void ST7586_cmd(enum ST7586_INSTRUCTION_TABLE cmd);

// VOP -> Contrast control, basically. Default-Value is 0x0F0
uint16_t ST7586_incVOP();

uint16_t ST7586_decVOP();

// usable VOP Voltage between 7.44V and 18V (0x060 to 0x168)
// Size: 9 bits!
uint16_t ST7586_setVOP(uint16_t vop);

uint16_t ST7586_getVOP();

void ST7586_fill(uint8_t color);

void ST7586_setDispPosition(uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2);

void ST7586_setPixel(uint16_t x, uint8_t y, uint8_t color);

#ifdef ST7586_PARALLEL
uint8_t ST7586_getPixel(uint16_t x, uint8_t y);
#else
// Unable to read display in serial mode Q~Q
#define ST7586_getPixel(x,y)	asm("NOP")
#endif

#ifdef ST7586_SERIAL
enum ST7586_errorCodes {mallocFail=1,XYoutofDispArea=2,XY1biggerThanXY2=3,bufferFail=4};
struct ST7586_reservedArea
{
	uint8_t *drawBuffer;	// Virtual Drawing Area
	uint16_t bufferSize;	// Size of the Buffer
	uint16_t x1,y1,x2,y2,cx,cy;	// Start and End Coordinates
	enum ST7586_errorCodes errorCode;		// In case reserveArea Function fails, error is here stored.
};

// Temporarly reserve a drawing area on the uC's memory, drawing on it before sending it to the display
struct ST7586_reservedArea ST7586_getReservedAreaMalloc(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

struct ST7586_reservedArea ST7586_getReservedAreaExBuffer(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *buffer, uint16_t buffersize);

void ST7586_setPixelReservedArea(struct ST7586_reservedArea *inst, uint16_t x, uint16_t y, uint8_t color);

void ST7586_sendReservedArea(struct ST7586_reservedArea *inst);

void ST7586_destroyReservedArea(struct ST7586_reservedArea *inst);


// Force the currently stored pixel out. Only required when using setPixel()
void ST7586_forcePixelUpdate();

#elif ST7586_PARALLEL
#define ST7586_forcePixelUpdate()	asm("NOP")
#endif

void ST7586_memset(uint8_t *array, char c, char arSize);

#endif /* ST7586_H_ */