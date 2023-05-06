## Функции библиотеки button.h
### void init_button(t_Bttn *ptrbttn, GPIO_TypeDef *_port, uint16_t _pin)  
Функция инициализирует такие параметры кнопки как: порт, пин и записывает их в структуру t_Bttn.
- **Параметры**  
**ptrbttn:** адрес переменной типа t_Bttn   
**_port:** указатель на порт подключаемой кнопки  
**_pin:** пин подключаемой кнопки

### buttonAction buttonMultProcessing(t_Bttn *bttn, uint16_t time)  
Функция вычисляет состояние подключенной кнопки и возвращает его. Возможные сосотаяния кнопки:  
*buttonNotPress* - кнопка не нажата   
*buttonShortPress* - короткое нажатие кнопки(120 милли секунд)  
*buttonLongPress* - долгое нажатие кнопки(1 секунда)  
*buttonDoublePress* - двойное нажатие кнопки(2 нажатия кнопки за время меньшее 120 милли секунд)
- **Параметры**  
**ptrbttn:** адрес переменной типа t_Bttn   
**time:** шаг времени  

### buttonAction buttonSinglProcessing(t_Bttn *bttn, uint16_t time)  
Функция вычисляет состояние подключенной кнопки и возвращает его. Возможные сосотаяния кнопки:  
*buttonNotPress* - кнопка не нажата   
*buttonShortPress* - короткое нажатие кнопки(120 милли секунд)  
- **Параметры**  
**ptrbttn:** адрес переменной типа t_Bttn   
**time:** шаг времени  

## Инициализация
Создать переменную типа t_Bttn
```C
t_Bttn snglButton;
t_Bttn mltButton;
```
Вызвать функцию инициализации.
```C
init_button(&snglButton, GPIOA, GPIO_PIN_10);
init_button(&mltButton, GPIOD, GPIO_PIN_10);
```
Создать до цикла while(1) переменную результата обработки кнопки
```C
buttonAction resSnglBttn;
buttonAction resMltBttn;
```
В цикле while вызвать функцию buttonSinglProcessing, для вычисления состояния батареи.
```C
while (1)
{
  resSnglBttn = buttonSinglProcessing(&snglButton, 10);
	resMltBttn = buttonMultProcessing(&mltButton, 10);
  
  //Singl	
	if(resSnglBttn == buttonShortPress)
	{
		//do something
	}
		
	//Mult
	if(resMltBttn == buttonShortPress)
	{
		//do something
	}
	if(resMltBttn == buttonLongPress)
	{
		//do something
	}
	if(resMltBttn == buttonDoublePress)
	{
		//do something
	}
		
	HAL_Delay(10);
}
```
