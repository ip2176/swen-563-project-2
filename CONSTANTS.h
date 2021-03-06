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

// Operational codes, these should only change the last three bits in a byte
#define MOV (32)																				 // This represents: 0b001_00000
#define WAIT (64)																				 // This represents: 0b010_00000
#define LOOP (128)																 			 // This represents: 0b100_00000
#define END_LOOP (160)																	 // This represents: 0b101_00000
#define RECIPE_END (0)																	 // This represents: 0b000_00000
#define ERRONEOUS (192)																	 // This represents: 0b110_00000

// We need to use one byte to define both our operational code and
// the parameter passed to it, so use the last three bytes to define
// the operational code, and the first five to define the parameter
#define OPERATIONAL_CODE_MASK (224) 										 // This represents the following: 0b11100000
#define PARAMETER_MASK (31) 														 // This represents the following: 0b00011111

// General defines
#define MAX_DELAY (1000)																 // The maximum delay time possible for moving a servo
#define SERVO_0 (0)																			 // Used when selecting timer related items for servo 0
#define SERVO_1 (1)																			 // Used when selecting timer related items for servo 1
#define RESTART (1)																			 // Used when fixing up the radio data when a b is entered
#define NO_RESTART (0)																	 // Used when fixing up the radio data when a b is entered
#define RECIPE_MOVE (1)																	 // Used when moving the servo in a recipe, used for calulating delay
#define NON_RECIPE_MOVE (0)															 // Used when moving the servo outside a recipe, used for calulating delay
#define INSIDE_RECIPE_LOOP (1)													 // Used to determine if we are inside a recipe loop
#define LOOP_END_COUNT (0)															 // Used to determine the of the end of a recipe loop
#define RECIPE_LOOP_MODIFIER (1)												 // We subtract one from the loop count to get the length not the size
#define MICROSECOND_CONVERSION (10000)									 // Used for determining the delay time in Helper.c
#define WAIT_TIME_CONVERSION (100)											 // Used for the WAIT opcode (represents 1/10 of a second)
#define MAX_RECIPE_SIZE (100)														 // Used to determine the maximum recipe size
#define NUMBER_OF_RECIPES (6)													   // The number of test recipes
#define NUMBER_OF_SERVOS (2)														 // The number of motors we can move
#define OUTPUT_BUFFER_SIZE (2000)                        // Arbitrary bad code, I know
#define COMMAND_BUFFER_SIZE (2)													 // Used for setting the current command for each motor 
#define SUCCESS (1)                                      // Used for some int returning functions
#define FAILURE (0)                                      // Used for some int returning functions
#define VALID_X  ("Xx")                                  // Used to check if the user enered X's into the prompt
#define VALID_P  ("Pp")                                  // Used to check if the user enered p's into the prompt
#define VALID_Y  ("Yy")                           			 // Used to check if the user wants to continue recipe execution after error
#define VALID_YES_NO  ("YyNn")                           // Used to check if the user wants to continue recipe execution after error
#define ASCII_NEWLINE  (13)                              // Used to check for newlines
#define ASCII_BACKSPACE (127)	   												 // Used to check for backspaces
#define ASCII_TERMINAL_CHARACTER (62)										 // This character is used for the input terminal
#define PRINT_NEWLINE (1)                                // Tells the real time printer to print a newline
#define NO_NEWLINE (0)                                   // Tells the real time printer not to print a newline
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

// Define our CHAN 1 Timer, used for timing the 
// movement of servo 0
// Defines for the TIM3 timer to make the constants more human readbale
#define TIMER_3_ENABLE (RCC_APB1ENR1_TIM3EN)             // This allow us to enable the TIM3 timer
#define TIMER_3 (RCC->APB1ENR1)                          // The timer 2 register
#define TIMER_3_PRESCALER (TIM3->PSC)                    // The timer 2 presclater 
#define TIMER_3_PRESCALER_ENABLE (TIM3->EGR)             // Memory are for enabling the prescaler for TIM3
#define TIMER_3_PRESCALER_FORCE_LOAD (TIM_EGR_UG)        // Force the reload of the precaler value
#define TIMER_3_CAPTURE_INPUT (TIM3->CCER)               // The TIM3 input memory area
#define TIMER_3_CAPTURE_COMPARE (TIM3->CCMR1)            // The TIM3 capture/compare memory area
#define TIMER_3_CAPTURE  (TIM3->CR1)                     // The memory area to enable capturing
#define TIMER_3_CLOCK_TIMER_RESULTS (TIM3->CCR1)         // The TIM3 clock (where we get the measurement results)
#define TIMER_3_COUNT (TIM3->CNT)                        // The TIM3 count
#define TIMER_3_CAPTURE_SET_TO_INPUT (ENABLE)            // Set capture compare to input
#define TIMER_3_ENABLE_INPUT_CAPTURE (ENABLE)            // Enable input capture
#define TIMER_3_DISABLE_INPUT_CAPTURE (DISABLE)          // Disable input capture
#define TIMER_3_4_PRESCLAER_RATIO (8000)                 // The TIM3 and TIM4 timer starts in 80Mhz so divide it by 80 so we match the GPIO clock rate of 1Mhz

// Define our CHAN 2 Timer, used for timing the 
// movement of servo 1
// Defines for the TIM4 timer to make the constants more human readbale
#define TIMER_4_ENABLE (RCC_APB1ENR1_TIM4EN)             // This allow us to enable the TIM4 timer
#define TIMER_4 (RCC->APB1ENR1)                          // The timer 4 register
#define TIMER_4_PRESCALER (TIM4->PSC)                    // The timer 4 presclater 
#define TIMER_4_PRESCALER_ENABLE (TIM4->EGR)             // Memory are for enabling the prescaler for TIM4
#define TIMER_4_PRESCALER_FORCE_LOAD (TIM_EGR_UG)        // Force the reload of the precaler value
#define TIMER_4_CAPTURE_INPUT (TIM4->CCER)               // The TIM4 input memory area
#define TIMER_4_CAPTURE_COMPARE (TIM4->CCMR1)            // The TIM4 capture/compare memory area
#define TIMER_4_CAPTURE  (TIM4->CR1)                     // The memory area to enable capturing
#define TIMER_4_CLOCK_TIMER_RESULTS (TIM4->CCR1)         // The TIM4 clock (where we get the measurement results)
#define TIMER_4_COUNT (TIM4->CNT)                        // The TIM4 count
#define TIMER_4_CAPTURE_SET_TO_INPUT (ENABLE)            // Set capture compare to input
#define TIMER_4_ENABLE_INPUT_CAPTURE (ENABLE)            // Enable input capture
#define TIMER_4_DISABLE_INPUT_CAPTURE (DISABLE)          // Disable input capture


// Defines for the GPIO to make the constants more human readbale
#define GPIO_CLOCK_ENABLE (RCC_AHB2ENR_GPIOAEN)          // This allows us to enable the GPIO timer
#define GPIO_CLOCK (RCC->AHB2ENR)                        // The GPIO clock
#define GPIO_A_PINS (GPIOA->MODER)                       // The GPIO A pin set
#define GPIO_ALTERNATE_FUNCTION_MODE (0xA)               // For GPIO A 0 pin, this allows us to enable in alternate function mode for pins PA0 and PA1
#define GPIO_PA0_ALTERNATE_FUNCTION (GPIOA->AFR[0])      // GPIO PIN A 0 alternate function
#define GPIO_A_PA0_PA1_ALETERNATE_FUNCTION_ENABLE (0x11) // Enable alternate function on PA0 and PA1

// Defines for the degrees the motor can move
#define ZERO_DEGREES (5)
#define THIRY_TWO_DEGREES (7)
#define SIXTY_FOUR_DEGREES (10)
#define NINETY_SIX_DEGREES (13)
#define ONE_HUNDRED_AND_TWENTY_EIGHT_DEGREES (16)
#define ONE_HUNDRED_AND_SIXTY_DEGREES (19)

// Default servo_data values
#define RECIPE_INDEX_DEFAULT (0)
#define RECIPE_INSTRUCTION_INDEX_DEFAULT (0)
#define INSIDE_RECIPE_LOOP_DEFAULT (0)
#define RECIPE_LOOP_COUNT_DEFAULT (0)
#define RECIPE_LOOP_INDEX_DEFAULT (0)
#define LAST_START_TIME_DEFAULT (0)
#define TARGET_POSITION_DEFAULT (zero_degrees)
#define TOTAL_DELAY_DEFAULT (0)

// This was taken from here: https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
// Used for printing in binary format
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
	(byte & 0x80 ? '1' : '0'), \
	(byte & 0x40 ? '1' : '0'), \
	(byte & 0x20 ? '1' : '0'), \
	(byte & 0x10 ? '1' : '0'), \
	(byte & 0x08 ? '1' : '0'), \
	(byte & 0x04 ? '1' : '0'), \
	(byte & 0x02 ? '1' : '0'), \
	(byte & 0x01 ? '1' : '0') 

// Use these defines for calculating servo delays
#define ONE_STEP_SERVO_DELAY ((uint16_t)200) 						 // The general time to move a servo one step (in milliseconds)
#define RECIPE_SERVO_DELAY ((uint16_t)1000)						   // The recipes need a different scale for the time

// Keep track of the state of the servo
typedef enum {
	inactive,
	active,
	paused,
} servo_status;

// Keep track of the status of the servos while in a recipe
typedef enum {
	idle,
	running,
} recipe_status;

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
	servo_status status;					// This tells us the current state of the servo (paused, or running)
	position position;						// This tells us the current position each servo is in	
	int recipe_index;							// This tells us which recipe we are on
	int recipe_instruction_index; // This tells us which instruction inside of the current recipe we are on
	int recipe_loop_count;				// This tells us how many times we have looped in a recipe
	int inside_recipe_loop;				// This tells us if we are inside a loop in a recipe
	int recipe_loop_index;				// This tells us where we are in each loop inside the recipe
	uint16_t last_start_time;     // This tells us the last time a motor started moving
	position target_position;			// This is used when calculating if the motor is ready to move again yet
	uint16_t total_delay;					// This is used when calculating if the motor is ready to move again yet
	recipe_status recipe_status;  // Used to keep track of the servos while executing recipes
} servo_data;

// Use a struct to contain the current opcode and parameter while processing
// recipes
typedef struct{
	uint8_t opcode;  					// This is the opcode for the current recipe action
	uint8_t parameter;				// This is the parameter for the opcode action
} current_instruction;

// Define the array that we will carry our pulse width data in
extern int positions[END_OF_POSITION_ARRAY];																										

#endif
