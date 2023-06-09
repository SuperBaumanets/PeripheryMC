#include "ky023.h" 

#define DEBUG

//INIT
//================================================================================================================================================
extern ADC_HandleTypeDef hadc1;

static uint8_t cntGoToCalibrate;
static uint8_t inCalibrate;

static uint16_t valXmax = 4096;
static uint16_t valXmin = 0;
static uint16_t valXmean = 2048;

static uint16_t valYmax = 4096;
static uint16_t valYmin = 0;
static uint16_t valYmean = 2048;

uint8_t KY023_EffortsVRXVRY[4];
uint16_t KY023_CursorVRXVRY[4];

void KY023_Init(t_KY023 *ptrky023, 
								GPIO_TypeDef *_portVRX, uint16_t _pinVRX, 
								GPIO_TypeDef *_portVRY,  uint16_t _pinVRY, 
								GPIO_TypeDef *_portSW,  uint16_t _pinSW)
{
	ptrky023 -> portVRX = _portVRX;					
  ptrky023 -> pinVRX = _pinVRX;	
	ptrky023 -> valVRX = 0;
	
	ptrky023 -> portVRY = _portVRY;					
  ptrky023 -> pinVRY = _pinVRY;
	ptrky023 -> valVRY = 0;
	
	ptrky023 -> portSW = _portSW;					
  ptrky023 -> pinSW = _pinSW;
	
	t_Bttn _bttnSW;
	ptrky023 -> bttnSW = _bttnSW;
	ptrky023 -> resMltKY023 = buttonNotPress;
	init_button(&(ptrky023 -> bttnSW), ptrky023 -> portSW, ptrky023 -> pinSW);
	
	cntGoToCalibrate = 0;
	inCalibrate = 0;
	
	ptrky023 -> dpi  = 1;
}
//================================================================================================================================================


//SETTING
//================================================================================================================================================
void KY023_CalibrateVRXVRY(t_KY023 *ptrky023)
{
	if((ptrky023 -> valVRX) < valXmin)
		valXmin = ptrky023 -> valVRX;
	
	if((ptrky023 -> valVRX) > valXmax)
		valXmax = ptrky023 -> valVRX;
	
	if((ptrky023 -> valVRY) < valYmin)
		valYmin = ptrky023 -> valVRY;
	
	if((ptrky023 -> valVRY) > valYmax)
		valYmax = ptrky023 -> valVRY;
}


void KY023_SetOrigin(t_KY023 *ptrky023)
{
	
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ST7735_H_
void KY023_SetSensitivity(t_KY023 *ptrky023, uint8_t sensitivity)
{
	if(sensitivity > 10)
		ptrky023 -> dpi = 10;
	
	switch(sensitivity){
		case 1: ptrky023 -> dpi = 1;
			break;
		case 2: ptrky023 -> dpi = 2;
			break;
		case 3: ptrky023 -> dpi = 4;
			break;
		case 4: ptrky023 -> dpi = 8;
			break;
		case 5: ptrky023 -> dpi = 16;
			break;
		case 6: ptrky023 -> dpi = 32;
			break;
		case 7: ptrky023 -> dpi = 64;
			break;
		case 8: ptrky023 -> dpi = 128;
			break;
		case 9: ptrky023 -> dpi = 256;
			break;
		case 10: ptrky023 -> dpi = 512;
			break;
	}
}

uint8_t KY023_GetSensitivity(t_KY023 *ptrky023){
	return ptrky023 -> dpi;
}
#endif //define ST7735_H_


void KY023_SetButtonFunc()
{
	
}
//================================================================================================================================================


// PROCESSING
//================================================================================================================================================
void joystickProcessing(t_KY023 *ptrky023)
{
	HAL_ADCEx_InjectedStart(&hadc1); 
  HAL_ADC_PollForConversion(&hadc1,10); 
  ptrky023 -> valVRX = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
	
	HAL_ADCEx_InjectedStart(&hadc1); 
	HAL_ADC_PollForConversion(&hadc1,10); 
  ptrky023 -> valVRY = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
	
	#ifdef DEBUG
	ST7735S_SetCursor(0, 10, 0);
	ST7735S_PrintCharString("Current value:", ST7735_COLOR_WHITE, 1, 1);
	ST7735S_SetCursor(0, 20, 0);
	ST7735S_PrintCharString("X:", ST7735_COLOR_BLUE, 1, 1);
	ST7735S_SetCursor(16, 20, 0);
	ST7735S_PrintIntNum(ptrky023 -> valVRX, ST7735_COLOR_BLUE, 1, 1);
	ST7735S_SetCursor(0, 30, 0);
	ST7735S_PrintCharString("Y:", ST7735_COLOR_RED, 1, 1);
	ST7735S_SetCursor(16, 30, 0);
	ST7735S_PrintIntNum(ptrky023 -> valVRY, ST7735_COLOR_RED, 1, 1);
	#endif //DEBUG
	
	HAL_ADCEx_InjectedStop(&hadc1);
}

void buttonProcessing(t_KY023 *ptrky023, uint16_t time)
{
	ptrky023 -> resMltKY023 = buttonMultProcessing(&(ptrky023 -> bttnSW), time);
	
	//short press
	if(ptrky023 -> resMltKY023 == buttonShortPress)
	{
		//do something
	}
	
	//long press
	if(ptrky023 -> resMltKY023 == buttonLongPress)
	{
		ST7735S_DrawFillRectangle(0, 3, 1, 1, ST7735_COLOR_RED);
		cntGoToCalibrate++;
		if(cntGoToCalibrate == 2)
		{
			ST7735S_DrawFillRectangle(0, 0, 1, 1, ST7735_COLOR_YELLOW);
			
			inCalibrate = 1;
			valXmin = ptrky023 -> valVRX;
			valXmax = ptrky023 -> valVRX;
			valYmin = ptrky023 -> valVRY;
			valYmax = ptrky023 -> valVRY;
		}
		
		if(cntGoToCalibrate == 3)
		{
			ST7735S_DrawFillRectangle(0, 1, 1, 1, ST7735_COLOR_GREEN);
			
			valXmean = ptrky023 -> valVRX;
			valYmean = ptrky023 -> valVRY;
		}
		
		if(cntGoToCalibrate == 5)
		{
			inCalibrate = 0;
			cntGoToCalibrate = 0;
		}
	}
	
	//double press
	if(ptrky023 -> resMltKY023 == buttonDoublePress)
	{
		//do something
	}
}

void KY023_Processing(t_KY023 *ptrky023, uint16_t time)
{
	joystickProcessing(ptrky023);
	
	buttonProcessing(ptrky023, time);
	
	if(inCalibrate == 1)
	{
		KY023_CalibrateVRXVRY(ptrky023);
	}
}
//================================================================================================================================================


// User`s Functions
//================================================================================================================================================
#ifdef ST7735_H_
void KY023_CursorMode(t_KY023 *ptrky023)
{
	uint16_t lenXmin = valXmean - valXmin;
	uint16_t lenXmax = valXmax - valXmean;
	uint16_t lenYmin = valYmean - valYmin;
	uint16_t lenYmax = valYmax - valYmean;
	
	uint16_t stepDpiXmin = lenXmin / ptrky023 -> dpi;
	uint16_t stepDpiXmax = lenXmax / ptrky023 -> dpi;
	uint16_t stepDpiYmin = lenYmin / ptrky023 -> dpi;
	uint16_t stepDpiYmax = lenYmax / ptrky023 -> dpi;
	
	if(((ptrky023 -> valVRX) <= (valXmean + 5) ) && ((ptrky023 -> valVRX) >= (valXmean - 5) )){
		KY023_CursorVRXVRY[0] = 0;
		KY023_CursorVRXVRY[1] = 0;
	}
	else{
		if((ptrky023 -> valVRX) < valXmean){
			uint16_t curLenXmin = valXmean - (ptrky023 -> valVRX);
			KY023_CursorVRXVRY[0] = (float)(curLenXmin / stepDpiXmin) + 1;
			if(KY023_CursorVRXVRY[0] > ptrky023 -> dpi){
				KY023_CursorVRXVRY[0] = ptrky023 -> dpi; 
			}
		}
		else{
			uint16_t curLenXmax = (ptrky023 -> valVRX) - valXmean;
			KY023_CursorVRXVRY[1] = (float)(curLenXmax / stepDpiXmax) + 1;
			if(KY023_CursorVRXVRY[1] > ptrky023 -> dpi){
				KY023_CursorVRXVRY[1] = ptrky023 -> dpi; 
			}
		}
	}
	if(((ptrky023 -> valVRY) <= (valYmean + 5) ) && ((ptrky023 -> valVRY) >= (valYmean - 5) )){
		KY023_CursorVRXVRY[2] = 0;
		KY023_CursorVRXVRY[3] = 0;
	}
	else{
		if((ptrky023 -> valVRY) < valYmean){
			uint16_t curLenYmin = valYmean - (ptrky023 -> valVRY);
			KY023_CursorVRXVRY[2] = (float)(curLenYmin / stepDpiYmin) + 1;
			if(KY023_CursorVRXVRY[2] > ptrky023 -> dpi){
				KY023_CursorVRXVRY[2] = ptrky023 -> dpi; 
			}
		}
		else{
			uint16_t curLenYmax = (ptrky023 -> valVRY) - valYmean;
			KY023_CursorVRXVRY[3] = (float)(curLenYmax / stepDpiYmax) + 1;
			if(KY023_CursorVRXVRY[3] > ptrky023 -> dpi){
				KY023_CursorVRXVRY[3] = ptrky023 -> dpi; 
			}
		}
	}		
}
#endif //define ST7735_H_ 


uint16_t KY023_GetAngle(t_KY023 *ptrky023)
{
	
}


void KY023_GetEffortVRXVRY(t_KY023 *ptrky023)
{
	for(uint8_t i = 0; i < 4; i++)
		KY023_EffortsVRXVRY[i] = 0;
	
	if((ptrky023 -> valVRX) < valXmean){
		float lenXmin = valXmean - (ptrky023 -> valVRX);
		KY023_EffortsVRXVRY[0] = (float)((lenXmin / (valXmean - valXmin))) * 100;
		if(KY023_EffortsVRXVRY[0] > 100)
			KY023_EffortsVRXVRY[0] = 100;
	}
	else{
		float lenXmax = (ptrky023 -> valVRX) - valXmean;
		KY023_EffortsVRXVRY[1] = (float)(lenXmax / (valXmax - valXmean)) * 100;
		if(KY023_EffortsVRXVRY[1] > 100)
			KY023_EffortsVRXVRY[1] = 100;
	}
	
	if((ptrky023 -> valVRY) < valYmean){
		float lenYmin = valYmean - (ptrky023 -> valVRY);
		KY023_EffortsVRXVRY[2] = (float)((lenYmin / (valYmean - valYmin))) * 100;
		if(KY023_EffortsVRXVRY[2] > 100)
			KY023_EffortsVRXVRY[2] = 100;
	}
	else{
		float lenYmax = (ptrky023 -> valVRY) - valYmean;
		KY023_EffortsVRXVRY[3] = (float)(lenYmax / (valYmax - valYmean)) * 100;
		if(KY023_EffortsVRXVRY[3] > 100)
			KY023_EffortsVRXVRY[3] = 100;
	}	
}
//================================================================================================================================================




