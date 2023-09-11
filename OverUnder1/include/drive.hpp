#include "main.h"

#ifndef DRIVE_HPP
#define DRIVE_HPP

class Drive
{
private:
    // int lbp, lmp, lfp, rbp, rmp, rfp;
    pros::Motor leftback, leftmiddle, leftfront, rightback, rightmiddle, rightfront;
public:
    Drive::Drive (int lb, int lm, int lf, int rb, int rm, int rf):leftback(lb, pros::E_MOTOR_GEARSET_06), leftmiddle(lm, pros::E_MOTOR_GEARSET_06), leftfront(lf, pros::E_MOTOR_GEARSET_06), rightback(rb, pros::E_MOTOR_GEARSET_06), rightmiddle(rm, pros::E_MOTOR_GEARSET_06), rightfront(rf, pros::E_MOTOR_GEARSET_06)  {}
    void setleft (int velocity) {}
    void setright (int velocity) {}
    void allcoast () {}
    void allbrake () {}

};


#endif