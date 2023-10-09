#include "main.h"

#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen //representing a 6-motor drivetrain
{
private:



public:
    Screen (int port);
    void inmax ();//set the left-side of the drivebase at a certain RPM
    void off (); //set the right-side of the drivebase at a certain RPM
    void outmax (); //set all 6 motors in the drivebase to 'coast'
    void movespeed (int velocity); //set all 6 motors in thedrivebase to 'hold'

};


#endif