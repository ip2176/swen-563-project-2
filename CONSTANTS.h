/*
  This file contains all fo the constant definitions used for this project.  They are
  consolidated for easy reading and access
*/
#ifndef _CONSTANTS_
#define _CONSTANTS_

// General imports used in various files
#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Operational codes, these should only change the last three bits in a byte
#define MOV (32)																				 // This represents: 0b001_00000
#define WAIT (64)																				 // This represents: 0b010_00000
#define LOOP (128)																 			 // This represents: 0b100_00000
#define END_LOOP (160)																	 // This represents: 0b101_00000
#define RECIPE_END (0)																	 // This represents: 0b000_00000

// We need to use one byte to define both our operational code and
// the parameter passed to it, so use the last three bytes to define
// the operational code, and the first five to define the parameter
#define OPERATIONAL_CODE_MASK (224) 										 // This represents the following: 0b11100000
#define PARAMETER_MASK (31) 														 // This represents the following: 0b00011111

// General defines
#define ONE_THOUSAND (1000)															 // Used for determining the delay time in Timer.c
#define MAX_RECIPE_SIZE (100)														 // Used to determine the maximum recipe size
#define NUMBER_OF_RECIPES (4)													   // The number of test recipes
#define NUMBER_OF_SERVOS (2)														 // The number of motors we can move
#define OUTPUT_BUFFER_SIZE (1000)                        // Arbitrary bad code, I know
#define COMMAND_BUFFER_SIZE (2)													 // Used for setting the current command for each motor 
#define SUCCESS (1)                                      // Used for some int returning functions
#define FAILURE (0)                                      // Used for some int returning functions
#define VALID_X  ("Xx")                                  // Used to check if the user enered X's into the prompt
#define ASCII_NEWLINE  (13)                              // Ugh, have to check for newlines
#define ASCII_BACKSPACE (127)	   												 // Used to check for backspaces
#define ASCII_TERMINAL_CHARACTER (62)												 	 // This character is used for the input terminal
#define PRINT_NEWLINE (1)                                // Tells the real time printer to print a newline
#define NO_NEWLINE (0)                                   // Tellks the real time printer not to print a newline
#define REAL_TIME_BUFFER_SIZE (1)                        // Used to output the users input in real time
#define REAL_TIME_BUFFER_START (0)                       // Used for printing out real time data as its entered in
#define CARRIAGE_RETURN_NEWLINE ("\r\n")                 // Used in strings in the program
#define DASHES ("--------------------------------------------------------------------------------") // Used to make printing look nice

// General use 
#define CLEAR   (~(0xFFFFFFFF))                          // Constant to clear a 32 bit register
#define ENABLE  (0x1)                                    // Enable constant
#define DISABLE (0x0)                                    // Disable constant

// Defines for the TIM2 timer to make the constants more human readbale
#define TIMER_2_ENABLE (RCC_APB1ENR1_TIM2EN)             // This allow us to enable the TIM2 timer
#define TIMER_2 (RCC->APB1ENR1)                          // The timer 2 register
#define TIMER_2_PRESCALER (TIM2->PSC)                    // The timer 2 presclater 
#define TIMER_2_PRESCALER_ENABLE (TIM2->EGR)             // Memory are for enabling the prescaler for TIM2
#define TIMER_2_PRESCALER_FORCE_LOAD (TIM_EGR_UG)        // Force the reload of the precaler value
#define TIMER_2_CAPTURE_INPUT (TIM2->CCER)               // The TIM2 input memory area
#define TIMER_2_CAPTURE_COMPARE (TIM2->CCMR1)            // The TIM2 capture/compare memory area
#define TIMER_2_CAPTURE  (TIM2->CR1)                     // The memory area to enable capturing
#define TIMER_2_MOTOR_1 (TIM2->CCR1)         						 // The TIM2 clock (where we enter in how long to move the first motor)
#define TIMER_2_MOTOR_2 (TIM2->CCR2)         						 // The TIM2 clock (where we enter in how long to move the second motor)
#define TIMER_2_ENABLE_INPUT_CAPTURE (ENABLE)            // Enable input capture
#define TIMER_2_DISABLE_INPUT_CAPTURE (DISABLE)          // Disable input capture
#define TIMER_2_PRESCLAER_RATIO (8000)                   // The TIM2 timer starts in 80Mhz so divide it by 8000 so we match pulse ratio of 1 KHz
#define TIMER_2_OUTPUT_MODE_CHAN1_CHAN2 (~(0x00000303))	 // Used to set TIM2 channels to output mode
#define TIMER_2_CHAN1_CHAN2_ENABLE_OUTPUT_COMPARE (0x00006868) // Used to set TIM2 CHAN1 and CHAN2 to output compare and preload
#define TIMER_2_ARR_PRESCALER (TIM2->ARR)								 // Used to hold the prescaler value (the bus frequecy is 2x the GPIO frequency, so we scale down again)
#define TIMER_2_SECOND_PRESCALE_RATIO (200)							 // See the above explanation, scale the frequency down again so it matches the GPIO frequency
#define TIMER_2_AUTOLOAD_PRELOAD (0x00000080)						 // Set TIM2 to autoload preloaded values
#define TIMER_2_CHAN1_CHAN2_ENABLE_OUTPUT (0x00000011)	 // Enable channel 1 and 2 as output on TIM2

// Defines for the GPIO to make the constants more human readbale
#define GPIO_CLOCK_ENABLE (RCC_AHB2ENR_GPIOAEN)          // This allows us to enable the GPIO timer
#define GPIO_CLOCK (RCC->AHB2ENR)                        // The GPIO clock
#define GPIO_A_PINS (GPIOA->MODER)                       // The GPIO A pin set
#define GPIO_ALTERNATE_FUNCTION_MODE (0xA)               // For GPIO A 0 pin, this allows us to enable in alternate function mode for pins PA0 and PA1
#define GPIO_PA0_ALTERNATE_FUNCTION (GPIOA->AFR[0])      // GPIO PIN A 0 alternate function
#define GPIO_A_PA0_PA1_ALETERNATE_FUNCTION_ENABLE (0x11) // Enable alternate function on PA0 and PA1

// Defines for the degrees the motor can move
#define ZERO_DEGREES (4)
#define THIRY_TWO_DEGREES (7)
#define SIXTY_FOUR_DEGREES (10)
#define NINETY_SIX_DEGREES (13)
#define ONE_HUNDRED_AND_TWENTY_EIGHT_DEGREES (16)
#define ONE_HUNDRED_AND_SIXTY_DEGREES (19)

// Keep track of the state of the servo
typedef enum {
	paused,
	active
} servo_status;

// This enumerator allows us to select positions without # defining each of them
typedef enum {
	zero_degrees,															// 0 degrees (4)
	thiry_two_degrees,												// 32 degrees (7)
	sixty_four_degrees,												// 64 degrees (10)
	ninety_six_degrees,												// 96 degrees (13)
	one_hundred_and_twenty_eight_degrees,			// 128 degrees (16)
	one_hundred_and_sixty_degrees,						// 160 degrees (19)
	END_OF_POSITION_ARRAY     								// This is just used because its easier to track the array size
} position;

// Keep track of various items that describe the state of the servo
typedef struct{
	servo_status status;			// This tells us the current state of the servo (paused, or running)
	position position;				// This tells us the current position each servo is in	
	int recipe_index;					// This tells us where we are in a recipe
	int recipe_loop_count;		// This tells us how many times we have looped in a recipe
	int inside_recipe_loop;		// This tells us if we are inside a loop in a recipe
	int recipe_loop_index;		// This tells us where we are in each loop inside the recipe
} servo_data;

// Define the array that we will carry our pulse width data in
extern int positions[END_OF_POSITION_ARRAY];																										

#endif
