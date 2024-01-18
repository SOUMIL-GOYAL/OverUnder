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
    if (status == false) {
        openboth();
    } else if (status == true) {
        closeboth();
    }
}