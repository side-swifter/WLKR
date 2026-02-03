#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;




// Top ten pistons
inline ez::Piston scraper('A');  
inline ez::Piston descore('B'); 
inline ez::Piston switcher('C');   





// elite motors
inline pros::MotorGroup intake({-9, 10}, pros::v5::MotorGears::blue);