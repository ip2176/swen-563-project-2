/*
  Helper function definitions
*/

#include "Helper.h"

/*
  Check the input string and see if we have a valid character in it.
  Used to check the user input

  Input: input - The string to check if it is in valid characters
         valid_characters - A string of valid characters input should
                            be checked against
  Output: An integer for success (1) if the input string contained any 
          valid characters or failure (0) if it didn't
*/
int check_for_valid_input(char *input, char *valid_characters){
  if(!strpbrk(input, valid_characters)){
    return FAILURE;
  }
  return SUCCESS;
}

/*
	Helper function to check if the user wishes to continue

	Output:
		An integer indicating that the user wishes to continue
*/
int check_for_continuation(){
	char input = NULL;
	int keep_going = 0;
	usart_write_simple("Do you wish to skip to the next instruction? (Yy or Nn):");
	usart_terminal_character_simple();
	input = usart_read_simple();

	// Write out what the user wrote
	usart_real_time_write(input, PRINT_NEWLINE);

	// Get valid input
	while(!check_for_valid_input(&input, VALID_YES_NO)){
		usart_write_simple("");
		usart_write_data_string("Invalid input (%c) please enter Yy or Nn:", input);
		usart_terminal_character_simple();
		input = usart_read_simple();

		// Write out what the user wrote
		usart_real_time_write(input, PRINT_NEWLINE);
	}

	// Check if the user wants to continue
	if(check_for_valid_input(&input, VALID_Y)){
		keep_going = 1;
	}
	return keep_going;
}

/*
  Helper function to keep the board form doing anythiung else until
  reset is pushed, unless the user wishes to continue

	Output:
		An integer indicating that the user wishes to continue
*/
int exit_program(){
	int keep_going = 0;
	keep_going = check_for_continuation();

	// Check if the user wants to keep going and if not, 
	// 'exit' the program
	if(!keep_going){
		usart_write_simple("Exiting the program");
		while(1);
	}

	// Return if we don't 'exit' first
	return keep_going;
}

/*
  Helper function to print the banner for the program
*/
void print_banner(){
  usart_write_simple("");
	usart_write_simple("Enter commands to control motor execution");
	usart_write_simple("   --The first letter controls the first servo");
	usart_write_simple("   --The second letter controls the second servo");
	usart_write_simple("   --Available letters:");
	usart_write_simple("      --L or l: Turn the servo left if possible");
	usart_write_simple("      --R or r: Turn the servo right if possible");
	usart_write_simple("      --C or c: Continue execution of a recipe on the servo");
	usart_write_simple("      --P or p: Pause execution of a recipe on the servo");
	usart_write_simple("      --N or n: No op on the servo");
	usart_write_simple("      --B or b: Begin execution of a recipe on the servo immediately");
	usart_write_simple("Example: Enter 'Cc' to begin recipe execution on each servo");
}

/*
	This function handles delaying by a number of milliseconds

	Input: 
		delay_time - The number of milliseconds to delay
 **/
void delay(uint32_t delay_time) {

	// Convert to microseconds
	delay_time = (MICROSECOND_CONVERSION * delay_time);

	// Do nothing for a while!  Hopefully this doesn't get compiled out ...
	for(uint32_t index = 0; index < delay_time; index++);
}

/*
  This funtion sets the TIM2 output correctly, then updates our data 
	struct so we hold the correct data, then finally delays before allowing
	the next movement

	Input:
		motor_num 	    - An integer that specifies the number of the motor to move
		motor     	    - The motor struct refernce to update
    target_position - The position we want to move to
*/
void move_servo(int motor_num, servo_data *motor, uint16_t target_position){
	position last_position;
	position new_position = (position)target_position;
	uint16_t number_of_steps = 0;

	// Move the servo first
	if(motor_num == 0){
		TIMER_2_MOTOR_1 = positions[new_position];
	}
	else {
		TIMER_2_MOTOR_2 = positions[new_position]; 
	}

	// Update the position data and delay appropriately
	last_position = motor->position;
	motor->position = new_position;
	number_of_steps = abs(last_position - new_position);
	delay(ONE_STEP_SERVO_DELAY * number_of_steps);
}

/*
	This wrapper function resets the target servo to zero degrees

	Input:
		motor_num 	    - An integer that specifies the number of the motor to move
		motor     	    - The motor struct refernce to update
*/
void reset_servo(int motor_num, servo_data *motor){
	move_servo(motor_num, motor, zero_degrees);
}

/*
  This funtion sets the default values for the motor struct passed to it

	Input:
		motors - The array of motor struct refernces to update
*/
void servo_data_init(servo_data *motors){

	// Loop through each servo_data
	for(int servo_data_index; servo_data_index < NUMBER_OF_SERVOS; servo_data_index++){
		motors[servo_data_index].position = zero_degrees;
		motors[servo_data_index].recipe_index = RECIPE_INDEX_DEFAULT;
		motors[servo_data_index].recipe_instruction_index = RECIPE_INSTRUCTION_INDEX_DEFAULT;
		motors[servo_data_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
		motors[servo_data_index].recipe_loop_count = RECIPE_LOOP_COUNT_DEFAULT;
		motors[servo_data_index].recipe_loop_index = RECIPE_LOOP_INDEX_DEFAULT;
		motors[servo_data_index].status = inactive;
	}
}

/*
	This function returns an OPCODE when given a byte register

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns the opcode as a uint8_t
*/
uint8_t get_opcode(uint8_t byte_register){
	return byte_register & OPERATIONAL_CODE_MASK;
}

/*
	This function returns an PARAMETER when given a byte register

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns the parameter as a uint8_t
*/
uint8_t get_parameter(uint8_t byte_register){
	return byte_register & PARAMETER_MASK;
}

/*
	This function returns an instruction struct when given a byte register
	The function packs the struct with the current opcode and parameter for
	the opcode

	Input:
		byte_register - A byte register (a uint8_t)
	Output:
		The function returns a struct containing the opcode and the parameter
*/
current_instruction get_instruction(uint8_t byte_register){
	current_instruction instruction_struct;
	instruction_struct.opcode = get_opcode(byte_register);
	instruction_struct.parameter = get_parameter(byte_register);
	return instruction_struct;
}

/*
	Helper function to abstract away the details of determining if a movement
	would place the servo out of bounds
	
	Input:
		instruction - An instruction struct to check if the current instruction from

	Output:
		This function returns true if the instruction is in bounds, false otherwise
*/
int instruction_in_bounds(current_instruction instruction){

	// Since the instruction is always going to be an unsigned integer, the comparison to
	// zero is pointless.  It will always be greater than or eaual to zero
	return (instruction.parameter <= one_hundred_and_sixty_degrees);
}

/*
	This function handles the logic for incrementing the recipe number for the
	given motor data

	Input:
		motor - The motor struct refernce to update
*/
void increment_recipe(servo_data *motor){
	if(motor->recipe_index >= NUMBER_OF_RECIPES - 1){
		motor->recipe_index = RECIPE_INDEX_DEFAULT;
	}
	else{
		motor->recipe_index++;
	}
}

/*
	Fixup the servo data on the given servo

	Input:
		index  - The motor servo data index
		motors - The array of motor struct refernces to update
*/
void fixup_servo_data(int index, servo_data *motor){
	usart_write_data_string("Recipe %d complete for servo %d, resetting servo %d to starting position ...", motor->recipe_index, index, index);
						
	// Reset the servo position back to 0 degrees so the next recipe starts in a known position
	reset_servo(index, motor);

	// Reset back to the first recipe if we have already completed all of the recipes
	increment_recipe(motor);

	// Set the servo back inactive
	motor->status = inactive;

	// Reset the instruction index
	motor->recipe_instruction_index = RECIPE_INSTRUCTION_INDEX_DEFAULT;
	
	// Make sure we indicate we are not in a loop if that wasn't set
	motor->recipe_loop_count = RECIPE_LOOP_COUNT_DEFAULT;
	motor->recipe_loop_index = RECIPE_LOOP_INDEX_DEFAULT;
	motor->inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
}

/*
	Fixup the servo data if need be on all servos

	Input:
		motors - The array of motor struct refernces to update
*/
void fixup_servo_data_multiple(servo_data *motors){

	// Loop through each servo_data
	for(int servo_data_index; servo_data_index < NUMBER_OF_SERVOS; servo_data_index++){
		if(motors[servo_data_index].recipe_instruction_index != RECIPE_INSTRUCTION_INDEX_DEFAULT){
			fixup_servo_data(servo_data_index, &motors[servo_data_index]);
		}
	}
}
