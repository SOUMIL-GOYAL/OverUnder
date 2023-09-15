#include "main.h"

#ifndef CATAPULT_HPP
#define CATAPULT_HPP

    class Catapult {
        private:
            pros::Motor motor;
            pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_06; //6:1 gear-ratio (blue cartridge)
            pros::Rotation sensor;
            bool shot = false;
        public:
            Catapult (int motorport, int sensorport);
            void launch ();
            void reload ();
            void getangle();
            
    };
#endif