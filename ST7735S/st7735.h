#ifndef ST7735_H_
#define ST7735_H_

//Includes
//=============================================================================================================================
#include "stdbool.h"
#include "stm32f4xx_hal.h"
//=============================================================================================================================


//Declarations and definitions
//=============================================================================================================================
#define ST7735_COLOR_RED                                                0xF800
#define ST7735_COLOR_GREEN                                              0x07E0
#define ST7735_COLOR_BLUE                                               0x001F
#define ST7735_COLOR_YELLOW                                             0xFFE0
#define ST7735_COLOR_WHITE                                              0xFFFF
#define ST7735_COLOR_BLACK                                              0x0000
#define ST7735_COLOR_ORANGE                                             0xFA20


#define ST7735_RESET_PORT                                               (GPIOD)
#define ST7735_RESET_PIN                                                (GPIO_PIN_2)

#define ST7735_CS_PORT                                                  (GPIOD)
#define ST7735_CS_PIN                                                   (GPIO_PIN_1)

#define ST7735_DC_PORT                                                  (GPIOD)
#define ST7735_DC_PIN                                                   (GPIO_PIN_3)


#define ST7735_WIDTH 																										128
#define ST7735_HEIGHT 																									160


//System function command
#define ST7735_NOP                                                      0x00 // No operation
#define ST7735_SWRESET                                                  0x01 // Software reset
#define ST7735_RDDID                                                    0x04 // Read display id
#define ST7735_RDDST                                                    0x09 // Read display status

#define ST7735_SLPIN                                                    0x10 // Sleep in and booster off
#define ST7735_SLPOUT                                                   0x11 // Sleep out and booster on
#define ST7735_PTLON                                                    0x12 // Partial mode on
#define ST7735_NORON                                                    0x13 // Partial mode off

#define ST7735_INVOFF                                                   0x20 // Display inversion off
#define ST7735_INVON                                                    0x21 // Display inversion on
#define ST7735_DISPOFF                                                  0x28 // Display on
#define ST7735_DISPON                                                   0x29 // Display off

#define ST7735_CASET                                                    0x2A // Column address set
#define ST7735_RASET                                                    0x2B // Row address set
#define ST7735_RAMWR                                                    0x2C // Memory write
#define ST7735_RAMRD                                                    0x2E // Memory read

#define ST7735_PTLAR                                                    0x30 // Partial stert/End address
#define ST7735_COLMOD                                                   0x3A // Interface pixel format
#define ST7735_MADCTL                                                   0x36 // Memory data access control

#define ST7735_FRMCTR1                                                  0xB1  //Frame rate control
#define ST7735_FRMCTR2                                                  0xB2
#define ST7735_FRMCTR3                                                  0xB3

#define ST7735_INVCTR                                                   0xB4
#define ST7735_DISSET5                                                  0xB6

#define ST7735_PWCTR1                                                   0xC0 //Power control setting 
#define ST7735_PWCTR2                                                   0xC1
#define ST7735_PWCTR3                                                   0xC2
#define ST7735_PWCTR4                                                   0xC3
#define ST7735_PWCTR5                                                   0xC4
#define ST7735_PWCTR6                                                   0xFC

#define ST7735_VMCTR1                                                   0xC5

#define ST7735_RDID1                                                    0xDA
#define ST7735_RDID2                                                    0xDB
#define ST7735_RDID3                                                    0xDC
#define ST7735_RDID4                                                    0xDD


#define ST7735_GMCTRP1                                                  0xE0
#define ST7735_GMCTRN1                                                  0xE1

#define ST7735_PTLAR																										0x30
#define ST7735_SCRLAR																										0x33

#define ST7735_TEOFF																										0x34
#define ST7735_TEON																											0x35

#define ST7735_SPI_TIMEOUT                                              100
//=============================================================================================================================


//Variables
//=============================================================================================================================
//variables of cursor
typedef struct __cursor_typedef
{
	uint16_t x;
	uint16_t y;
}cursor_t;


//variables of screen
typedef struct __screen_typedef
{
	//x display limits
	uint16_t dsplmt_xstart; //display's coordinate of up left corner
	uint16_t dsplmt_xend; 	//display's coordinate of up right corner

	//y display limits
	uint16_t dsplmt_ystart; //display's coordinate of down left corner
	uint16_t dsplmt_yend; 	//display's coordinate of down right corner
	
	uint16_t color_bckgrnd; //color background screen
}screen_t;


//=============================================================================================================================


//Functions
//=============================================================================================================================

//initialization
//--------------------------------------------------------------------------------------------------------

//@brief 							screen's initialization
void ST7735S_Init();


//@brief 							Set screen rotate. Default init is 0 (screen's loop up).
//										AlWAYS X-Y address(0,0) locate in up left corner of screen.
//@param angle				set 0   -> portrait
//										set 90  -> right turn
//										set 180 -> upside down
//										set 270 -> left turn
void ST7735S_SetScreenRotate(uint16_t angle);


//@brief 							fill thr screen
//@param color				color of screen
void ST7735S_FillScreen(uint16_t color);


void ST7735S_SetPartialArea(uint16_t l_start, uint16_t l_finish);
//--------------------------------------------------------------------------------------------------------


//Color
//--------------------------------------------------------------------------------------------------------

//@brief							function used to set 16 bits color
//@param red_5 				5 bits are used to set RED,   maximum intensity is 2^5 = 31 
//@param green_6 			6 bits are used to set GREEN, maximum intensity is 2^6 = 61 
//@param blue_5 			5 bits are used to set BLUE,  maximum intensity is 2^5 = 31 
//@return obtained_color      
uint16_t ST7735S_SetColor16(uint16_t red_5, uint16_t green_6, uint16_t blue_5);


//INEXACT REPRESENTATION!! THERE ARE INACCURACY!!
//@brief						function used to representation color as (255, 255, 255) 
//@param red_8 			intensity value of   RED(0..255)   which will be represented as intensity   RED(0..31)
//@param green_8 		intensity value of GREEN(0..255)   which will be represented as intensity GREEN(0..61)	
//@param blue_8			intensity value of  BLUE(0..255)   which will be represented as intensity  BLUE(0..31)
//@return obtained_color
uint16_t ST7735S_ConvColor24to16(uint16_t red_8, uint16_t green_8, uint16_t blue_8);
//--------------------------------------------------------------------------------------------------------


//Geometric primitives
//--------------------------------------------------------------------------------------------------------

//@param c_pos 					coordinate x0 of pixel
//@param r_pos					coordinate y0 of pixel
//@param color 					the pixel will be (uint16_t color) color
void ST7735S_DrawPixel(uint16_t x, uint16_t y, uint16_t color);


//@param x0						coordinate x0 of start point
//@param x1						coordinate x1 of finish point
//@param height				height of rectangle		
//@param color				the line will be (uint16_t color) color
void ST7735S_DrawFastVrtLine(int16_t x0, int16_t y0, uint16_t height, uint16_t color);


//@param y0						coordinate y0 of start point
//@param y1						coordinate y1 of finish point
//@param x						width of rectangle
//@param color				the line will be (uint16_t color) color
void ST7735S_DrawFastHrznLine(int16_t x0, int16_t y0, uint16_t width, uint16_t color);


//@brief							Line drawing by Bresenham's line algorithm 
//@param x0						coordinate x0 of start point
//@param y0						coordinate y0 of start point
//@param x1						coordinate x1 of finish point
//@param y1						coordinate y1 of finish point
//@param color 				the line will be (uint16_t color) color
void ST7735S_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


//@brief							Line drawing by Bresenham's line algorithm 
//@param x0						coordinate x0 of start point
//@param y0						coordinate y0 of start point
//@param x1						coordinate x1 of finish point
//@param y1						coordinate y1 of finish point
//the line will be with smooth transition from color_x0y0 to color_x1y1. For the gradien is used linear interpolation
//@param color_x0y0 	color of start point
//@param color_x1y1		color of finish point
void ST7735S_DrawLine2Color(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color_x0y0, uint16_t color_x1y1);


//@param c_start			coordinate x0 of start point
//@param r_start			coordinate y0 of start point
//@param width				width of rectangle
//@param height				height of rectangle
//@param color				the rectangle will be (uint16_t color) color
void ST7735S_DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
void ST7735S_DrawFillRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color);


//@brief 							Circle drawing by midpoint circle drawing algorithm
//@param x0						coordinate x0 of circle center
//@param y0						coordinate y0 of circle center
//@param r						radius of the circle
//@param color				color of the circle
void ST7735S_DrawCircle(uint16_t x0, uint16_t y0, int16_t r, uint16_t color);
void ST7735S_DrawFillCircle(uint16_t x0, uint16_t y0, int16_t r, uint16_t color);


//@param x0						coordinate x0 of first point
//@param y0						coordinate y0 of first point
//@param x1						coordinate x1 of second point
//@param y1						coordinate y1 of second point
//@param x2						coordinate x0 of third point
//@param y2						coordinate y0 of third point
void ST7735S_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ST7735S_DrawFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
//--------------------------------------------------------------------------------------------------------


//Write text
//--------------------------------------------------------------------------------------------------------

//@brief 							Sets the position from which the char/text will be printed. 
//										The start of the char is the up left corner.
//@param x						coordinate x 
//@param y						coordinate y
//@param auto_center	coordinate x and y has position middle of the screen
void ST7735S_SetCursor(int16_t x, int16_t y, _Bool auto_center);


//@brief 							Print ONLY the standard ASCII symbol.(Before use check if font5x7.h is included!) 
//@param c						ASCII symbol 
//@param color				Ñolor of the symbol
//										For standard size of ASCII symbol(5x7 pixel) -> size_x = 1, size_y = 1
//@param size_x				The width size of the "pixel" from which the symbol is built 
//@param size_y				The height size of the "pixel" from which the symbol is built
void ST7735S_PrintChar(unsigned char c, uint16_t color, uint8_t size_x, uint8_t size_y);


//@brief 							Print the standard ASCII string.(Before use check if font5x7.h is included!) 
//@param message[]		char string
//@param color				Ñolor of the symbol
//										For standard size of ASCII symbol(5x7 pixel) -> size_x = 1, size_y = 1
//@param size_x				The width size of the "pixel" from which the symbol is built 
//@param size_y				The height size of the "pixel" from which the symbol is built
void ST7735S_PrintCharString(const char message[], uint16_t color, uint8_t size_x, uint8_t size_y);


//@brief 							Print an integer converted to a string 
//@param num					an integer 
//@param color				Ñolor of the symbol
//										For standard size of ASCII symbol(5x7 pixel) -> size_x = 1, size_y = 1
//@param size_x				The width size of the "pixel" from which the symbol is built 
//@param size_y				The height size of the "pixel" from which the symbol is built
void ST7735S_PrintIntNum(int32_t num, uint16_t color, uint8_t size_x, uint8_t size_y);


//@brief 							Print an float converted to a string 
//@param num					an integer 
//@param color				Ñolor of the symbol
//										For standard size of ASCII symbol(5x7 pixel) -> size_x = 1, size_y = 1
//@param size_x				The width size of the "pixel" from which the symbol is built 
//@param size_y				The height size of the "pixel" from which the symbol is built
void ST7735S_PrintFloatNum(float floatNumber, uint16_t color, uint8_t size_x, uint8_t size_y);
//--------------------------------------------------------------------------------------------------------


void load_imag(uint8_t x0, uint8_t y0, uint16_t pic_width, uint16_t pic_height, const uint16_t *picture);
//=============================================================================================================================
#endif // #ifndef ST7735_H_
