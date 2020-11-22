/* 
 * File:   glcd_library
 * Author: gfcwfzkm
 */

#ifndef GLCD_LIBRARY_H
#define	GLCD_LIBRARY_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "ST7586.h"

#ifdef ST7586_H_
void glcd_useBuffer(struct ST7586_reservedArea *inst);
void glcd_useNormalMethod();
uint8_t glcd_getCurrentMethod();
void glcd_ST7586_setPixel(uint16_t x, uint16_t y, uint8_t c);
void glcd_buf_print(uint16_t x, uint16_t y,char *character);
void glcd_buf_print_progmem(uint16_t x, uint16_t y,const char *character);
#define glcd_buf_print_P(x,y,__s)	glcd_buf_print_progmem(x,y,PSTR(__s))
#endif

#define GLCD_WIDTH			ST7586_DISPLAYSIZE_X
#define GLCD_HEIGHT			ST7586_DISPLAYSIZE_Y
#define GLCD_SETXY(x,y,c)	glcd_ST7586_setPixel(x,y,c)
#define GLCD_GETXY(x,y)		
#define GLCD_INVXY(x,y)		
#define GLCD_TEXTFGCOLOR	ST7586_COLOR_BLACK
#define GLCD_TEXTBGCOLOR	ST7586_COLOR_WHITE
#define GLCD_TEXTSIZE		1

void glcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);
void glcd_draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);
void glcd_draw_rectangle_filled(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);
void glcd_circle(uint16_t cx, uint16_t cy, uint16_t radius, uint8_t colour);
void glcd_drawBarV(uint16_t x0, uint16_t y0, uint16_t h, uint16_t w, uint16_t hFilled, uint8_t filledColor, uint8_t bgColor);
void glcd_drawBarH(uint16_t x0, uint16_t y0, uint16_t h, uint16_t w, uint16_t wFilled, uint8_t filledColor, uint8_t bgColor);
void glcd_load_image(const char image[], uint8_t colour);
void glcd_clear(uint8_t colour);
void glcd_invert();

void glcd_setTextBGcolor(uint8_t bgcolor);
void glcd_setTextFGcolor(uint8_t fgcolor);
void glcd_setTextSize(uint8_t size);
void glcd_char(uint16_t x, uint16_t y, char character, uint8_t fgcolor, uint8_t bgcolor, uint8_t size);
void glcd_print(uint16_t x, uint16_t y,char *character);
void glcd_print_progmem(uint16_t x, uint16_t y,const char *character);
#define glcd_print_P(x,y,__s)	glcd_print_progmem(x,y,PSTR(__s))

#endif	/* XC_HEADER_TEMPLATE_H */
