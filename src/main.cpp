#include "main.h"

#define pi 3.14159265359

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);
pros::Motor back_left_drive(17);
pros::Motor front_left_drive(19);
pros::Motor back_right_drive(20, true);
pros::Motor front_right_drive(10, true);

pros::Motor lift(2);

pros::Motor claw(1);

// Partner motors

pros::Motor partner_left_drive(8);
pros::Motor partner_right_drive(9, true);

/**
 * Blocks until the specified motors are done moving the specified distance (in inches) at the specified velocity.
*/
void move(pros::Motor motors[], int num_motors, double distance, int velocity) {
	double ticks = distance * 225 / pi;

	for (int i = 0; i < num_motors; i++) {
		motors[i].tare_position();
		motors[i].move_absolute(ticks, velocity);
		pros::lcd::print(2 + i, "%f %f", ticks, motors[i].get_position());
	}
	pros::lcd::print(0, "NOT DONE");
	bool all_motors_done = false;
	while (!all_motors_done) {
		for (int i = 0; i < num_motors; i++) {
			// Check if the motor is not done moving (+ or - 5 units)
			// i.e., if |distance - motor position| > 5
			all_motors_done = true;
			if (fabs(ticks - motors[i].get_position()) > 5) {
				all_motors_done = false;
				break;
			}
		}
		pros::delay(2);
	}
	pros::lcd::print(0, "DONE");
}

/**
 * Rotates the bot the specified number of degrees (can be positive or negative) at the specified velocity
*/
void rotateMain(double degrees, int velocity) {

	double ticks = degrees * 32000 / 3555;

	back_left_drive.tare_position();
	front_left_drive.tare_position();
	back_right_drive.tare_position();
	front_right_drive.tare_position();

	back_left_drive.move_absolute(ticks, velocity);
	front_left_drive.move_absolute(ticks, velocity);
	back_right_drive.move_absolute(-ticks, velocity);
	front_right_drive.move_absolute(-ticks, velocity);
	pros::lcd::print(1, "bro wth. %d, %d, %d, %d, %d", ticks, back_left_drive.get_position(), front_left_drive.get_position(), back_right_drive.get_position(), front_right_drive.get_position());
	while (!((back_left_drive.get_position() < ticks + 5) && (back_left_drive.get_position() > ticks - 5))) {
		pros::delay(2);
	}
}

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

	// botMove(4, 127);

	// rotateMain(360*10, 50);

	front_left_drive = -127;
	front_right_drive = -127;
	back_left_drive = -127;
	back_right_drive = -127;

	pros::delay(250);

	front_left_drive = 127;
	front_right_drive = 127;
	back_left_drive = 127;
	back_right_drive = 127;

	pros::delay(250);

	front_left_drive = 80;
	front_right_drive = 50;
	back_left_drive = 80;
	back_right_drive = 50;
	claw = 40;

	pros::delay(1500);

	front_left_drive = 0;
	front_right_drive = 0;
	back_left_drive = 0;
	back_right_drive = 0;
	claw = -100;

	pros::delay(500);

	claw = -20;

	front_left_drive = 127;
	back_left_drive = 127;
	front_right_drive = -127;
	back_right_drive = -127;

	pros::delay(1050);

	front_left_drive = 0;
	front_right_drive = 0;
	back_left_drive = 0;
	back_right_drive = 0;

	pros::delay(500);

	front_left_drive = 127;
	front_right_drive = 127;
	back_left_drive = 127;
	back_right_drive = 127;

	pros::delay(500);

	front_left_drive = 0;
	front_right_drive = 0;
	back_left_drive = 0;
	back_right_drive = 0;

	/*pros::Motor motors[] = {front_left_drive, front_right_drive, back_left_drive, back_right_drive};
	move(motors, 4, -3, 127);
	pros::lcd::print(1, "WAITING");
	pros::delay(20);
	pros::lcd::print(1, "GOINg");
	move(motors, 4, 3, 127);

	pros::lcd::print(1, "1, jjkkkkkkkkk");
	rotateMain(120, 100);
	pros::delay(20);

	pros::lcd::print(1, "JFJFJFFJFJFJFJFJF");
	move(motors, 4, 20, 127);
	pros::lcd::print(1, "jfjfjfjfjjfjfjf");
	pros::delay(20);

	pros::lcd::print(1, "gaming");
	rotateMain(-110, 127);
	pros::lcd::print(1, ".....");
	pros::delay(200);

	claw = 127;
	pros::delay(2000);
	claw = 0;

	pros::delay(20);

	move(motors, 4, 20, 127);
	front_left_drive = 100;
	back_left_drive = 100;
	front_right_drive = 127;
	back_right_drive = 127;

	pros::delay(750);

	front_left_drive = 0;
	back_left_drive = 0;
	front_right_drive = 0;
	back_right_drive = 0;

	pros::delay(20);

	claw = -127;
	pros::delay(1500);
	claw = 0;

	pros::delay(20);

	rotateMain(-165, 127);

	pros::delay(20);

	move(motors, 4, 40, 127);

	pros::delay(20);*/


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
		} else {
			current_lift_power = lift_target;
		}
		lift = current_lift_power;

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
