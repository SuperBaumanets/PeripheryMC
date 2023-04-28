#include "button.h"
#include "st7735.h"

//Function
//=============================================================================================================================

// Button initialization
//-------------------------------------------------------------------------
void init_button(t_Bttn *ptrbttn, GPIO_TypeDef *_port, uint16_t _pin)
{
	ptrbttn -> port = _port;
	ptrbttn -> pin = _pin;
	
	ptrbttn -> flagPress = 0;
	ptrbttn -> flgFstAssigment = 0;
	ptrbttn -> pressingTime = 0;
	ptrbttn -> cntButtonPress = 0;
}
//-------------------------------------------------------------------------

// Multifunction button state calculation
//-------------------------------------------------------------------------
buttonAction buttonMultProcessing(t_Bttn *bttn, uint16_t time)
{
	if(bttn -> flgFstAssigment == 0)
	{	
		// set button functional of button
		bttn -> buttFunc = buttonMultTask;
		bttn -> flgFstAssigment = 1;
	}
	
	if(bttn -> buttFunc == buttonSinglTask){
		return buttonNotPress;
	}
	//for storage of button click code
  buttonAction temp = buttonNotPress;
		
	uint8_t current_state = HAL_GPIO_ReadPin(bttn -> port, bttn -> pin);
	
  //if button press
  if(current_state == GPIO_BUTTON_PRESS)
	{
    //counting the time of pressing
    bttn -> pressingTime += time;
	
    //overflow protection
    if(bttn -> pressingTime >= MAX_PRESS_DURATION)
			bttn -> pressingTime  = MAX_PRESS_DURATION;
    
    //check debounce
    if(bttn -> pressingTime >= BUTTON_PRESS_TIME && bttn -> flagPress == 0)
		{
			bttn -> stat = buttonPress;
			temp = bttn -> stat;
			
      bttn -> flagPress = 1;
			
			bttn -> cntButtonPress++;
    }
  }
  //if button is not pressed
  else
	{
    // check how long the button was pressed
    if(bttn -> pressingTime >= BUTTON_LONG_PRESS_TIME)
		{
			bttn -> stat = buttonLongPress;
		  temp = bttn -> stat;
			
			bttn -> cntButtonPress = 0;
		}
    else if(bttn -> pressingTime >= BUTTON_SHORT_PRESS_TIME)
		{
			bttn -> stat = buttonShortPress;
		  temp = bttn -> stat;
			
			bttn -> cntButtonPress = 0;
		}
		// check how many times the button was pressed
		else if(bttn -> cntButtonPress == 2 && bttn -> pressingTime < BUTTON_SHORT_PRESS_TIME)
		{
			bttn -> stat = buttonDoublePress;
			temp = bttn -> stat;
			
			bttn -> cntButtonPress = 0;
		}
    // reset for next measurement
    bttn -> pressingTime = 0;
    bttn -> flagPress = 0;
  }

  // return result of button processing
  return temp;
}
//-------------------------------------------------------------------------

// Singlefunction button state calculation
//-------------------------------------------------------------------------
buttonAction buttonSinglProcessing(t_Bttn *bttn, uint16_t time)
{	
	if(bttn -> flgFstAssigment == 0)
	{	
		// set button functional of button
		bttn -> buttFunc = buttonSinglTask;
		bttn -> flgFstAssigment = 1;
	}
	
	if(bttn -> buttFunc == buttonMultTask){
		return buttonNotPress;
	}
	
	//for storage of button click code
  buttonAction temp = buttonNotPress;
		
	uint8_t current_state = HAL_GPIO_ReadPin(bttn -> port, bttn -> pin);
	
  //if button press
  if(current_state == GPIO_BUTTON_PRESS)
	{
    //counting the time of pressing
    bttn -> pressingTime += time;
	
    //overflow protection
    if(bttn -> pressingTime >= MAX_PRESS_DURATION)
			bttn -> pressingTime  = MAX_PRESS_DURATION;
    
    //check debounce
    if(bttn -> pressingTime >= BUTTON_PRESS_TIME && bttn -> flagPress == 0)
		{
			bttn -> stat = buttonPress;
			temp = bttn -> stat;
			
      bttn -> flagPress = 1;
    }
  }
  //if button is not pressed
  else
	{
    // check how long the button was pressed
		if(bttn -> pressingTime >= BUTTON_SHORT_PRESS_TIME)
		{
			bttn -> stat = buttonShortPress;
		  temp = bttn -> stat;
		}
		
    // reset for next measurement
    bttn -> pressingTime = 0;
    bttn -> flagPress = 0;
  }

  // return result of button processing
  return temp;
}

//=============================================================================================================================