/*
  Header file for helper function definitions
*/

#include "USART_Helper.h"

#include <string.h>

/*
  Check the input string and see if we have a valid character in it.
  Used to check the user input

  Input: input - The string to check if it is in valid characters
         valid_characters - A string of valid characters input should
                            be checked against
  Output: An integer for success (1) if the input string contained any 
          valid characters or failure (0) if it didn't
*/
int check_for_valid_input(char *input, char *valid_characters);

/*
	Helper function to check if the user wishes to continue

	Output:
		An integer indicating that the user wishes to continue
*/
int check_for_continuation(void);

/*
  Helper function to keep the board form doing anythiung else until
  reset is pushed, unless the user wishes to continue

	Output:
		An integer indicating that the user wishes to continue
*/
int exit_program(void);

/*
  Helper function to print the banner for the program
*/
void print_banner(void);

/*
	Helper function to delay a certain number of milliseconds
*/
void delay(uint32_t delay_time);

/*
	This helper function calculates the delay to be used when moving a servo

	Input:
		last_position - The last position the servo was in
		new_position  - The next position for the servo

	Output:
		This function returns the total delay time the servo should wait for
		the given move
*/
uint16_t calculate_delay(position last_position, position new_position);

/*
  This funtion sets the TIM2 output correctly, then updates our data struct so we hold the correct data

	Input:
		motor_num - An integer that specifies the number of the motor to move
		motor     - The motor struct refernce to update
    target_position - The position we want to move to

	Output:
		A 16 bit unsigned integer corresponding to the total time we should delay for the move
*/
uint16_t move_servo(int motor_num, servo_data *motor, uint16_t target_position);

/*
	This wrapper function resets the target servo to zero degrees

	Input:
		motor_num 	    - An integer that specifies the number of the motor to move
		motor     	    - The motor struct refernce to update
*/
void reset_servo(int motor_num, servo_data *motor);

/*
  This funtion sets the default values for the motor struct passed to it

	Input:
		motors - The array of motor struct refernces to update
*/
void servo_data_init(servo_data *motors);

/*
	This function returns an OPCODE when given a byte register

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns the opcode as a uint8_t
*/
uint8_t get_opcode(uint8_t byte_register);

/*
	This function returns an PARAMETER when given a byte register

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns the parameter as a uint8_t
*/
uint8_t get_parameter(uint8_t byte_register);

/*
	This function returns an instruction struct when given a byte register
	The function packs the struct with the current opcode and parameter for
	the opcode

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns a struct containing the opcode and the parameter
*/
current_instruction get_instruction(uint8_t byte_register);

/*
	Helper function to abstract away the details of determining if a movement
	would place the servo out of bounds
	
	Input:
		instruction - An instruction struct to check if the current instruction from

	Output:
		This function returns true if the instruction is in bounds, false otherwise
*/
int instruction_in_bounds(current_instruction instruction);

/*
	This function handles the logic for incrementing the recipe number for the
	given motor data

	Input:
		motor - The motor struct refernce to update
*/
void increment_recipe(servo_data *motor);

/*
	Fixup the servo data on the given servo

	Input:
		index  - The motor servo data index
		motors - The array of motor struct refernces to update
*/
void fixup_servo_data(int index, servo_data *motor);

/*
	Fixup the servo data if need be on all servos

	Input:
		motors - The array of motor struct refernces to update
*/
void fixup_servo_data_multiple(servo_data *motors);

/*
	This helper function determines if a servo is ready to move yet
*/
int servo_ready(int servo_num, servo_data *motors);
