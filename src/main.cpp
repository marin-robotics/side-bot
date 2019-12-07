#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);
pros::Motor back_left_drive(19);
pros::Motor front_left_drive(20);
pros::Motor back_right_drive(9, true);
pros::Motor front_right_drive(2, true);

pros::Motor strafe_drive(3);

pros::Motor right_lift(1);
pros::Motor left_lift(11, true);

pros::Motor claw(12);

// Partner motors

pros::Motor partner_left_drive(21);
pros::Motor partner_right_drive(22);

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
	pros::lcd::set_text(1, "Hello PROS User!");

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
void autonomous() {
	// move forward
	back_left_drive = 127, front_left_drive = 127, back_right_drive = 127, front_right_drive = 127;
	pros::delay(1000);
	// move backwards
	back_left_drive = -127, front_left_drive = -127, back_right_drive = -127, front_right_drive = -127;
	pros::delay(1000);
	// stop movement
	back_left_drive = 0, front_left_drive = 0, back_right_drive = 0, front_right_drive = 0;
}

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
	// When true, claw will be run at -15 to hold a cube
	bool claw_hold = false;
	int current_lift_power = 0;
	while (true) {
		int drive_left = master.get_analog(ANALOG_LEFT_Y);
		int drive_right = master.get_analog(ANALOG_RIGHT_Y);
		back_left_drive = drive_left;
		front_left_drive = drive_left;
		back_right_drive = drive_right;
		front_right_drive = drive_right;

		int drive_side = master.get_analog(ANALOG_RIGHT_X);
		int left_x = master.get_analog(ANALOG_LEFT_X);

		if (left_x != 0) {
			drive_side = left_x;
		}
		strafe_drive = drive_side;

		bool lift_down = master.get_digital(DIGITAL_L1);
		bool lift_up = master.get_digital(DIGITAL_R1);
		int lift_target = 0;
		if (lift_down) {
			lift_target = -127;
		} else if (lift_up) {
			lift_target = 127;
		} else {
			lift_target = 0;
		}
		// go to target
		if (abs(current_lift_power - lift_target) > 5) {
			if (current_lift_power < lift_target) {
				current_lift_power += 5;
			} else {
				current_lift_power -= 5;
			}
		}
		right_lift = current_lift_power;
		left_lift = current_lift_power;

		bool claw_open = master.get_digital(DIGITAL_L2);
		bool claw_close = master.get_digital(DIGITAL_R2);
		if (claw_open) {
			claw = 127;
			claw_hold = false;
		} else if (claw_close) {
			claw = -127;
			claw_hold = true;
		} else if (claw_hold) {
			claw = -15;
		} else {
			claw = 0;
		}

		// Partner controller code

		int partner_left_y = partner.get_analog(ANALOG_LEFT_Y);
		int partner_right_y = partner.get_analog(ANALOG_RIGHT_Y);

		partner_left_drive = partner_left_y;
		partner_right_drive = partner_right_y;

		pros::delay(20);
	}
}
