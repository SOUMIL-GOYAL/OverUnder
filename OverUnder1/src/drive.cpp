#include "drive.hpp"
#include "main.h"

Drive::Drive (int lb, int lm, int lf, int rb, int rm, int rf): leftback(lb, pros::E_MOTOR_GEARSET_06), leftmiddle(lm, pros::E_MOTOR_GEARSET_06), leftfront(lf, pros::E_MOTOR_GEARSET_06), rightback(rb, pros::E_MOTOR_GEARSET_06), rightmiddle(rm, pros::E_MOTOR_GEARSET_06), rightfront(rf, pros::E_MOTOR_GEARSET_06) {
    // leftback = lb;
    // leftmiddle = lm;
    // leftfront = lf;
    // rightback = rb;
    // rightmiddle = rm;
    // rightfront = rf;
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


