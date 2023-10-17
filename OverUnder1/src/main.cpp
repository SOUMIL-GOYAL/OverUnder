#include "main.h"

#include "drive.hpp"
#include "intake.hpp"
#include "catapult.hpp"
#include "pneumaticgroup.hpp"

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
	Drive base(-4,-13,-3,8,18,9); //create a new Drive object
	Intake intake(11); //create a new Intake object
	Catapult catapult(5, 12, 9000, 19000); //create a new Catapult object
	pros::Controller master(pros::E_CONTROLLER_MASTER); //create a new pros::Controller object (remote)
	Pneumaticgroup flaps('A', false); //create a new pneumatic group for the flaps

	base.allcoast(); //since it's driver-control, let the motors not auto-brake

	

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

						 
		pros::lcd::set_text(3, "plz drive"); //upload tester

		
		double x = -1* (pow(1.0471285480509, -1 * master.get_analog(ANALOG_RIGHT_X))) + pow(1.0471285480509, master.get_analog(ANALOG_RIGHT_X)); //exponential function for turning tuning
		int y = master.get_analog(ANALOG_LEFT_Y); //linear function for forwards/backwards movement

		if (x <= 10 && x >= -10) { //tolerance/deadband for turning
			x = 0;
		}
		if (y <= 10 && y >= -10) { //tolerance/deadband for moving
			y = 0;
		}

		base.setleft((y+ x)*6); //multiply by 6 for RPM value and set wheel speeds
		base.setright((y - x)*6); //multiply by 6 for RPM value and set wheel speeds

		// Intake commands on L1, L2, and X
		if (master.get_digital(DIGITAL_L1)) {
			intake.inmax();
		} else if (master.get_digital(DIGITAL_L2)) {
			intake.outmax();
		} else if (master.get_digital(DIGITAL_X)) {
			intake.off();
		}

		// Catapult commands on R1 and Down
		if (master.get_digital(DIGITAL_R1)){
			catapult.launch(); //sets request for taskmanager
		} else {
			catapult.reload(); //sets request for taskmanager
		}
		if (master.get_digital_new_press(DIGITAL_DOWN)) {
			catapult.rapidfire(); //sets request for taskmanager
		}
		
		catapult.taskmanager(); //calling taskmanager on every cycle to handle catapult logic

		// flaps only open during R2 pressed, then auto-close
		if (master.get_digital(DIGITAL_R2)) {
			flaps.openboth();
		} else {
			flaps.closeboth();
		}
		
		pros::delay(20); //20 msec before re-running the loop
	}
}
   
