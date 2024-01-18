#include "climber.hpp"
#include "main.h"


Climber::Climber (Pneumaticgroup lift, Pneumaticgroup hook, Pneumaticgroup ptoitem) : lifter(lift), hooker(hook), pto(ptoitem) {
    // Climber::lifter = lift;
    // Climber::hooker = hook;
    // Climber::pto = ptoitem;
}

void Climber::goup() {
    lifter.toggle();
    pros::delay(500);
    hooker.toggle();
    pros::delay(500);
    pto.toggle();
    pros::delay(500);
    lifter.toggle();
}

