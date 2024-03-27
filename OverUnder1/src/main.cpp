//default, do not modify
#include "main.h"
#include "lemlib/api.hpp"

//custom hpp files
#include "drive.hpp"
#include "intake.hpp"
#include "catapult.hpp"
#include "pneumaticgroup.hpp"
#include "climber.hpp"

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
	base.chassis.calibrate();
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



//Constructors for every subsystem to be used in both autonomous and tele-op
Drive base(-4,-13,-19,8,14,9, 17, 10); //create a new Drive object
Intake intake(2); //create a new Intake object
Catapult catapult(-5, 12, 170*100, 200*100); //create a new Catapult object
pros::Controller master(pros::E_CONTROLLER_MASTER); //create a new pros::Controller object (remote)
Pneumaticgroup flaps('A', false); //create a new pneumatic group for the flaps, set the starting as inverse
pros::ADIAnalogIn selector('B'); //create a new analog sensor for the selector
Pneumaticgroup lifter('C', false); //create a new pneumatic group for the lifter, set the starting as inverse
Pneumaticgroup pto('D', false); //create a new pneumatic group for the gear-mover, set the starting as inverse
Pneumaticgroup claw('E', false); //create a new pneumatic group for the claw, set the starting as inverse
Pneumaticgroup awp('F', false); //create a new pneumatic group for the stick, set the starting as inverse
pros::ADIAnalogIn loaddetector ('G'); //create a new analog sensor for the detector
Climber climb(lifter, claw, pto); //create a new Climb object


//Function defined here for use as a task (thread) later
void cata () {
	while (true) {
		catapult.taskmanager();
		pros::delay(30);
	}
	
}

//Function for basic x and y on GUI
void printlocation () {
	while (true) {
		base.printposition();
		pros::delay(30);
	}
	
}

void autonomous() {
	if (selector.get_value() >= 2800 && selector.get_value() <= 3310) {//opposing side (offensive)
		pros::lcd::set_text(3, "offense");
		//leaving the starting ball
		base.go(-32);
		base.turnto(90);
		intake.inmax();
		//ball-collecting
		base.go(23);
		intake.inmax();
		base.turnto(180);
		base.go(14); 
		base.turnto(-90);
		base.go(8);
		intake.outmax();
		base.go(-8);
		flaps.openboth();
		base.gotime(.7, true);
		flaps.closeboth();
		//ending-touch
		base.go(-28);
		base.turnto(180);
		awp.openboth();
		base.gotime(2, false);
	} else if (selector.get_value() >= 1500 && selector.get_value() <= 2100) {
		pros::lcd::set_text(3, "Win Point");
		//removal
		base.go(5);
		awp.openboth();
		base.go(-6);
		base.turnto(-27);
		base.go(5);
		awp.closeboth();
		//scoring
		base.turnto(0);
		base.go(2);
		base.turnto(45);
		base.gotime(.4, true);
		//positioning
		base.go(-5);
		base.turnto(0);
		base.go(-19.5);
		//touching
		base.turnto(-45);
		base.go(-33);


	} else {//skills
		pros::lcd::set_text(3, "new skills");
		//for stability in large movements
		base.allbrake();
		
		//thread
		pros::Task my_task(cata);
		 for (int i = 0; i < 48; i++) {
		 	catapult.reload();
		 	pros::delay(.8 * 1000);
		 	catapult.launch();
		}
		//end thread and start to move
		catapult.requestemergency(true);
		intake.outmax();
		base.turnto(-25);
		base.go(50);
		base.turnto(-145);
		//pushing balls in from 3 angles
		base.gotime(2, false);
		base.go(25);
		base.turnto(0);
		flaps.openboth();
		base.gotime(.75, true);
		base.go(-20);
		flaps.closeboth();
		base.turnto(-55);
		base.go(40);
		base.turnto(70);
		flaps.openboth();
		base.gotime(1, true);
		flaps.closeboth();
		base.gotime(.6, false);

	}

	pros::lcd::set_text(3, "finished");

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

bool climbingmode = false; //used for protection-logic
void opcontrol() { //huge, repeated loop
	base.allcoast(); //since it's driver-control, let the motors not auto-brake
	awp.closeboth(); //safety function in case of autonomous failure
	while (true) { //the loop

		//default, tests if GUI is updating
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);


		pros::lcd::set_text(3, "climb test"); //upload tester (change the string to see if the code is new)


		//exponentially-tuned turning
		double x = -1* (pow(1.0471285480509, -1 * master.get_analog(ANALOG_RIGHT_X))) + pow(1.0471285480509, master.get_analog(ANALOG_RIGHT_X)); //exponential function for turning tuning
		//linear function for forwards/backwards movement
		int y = master.get_analog(ANALOG_LEFT_Y); 

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
		if (master.get_digital(DIGITAL_B)) {
			catapult.requestemergency(true);
			pros::lcd::set_text(3, "emergency test"); //upload tester (change the string to see if the code is new)
		} else {
			catapult.requestemergency(false); //In case of hardware emergency, relieve motor
		}
		if (master.get_digital(DIGITAL_R1)){
			catapult.launch(); //sets a launching request for taskmanager
		} else {
			catapult.reload(); //sets reload request (default) for taskmanager
		}
		if (master.get_digital_new_press(DIGITAL_DOWN)) {
			catapult.rapidfire(); //sets perpetual rapidfire request for taskmanager
		}
		

		// flaps only open during R2 pressed, then auto-close
		if (master.get_digital(DIGITAL_R2)) {
			flaps.openboth();
		} else {
			flaps.closeboth();
		}


		//climbingmode protection frm accidental pressings
		if (master.get_digital_new_press(DIGITAL_LEFT)) {
			if (climbingmode == true) {
				climbingmode = false;
			} else if (climbingmode == false) {
				climbingmode = true;
			}
		}

		//new functions 'unlock' if climbing mode is activated
		if (climbingmode == true) {
			catapult.requestemergency(true);
			if (master.get_digital_new_press(DIGITAL_UP)) {
				lifter.toggle();
			}
			if (master.get_digital_new_press(DIGITAL_RIGHT)) {
				pto.toggle();
			}
			if (master.get_digital_new_press(DIGITAL_Y)) {
				claw.toggle();
			}
			if (master.get_digital_new_press(DIGITAL_A)) {
				climb.goup();
			}
		}


		base.printposition(); //using GUI function
		catapult.taskmanager(); //calling taskmanager on every cycle to handle catapult logic

		pros::delay(20); //20 msec before re-running the loop
	}
}