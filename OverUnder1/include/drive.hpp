#include "main.h"
#include "lemlib/api.hpp"


#ifndef DRIVE_HPP
#define DRIVE_HPP

class Drive //representing a 6-motor drivetrain
{
private:
    pros::Motor leftback, leftmiddle, leftfront, rightback, rightmiddle, rightfront;
    pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_06; //6:1 gear-ratio (blue cartridge)
    pros::IMU inertia;
    pros::Distance distance;
    //okapi::ChassisControllerIntegrated odomoetry;
    //okapi::ChassisControllerPID odometry;
    // std::shared_ptr<okapi::OdomChassisController> odometry;

    pros::MotorGroup leftmotors;
    pros::MotorGroup rightmotors;
    lemlib::Drivetrain_t drivetrain;
    lemlib::OdomSensors_t sensors;
    lemlib::ChassisController_t lateralController;
    lemlib::ChassisController_t angularController;
    lemlib::Chassis chassis;


    double DL;
    double DR;

    double kp, ki, ka;


public:
    Drive (int lb, int lm, int lf, int rb, int rm, int rf, int inertialport, int distanceport);

    void setleft (int velocity);//set the left-side of the drivebase at a certain RPM
    void setright (int velocity); //set the right-side of the drivebase at a certain RPM
    void allcoast (); //set all 6 motors in the drivebase to 'coast'
    void allbrake (); //set all 6 motors in thedrivebase to 'hold'


    void turnto(double goal);

    void go(double inches);
    void gotime(double secs, bool direction);
    void godistance(double inches);

    void printposition();

};


#endif