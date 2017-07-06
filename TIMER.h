/*
  Function declarations for the TIM2 functions
*/

#include "stm32l476xx.h"
#include "CONSTANTS.h"

/*
	This function handles enabling the TIM2 channels as ouputs and setting the
	prescaler to the right value (8000) in this case to get the right duty cycle
	for movement
*/
void timer_init(void);
