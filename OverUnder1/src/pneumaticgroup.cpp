#include "pneumaticgroup.hpp"
#include "main.h"

Pneumaticgroup::Pneumaticgroup (char port, bool start): solenoid(port) {
    status = start;
}

void Pneumaticgroup::openboth () {
    if (status == false) {
        solenoid.set_value(true);
        status = true;
    }
}

void Pneumaticgroup::closeboth () {
    if (status == true) {
        solenoid.set_value(false);
        status = false;
    }
}

void Pneumaticgroup::toggle () {
    if (status == true) {
        solenoid.set_value(false);
        status = false;
    } else if (status == false) {
        solenoid.set_value(true);
        status = true;
    }
}