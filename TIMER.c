/*
  The timer file is used to encapsulate the functions that relate to using the TIM2 register
*/

#include "TIMER.h"

// Define our positions here so they propegate up to main.c
int positions[END_OF_POSITION_ARRAY] = {
	ZERO_DEGREES, 
	THIRY_TWO_DEGREES,
	SIXTY_FOUR_DEGREES,
	NINETY_SIX_DEGREES,
	ONE_HUNDRED_AND_TWENTY_EIGHT_DEGREES,
	ONE_HUNDRED_AND_SIXTY_DEGREES
};

/*
	This function handles enabling the TIM2 channels as ouputs and setting the
	prescaler to the right value (8000) in this case to get the right duty cycle
	for movement
*/
void timer_init(){

	TIMER_2 |= TIMER_2_ENABLE;																						// Enable timer 2
	TIMER_2_PRESCALER = TIMER_2_PRESCLAER_RATIO;		 						          // Set the prescaler value for timer 2
	TIMER_2_CAPTURE_COMPARE &= TIMER_2_OUTPUT_MODE_CHAN1_CHAN2;  				  // Set CHAN1 and CHAN2 of TIM2 to output mode
	TIMER_2_CAPTURE_COMPARE |= TIMER_2_CHAN1_CHAN2_ENABLE_OUTPUT_COMPARE; // Set TIM2 CHAN1 and CHAN2 to output compare and preload
	TIMER_2_CAPTURE |= TIMER_2_AUTOLOAD_PRELOAD;  												// Autoload the preloaded values
	TIMER_2_CAPTURE_INPUT |= TIMER_2_CHAN1_CHAN2_ENABLE_OUTPUT; 					// Enable channel 1 and 2 as output
	TIMER_2_ARR_PRESCALER = TIMER_2_SECOND_PRESCALE_RATIO;							  // Scale the TIM2 value down again to match the GPIO frequency
	
	// Initialize the servo positions, so we know where they should point when
	// we start the program
	TIMER_2_MOTOR_1 = positions[zero_degrees];
	TIMER_2_MOTOR_2 = positions[zero_degrees];

	// Force the load of the prescaler values
	TIMER_2_PRESCALER_ENABLE |= TIMER_2_PRESCALER_FORCE_LOAD;  

	// Finally enable the timer register
	TIMER_2_CAPTURE = TIMER_2_ENABLE_INPUT_CAPTURE;
}

/*
  Helper function to take care of initializing the timer registers
	for timing the servos.  Loads the prescaler value (forces the load),
	then enables capture input
*/
void servo_timers_init(){

  // Enable the TIM3 clock
  TIMER_3 |= TIMER_3_ENABLE;
  TIMER_4 |= TIMER_4_ENABLE;

  // Adjust the TIM3 device
  TIMER_3_PRESCALER          = TIMER_3_4_PRESCLAER_RATIO;      // Load a prescale value to make the TIM3 clock rate match the GPIO clock rate
  TIMER_3_PRESCALER_ENABLE   |= TIMER_3_PRESCALER_FORCE_LOAD;  // Force the board to load the prescaler value and match the clock rates
  TIMER_3_CAPTURE_INPUT      &= CLEAR;                         // Clear the input register
  TIMER_3_CAPTURE_COMPARE    |= TIMER_3_CAPTURE_SET_TO_INPUT;  // Make sure the capture channel is set to input mode
  TIMER_3_CAPTURE_INPUT      |= TIMER_3_ENABLE_INPUT_CAPTURE;   // Enable capturing data on TIM3

  // Adjust the TIM4 device
  TIMER_4_PRESCALER          = TIMER_3_4_PRESCLAER_RATIO;      // Load a prescale value to make the TIM4 clock rate match the GPIO clock rate
  TIMER_4_PRESCALER_ENABLE   |= TIMER_4_PRESCALER_FORCE_LOAD;  // Force the board to load the prescaler value and match the clock rates
  TIMER_4_CAPTURE_INPUT      &= CLEAR;                         // Clear the input register
  TIMER_4_CAPTURE_COMPARE    |= TIMER_4_CAPTURE_SET_TO_INPUT;  // Make sure the capture channel is set to input mode
  TIMER_4_CAPTURE_INPUT      |= TIMER_4_ENABLE_INPUT_CAPTURE;   // Enable capturing data on TIM4
}

/*
  Helper function to enable the capture bit on the TIM3 or TIM4 device so
  it starts counting
*/
void start_timer(int servo_num){
	if(servo_num == SERVO_0){
		TIMER_3_CAPTURE |= TIMER_3_ENABLE_INPUT_CAPTURE;
	}
	else {
		TIMER_4_CAPTURE |= TIMER_4_ENABLE_INPUT_CAPTURE;
	}
}

/*
	This helper function starts both of the timers
*/
void start_timers(){
	for(int servo_num = 0; servo_num < NUMBER_OF_SERVOS; servo_num++){
		start_timer(servo_num);
	}
}

/*
  Helper function to disable the capture bit on the TIM3 or TIM4 device so
  it stops counting
*/
void stop_timer(int servo_num){
	if(servo_num == SERVO_0){
		TIMER_3_CAPTURE &= TIMER_3_DISABLE_INPUT_CAPTURE;
	}
	else {
		TIMER_4_CAPTURE &= TIMER_4_DISABLE_INPUT_CAPTURE;
	}
}

/*
	This helper function stops both of the timers
*/
void stop_timers(){
	for(int servo_num = 0; servo_num < NUMBER_OF_SERVOS; servo_num++){
		stop_timer(servo_num);
	}
}

/*
  Helper function that returns the value of the current
  count in the TIM3 or TIM4 results register

  Output: An unsigned 16bit integer that represents the value
          in the register, this value is the millisecond
          representation of the current measurement of the time
*/
uint16_t get_current_measurement(int servo_num){
	if(servo_num == SERVO_0){
		return (uint16_t)TIMER_3_CLOCK_TIMER_RESULTS;
	}
	else {
		return (uint16_t)TIMER_4_CLOCK_TIMER_RESULTS;
	}
}

/*
  Helper function that returns the value of the current
  count in the TIM3 or TIM4 clock count register

  Output: An unsigned 16bit integer that represents the value
          in the register, this value is the millisecond
          representation of the current measurement of the count
*/
uint16_t get_current_time(int servo_num){
	if(servo_num == SERVO_0){
		return (uint16_t)TIMER_3_COUNT;
	}
	else {
		return (uint16_t)TIMER_4_COUNT;
	}
}
