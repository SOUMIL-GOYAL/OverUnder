#include "catapult.hpp"
#include "main.h"

Catapult::Catapult (int motorport, int sensorport, int top_angle, int bottom_angle):motor(motorport, ratio), sensor(sensorport){
    top = top_angle;
    bottom = bottom_angle;
    requestshot = false;

    kp = .04;
    ka = 1;
    
}

void Catapult::taskmanager () {
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    isready();
    if (rapidfireon == true) {
        motor.move_velocity(-600);
    } else if (requestshot == true) {
                if (getangle() >= top) {
                    motor.move_velocity(-300);
                } else {
                    requestshot = false;
                    reload();
                }
    } else if (requestreload == true) {
        if (getangle() < bottom){
            int error = bottom - getangle();

            if (error <= tolerance && error >= -tolerance) {

            } else {
                double result = kp * error;
                motor.move_velocity(-result);
                previousresult = result;

            }
        } else {
            motor.move_velocity(0);
        }
    }
}

int Catapult::getangle () {
    return sensor.get_angle();
}

void Catapult::launch () {
    requestshot = true;

}

void Catapult::reload () {
    if (requestshot == false) {
        requestreload = true;
    }
}

bool Catapult::isready () {
    if (getangle() >= bottom) {
        return true;
    } else {
        return false;
    }
}


void Catapult::rapidfire () {
    if (rapidfireon == false) {
        rapidfireon = true;
    } else if (rapidfireon == true) {
        rapidfireon = false;
    }
}