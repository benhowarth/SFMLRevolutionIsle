//
// Created by Ben on 23/02/2019.
//
#include <cmath>
#include "noise.h"

float getNoise(float x, float y){
    return(std::sin(x*std::exp(y)+cos(y)));
}