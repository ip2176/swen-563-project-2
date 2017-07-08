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
  Helper function to keep the board form doing anythiung else until
  reset is pushed
*/
void exit_program(){
  usart_write_simple("Exiting the program");
  while(1);
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
	This function handles delaying by a number of seconds (for when we move the servos)
	Takes from a stack overflow answer, seems to work great

	Input: 
		delay_time - The number of milliseconds to delay
 **/
void delay(int delay_time) {
	long pause;
	clock_t now,then;

	pause = delay_time * (CLOCKS_PER_SEC / ONE_THOUSAND);
	now = then = clock();
	while((now-then) < pause){
			now = clock();
	}
}

/*
  This funtion sets the TIM2 output correctly, then updates our data struct so we hold the correct data

	Input:
		motor_num 	    - An integer that specifies the number of the motor to move
		motor     	    - The motor struct refernce to update
    target_position - The position we want to move to
*/
void move_servo(int motor_num, servo_data *motor, uint16_t target_position){

	if(motor_num == 0){
		TIMER_2_MOTOR_1 = positions[target_position];
	}
	else {
		TIMER_2_MOTOR_2 = positions[target_position]; 
	}
	motor->position = (position)target_position;
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
		motors[servo_data_index].recipe_index = RECIPE_START_INDEX_DEFAULT;
		motors[servo_data_index].inside_recipe_loop = INSIDE_RECIPE_DEFAULT;
		motors[servo_data_index].recipe_loop_count = RECIPE_LOOP_COUNT_DEFAULT;
		motors[servo_data_index].recipe_loop_index = RECIPE_LOOP_INDEX_DEFAULT;
		motors[servo_data_index].status = inactive;
	}
}
