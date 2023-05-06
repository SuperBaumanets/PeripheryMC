### Подтягивающие резисторы
В данной библиотеке используется подтяжка резистора вверх(Pull-up).  
При нажатии кнопки - *низкий уровень*  
При отпускании кнопки - *высокий уровень*  
```C
#define GPIO_BUTTON_NOT_PRESS                       (GPIO_PIN_SET)
#define GPIO_BUTTON_PRESS                           (GPIO_PIN_RESET)
```
При желание использовать подтяжку вниз (pull-down) необходимо изменить конфигурация GPIO Pull-up/Pull-down в CubeMX.  
Или непосредственно main.c изменить GPIO_InitStruct.Pull с GPIO_PULLUP на GPIO_PULLDOWN;
```C
/*Configure GPIO pin : PD10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP; //GPIO_PULLDOWN 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
```
