//
// Created by Ben on 08/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_NOISE_H
#define SFMLREVOLUTIONISLE_NOISE_H

#include <cmath>

float getNoise(float x, float y){
    return(std::sin(x*std::exp(y)+cos(y)));
}

#endif //SFMLREVOLUTIONISLE_NOISE_H
