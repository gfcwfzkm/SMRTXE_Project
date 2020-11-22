/*
 * sxe_driver.c
 *
 * Created: 20.09.2018 07:33:05
 *  Author: gfcwfzkm
 */ 

#include "sxe_driver.h"

//Keyboard Layout (SK# are screen keys: top to bottom 0-4 on left, 5-9 on right):
//                ROW1|ROW2|ROW3|ROW4|ROW5|ROW6|ROW7|ROW8|ROW9|ROW10
//           COL1    1|   2|   3|   4|   5|   6|   7|   8|   9|    0
//           COL2    Q|   W|   E|   R|   T|   Y|   U|   I|   O|    P
//           COL3    A|   S|   D|   F|   G|   H|   J|   K|   L| Bksp
//           COL4 Shft|   Z|   X|   C|   V|   B|   N|Down| NC |  Up
//           COL5  Sym|Frac|Root| Exp| Spc|   ,|   .|   M|Left|Right
//           COL6  SK0| SK1| SK2| SK3| SK4| SK5| SK6| SK7| SK8|  SK9

// Shift and Sym keys won't be returned, thus they are zero here.
const uint8_t KeysTable_normal[60] PROGMEM = {
	'1',	'2',	'3',	'4',	'5',	'6',	'7',	'8',	'9',	'0',
	'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',	'o',	'p',
	'a',	's',	'd',	'f',	'g',	'h',	'j',	'k',	'l',	KEY_DELETE,
	KEY_SHT,'z',	'x',	'c',	'v',	'b',	'n',	KEY_DOWN,KEY_NU,KEY_UP,
	KEY_SYM,KEY_DIV,KEY_SQRT,KEY_EXP,KEY_SPACE,',',	'.',	'm',	KEY_LEFT,KEY_RIGHT,
	KEY_S0,	KEY_S1,	KEY_S2,	KEY_S3,	KEY_S4,	KEY_S5,	KEY_S6,	KEY_S7,	KEY_S8,	KEY_S9
};

const uint8_t KeysTable_shift[60] PROGMEM = {
	'1',	'2',	'3',	'4',	'5',	'6',	'7',	'8',	'9',	'0',
	'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',	'O',	'P',
	'A',	'S',	'D',	'F',	'G',	'H',	'J',	'K',	'L',	KEY_ENTER,
	KEY_SHT,'Z',	'X',	'C',	'V',	'B',	'N',	KEY_DOWN,KEY_NU,KEY_UP,
	KEY_SYM,KEY_DIV,KEY_SQRT,KEY_EXP,KEY_SPACE,',',	'.',	'M',	KEY_LEFT,KEY_RIGHT,
	KEY_S0,	KEY_S1,	KEY_S2,	KEY_S3,	KEY_S4,	KEY_S5,	KEY_S6,	KEY_S7,	KEY_S8,	KEY_S9
};

// Not all keys have a 'symbol' key, thus they return zero
const uint8_t KeysTable_sym[60] PROGMEM = {
	'!',	KEY_PI,	KEY_THETA,'$',	'%',	'�',KEY_APOSTROPH,'"',	'(',	')',
	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	'[',	']',
	'=',	'+',	'-',	KEY_NU,	KEY_NU,	KEY_NU,	KEY_NU,	':',	'?',	KEY_BCKSPC,
	KEY_SHT,'*',	'/',	KEY_NU,	KEY_NU,	KEY_NU,	KEY_EQSMALLER,KEY_DOWN,KEY_NU,KEY_UP,
	KEY_SYM,KEY_DIV,KEY_SQRT,KEY_EXP,KEY_MENU,'<',	'>',	KEY_EQBIGGER,KEY_LEFT,KEY_RIGHT,
	KEY_S0,	KEY_S1,	KEY_S2,	KEY_S3,	KEY_S4,	KEY_S5,	KEY_S6,	KEY_S7,	KEY_S8,	KEY_S9
};

enum ddrmode {INPUT=0, OUTPUT=1};
enum iomode {HIGH=1,LOW=0};

struct keyboardHandler kbHandler;

void selectRow(uint8_t rowNum, enum iomode _io, enum ddrmode _ddr);
uint8_t selectCol(uint8_t colNum, enum iomode _io, enum ddrmode _ddr);

void sxe_initHW()
{
	DISPDDR_RST |= (1<<DISPPIN_RST);
	DISPDDR_A0 |= (1<<DISPPIN_A0);
	DISPDDR_CE |= (1<<DISPPIN_CE);
	EEPROMDDR |= (1<<EEPROMCS);
	EEPROMPORT |= (1<<EEPROMCS);
	
	for (uint8_t i = 0; i < 10; i++)
	{
		selectRow(i,HIGH,INPUT);
	}
	
	hwSPI_configurePins(&SPIPORT, &SPIDDR, SPISCK, SPIMOSI, SPISS);	// Configure the SPI I/Os
	hwSPI_init();										// init / start SPI.
	
	hwSPI_setClockDivider(hwSPI_CLOCK_DIV2);			// Experimental. Surprisingly, DIV4 SPI speed works fine
	//hwSPI_setClockDivider(hwSPI_CLOCK_DIV8);			// Display SCLK"H" is 140, "L" is 60 -> DIV8 is the fastest speed the Display allows (at 16MHz) -> 500ns period.
	
	ST7586_init(0);
}

void selectRow(uint8_t rowNum, enum iomode _io, enum ddrmode _ddr)
 {
	 volatile uint8_t *port, *ddr;
	 uint8_t pin;
	switch(rowNum)
	{
		case 0:
			port = &KEYBOARDROWPORT2;
			ddr = &KEYBOARDROWDDR2;
			pin = KEYBOARDROW12;
		break;
		case 1:
			port = &KEYBOARDROWPORT3;
			ddr = &KEYBOARDROWDDR3;
			pin = KEYBOARDROW23;
		break;
		case 2:
			port = &KEYBOARDROWPORT3;
			ddr = &KEYBOARDROWDDR3;
			pin = KEYBOARDROW33;
		break;
		case 3:
			port = &KEYBOARDROWPORT2;
			ddr = &KEYBOARDROWDDR2;
			pin = KEYBOARDROW42;
		break;
		case 4:
			port = &KEYBOARDROWPORT2;
			ddr = &KEYBOARDROWDDR2;
			pin = KEYBOARDROW52;
		break;
		case 5:
			port = &KEYBOARDROWPORT1;
			ddr = &KEYBOARDROWDDR1;
			pin = KEYBOARDROW61;
		break;
		case 6:
			port = &KEYBOARDROWPORT4;
			ddr = &KEYBOARDROWDDR4;
			pin = KEYBOARDROW74;
		break;
		case 7:
			port = &KEYBOARDROWPORT4;
			ddr = &KEYBOARDROWDDR4;
			pin = KEYBOARDROW84;
		break;
		case 8:
			port = &KEYBOARDROWPORT1;
			ddr = &KEYBOARDROWDDR1;
			pin = KEYBOARDROW91;
		break;
		case 9:
			port = &KEYBOARDROWPORT1;
			ddr = &KEYBOARDROWDDR1;
			pin = KEYBOARDROW101;
		break;
		default:
			return;
	}
	
	if (_ddr)
	{
		*ddr |= (1<<pin);
	}
	else
	{
		*ddr &=~ (1<<pin);
	}
	
	if (_io)
	{
		*port |= (1<<pin);
	}
	else
	{
		*port &=~ (1<<pin);
	}
 }
 
uint8_t selectCol(uint8_t colNum, enum iomode _io, enum ddrmode _ddr)
{
	volatile uint8_t *port, *ddr, *pin;
	uint8_t number;
	switch(colNum)
	{	
		case 0:
			port = &KEYBOARDCOLPORT2;
			ddr = &KEYBOARDCOLDDR2;
			pin = &KEYBOARDCOLPIN2;
			number = KEYBOARDCOL12;
		break;
		case 1:
			port = &KEYBOARDCOLPORT1;
			ddr = &KEYBOARDCOLDDR1;
			pin = &KEYBOARDCOLPIN1;
			number = KEYBOARDCOL21;
		break;
		case 2:
			port = &KEYBOARDCOLPORT1;
			ddr = &KEYBOARDCOLDDR1;
			pin = &KEYBOARDCOLPIN1;
			number = KEYBOARDCOL31;
		break;
		case 3:
			port = &KEYBOARDCOLPORT1;
			ddr = &KEYBOARDCOLDDR1;
			pin = &KEYBOARDCOLPIN1;
			number = KEYBOARDCOL41;
		break;
		case 4:
			port = &KEYBOARDCOLPORT1;
			ddr = &KEYBOARDCOLDDR1;
			pin = &KEYBOARDCOLPIN1;
			number = KEYBOARDCOL51;
		break;
		case 5:
			port = &KEYBOARDCOLPORT2;
			ddr = &KEYBOARDCOLDDR2;
			pin = &KEYBOARDCOLPIN2;
			number = KEYBOARDCOL62;
		break;
		default:
			return 0;
	}
	
	if (_ddr)
	{
		*ddr |= (1<<number);
	}
	else
	{
		*ddr &=~ (1<<number);
	}
	
	if (_io)
	{
		*port |= (1<<number);
	}
	else
	{
		*port &=~ (1<<number);
	}
	return (*pin & (1<<number));
}

uint8_t readCol()
{
	 uint8_t temp = 0;
	 for (uint8_t i = 0; i < 6; i++)
	 {
		 temp |= (selectCol(i,LOW,INPUT) ? (1<<i) : 0);
	 }
	 return temp;
}

void sxe_scanKeyboard()
{
	for (uint8_t i = 0; i < 10; i++)
	{
		selectRow(i,LOW,OUTPUT);
		_delay_us(50);
		kbHandler.keys[i] = (~readCol() & 0x3F);	// Make active-low to active-high!
		selectRow(i,LOW,INPUT);
	}
	kbHandler.shift = ((kbHandler.keys[0] & (1<<3)) ? 1 : 0);
	kbHandler.sym = ((kbHandler.keys[0] & (1<<4)) ? 1 : 0);
}

// if a key is pressed, return it but only once even if it's still pressed in the next call
// instead, the next unchecked key will be checked and read
uint8_t sxe_getPressedKey(void)
{
	sxe_scanKeyboard();
	uint8_t temp;
	for (uint8_t row = 0; row < 10; row++)
	{
		for (uint8_t col = 0; col < 6; col++)
		{
			if (kbHandler.keys[row] & (1<<col))
			{
				if (!(kbHandler.keysPressed[row] & (1<<col)))
				{
					kbHandler.keysPressed[row] |= (1<<col);

					if (kbHandler.shift)
					{
						temp = pgm_read_byte(&(KeysTable_shift[(col * 10) + row]));
					}
					else if (kbHandler.sym)
					{
						temp = pgm_read_byte(&(KeysTable_sym[(col * 10) + row]));
					}
					else
					{
						temp = pgm_read_byte(&(KeysTable_normal[(col * 10) + row]));
					}
					
					if ((temp == KEY_NU) || (temp == KEY_SYM) || (temp == KEY_SHT))
					{
						continue;
					}
					return temp;	
				}
			}
			else
			{
				kbHandler.keysPressed[row] &=~ (1<<col);
			}
		}
	}
	return 0;
}

uint8_t sxe_getReleasedKey(void)
{
	sxe_scanKeyboard();
	uint8_t releasedKey;
	for (uint8_t row = 0; row < 10; row++)
	{
		for (uint8_t col = 0; col < 6; col++)
		{
			if (kbHandler.keys[row] & (1<<col))
			{
				kbHandler.keysRelease[row] |= (1<<col);
			}
			else
			{
				if (kbHandler.keysRelease[row] & (1<<col))
				{
					kbHandler.keysRelease[row] &=~ (1<<col);
					
					if (kbHandler.shift)
					{
						releasedKey = pgm_read_byte(&(KeysTable_shift[(col * 10) + row]));
					}
					else if (kbHandler.sym)
					{
						releasedKey = pgm_read_byte(&(KeysTable_sym[(col * 10) + row]));
					}
					else
					{
						releasedKey = pgm_read_byte(&(KeysTable_normal[(col * 10) + row]));
					}
					
					if ((releasedKey == KEY_NU) || (releasedKey == KEY_SYM) || (releasedKey == KEY_SHT))
					{
						continue;
					}
					return releasedKey;
				}
			}
		}
	}
	return 0;
}

uint8_t sxe_isKeyMapPressed(uint8_t row, uint8_t col)
{
	uint8_t checkedKey = 0;
	sxe_scanKeyboard();
	if (kbHandler.keys[row] & (1<<col))
	{
		if (kbHandler.shift)
		{
			checkedKey = pgm_read_byte(&(KeysTable_shift[(col * 10) + row]));
		}
		else if (kbHandler.sym)
		{
			checkedKey = pgm_read_byte(&(KeysTable_sym[(col * 10) + row]));
		}
		else
		{
			checkedKey = pgm_read_byte(&(KeysTable_normal[(col * 10) + row]));
		}
	}
	return (checkedKey);
}

uint8_t sxe_isKeyPressed(uint8_t pkey, uint8_t ignoreShiftSym)
{
	uint8_t isPressed;
	sxe_scanKeyboard();
	for (uint8_t row = 0; row < 10; row++)
	{
		for (uint8_t col = 0; col < 6; col++)
		{
			if (kbHandler.keys[row] & (1<<col))
			{
				if (kbHandler.shift)
				{
					isPressed = pgm_read_byte(&(KeysTable_shift[(col * 10) + row]));
				}
				else if (kbHandler.sym)
				{
					isPressed = pgm_read_byte(&(KeysTable_sym[(col * 10) + row]));
				}
				else
				{
					isPressed = pgm_read_byte(&(KeysTable_normal[(col * 10) + row]));
				}
				
				if (isPressed == pkey)
				{
					return isPressed;
				}
			}
		}
	}
	return 0;
}