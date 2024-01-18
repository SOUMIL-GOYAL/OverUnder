#include "climber.hpp"
#include "main.h"


Climber::Climber (Pneumaticgroup lift, Pneumaticgroup hook, Pneumaticgroup ptoitem) : lifter(lift), hooker(hook), pto(ptoitem) {
    // Climber::lifter = lift;
    // Climber::hooker = hook;
    // Climber::pto = ptoitem;
}

void Climber::goup() {
    int delaybetween = 300;
    lifter.toggle();
    // pros::delay(delaybetween);
    pto.toggle();
    pros::delay(delaybetween);
    hooker.toggle();
    pros::delay(delaybetween);
    lifter.toggle();
}

