#include "main.h"

#ifndef CATAPULT_HPP
#define CATAPULT_HPP

    class Catapult {
        private:
            pros::Motor motor;
            pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_06; //6:1 gear-ratio (blue cartridge)
            pros::Rotation sensor;
            bool requestshot = false;
            bool requestreload = true;
            int top;
            int bottom;
            bool ready;
            int amount = 300;
            int shootangle;
        public:
            Catapult (int motorport, int sensorport, int top_angle, int bottom_angle, int shoot_angle);
            void launch ();
            void reload ();
            int getangle();
            bool isready();
            void taskmanager ();

            
    };
#endif