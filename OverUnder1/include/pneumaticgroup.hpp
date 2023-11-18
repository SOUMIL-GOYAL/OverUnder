#include "main.h"

#ifndef PNEUMATICGROUP_HPP
#define PNEUMATICGROUP_HPP

    class Pneumaticgroup {
        private:
            pros::ADIDigitalOut solenoid;
            bool status;

        public:
            Pneumaticgroup (char port, bool start);
            void closeboth();
            void openboth ();

            void toggle();

    };


#endif
