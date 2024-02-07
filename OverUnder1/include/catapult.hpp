#include "main.h"

#ifndef CATAPULT_HPP
#define CATAPULT_HPP

    class Catapult {
        private:
            pros::Motor motor;
            pros::motor_gearset_e ratio = pros::E_MOTOR_GEARSET_36; //18:1 gear-ratio (green cartridge)
            pros::Rotation sensor;
            bool requestshot = false;
            bool requestreload = true;
            bool rapidfireon = false;
            bool emergency = false;
            int top;
            int bottom;
            int tolerance;
            double kp;
            double ka;
        public:
            Catapult (int motorport, int sensorport, int top_angle, int bottom_angle);
            void launch ();
            void reload ();
            void rapidfire();
            void requestemergency(bool value);

            void taskmanager ();
            

            int getangle();

            void autofire(int seconds);
            
    };
#endif