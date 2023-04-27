// Includes
//=============================================================================================================================
#include "st7735.h"
#include "font5x7.h"
#include "math.h"
//=============================================================================================================================



//Declarations and definitions 
//=============================================================================================================================
extern SPI_HandleTypeDef hspi1;

cursor_t crsr1;

screen_t scr1 = {0, ST7735_WIDTH, 0, ST7735_HEIGHT};
//=============================================================================================================================


//Functions 
//================================================================================================================================================

//----------------------------------------------------------------------------
void ST7735_SendByte(uint8_t data)
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);  
  SPI1->DR = data;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void ST7735_WaitLastData()
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);
  while((SPI1->SR & SPI_SR_BSY) != RESET);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
static void ST7735_SendCommand(uint8_t data)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_RESET);
  ST7735_SendByte(data);
  ST7735_WaitLastData();
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void ST7735_SendData(uint8_t data)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);  
  ST7735_SendByte(data);
  ST7735_WaitLastData();
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void ST7735_SendDataMultiple(uint8_t *data, uint32_t num)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
  
  for (uint32_t i = 0; i < num; i++)
  {
    ST7735_SendByte(*data);
    data++;
  }
  
  ST7735_WaitLastData();
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void ST7735_Init()
{ 
  SPI1->CR1 |= SPI_CR1_SPE;
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
    
  ST7735_SendCommand(ST7735_SWRESET);  
  HAL_Delay(150);

  ST7735_SendCommand(ST7735_SLPOUT);  
  HAL_Delay(500);

  ST7735_SendCommand(ST7735_FRMCTR1);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_FRMCTR2);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_FRMCTR3);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_INVCTR);
  ST7735_SendData(0x07);

  ST7735_SendCommand(ST7735_PWCTR1);
  ST7735_SendData(0xA2);
  ST7735_SendData(0x02);
  ST7735_SendData(0x84);

  ST7735_SendCommand(ST7735_PWCTR2);
  ST7735_SendData(0xC5);

  ST7735_SendCommand(ST7735_PWCTR3);
  ST7735_SendData(0x0A);
  ST7735_SendData(0x00);

  ST7735_SendCommand(ST7735_PWCTR4);
  ST7735_SendData(0x8A);
  ST7735_SendData(0x2A);

  ST7735_SendCommand(ST7735_PWCTR5);
  ST7735_SendData(0x8A);
  ST7735_SendData(0xEE);

  ST7735_SendCommand(ST7735_VMCTR1);
  ST7735_SendData(0x0E);

  ST7735_SendCommand(ST7735_INVOFF);
	
  ST7735_SendCommand(ST7735_MADCTL);									//Display data direction
  ST7735_SendData(0x00);															//Normal: X-Y address(0,0) locate in up left corner of screen (screen's loop up)

  ST7735_SendCommand(ST7735_COLMOD);
	ST7735_SendData(0x05); 														  //16 bits/pixel R5b(31)/G6b(63)/B5b(31) R0R1R2R3|R4G0G1G2|G3G4G5B0|B1B2B3B4 
	
  ST7735_SendCommand(ST7735_GMCTRP1);
  ST7735_SendData(0x02);
  ST7735_SendData(0x1c);
  ST7735_SendData(0x07);
  ST7735_SendData(0x12);
  ST7735_SendData(0x37);
  ST7735_SendData(0x32);
  ST7735_SendData(0x29);
  ST7735_SendData(0x2d);
  ST7735_SendData(0x29);
  ST7735_SendData(0x25);
  ST7735_SendData(0x2B);
  ST7735_SendData(0x39);	
  ST7735_SendData(0x00);
  ST7735_SendData(0x01);
  ST7735_SendData(0x03);
  ST7735_SendData(0x10);

  ST7735_SendCommand(ST7735_GMCTRN1);
  ST7735_SendData(0x03);
  ST7735_SendData(0x1d);
  ST7735_SendData(0x07);
  ST7735_SendData(0x06);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x29);
  ST7735_SendData(0x2D);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x37);
  ST7735_SendData(0x3F);	
  ST7735_SendData(0x00);
  ST7735_SendData(0x00);
  ST7735_SendData(0x02);
  ST7735_SendData(0x10);
	
	ST7735_SendCommand(ST7735_TEON);

  ST7735_SendCommand(ST7735_NORON);
  HAL_Delay(10);

  ST7735_SendCommand(ST7735_DISPON);
  HAL_Delay(100);

  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}
//----------------------------------------------------------------------------


/*
//----------------------------------------------------------------------------
void set_partialArea(uint16_t l_start, uint16_t l_finish)
{
	uint8_t parametr[4];
  
  parametr[0] = (l_start & 0xFF00) >> 8;
	parametr[1] = l_start & 0x00FF;
  parametr[2] = (l_finish & 0xFF00) >> 8;
  parametr[3] = l_finish & 0x00FF;
  
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	
  ST7735_SendCommand(ST7735_PTLAR);
  ST7735_SendDataMultiple(parametr, 4);
	
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}
//----------------------------------------------------------------------------
*/


//----------------------------------------------------------------------------
void set_ScreenRotate(uint16_t angle)
{
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	
	switch(angle)
	{
		case 0:
			ST7735_SendCommand(ST7735_MADCTL);
			ST7735_SendData(0x00);
			
			scr1.dsplmt_xend = ST7735_WIDTH;
			scr1.dsplmt_yend = ST7735_HEIGHT;
					break;
		
		case 90:
			ST7735_SendCommand(ST7735_MADCTL);
			ST7735_SendData(0xA0); 
			
			scr1.dsplmt_xend = ST7735_HEIGHT;
			scr1.dsplmt_yend = ST7735_WIDTH;
					break;
		
		case 180:
			ST7735_SendCommand(ST7735_MADCTL);
			ST7735_SendData(0xC0); 

			scr1.dsplmt_xend = ST7735_WIDTH;
			scr1.dsplmt_yend = ST7735_HEIGHT;
					break;
		
		case 270:
			ST7735_SendCommand(ST7735_MADCTL);
			ST7735_SendData(0x60); 
		
			scr1.dsplmt_xend = ST7735_HEIGHT;
			scr1.dsplmt_yend = ST7735_WIDTH;
					break;			
	}
	
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);		
}
//----------------------------------------------------------------------------


void fillScreen(uint16_t color)
{
	plot_FillRectangle(0, 0, scr1.dsplmt_xend, scr1.dsplmt_yend, color);
	scr1.color_bckgrnd = color;
}


//----------------------------------------------------------------------------
void ST7735_SetColAddr(uint16_t c_start, uint16_t c_stop)
{
  uint8_t data[4];
  
  data[0] = (c_start & 0xFF00) >> 8;
  data[1] = c_start & 0x00FF;
  data[2] = (c_stop & 0xFF00) >> 8;
  data[3] = c_stop & 0x00FF;
  
  ST7735_SendCommand(ST7735_CASET);
  ST7735_SendDataMultiple(data, 4);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void ST7735_SetRowAddr(uint16_t r_start, uint16_t r_stop)
{
  uint8_t data[4];
  
  data[0] = (r_start & 0xFF00) >> 8;
  data[1] = r_start & 0x00FF;
  data[2] = (r_stop & 0xFF00) >> 8;
  data[3] = r_stop & 0x00FF;

  ST7735_SendCommand(ST7735_RASET);
  ST7735_SendDataMultiple(data, 4);
}
//----------------------------------------------------------------------------
//================================================================================================================================================


//COLOR
//================================================================================================================================================

//----------------------------------------------------------------------------------------------------------------
uint16_t set_Color16(uint16_t red_5, uint16_t green_6, uint16_t blue_5)
{
	uint16_t RGB[3] = {0x0000, 0x0000, 0x0000}; 
	uint16_t RGBtrnsf[3];
	uint16_t obtained_color;
		
	RGB[0] += red_5; 
	RGB[1] += green_6;
	RGB[2] += blue_5;
	
	RGBtrnsf[0] = RGB[0]<< 11;
	RGBtrnsf[1] = RGB[1]<< 5;
	RGBtrnsf[2] = RGB[2];
	
	obtained_color = RGBtrnsf[0] | RGBtrnsf[1] | RGBtrnsf[2];
	
	return obtained_color;
}
//----------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------
uint16_t conv_Color24to16(uint16_t red_8, uint16_t green_8, uint16_t blue_8)
{
	uint16_t RGB[3] = {0x0000, 0x0000, 0x0000};
	uint16_t RGBtrnsf[3];
	uint16_t obtained_color;
	
	uint16_t   red_5,   prcnt_red_5 ;
	uint16_t green_6, prcnt_green_6;
	uint16_t  blue_5,  prcnt_blue_5;
	
	uint8_t prcnt_100 = 100;
	
	uint8_t RGB24mxvl = 255; 
	uint8_t redmxvl = 31;
	uint8_t greenmxvl = 61;
	uint8_t bluemxvl = 31;
	
	//proportions are used to find the relations between colors
	//step 1
	//RGB24mxvl(255)     = prcnt_100(100%)
  //red_8(input color value) = prcnt_red_5(OUTPUT persent value) 	
	prcnt_red_5 = red_8 * prcnt_100 / RGB24mxvl;
	
	//step 2
	//redmxvl(31)         = prcnt_100(100%)
  //red_5(OUTPUT color value) = prcnt_red_5(input persent value) 	
	red_5 = redmxvl * prcnt_red_5 / prcnt_100;
	
	prcnt_green_6 = green_8 * prcnt_100 / RGB24mxvl;
	green_6 = greenmxvl * prcnt_green_6 / prcnt_100;
	
	prcnt_blue_5 = green_8 * prcnt_100 / RGB24mxvl;
	blue_5 = bluemxvl * prcnt_blue_5 / prcnt_100;
	
	RGB[0] += red_5; 
	RGB[1] += green_6;
	RGB[2] += blue_5;
	
	RGBtrnsf[0] = RGB[0]<< 11;
	RGBtrnsf[1] = RGB[1]<< 5;
	RGBtrnsf[2] = RGB[2];
	
	obtained_color = RGBtrnsf[0] | RGBtrnsf[1] | RGBtrnsf[2];
	
	return obtained_color;
}
//----------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------
//function interpolationcolor gets values in line coordinate system.	
//@param  uint16_t p_start 		start  line's coordinate
//@param  uint16_t p_finish		finish line's coordinate
//@param  uint16_t step				coordinate from start to finish line's coordinate
//@return trnsf_color;
uint16_t InterpolationColor(uint16_t p_start, uint16_t p_finish, uint16_t step,  uint16_t color_x0y0, uint16_t color_x1y1)
{
	uint16_t trnsf_color;
	
	uint16_t RGBx0y0[3];
	uint16_t RGBx1y1[3];
	uint16_t RGBtrnsf[3];
	
	uint16_t Redmask = 0xF800;
	uint16_t Greenmask = 0x07E0;
	uint16_t Bluemask = 0x001F;
		
	RGBx0y0[0] = (color_x0y0 & Redmask) >> 11;
	RGBx0y0[1] = (color_x0y0 & Greenmask) >> 5;
	RGBx0y0[2] = color_x0y0 & Bluemask;	
	
	RGBx1y1[0] = (color_x1y1 & Redmask) >> 11;
	RGBx1y1[1] = (color_x1y1 & Greenmask) >> 5;
	RGBx1y1[2] = color_x1y1 & Bluemask;	
	
	uint16_t lenght = p_finish - p_start; //line size
	
	//in general the algorithm has the form:
	//   lenght   |R_x0y0|			0     |R_x1y1|	 |R_trnsf|
	//  ------- * |G_x0y0| + ------ * |G_x1y1| = |G_trnsf|
	//   lenght   |B_x0y0|	 lenght   |B_x1y1|	 |B_trnsf|
	
	//  lenght-1   |R_x0y0|		 0+1     |R_x1y1|	  |R_trnsf|
	//  -------- * |G_x0y0| + ------ * |G_x1y1| = |G_trnsf|
	//   lenght    |B_x0y0|	  lenght   |B_x1y1|	  |B_trnsf|
	
	//  lenght-2   |R_x0y0|		 0+2     |R_x1y1|	  |R_trnsf|
	//  -------- * |G_x0y0| + ------ * |G_x1y1| = |G_trnsf|
	//   lenght    |B_x0y0|	  lenght   |B_x1y1|	  |B_trnsf|
	
	//...
	//...
	//...
	
	//     0      |R_x0y0|	 lenght   |R_x1y1|	 |R_trnsf|
	//  ------- * |G_x0y0| + ------ * |G_x1y1| = |G_trnsf|
	//   lenght   |B_x0y0|	 lenght   |B_x1y1|	 |B_trnsf|
	
	//***************************************************************************************************************************************
	for(uint8_t color = 0; color < 3; color++)
		RGBtrnsf[color] = ( ( (lenght - (step - p_start) ) * RGBx0y0[color] ) / lenght )  + ( ( (step - p_start) ) * RGBx1y1[color] ) / lenght;		
	//***************************************************************************************************************************************
	
	RGBtrnsf[0] = RGBtrnsf[0] << 11;
	RGBtrnsf[1] = RGBtrnsf[1] << 5;
	
	trnsf_color = RGBtrnsf[0] | RGBtrnsf[1] | RGBtrnsf[2];
	
	return trnsf_color;
}
//----------------------------------------------------------------------------------------------------------------

//================================================================================================================================================


//PLOT FIGURES
//================================================================================================================================================

//Draw pixel
//----------------------------------------------------------------------------------------------------------------
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if( (x < scr1.dsplmt_xstart) || (x >= scr1.dsplmt_xend) || (y < scr1.dsplmt_ystart) || (y >= scr1.dsplmt_yend) )
		return;
	
	else
	{
		uint8_t colorBytes[2];
		colorBytes[0] = (color & 0xFF00) >> 8;
		colorBytes[1] = color & 0x00FF;
	
		HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	
		//send position
		ST7735_SetColAddr(x, x);
		ST7735_SetRowAddr(y, y);
	
		ST7735_SendCommand(ST7735_RAMWR);
  
		HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
  
		ST7735_SendByte(colorBytes[0]);
		ST7735_SendByte(colorBytes[1]);
	
		ST7735_WaitLastData();
		HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
	}
}
//----------------------------------------------------------------------------------------------------------------


//plot vertical line 1 color
//----------------------------------------------------------------------------------------------------------------
void plot_FastVrtLine(int16_t x0, int16_t y0, uint16_t height, uint16_t color)
{
	if( (x0 < scr1.dsplmt_xstart) || (x0 >= scr1.dsplmt_xend) || (y0 >= scr1.dsplmt_yend) )
		return;
	
	if(y0 < scr1.dsplmt_ystart)
	{
		height = height - (scr1.dsplmt_ystart - y0);
		y0 = scr1.dsplmt_ystart;
	}
	
	if(height < 1)
		return;
	
	if(y0 + height - 1 >= scr1.dsplmt_yend) 
		height = scr1.dsplmt_yend - y0; //height = height - (y0 + height - scr1.dsplmt_yend)
	
	uint8_t colorBytes[2];
	colorBytes[0] = (color & 0xFF00) >> 8;
	colorBytes[1] = color & 0x00FF;
		
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	ST7735_SetColAddr(x0, x0);
	ST7735_SetRowAddr(y0, y0 + height - 1);
		
	ST7735_SendCommand(ST7735_RAMWR);
		
	HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
		
	uint32_t size = height;
	for (uint32_t i = 0; i < size; i++)
	{
		ST7735_SendByte(colorBytes[0]);
		ST7735_SendByte(colorBytes[1]);
	}
		
	ST7735_WaitLastData();
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------------------


//plot horizontal line 1 color
//----------------------------------------------------------------------------------------------------------------
void plot_FastHrznLine(int16_t x0, int16_t y0, uint16_t width, uint16_t color)
{
	if( (y0 < scr1.dsplmt_ystart) || (y0 >= scr1.dsplmt_yend) || (x0 >= scr1.dsplmt_yend) )
		return;
	
	if(x0 < scr1.dsplmt_xstart)
	{
		width = width - (scr1.dsplmt_xstart - x0);
		x0 = scr1.dsplmt_xstart;
	}
	
	if(width < 1)
		return;
	
	if(y0 + width - 1 >= scr1.dsplmt_xend) 
		width = scr1.dsplmt_xend - y0; 
	

	uint8_t colorBytes[2];
	colorBytes[0] = (color & 0xFF00) >> 8;
	colorBytes[1] = color & 0x00FF;
		
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	ST7735_SetColAddr(x0, x0 + width - 1);
	ST7735_SetRowAddr(y0, y0);
		
	ST7735_SendCommand(ST7735_RAMWR);
		
	HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
		
	uint32_t size = width;
	for (uint32_t i = 0; i < size; i++)
	{
		ST7735_SendByte(colorBytes[0]);
		ST7735_SendByte(colorBytes[1]);
	}
		
	ST7735_WaitLastData();
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------------------


//plot line 1 color
//----------------------------------------------------------------------------------------------------------------
//Slopes between 0 and -1 by checking whether y needs to increase or decrease (dy < 0)
void plot_line_low(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t x, y;
	
	int16_t yi = 1;
	
	int16_t D;
	
	if(dy < 0)
	{
		yi = - 1;
		dy = - dy;
	}
	
	D = 2 * dy - dx;
	y = y0;
	
	for(x = x0; x <= x1; x++)
	{
		ST7735_DrawPixel(x, y, color);
		if(D > 0)
		{
			y = y + yi;
			D = D + 2*(dy - dx);
		}
		else
			D = D + 2*dy;
	}
}

//an implementation for positive or negative steep slopes 
void plot_line_high(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t x, y;
	
	int16_t xi = 1;
	
	int16_t D;
	
	if(dx < 0)
	{
		xi = - 1;
		dx = - dx;
	}
	
	D = 2 * dx - dy;
	x = x0;
	
	for(y = y0; y <= y1; y++)
	{
		ST7735_DrawPixel(x, y, color);
		if(D > 0)
		{
			x = x + xi;
			D = D + 2*(dx - dy);
		}
		else
			D = D + 2*dx;
	}
}

//detect whether x1 > x0 or y1 > y0 and reverse the input coordinates before drawing
void plot_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if( (x0 < scr1.dsplmt_xstart) || (x0 >= scr1.dsplmt_xend) || (y0 < scr1.dsplmt_ystart) || (y0 >= scr1.dsplmt_yend) )
		return;
	
	if( (x1 < scr1.dsplmt_xstart) || (x1 >= scr1.dsplmt_xend) || (y1 < scr1.dsplmt_ystart) || (y1 >= scr1.dsplmt_yend) )
		return;
	
	else
	{
		if( abs((int)(y1 - y0)) < abs((int)(x1 - x0)) )
		{
			if(x0 > x1)
				plot_line_low(x1, y1, x0, y0, color);
			else
				plot_line_low(x0, y0, x1, y1, color);
		}
		else
		{
			if(y0 > y1)
				plot_line_high(x1, y1, x0, y0, color);
			else
				plot_line_high(x0, y0, x1, y1, color);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------


//plot line 2 color
//----------------------------------------------------------------------------------------------------------------
void plot_line_low_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color_x0y0, uint16_t color_x1y1)
{
	uint16_t color;
	
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t x, y;
	
	int16_t yi = 1;
	
	int16_t D;
	
	if(dy < 0)
	{
		yi = - 1;
		dy = - dy;
	}
	
	D = 2 * dy - dx;
	y = y0;
	
	for(x = x0; x <= x1; x++)
	{
		color = InterpolationColor(x0, x1, x, color_x0y0, color_x1y1);
		ST7735_DrawPixel(x, y, color);
		if(D > 0)
		{
			y = y + yi;
			D = D + 2*(dy - dx);
		}
		else
			D = D + 2*dy;
	}
}

void plot_line_high_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color_x0y0, uint16_t color_x1y1)
{
	uint16_t color;
	
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t x, y;
	
	int16_t xi = 1;
	
	int16_t D;
	
	if(dx < 0)
	{
		xi = - 1;
		dx = - dx;
	}
	
	D = 2 * dx - dy;
	x = x0;
	
	for(y = y0; y <= y1; y++)
	{
		color = InterpolationColor(y0, y1, y, color_x0y0, color_x1y1);
		ST7735_DrawPixel(x, y, color);
		if(D > 0)
		{
			x = x + xi;
			D = D + 2*(dx - dy);
		}
		else
			D = D + 2*dx;
	}
}


void plot_Line2Color(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color_x0y0, uint16_t color_x1y1)
{
	if( (x0 < scr1.dsplmt_xstart) || (x0 >= scr1.dsplmt_xend) || (y0 < scr1.dsplmt_ystart) || (y0 >= scr1.dsplmt_yend) )
		return;
	
	if( (x1 < scr1.dsplmt_xstart) || (x1 >= scr1.dsplmt_xend) || (y1 < scr1.dsplmt_ystart) || (y1 >= scr1.dsplmt_yend) )
		return;
	
	else
	{
		if( abs((int)(y1 - y0)) < abs((int)(x1 - x0)) )
		{
			if(x0 > x1)
				plot_line_low_2(x1, y1, x0, y0, color_x0y0, color_x1y1);
			else
				plot_line_low_2(x0, y0, x1, y1, color_x0y0, color_x1y1);
		}
		else
		{
			if(y0 > y1)
				plot_line_high_2(x1, y1, x0, y0, color_x0y0, color_x1y1);
			else
				plot_line_high_2(x0, y0, x1, y1, color_x0y0, color_x1y1);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------


//Draw not fill rectangle
//----------------------------------------------------------------------------------------------------------------
void plot_Rectangle(int16_t x0, int16_t y0, int16_t width, int16_t height, uint16_t color)
{
	if((x0 >= scr1.dsplmt_xend) || (y0 >= scr1.dsplmt_yend))
		return;
	
	if( ( (x0 + width) <= scr1.dsplmt_xstart ) || ( (y0 + height) <= scr1.dsplmt_ystart))
		return;
	
	if(x0 < scr1.dsplmt_xstart)
	{	
		width = width - (scr1.dsplmt_xstart - x0); 
		x0 = scr1.dsplmt_xstart; 	
	}
	
	if(y0 < scr1.dsplmt_ystart) 
	{	
		height = height - (scr1.dsplmt_ystart - y0); 
		y0 = scr1.dsplmt_ystart; 	
	}
	
	if((x0 + width - 1) >= scr1.dsplmt_xend)
		width = scr1.dsplmt_xend - x0;
	
	if((y0 + height - 1) >= scr1.dsplmt_yend)
		height = scr1.dsplmt_yend - y0;
	
	plot_FastHrznLine(x0, y0, width, color);
	plot_FastHrznLine(x0, y0 + height - 1, width, color);
	plot_FastVrtLine(x0, y0, height, color);
	plot_FastVrtLine(x0 + width - 1, y0, height, color);

}
//----------------------------------------------------------------------------------------------------------------


//Draw fill rectangle
//----------------------------------------------------------------------------------------------------------------
void plot_FillRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color)
{
	if((x0 >= scr1.dsplmt_xend) || (y0 >= scr1.dsplmt_yend))
		return;
	
	if( ( (x0 + width) <= scr1.dsplmt_xstart ) || ( (y0 + height) <= scr1.dsplmt_ystart) )
		return;
	
	if(x0 < scr1.dsplmt_xstart)
	{	
		width = width - (scr1.dsplmt_xstart - x0); 
		x0 = scr1.dsplmt_xstart; 	
	}
	
	if(y0 < scr1.dsplmt_ystart) 
	{	
		height = height - (scr1.dsplmt_ystart - y0); 
		y0 = scr1.dsplmt_ystart; 	
	}
	
	if((x0 + width - 1) >= scr1.dsplmt_xend)
		width = scr1.dsplmt_xend - x0;
	
	if((y0 + height - 1) >= scr1.dsplmt_yend)
		height = scr1.dsplmt_yend - y0;
	
	uint8_t colorBytes[2];
	colorBytes[0] = (color & 0xFF00) >> 8;
	colorBytes[1] = color & 0x00FF;
		
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
	ST7735_SetColAddr(x0, x0 + width - 1);
	ST7735_SetRowAddr(y0, y0 + height- 1);
		
	ST7735_SendCommand(ST7735_RAMWR);
		
	HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
		
	uint32_t size = height * width;
	for (uint32_t i = 0; i < size; i++)
	{
		ST7735_SendByte(colorBytes[0]);
		ST7735_SendByte(colorBytes[1]);
	}
		
	ST7735_WaitLastData();
	HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
	
}
//----------------------------------------------------------------------------------------------------------------


//plot not fill circle
//----------------------------------------------------------------------------------------------------------------
void plot_Circle( uint16_t x0, uint16_t y0, int16_t r, uint16_t color)
{
	if((x0 >= scr1.dsplmt_xend) || (y0 >= scr1.dsplmt_yend))
		return;
	
	if( ( (x0 + r) <= scr1.dsplmt_xstart ) || ( (y0 + r) <= scr1.dsplmt_ystart))
		return;
	
	if(x0 < scr1.dsplmt_xstart)
		x0 = scr1.dsplmt_xstart + r; 
	
	if(y0 < scr1.dsplmt_ystart) 
		y0 = scr1.dsplmt_ystart + r; 	
	
	if((x0 + r - 1) >= scr1.dsplmt_xend)
		x0 = scr1.dsplmt_xend - r - 1;
	
	if((y0 + r - 1) >= scr1.dsplmt_yend)
		y0 = scr1.dsplmt_xend - r - 1;
	
	//starting coordinates
	int16_t x = 0;
	int16_t y = r;
	
	//The value of p is calculated at the mid-point of the two contending pixels (x - 0.5, y+1)
	//p = (r – 0.5)^2 + (0+1)^2 – r^2 = =1.25 – r = 1 - r (because integer value)
	int16_t p = 1 - r;
	
	do
	{
		ST7735_DrawPixel(x0 + x, y0 + y, color);
		ST7735_DrawPixel(x0 + y, y0 + x, color);
		ST7735_DrawPixel(x0 - y, y0 + x, color);
		ST7735_DrawPixel(x0 - x, y0 + y, color);
		ST7735_DrawPixel(x0 - x, y0 - y, color);
		ST7735_DrawPixel(x0 - y, y0 - x, color);
		ST7735_DrawPixel(x0 + y, y0 - x, color);
		ST7735_DrawPixel(x0 + x, y0 - y, color);
		
		//inside the circle
		if(p < 0)
			p += (2*x) + 1;
		
		//p > 0 -> outside circle
		else
		{
			y = y - 1;
			p += (2*x) - (2*y) + 1;
		}
		
		x++;
	}
	while(y >= x);	
}
//----------------------------------------------------------------------------------------------------------------


//plot fill circle
//----------------------------------------------------------------------------------------------------------------
void plot_FillCircle(uint16_t x0, uint16_t y0, int16_t r, uint16_t color)
{
	if((x0 >= scr1.dsplmt_xend) || (y0 >= scr1.dsplmt_yend))
		return;
	
	if( ( (x0 + r) <= scr1.dsplmt_xstart ) || ( (y0 + r) <= scr1.dsplmt_ystart))
		return;
	
	if(x0 < scr1.dsplmt_xstart)
		x0 = scr1.dsplmt_xstart + r; 
	
	if(y0 < scr1.dsplmt_ystart) 
		y0 = scr1.dsplmt_ystart + r; 	
	
	if((x0 + r - 1) >= scr1.dsplmt_xend)
		x0 = scr1.dsplmt_xend - r - 1;
	
	if((y0 + r - 1) >= scr1.dsplmt_yend)
		y0 = scr1.dsplmt_xend - r - 1;
	
	//starting coordinates
	int16_t x = 0;
	int16_t y = r;
	
	//The value of p is calculated at the mid-point of the two contending pixels (x - 0.5, y+1)
	//p = (r – 0.5)^2 + (0+1)^2 – r^2 = =1.25 – r = 1 - r (because integer value)
	int16_t p = 1 - r;
	
	do
	{
		plot_FastHrznLine(x0 - x, y0 + y, 2*x, color);
		plot_FastHrznLine(x0 - y, y0 + x, 2*y, color);
		plot_FastHrznLine(x0 - y, y0 - x, 2*y, color);
		plot_FastHrznLine(x0 - x, y0 - y, 2*x, color);
		
		//inside the circle
		if(p < 0)
			p += (2*x) + 1;
		
		//p > 0 -> outside circle
		else
		{
			y = y - 1;
			p += (2*x) - (2*y) + 1;
		}
		
		x++;
	}
	while(y >= x);	
}
//----------------------------------------------------------------------------------------------------------------


//plot not filltriangle
//----------------------------------------------------------------------------------------------------------------
void plot_Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
	if( (x0 < scr1.dsplmt_xstart) || (x0 >= scr1.dsplmt_xend) || (y0 < scr1.dsplmt_ystart) || (y0 >= scr1.dsplmt_yend) )
		return;
	
	if( (x1 < scr1.dsplmt_xstart) || (x1 >= scr1.dsplmt_xend) || (y1 < scr1.dsplmt_ystart) || (y1 >= scr1.dsplmt_yend) )
		return;
	
	else
	{
		plot_Line(x0, y0, x1, y1, color);
		plot_Line(x1, y1, x2, y2, color);
		plot_Line(x2, y2, x0, y0, color);
	}
}
//----------------------------------------------------------------------------------------------------------------


//plot fill triangle
//----------------------------------------------------------------------------------------------------------------
void swap(int16_t *a, int16_t *b)
{
	int buf;
  buf = *a;
  *a = *b;
  *b = buf;	
}

void plot_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
	if( (x0 < scr1.dsplmt_xstart) || (x0 >= scr1.dsplmt_xend) || (y0 < scr1.dsplmt_ystart) || (y0 >= scr1.dsplmt_yend) )
		return;
	
	if( (x1 < scr1.dsplmt_xstart) || (x1 >= scr1.dsplmt_xend) || (y1 < scr1.dsplmt_ystart) || (y1 >= scr1.dsplmt_yend) )
		return;
	
	if( (x2 < scr1.dsplmt_xstart) || (x2 >= scr1.dsplmt_xend) || (y2 < scr1.dsplmt_ystart) || (y2 >= scr1.dsplmt_yend) )
		return;

	else
	{
		int16_t abscissa_x0;
		int16_t abscissa_x1;
		int16_t y, saved_y;

		// Sort coordinates p(x0, y0), p(x1, y1), p(x2, y2) 
		//(y2 >= y1 >= y0)
		if (y0 > y1) 
		{
			swap(&y0, &y1); 
			swap(&x0, &x1);
		}
		if (y1 > y2) 
		{
			swap(&y2, &y1); 
			swap(&x2, &x1);
		}
		if (y0 > y1) 
		{
			swap(&y0, &y1); 
			swap(&x0, &x1);
		}

		//all on same line
		//****************************************
		if(y0 == y2 )
		{
			abscissa_x0 = abscissa_x1 = x0;
			if(x1 < abscissa_x0)      
				abscissa_x0 = x1;
			else if(x1 > abscissa_x1) 
				abscissa_x1 = x1;
			if(x2 < abscissa_x0)      
				abscissa_x0 = x2;
			else if(x2 > abscissa_x1) 
				abscissa_x1 = x2;
			plot_FastHrznLine(abscissa_x0, y0, abscissa_x1 - abscissa_x0 + 1, color);
			return;
		}
		//****************************************
		
		//x-axis increments for three side triangle
		int32_t dx01 = x1 - x0;
		int32_t dx02 = x2 - x0;
		int32_t dx12 = x2 - x1;
		
		//y-axis increments for three side triangle
		int32_t dy01 = y1 - y0;
		int32_t dy02 = y2 - y0;
		int32_t dy12 = y2 - y1;
		
		int32_t abscissa_trnglx0 = 0;
		int32_t abscissa_trnglx1 = 0;

		if(y1 == y2) 
			saved_y = y1;   // Include y1 scanline
		else         
			saved_y = y1 - 1; // Skip it

		//up triangle
		for(y = y0; y <= saved_y; y++) 
		{
			//equation of line 
			//abscissa_x0 = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
			//abscissa_x1 = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
			abscissa_x0 = x0 + abscissa_trnglx0 / dy01;
			abscissa_x1 = x0 + abscissa_trnglx1 / dy02;
			
			abscissa_trnglx0 += dx01;
			abscissa_trnglx1 += dx02;
			
			if(abscissa_x0 > abscissa_x1) 
				swap(&abscissa_x0, &abscissa_x1);
			
			plot_FastHrznLine(abscissa_x0, y, abscissa_x1 - abscissa_x0 + 1, color);
		}

		abscissa_trnglx0 = dx12 * (y - y1);
		abscissa_trnglx1 = dx02 * (y - y0);
		
		//dawn triangle
		for(; y <= y2; y++) 
		{
			//equation of line 
			//abscissa_x0 = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
			//abscissa_x1 = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
			abscissa_x0 = x1 + abscissa_trnglx0 / dy12;
			abscissa_x1 = x0 + abscissa_trnglx1 / dy02;
			
			abscissa_trnglx0 += dx12;
			abscissa_trnglx1 += dx02;
			
			if(abscissa_x0 > abscissa_x1)
				swap(&abscissa_x0, &abscissa_x1);
			
			plot_FastHrznLine(abscissa_x0, y, abscissa_x1 - abscissa_x0 + 1, color);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------
//================================================================================================================================================


//plot text
//================================================================================================================================================

//----------------------------------------------------------------------------------------------------------------
void set_cursor(int16_t x, int16_t y, _Bool auto_center) 
{
	if(auto_center == true)
	{		
		crsr1.x = scr1.dsplmt_xend / 2;
		crsr1.y = scr1.dsplmt_yend / 2;
		return;
	}

	if(x < 0) 
		x = 0;
	
	else if(x >= scr1.dsplmt_xend) 
		x = scr1.dsplmt_xend - 1;
	crsr1.x = x;
	
	if(y < 0)
		y = 0;
	
	else if(y >= scr1.dsplmt_yend) 
		y = scr1.dsplmt_yend - 1;
	crsr1.y = y;
}
//----------------------------------------------------------------------------------------------------------------


//print ASII symbol
//----------------------------------------------------------------------------------------------------------------
void print_Char(unsigned char c, uint16_t color, uint8_t size_x, uint8_t size_y)
{
	if( (crsr1.x >= scr1.dsplmt_xend) || (crsr1.y >= scr1.dsplmt_yend) || ( (crsr1.x + 6 - 1) < 0) || ((crsr1.y + 8 - 1) < 0) )
		return;
	
	if( color == scr1.color_bckgrnd)
		return;
	
	if( (size_x == 1) || (size_y == 1) )
	{
		uint8_t mask = 0x01;
		int16_t xoff, yoff;
				
		for (yoff = 0; yoff < 8; yoff++) 
		{
			uint8_t line = 0;
			
			for (xoff = 0; xoff < 5; xoff++) 
			{
				if(font5x7[c * 5 + xoff] & mask) 
					line |= 1;
				line <<= 1;
			}
			line >>= 1;
			
			xoff = 0;
			
			while(line) 
			{
				if (line == 0x1F) 
				{
					plot_FastHrznLine(crsr1.x + xoff, crsr1.y + yoff, 5, color);
					break;
				}
							
				else if(line == 0x1E) 
				{
					plot_FastHrznLine(crsr1.x + xoff, crsr1.y + yoff, 4, color);
					break;
				}
							
				else if((line & 0x1C) == 0x1C) 
				{
					plot_FastHrznLine(crsr1.x + xoff, crsr1.y + yoff, 3, color);
					line <<= 4;
					xoff += 4;
				} 
							
				else if((line & 0x18) == 0x18) 
				{
					plot_FastHrznLine(crsr1.x + xoff, crsr1.y + yoff, 2, color);
					line <<= 3;
					xoff += 3;
				}
							
				else if((line & 0x10) == 0x10) 
				{
					ST7735_DrawPixel(crsr1.x + xoff, crsr1.y + yoff, color);
					line <<= 2;
					xoff += 2;
				} 
							
				else 
				{
					line <<= 1;
					xoff += 1;
				}
			}
			mask = mask << 1;
		}
	}
	
	else
	{
		uint8_t mask = 0x01;
		int16_t xoff, yoff;
		
		for(yoff=0; yoff < 8; yoff++) 
		{
			uint8_t line = 0;
			for(xoff=0; xoff < 5; xoff++) 
			{
				if (font5x7[c * 5 + xoff] & mask) 
					line |= 1;
				line <<= 1;
			}
			
			line >>= 1;
			xoff = 0;
			while(line) 
			{
				if(line == 0x1F)
				{
					plot_FillRectangle(crsr1.x  + xoff * size_x, crsr1.y + yoff * size_y, 5 * size_x, size_y, color);
					break;
				} 
				else if(line == 0x1E)
				{
					plot_FillRectangle(crsr1.x  + xoff * size_x, crsr1.y + yoff * size_y, 4 * size_x, size_y, color);
					break;
				}
				else if((line & 0x1C) == 0x1C)
				{
					plot_FillRectangle(crsr1.x  + xoff * size_x, crsr1.y + yoff * size_y, 3 * size_x, size_y, color);
					line <<= 4;
					xoff += 4;
				}
				else if((line & 0x18) == 0x18)
				{
					plot_FillRectangle(crsr1.x  + xoff * size_x, crsr1.y + yoff * size_y, 2 * size_x, size_y, color);
					line <<= 3;
					xoff += 3;
				} 
				else if((line & 0x10) == 0x10)
				{
					plot_FillRectangle(crsr1.x  + xoff * size_x, crsr1.y + yoff * size_y, size_x, size_y, color);
					line <<= 2;
					xoff += 2;
				} 
				else 
				{
					line <<= 1;
					xoff += 1;
				}
			}
			mask = mask << 1;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------


//print ASII string
//----------------------------------------------------------------------------------------------------------------

//String size in char
uint16_t strLenChar(const char *ptrStr)
{
	uint16_t size = 0;
    
   while(*ptrStr)
   {
		 size++;
     ptrStr++;    
	 }
    return size;
}

void print_CharString(const char message[], uint16_t color, uint8_t size_x, uint8_t size_y)
{
	if( (crsr1.x >= scr1.dsplmt_xend) || (crsr1.y >= scr1.dsplmt_yend) )
		return;
	
	uint16_t len = strLenChar(message);
	
	if( ((crsr1.x + 6 * size_x * len - 1) > scr1.dsplmt_xend) )
		return;
	
	for(uint16_t i = 0; i <= len; i++)
	{
		set_cursor(crsr1.x, crsr1.y, false);
		print_Char(message[i], color, size_x, size_y);
		crsr1.x += size_x * 6;
	}
}
//----------------------------------------------------------------------------------------------------------------


//print int number
//----------------------------------------------------------------------------------------------------------------
void itoa(int n, char s[])
 {
	 uint32_t i; 
	 int32_t sign;

   if((sign = n) < 0) 
		 n = -n; 
	 
	 i = 0;
   do
	 {
		 s[i++] = n % 10 + '0';  
   }
	 while((n /= 10) > 0);   
     
	 if(sign < 0)
		 s[i++] = '-';
	 
	 s[i] = '\0';

	 //reverse string
	 uint8_t istr, jstr;
   char c;

   for(istr = 0, jstr = strLenChar(s)-1; istr<jstr; istr++, jstr--) 
	 {
		 c = s[istr];
     s[istr] = s[jstr];
     s[jstr] = c;
   }
 }

void print_IntNum(int32_t num, uint16_t color, uint8_t size_x, uint8_t size_y)
{
	char str[32];
	itoa(num, str);
	print_CharString(str, color, size_x, size_y);
}
//----------------------------------------------------------------------------------------------------------------
//================================================================================================================================================



//print float number
//----------------------------------------------------------------------------------------------------------------
void print_FloatNum(float floatNumber, uint16_t color, uint8_t size_x, uint8_t size_y)
{
  char str[32];               // Array to contain decimal string
  uint8_t ptr = 0;            // Initialise pointer for array
  int8_t  digits = 1;         // Count the digits to avoid array overflow
  float rounding = 0.5;       // Round up down delta

	uint8_t dp = 7;						  //Decimal portion
  
  // Adjust the rounding value
  for (uint8_t i = 0; i < dp; ++i) 
		rounding /= 10.0f;

  if (floatNumber < -rounding)
  {
    str[ptr++] = '-'; 						// Negative number
    str[ptr] = 0; 								// Put a null in the array as a precaution
    digits = 0;   								// Set digits to 0 to compensate so pointer value can be used later
    floatNumber = -floatNumber; 	// Make positive
  }

  floatNumber += rounding; 				// Round up or down

  
  uint32_t temp = floatNumber;

  // Put integer part into array
  itoa(temp, str + ptr);

  // Find out where the null is to get the digit count loaded
  while ((uint8_t)str[ptr] != 0) 
		ptr++; 	// Move the pointer along
  
	digits += ptr;         // Count the digits

  str[ptr++] = '.'; 		// Add decimal point
  str[ptr] = '0';   		// Add a dummy zero
  str[ptr + 1] = 0; 		// Add a null but don't increment pointer so it can be overwritten

  // Get the decimal portion
  floatNumber = floatNumber - temp;

  // Get decimal digits one by one and put in array
  // Limit digit count so we don't get a false sense of resolution
  uint8_t i = 0;
  while ((i < dp) && (digits < 9)) // while (i < dp) for no limit but array size must be increased
  {
    i++;
    floatNumber *= 10;       // for the next decimal
    temp = floatNumber;      // get the decimal
    itoa(temp, str + ptr);
    ptr++; digits++;         // Increment pointer and digits count
    floatNumber -= temp;     // Remove that digit
  }

  print_CharString(str, color, size_x, size_y);
}
//----------------------------------------------------------------------------------------------------------------
//================================================================================================================================================


//Load picture
//================================================================================================================================================

//----------------------------------------------------------------------------------------------------------------
void load_imag(uint8_t x0, uint8_t y0, uint16_t pic_width, uint16_t pic_height, const uint16_t *picture)
{
  while(x0 <= pic_height + x0) 
  {
    while(y0 <= pic_width)
    {
      ST7735_DrawPixel(x0, y0, *picture);
      picture++;
      y0++;
    }
    y0 = 0;
    x0++;
  } 
}
//----------------------------------------------------------------------------------------------------------------