#ifndef KY023_H_
#define KY023_H_

//Includes
//=============================================================================================================================
#include "stm32f4xx_hal.h"
#include "st7735.h"
#include "button.h"
//=============================================================================================================================


//Declarations and definitions
//=============================================================================================================================
#define CNST_DELAY 10
//=============================================================================================================================


// description of button parameter
//-----------------------------------------------------------------------------
typedef struct c_KY023
{
	GPIO_TypeDef *portVRX;					// number of port to which the VRX is connetced
  uint16_t pinVRX;								// number of pin to which the VRX is connected
	uint32_t valVRX;								// adc value of pin to which the VRX is connected
	
	GPIO_TypeDef *portVRY;					// number of port to which the VRY is connetced
  uint16_t pinVRY;								// number of pin to which the VRY is connected
	uint32_t valVRY;								// adc value of pin to which the VRY is connected
	
	//button
	GPIO_TypeDef *portSW;						// number of port to which the SW is connetced
  uint16_t pinSW;									// number of pin to which the SW is connected
	t_Bttn bttnSW;
	buttonAction resMltKY023;
	
}t_KY023;

extern uint8_t efforts[];
//-----------------------------------------------------------------------------

void KY023_Init(t_KY023 *ptrky023, 
								GPIO_TypeDef *_portVRX, uint16_t _pinVRX, 
								GPIO_TypeDef *_portVRY,  uint16_t _pinVRY, 
								GPIO_TypeDef *_portSW,  uint16_t _pinSW);

void KY023_Processing(t_KY023 *ptrky023, uint16_t time);
void KY023_GetEffortVRXVRY(t_KY023 *ptrky023);

#endif // #ifndef KY023_H_