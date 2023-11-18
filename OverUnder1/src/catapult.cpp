#include "catapult.hpp"
#include "main.h"

Catapult::Catapult (int motorport, int sensorport, int top_angle, int bottom_angle):motor(motorport, ratio), sensor(sensorport){
    top = top_angle;
    bottom = bottom_angle;
    requestshot = false;
    emergency = false;
    
    tolerance = 500;
    kp = .03;
    ka = 1;
    
}

void Catapult::taskmanager () {
    if (emergency == true) {
        motor.move_velocity(0);
        motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        return;
    } else if (rapidfireon == true) {
        motor.move_velocity(-600);
        motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    } else if (requestshot == true) {
        if (getangle() >= top) {
            motor.move_velocity(-300);
        } else {
            requestshot = false;
            reload();
        }
        motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    } else if (requestreload == true) {
        if (getangle() < bottom){
            int error = bottom - getangle();

            if (error <= tolerance && error >= -tolerance) {
                requestreload = false;
                motor.move_velocity(0);
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

            } else {
                double result = kp * error;
                motor.move_velocity(-result);
            }
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

        } else {
            motor.move_velocity(0);
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

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
            rapidfireon = false;

}

void Catapult::rapidfire () {
    if (rapidfireon == false) {
        rapidfireon = true;
    } else if (rapidfireon == true) {
        rapidfireon = false;
    }
}

void Catapult::requestemergency (bool value) {
    emergency = value;
}

void Catapult::autofire(int seconds) {
    int counter = 0;
    while (true) {
        motor.move_velocity(-600);
        counter++;
        if (counter / 10 >= seconds) {
            motor.move_velocity(0);
            break;
        }
        pros::delay(100);
    }
}