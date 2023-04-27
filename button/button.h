#ifndef BUTTON_H_
#define BUTTON_H_

//Includes
//=============================================================================================================================
#include "stm32f4xx_hal.h"
#include "st7735.h"
//=============================================================================================================================

//Declarations and definitions
//=============================================================================================================================
#define GPIO_BUTTON_NOT_PRESS                       (GPIO_PIN_SET)
#define GPIO_BUTTON_PRESS                           (GPIO_PIN_RESET)

//step of measuring the button press time																	
#define BUTTON_PRESS_TIME 													40		// button bounce time
#define BUTTON_SHORT_PRESS_TIME 										120 	// short press time
#define BUTTON_LONG_PRESS_TIME 											1000	// long press time
#define MAX_PRESS_DURATION BUTTON_LONG_PRESS_TIME			    //maximum possible pressing time

// state of the button
//-----------------------------------------------------------------------------
typedef enum  
{
  buttonNotPress,       // button not press
  buttonPress,     			// button press
  buttonShortPress,			// short press
  buttonLongPress,  		// long press
	buttonDoublePress,    // double press
}buttonAction;
//-----------------------------------------------------------------------------

// functional definition of button
//-----------------------------------------------------------------------------
typedef enum
{
	buttonMultTask,				// five state of button
	buttonSinglTask				// two state of button
}buttonFunctional;
//-----------------------------------------------------------------------------

// description of button parameter
//-----------------------------------------------------------------------------
typedef struct c_Bttn
{
	GPIO_TypeDef *port;					// number of port to which the button is connected
  uint16_t pin;								// number of pin to which the button is connected
	
	uint8_t flagPress;					// multiple sending of the code "buttonPress" by 
															// pressing the button is excluded
	
	buttonAction stat;					// button state
	
	buttonFunctional buttFunc;	// functional definition of button 
	uint8_t flgFstAssigment;		// check the first function assignment
	
	uint16_t pressingTime; 			// measurement of press duration 
	uint8_t cntButtonPress;			// button press counter
 
}t_Bttn;
//-----------------------------------------------------------------------------

//=============================================================================================================================


//Functions 
//================================================================================================================================================

//@brief														Function initializes the single tasking button parameters and writes them to the structure.
//																	initializes parameters:
//																	port of button
//																	pin of button
//@param *ptrbttn										pointer to button object				
//@param *port											port of button
//@param pin 												pin of button
void init_button(t_Bttn *ptrbttn, GPIO_TypeDef *_port, uint16_t _pin);


//@brief														Processing of button state:
//																	not press
//																	short press
//																	long press
//																	double press
//																	triple press
//@param *bttn											pointer to button
//@time															time step
//@return 													button action
buttonAction buttonMultProcessing(t_Bttn *bttn, uint16_t time);


//@brief														Processing of button state:
//																	not press
//																	short press
//@param *bttn											pointer to button
//@time															time step
//@return 													button action
buttonAction buttonSinglProcessing(t_Bttn *bttn, uint16_t time);
//================================================================================================================================================

#endif // #ifndef BUTTON_H_
