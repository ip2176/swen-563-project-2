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

/*
  Helper function to enable the capture bit on the TIM3 or TIM4 device so
  it starts counting
*/
void start_timer(int servo_num);

/*
	This helper function starts both of the timers
*/
void start_timers(void);


/*
  Helper function to disable the capture bit on the TIM3 or TIM4 device so
  it stops counting
*/
void stop_timer(int servo_num);

/*
	This helper function starts both of the timers
*/
void stop_timers(void);


/*
  Helper function to take care of initializing the timer registers
	for timing the servos.  Loads the prescaler value (forces the load),
	then enables capture input
*/
void servo_timers_init(void);

/*
  Helper function that returns the value of the current
  count in the TIM3 or TIM4 results register

  Output: An unsigned 16bit integer that represents the value
          in the register, this value is the millisecond
          representation of the current measurement of the time
*/
uint16_t get_current_measurement(int servo_num);

/*
  Helper function that returns the value of the current
  count in the TIM3 or TIM4 clock count register

  Output: An unsigned 16bit integer that represents the value
          in the register, this value is the millisecond
          representation of the current measurement of the count
*/
uint16_t get_current_time(int servo_num);
