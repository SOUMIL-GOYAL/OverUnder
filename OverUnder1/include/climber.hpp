#include "main.h"
#include "pneumaticgroup.hpp"

#ifndef CLIMBER_HPP
#define CLIMBER_HPP

    class Climber {
        private:
            Pneumaticgroup lifter;
            Pneumaticgroup hooker;
            Pneumaticgroup pto;
        public:
            Climber (Pneumaticgroup lift, Pneumaticgroup hook, Pneumaticgroup ptoitem);
            void goup();
    };
#endif