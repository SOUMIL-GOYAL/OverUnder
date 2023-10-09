#include "main.h"

#ifndef CATAPULT_HPP
#define CATAPULT_HPP

    class Catapult {
        private:
            pros::Motor motor;
            pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_18; //18:1 gear-ratio (green cartridge)
            pros::Rotation sensor;
            bool requestshot = false;
            bool requestreload = true;
            bool rapidfireon = false;
            int top;
            int bottom;
            int amount = 300;
            int tolerance = 500;
            double kp;
            double ka;
            double previousresult = 0;
        public:
            Catapult (int motorport, int sensorport, int top_angle, int bottom_angle);
            void launch ();
            void reload ();
            void rapidfire();
            
            bool isready();
            void taskmanager ();

            int getangle();
            
    };
#endif