#include "intake.hpp"
#include "main.h"

Intake::Intake (int port):intakemotor(port, ratio) {
}

void Intake::movespeed (int velocity) {
    intakemotor.move_velocity(velocity);
}

void Intake::inmax () {
    movespeed(600);
    intaking = true;
}

void Intake::outmax () {
    movespeed(-600);
    intaking = false;
}

void Intake::off () {
    intakemotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    intakemotor.brake();
    intaking = false;
}