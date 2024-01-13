#include "main.h"

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

    class Detector {
        private:
            pros::ADIAnalogIn sensor;
            int divider;
            bool starter;

        public:
            Detector (char port, int divider_value, bool starting);
            bool status ();

    };


#endif
