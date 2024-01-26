#include "drive.hpp"
#include "main.h"
#include "lemlib/api.hpp"


Drive::Drive (int lb, int lm, int lf, int rb, int rm, int rf, int inertialport, int distanceport)
: leftback(lb, pros::E_MOTOR_GEARSET_06), 
leftmiddle(lm, pros::E_MOTOR_GEARSET_06), 
leftfront(lf, pros::E_MOTOR_GEARSET_06), 
rightback(rb, pros::E_MOTOR_GEARSET_06), 
rightmiddle(rm, pros::E_MOTOR_GEARSET_06), 
rightfront(rf, pros::E_MOTOR_GEARSET_06),
inertia(inertialport), 
distance(distanceport),
leftmotors({leftback, leftmiddle, leftfront}),
rightmotors({rightback, rightmiddle, rightfront}),
drivetrain {
    &leftmotors, // left drivetrain motors
    &rightmotors, // right drivetrain motors
    12, // track width
    4, // wheel diameter
    600*(36/72) // wheel rpm
},
sensors {
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &inertia // inertial sensor
},
lateralController {
    8, // kP
    30, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    5 // slew rate
},
angularController {
    4, // kP
    40, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    40 // slew rate
},
chassis(drivetrain, lateralController, angularController, sensors) {

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

    kp = .5;
    ki = 0;
    ka = 5;

    leftback.tare_position();
    rightback.tare_position();
  
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

void Drive::turnto (double goal) {
    // double L = (leftback.get_position() / 360) * 4 * 3.1415926535;
    // double R = (rightback.get_position() / 360) * 4 * 3.1415926535;
    // return (fmod((L-R)/8.6, 2 * 3.1415926535));
    double kP = .4;
    double kI = .0000;//4
    double error = 0;
    double result;
    double totalerror = 0;

    while (true) {
    error = fmod((goal - ((lround(fmod(inertia.get_rotation(),360)) + 540) % 360 - 180) + 540), 360) - 180;
    totalerror += error;
    if (fabs(error) <= 2) {
        setleft(0);
        setright(0);
        allbrake();
        break;
    }
    result = (error * kP) + (totalerror * kI);
    setleft(result*6);
    setright(-result*6);
    pros::delay(20);
    }
}

void Drive::go (double inches) {
    double starting = leftback.get_position();
    double previousresult = 0;
    while (true) {
        double error = (inches/3.7699)*180 - leftback.get_position() + starting;

        double result = error * kp;

        if ((previousresult - result) <= -ka) {
            result = previousresult + ka;
        } else if ((previousresult - result) >= ka) {
            result = previousresult - ka;
        }
        
        setright(int(result));
        setleft(int(result));



        if (error <= 20 && error >= -20) {
            setleft(0);
            setright(0);
            break;
        }

        previousresult = result;

        pros::lcd::set_text(4, std::to_string(result));

        pros::delay(20);
    }
}


void Drive::gotime (double secs, bool direction) {
    if (direction) {
        setright(500);
        setleft(500);
    } else {
        setright(-500);
        setleft(-500);
    }
    
    pros::delay(secs * 1000);
    setleft(0);
    setright(0);
}

void Drive::godistance (double inches) {
    double previousresult = 0;
    int counter = 0;
    while (true) {
        // double error = (inches/3.7699)*180 - (distance.get()/3.7699)*180 + starting;

        //double error = (((distance.get() / 25.4) - inches)/3.7699) * 180 ;

        double error = distance.get() - (inches * 25.4);


        double result = error * kp;

        if ((previousresult - result) <= -ka) {
            result = previousresult + ka;
        } else if ((previousresult - result) >= ka) {
            result = previousresult - ka;
        }
        
        int speed = 200;

        if (error >= 0) {
            setright(speed);
            setleft(speed);
        } else {
           setright(-speed);
            setleft(-speed); 
        }
        



        if (error <= 25.4 && error >= -25.4) {
            setleft(0);
            setright(0);
            break;
        }

        if (counter >= 180) {
            setleft(0);
            setright(0);
            break;
        }

        previousresult = result;

        pros::lcd::set_text(4, "distance: " + std::to_string(distance.get()));
        pros::lcd::set_text(4, "error: " + std::to_string(error));

        counter++;
        pros::delay(20);
    }
}

void Drive::printposition () {
    lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
    pros::lcd::print(5, "x: %f", pose.x); // print the x position
    pros::lcd::print(6, "y: %f", pose.y); // print the y position
    pros::lcd::print(7, "heading: %f", pose.theta); // print the heading
}