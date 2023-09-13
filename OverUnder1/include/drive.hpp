#include "main.h"

#ifndef DRIVE_HPP
#define DRIVE_HPP

class Drive //representing a 6-motor drivetrain
{
private:
    pros::Motor leftback, leftmiddle, leftfront, rightback, rightmiddle, rightfront;
    pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_06; //6:1 gear-ratio (blue cartridge)
    //okapi::ChassisControllerIntegrated odomoetry;


public:
    Drive (int lb, int lm, int lf, int rb, int rm, int rf);
    void setleft (int velocity);//set the left-side of the drivebase at a certain RPM
    void setright (int velocity); //set the right-side of the drivebase at a certain RPM
    void allcoast (); //set all 6 motors in the drivebase to 'coast'
    void allbrake (); //set all 6 motors in thedrivebase to 'hold'

};


#endif