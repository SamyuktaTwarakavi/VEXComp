#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Welcome Artemis V");
	pros::lcd::set_text(1, "Samyukta, Vidita, Kylie, Vidushi & Yasha ");
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	
	// Individual motor control - Left motors on ports 1, 2, 3
	pros::Motor left1(1);
	pros::Motor left2(2);
	pros::Motor left3(3);
	
	// Individual motor control - Right motors on ports 11, 12, 13 (reversed)
	pros::Motor right1(-11);
	pros::Motor right2(-12);
	pros::Motor right3(-13);

	// Individual scale factors for each motor (adjust these values)
	// Start at 1.0 and adjust based on testing
	double left1_scale = 1.0; //// Compensate, if needed for right drift
	double left2_scale = 1.0;
	double left3_scale = 1.0;
	
	double right1_scale = 1.0;  // Compensate, if needed for left drift
	double right2_scale = 1.00;
	double right3_scale = 1.00;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// Arcade control scheme with FLIPPED turn direction
		int dir = master.get_analog(ANALOG_LEFT_Y);     // Forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  
		
		// Calculate base power for each side
		int left_power = dir - turn;
		int right_power = dir + turn;
		
		// Apply individual scale factors to each motor
		left1.move(left_power * left1_scale);
		left2.move(left_power * left2_scale);
		left3.move(left_power * left3_scale);
		
		right1.move(right_power * right1_scale);
		right2.move(right_power * right2_scale);
		right3.move(right_power * right3_scale);
		
		pros::delay(20);  // Run for 20 ms then update
	}
}