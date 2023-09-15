#include "main.h"

#ifndef INTAKE_HPP
#define INTAKE_HPP

class Intake //representing a 6-motor drivetrain
{
private:
    pros::Motor intakemotor;
    pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_06; //6:1 gear-ratio (blue cartridge)
    bool intaking = false;
    //okapi::ChassisControllerIntegrated odomoetry;


public:
    Intake (int port);
    void inmax ();//set the left-side of the drivebase at a certain RPM
    void off (); //set the right-side of the drivebase at a certain RPM
    void outmax (); //set all 6 motors in the drivebase to 'coast'
    void movespeed (int velocity); //set all 6 motors in thedrivebase to 'hold'

};


#endif