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
