#include "drive.hpp"
#include "main.h"

Drive::Drive (int lb, int lm, int lf, int rb, int rm, int rf)
: leftback(lb, pros::E_MOTOR_GEARSET_06), 
leftmiddle(lm, pros::E_MOTOR_GEARSET_06), 
leftfront(lf, pros::E_MOTOR_GEARSET_06), 
rightback(rb, pros::E_MOTOR_GEARSET_06), 
rightmiddle(rm, pros::E_MOTOR_GEARSET_06), 
rightfront(rf, pros::E_MOTOR_GEARSET_06) {

    // using namespace okapi;

    // std::shared_ptr<okapi::OdomChassisController> odometry = okapi::ChassisControllerBuilder()
    // .withMotors(lb, rb) // left motor is 1, right motor is 2 (reversed)
    // .withGains(
    //     {0.001, 0, 0.0001}, // Distance controller gains
    //     {0.001, 0, 0.0001}, // Turn controller gains
    //     {0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
    // )
    // // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
    // .withDimensions(okapi::AbstractMotor::gearset::blue, {{4_in, 11.5_in}, okapi::imev5BlueTPR})
    // .withOdometry() // use the same scales as the chassis (above)
    // .buildOdometry(); // build an odometry chassis

    double DL = 7;
    double DR = 7;

    double kp = 1, ki = 0, ka = 1;
}

void Drive::setleft (int velocity) {
    leftback.move_velocity(velocity);
    leftmiddle.move_velocity(velocity);
    leftfront.move_velocity(velocity);
}

void Drive::setright (int velocity) {
    rightback.move_velocity(velocity);
    rightmiddle.move_velocity(velocity);
    rightfront.move_velocity(velocity);
}

void Drive::allcoast () {
    leftback.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    leftmiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    leftfront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightback.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightmiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightfront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void Drive::allbrake () {
    leftback.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftmiddle.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftfront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightback.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightmiddle.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightfront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

double Drive::getangle () {
    return (fmod((leftback.get_position()-rightback.get_position())/(11.569), 360));

}

void Drive::go (double inches) {
    double previousresult = leftback.get_position();
    while (true) {
        double currentposition = leftback.get_position() - previous;
        double error = inches*(3.7699) - currentposition;

        double result = error * kp;

        // if ((previousresult - result) <= ka && (previousresult - result) >= -ka) {
        //     result = previousresult + ka;
        // }
        
        setright(result * 600);
        setleft(result * 600);


        previous = leftback.get_position();  

        // if (error <= 360 && error >= -360) {
        //     break;
        // }
    }
    
}




int Drive::getx () {

}


