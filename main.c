#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "helper.h"
#include "GPIO.h"
#include "TIMER.h"

// Constant declarations
servo_data motors[NUMBER_OF_SERVOS];														// Contains information on the various motor metrics

// Define a multidemensional array to contain every recipe
int recipes[NUMBER_OF_RECIPES][MAX_RECIPE_SIZE] = {
	
	// Recipe 0 is the test recipe given by the instructor
	{ 
		MOV + 0,
		MOV + 5,
		MOV + 0,
		MOV + 3,
		LOOP + 0,
		MOV + 1,
		MOV + 4,
		END_LOOP,
		MOV + 0,
		MOV + 2,
		WAIT + 0,
		MOV + 3,
		WAIT + 0,
		MOV + 2,
		MOV + 3,
		WAIT + 31,
		WAIT + 31,
		WAIT + 31,
		MOV + 4,
		RECIPE_END
	},

	// Recipe 1 is a recipe to verify the moves to all possible positions
	{
		MOV + 0,
		MOV + 1,
		MOV + 2,
		MOV + 3,
		MOV + 4,
		MOV + 5,
		RECIPE_END
	},

	// Recipe 2 is a custom recipe to use during testing.  It has distinct movement patterns
	// that will be easy to detect
	{
		MOV + 5,
		MOV + 0,
		MOV + 1,
		WAIT + 31,
		RECIPE_END
	},

	// Recipe 3 is a recipe with a deliberate error near the end of a task’s demo recipe, then
  // follow the erroneous command with a MOV command to a new position (Should not execute)
	{
		MOV + 5,
		ERRONEOUS + 1,  // Erroneous command
		MOV + 1,
		RECIPE_END
	},
	
	// Recipe 4 is a recipe to end normally (i.e. with an “RECIPE_END” command, followed
  // by a MOV command to a position different from the previous MOV
  // destination. This allows verification of the CONTINUE override (Not the way I coded it)
	// The recipe also contains a nested loop for demonstration of error catching
	{
		MOV + 2,
		LOOP + 1,
		MOV + 1,
		LOOP + 1,
		MOV + 5,
		END_LOOP,
		END_LOOP,
		RECIPE_END,
		MOV + 1,		// These should never run from here downwards
		RECIPE_END
	}
};

/*
  This funciton processes the input derived from the get_user_input function

	Input: 
		commands - An array that holds the commands issued to the motor

	Output:
		recipe_command_entered - 0 if no recipe command was entered, 1 if one was
*/
int process_user_input(char commands[COMMAND_BUFFER_SIZE]){
	int move_command_entered = 0;
	int recipe_command_entered = 0;
	int already_printed_warning = 0;
	uint16_t target_position;
	uint16_t current_delay_time = 0;
	
	// Figure out the command for each motor, the first command is for the
	// first motor, the second for the second motor
	for(int index = 0; index < NUMBER_OF_SERVOS; index++){

		switch(commands[index]){
			case 'B':
			case 'b':

				// Make sure we keep track of the motor status here
				// Also reset the recipe index, since 'B' should always start at the beginning
				motors[index].recipe_index = RECIPE_INDEX_DEFAULT;
				motors[index].recipe_instruction_index = RECIPE_INSTRUCTION_INDEX_DEFAULT;
				motors[index].status = active;
				recipe_command_entered = 1;
				break;
			case 'C':
			case 'c':

				// Make sure we keep track of the motor status here
				motors[index].status = active;
				recipe_command_entered = 1;
				break;
			case 'L':
			case 'l':
				target_position = motors[index].position - 1;
				if(motors[index].position != zero_degrees) {
					current_delay_time = move_servo(index, &motors[index], target_position);
					move_command_entered = 1;
				}
				else {
					usart_write_simple("");
					usart_write_data_string("Cannot move motor %d any more leftward, it is already at the max lefthand position", index);
				}
				break;
			case 'N':
			case 'n':

				// Do nothing
				break;
			case 'P':
			case 'p':

				// Make sure we keep track of the motor status here
				motors[index].status = inactive;
				break;

			case 'R':
			case 'r':
				target_position = motors[index].position + 1;
				if(motors[index].position != one_hundred_and_sixty_degrees) {
					current_delay_time = move_servo(index, &motors[index], target_position);
					move_command_entered = 1;
				}
				else {
					usart_write_simple("");
					usart_write_data_string("Cannot move motor %d more rightward, it is already at the max righthand position", index, target_position);
				}
				break;
			default:

				// Invalid input, let the user know, but only let them know once
				if(!already_printed_warning){
					usart_write_simple("");
					usart_write_data_string("Invalid command set: '%s', please try again", commands);
					already_printed_warning = 1;
				}
		}
	}

	// If the motor is moved, make sure we delay appropriately (this uses the blocking method because single moves
	// can block)
	if(move_command_entered){
		Green_LED_Off();
		Red_LED_On();
		delay(current_delay_time);
		Red_LED_Off();
		Green_LED_On();
	}
	return recipe_command_entered;
}

/*
	This function handles getting input from the user for the command set.
	Command sets can be any combination of the two letters LlRrPpNnCc or Bb.
	The letter X or x resets the input, backspace is interpreted and used to
	move the cursor backwards as expected

	Output:
		recipe_command_entered - An integer describing if the user entered in
														 a command having to do with a recipe
*/
int get_user_input(){
	int index, first = 0;
	int recipe_command_entered = 0;
	char input = NULL;
	char command_buffer[COMMAND_BUFFER_SIZE + 1] = {NULL, NULL, NULL};

	// Print our banner
	usart_write_simple("Enter a command set or 'Cc' to continue a recipe:");
	usart_terminal_character_simple();
	input = usart_read_simple();

	// Only accept two inputs before moving on
	while(input != ASCII_NEWLINE){

		// Check if the user entered an X first, if they did, then we have 
		// to write that out and break out of getting input
		if(check_for_valid_input(&input, VALID_X)){ 
			usart_real_time_write(input, PRINT_NEWLINE);
			usart_terminal_character_simple();
			return recipe_command_entered;
		};

		// Next handle when the user enters a backspace
		if(input == ASCII_BACKSPACE){

			// If the user enetered in a backspace, reverse the input index by 1
			if(index > first){
				index--;

				// Write out the backspace
				usart_real_time_write(input, NO_NEWLINE);
			}
		} 

		// Now store the character unless its a backspace and move on
		else {

			// Don't record out of bounds inputs
			if(index >= COMMAND_BUFFER_SIZE){
				break; 
			}

			// Store our character
			command_buffer[index] = input;

			// Move on to the next character
			index++;

			// Write out our character
			usart_real_time_write(input, NO_NEWLINE);
		}

		// Read again at the end
		input = usart_read_simple();
	}
	
	// Now that the input has been found and processed, process it into motor movements
	recipe_command_entered = process_user_input(command_buffer);
	usart_write_simple("");
	return recipe_command_entered;
}

/*
	This funciton handles reading in the recipe and processing the commands included in
	each recipe
*/
void process_recipe(){
	usart_write_simple("");
	usart_write_simple("Processing recipes ...");
	int recipe_ended, paused, keep_going = 0;
	char pause = NULL;
	
	// For each servo perform each action
	for(int servo_index = 0; servo_index < NUMBER_OF_SERVOS; servo_index++){
		
		// The motor has been iussued a recipe command and is active
		if(motors[servo_index].status == active){

			// We need to iterate through each item in each recipe, but don't progress past any recipe length
			for(int i = 0; i < MAX_RECIPE_SIZE; i++){
					
				// Break the current loop, go to the next loop (the next servo)
				if(recipe_ended){
					recipe_ended = 0;
					break;
				}

				// Check if the user paused execution, if they did, make both servos inactive, and break
				// The outer loop will be handled by the servo being inactive
				pause = usart_read_no_block();
				if(check_for_valid_input(&pause, VALID_P)){
					usart_terminal_character_simple();
					usart_real_time_write(pause, PRINT_NEWLINE);
					usart_write_simple("Pausing recipe execution ... ");
					for(int temp_servo_index = 0; temp_servo_index < NUMBER_OF_SERVOS; temp_servo_index++){
						motors[temp_servo_index].status = inactive;
					}

					// Indicate we have paused so that we do not print out the 'Recipe execution completed' message
					paused = 1;
					break;
				}

				// Get the current instruction object from the recipe
				current_instruction instruction = get_instruction(recipes[motors[servo_index].recipe_index][motors[servo_index].recipe_instruction_index]);

				// Perform all of the opcodes
				switch(instruction.opcode){
					
					// Servo movement
					case MOV:

						// The instruction is in bounds
						if(instruction_in_bounds(instruction)){
							move_servo(servo_index, &motors[servo_index], instruction.parameter);

							// We performed an action, increment the counter in the motor data in case we a pause
							motors[servo_index].recipe_instruction_index++;
						}
						
						// If the instruction is out of bounds fail and make the user restart
						else{
							usart_write_simple("");
							usart_write_data_string("ERROR: Current instruction parameter out of bounds "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));
							
							// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
							keep_going = exit_program();
							if(keep_going){
								motors[servo_index].recipe_instruction_index++;
							}
						}
						break;

					// Delay by a number of 1/10 of a seconds
					case WAIT:

						// Delay the appropriate amount of time
						delay(ONE_STEP_SERVO_DELAY * instruction.parameter);

						// Incement the instruction counter
						motors[servo_index].recipe_instruction_index++;
						break;

					// Indicate that we are looping instructions
					case LOOP:

						// This is an error, do not accept loops inside loops
						if(motors[servo_index].inside_recipe_loop == INSIDE_RECIPE_LOOP){ 
							usart_write_simple("");
							usart_write_data_string("ERROR: Current instruction parameter indicates nested loops "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));

							// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
							keep_going = exit_program();
							if(keep_going){
								motors[servo_index].recipe_instruction_index++;
								motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
							}
						}
						else{

							// Recipe instruction index increments here as well
							motors[servo_index].recipe_instruction_index++;
							motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP;
							motors[servo_index].recipe_loop_index = motors[servo_index].recipe_instruction_index;
							motors[servo_index].recipe_loop_count = instruction.parameter - RECIPE_LOOP_MODIFIER;
						}
						break;

					// Indicate that we are at the end of the loop section
					case END_LOOP:
						
						// check if we are in a loop, if not then we have found an error and need to quit
						if(motors[servo_index].inside_recipe_loop != INSIDE_RECIPE_LOOP){ 
							usart_write_simple("");
							usart_write_data_string("ERROR: Current instruction parameter indicates nested loops "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));

							// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
							keep_going = exit_program();
							if(keep_going){
								motors[servo_index].recipe_instruction_index++;
								motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
							}
						} 
						else{
							
							// This section indicates the end of the loop
							if(motors[servo_index].recipe_loop_count < LOOP_END_COUNT){
								
								motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
								motors[servo_index].recipe_instruction_index++;
							}

							// If we are still in the loop, then move back to the first index
							else {

								motors[servo_index].recipe_instruction_index = motors[servo_index].recipe_loop_index;

								// Decrement our counter.  When this reaches below 0 we stop looping
								motors[servo_index].recipe_loop_count--;
							}
						}
						break;

					// The end of the recipe
					case RECIPE_END:
						recipe_ended = 1;

						// Reset our stats back to expected positions
						fixup_servo_data(servo_index, &motors[servo_index]);

						break;

					// Invalid command found
					default:
						usart_write_data_string("Invalid recipe command encountered "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.opcode));
						
						// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
						keep_going = exit_program();
						if(keep_going){
							motors[servo_index].recipe_instruction_index++;
						}
						break;
				}
			}
		}
	}
	
	// Only fixup the recipe data if the maximum recipe size was reached and we didnt fix it earlier
	fixup_servo_data_multiple(motors);

	// Only print our status out if we didn't pause
	if(!paused){
		usart_write_simple("Recipe execution completed");
	}
}

/*
	This funciton handles reading in the recipe and processing the commands included in
	each recipe
*/
void process_recipe_no_block(){
	usart_write_simple("");
	usart_write_simple("Processing recipes ...");
	int recipe_ended, paused, keep_going = 0;
	char pause = NULL;

	// Start the servo timers
	start_timers();
	
	// We need to iterate through each item in each recipe, but don't progress past any recipe length
	for(int i = 0; i < MAX_RECIPE_SIZE; i++){
			
		// Break the current loop, go to the next loop (the next servo)
		if(recipe_ended){
			recipe_ended = 0;
			break;
		}
	
		// For each servo perform each action
		for(int servo_index = 0; servo_index < NUMBER_OF_SERVOS; servo_index++){
			
			// The motor has been iussued a recipe command and is active
			if(motors[servo_index].status == active){

				// Check if the motor is still moving
				if(servo_ready(servo_index, motors)){

					// Check if the user paused execution, if they did, make both servos inactive, and break
					// The outer loop will be handled by the servo being inactive
					pause = usart_read_no_block();
					if(check_for_valid_input(&pause, VALID_P)){
						usart_terminal_character_simple();
						usart_real_time_write(pause, PRINT_NEWLINE);
						usart_write_data_string("Pausing recipe execution on servo %d ...", servo_index);
						motors[servo_index].status = inactive;

						// Indicate we have paused so that we do not print out the 'Recipe execution completed' message
						paused = 1;
						break;
					}

					// Get the current instruction object from the recipe
					current_instruction instruction = get_instruction(recipes[motors[servo_index].recipe_index][motors[servo_index].recipe_instruction_index]);

					// Perform all of the opcodes
					switch(instruction.opcode){
						
						// Servo movement
						case MOV:

							// The instruction is in bounds
							if(instruction_in_bounds(instruction)){
								move_servo(servo_index, &motors[servo_index], instruction.parameter);

								// We performed an action, increment the counter in the motor data in case we a pause
								motors[servo_index].recipe_instruction_index++;
							}
							
							// If the instruction is out of bounds fail and make the user restart
							else{
								usart_write_simple("");
								usart_write_data_string("ERROR: Current instruction parameter out of bounds "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));
								
								// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
								keep_going = exit_program();
								if(keep_going){
									motors[servo_index].recipe_instruction_index++;
								}
							}
							break;

						// Delay by a number of 1/10 of a seconds
						case WAIT:

							// Delay the appropriate amount of time
							delay(ONE_STEP_SERVO_DELAY * instruction.parameter);

							// Incement the instruction counter
							motors[servo_index].recipe_instruction_index++;
							break;

						// Indicate that we are looping instructions
						case LOOP:

							// This is an error, do not accept loops inside loops
							if(motors[servo_index].inside_recipe_loop == INSIDE_RECIPE_LOOP){ 
								usart_write_simple("");
								usart_write_data_string("ERROR: Current instruction parameter indicates nested loops "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));

								// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
								keep_going = exit_program();
								if(keep_going){
									motors[servo_index].recipe_instruction_index++;
									motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
								}
							}
							else{

								// Recipe instruction index increments here as well
								motors[servo_index].recipe_instruction_index++;
								motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP;
								motors[servo_index].recipe_loop_index = motors[servo_index].recipe_instruction_index;
								motors[servo_index].recipe_loop_count = instruction.parameter - RECIPE_LOOP_MODIFIER;
							}
							break;

						// Indicate that we are at the end of the loop section
						case END_LOOP:
							
							// check if we are in a loop, if not then we have found an error and need to quit
							if(motors[servo_index].inside_recipe_loop != INSIDE_RECIPE_LOOP){ 
								usart_write_simple("");
								usart_write_data_string("ERROR: Current instruction parameter indicates nested loops "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.parameter));

								// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
								keep_going = exit_program();
								if(keep_going){
									motors[servo_index].recipe_instruction_index++;
									motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
								}
							} 
							else{
								
								// This section indicates the end of the loop
								if(motors[servo_index].recipe_loop_count < LOOP_END_COUNT){
									
									motors[servo_index].inside_recipe_loop = INSIDE_RECIPE_LOOP_DEFAULT;
									motors[servo_index].recipe_instruction_index++;
								}

								// If we are still in the loop, then move back to the first index
								else {

									motors[servo_index].recipe_instruction_index = motors[servo_index].recipe_loop_index;

									// Decrement our counter.  When this reaches below 0 we stop looping
									motors[servo_index].recipe_loop_count--;
								}
							}
							break;

						// The end of the recipe
						case RECIPE_END:
							recipe_ended = 1;

							// Reset our stats back to expected positions
							fixup_servo_data(servo_index, &motors[servo_index]);

							break;

						// Invalid command found
						default:
							usart_write_data_string("Invalid recipe command encountered "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(instruction.opcode));
							
							// Check with the user if they want to keep executing recipes, if so, increment the recipe instruction
							keep_going = exit_program();
							if(keep_going){
								motors[servo_index].recipe_instruction_index++;
							}
							break;
					}
				}
			}
		}
	}
	
	// Only fixup the recipe data if the maximum recipe size was reached and we didnt fix it earlier
	fixup_servo_data_multiple(motors);

	// Only print our status out if we didn't pause
	if(!paused){
		usart_write_simple("Recipe execution completed");
	}
	
	// Now stop the timers from counting
	stop_timers();
}

/*
	The main fucntion runs the main loop, that is:
		1. Initialize the various timers, clocks, pins, and LED's
		2. Print the user input banner
		3. Start the loop, turn the green led on to indicate user input is being
       gathered
		4. Gather user input
		5. Turn the green led off and the red led on to indicate that the recipe
			 is being processed if the user entered a command having to do with a
       recipe
    6. Return to step 3
*/
int main(void){
	int recipe_command_entered;

	// Initialize!
	
	System_Clock_Init();
	LED_Init();
	UART2_Init();
	gpio_init();
	timer_init();
	servo_timers_init();
	servo_data_init(motors);

	// Print our banner, let the user know how to proceed
	print_banner();

	// Main loop, process the user input and then execute it.  Show the status with a green
	// LED indicating user input can be entered, and a red led indicating that the commands are
	// being excecuted
	while(1){
			recipe_command_entered = 0;
			Green_LED_On();
			recipe_command_entered = get_user_input();
			Green_LED_Off();

			// Only process recipes if we got a command to run one 
			if(recipe_command_entered){
				Red_LED_On();
				process_recipe_no_block();
				Red_LED_Off();
			}
	}
}
