#include "detector.hpp"
#include "main.h"

Detector::Detector (char port, int divider_value, bool starting): sensor(port) {
    divider = divider_value;

    if (sensor.get_value() < divider) {
        starter = true;
    } 
    
}

bool Detector::status () {
    if (sensor.get_value() < divider) {
        return starter;
    } else {
        return !starter;
    }
}
