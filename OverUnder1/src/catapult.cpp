#include "catapult.hpp"
#include "main.h"

Catapult::Catapult (int motorport, int sensorport, int top_angle, int bottom_angle, int shoot_angle):motor(motorport, ratio), sensor(sensorport){
    top = top_angle;
    bottom = bottom_angle;
    requestshot = false;
    shootangle = shoot_angle;
    
}

void Catapult::taskmanager () {
    Catapult::motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    isready();
    if (Catapult::requestshot == true) {
            // if (Catapult::isready()) { //is the reload finished?
                if (Catapult::getangle() >= Catapult::top) {
                    Catapult::motor.move_velocity(-300);
                } else {
                    Catapult::requestshot = false;
                    Catapult::reload();
                }
            // }
    } else if (Catapult::requestreload == true) {
        if (Catapult::getangle() < Catapult::bottom){
            Catapult::motor.move_velocity(-300);
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
    if (Catapult::requestshot == false) {
            Catapult::requestreload = true;
    }
}

bool Catapult::isready () {
    if (Catapult::getangle() >= Catapult::bottom) {
        return true;
    } else {
        return false;
    }
}