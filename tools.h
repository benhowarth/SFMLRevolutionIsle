//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_TOOLS_H
#define SFMLREVOLUTIONISLE_TOOLS_H
#define M_PI 3.14159265358979323846

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// x range
extern int const colNo;
//y range
extern int const rowNo;

extern int const ISLAND_PRICE;

extern bool inControl;

extern int const screenWidth;
extern int const screenHeight;

extern sf::Font font;


enum CompassDir{N,E,S,W};

namespace vec {
    float Length(sf::Vector2f t_vec);
    float Length(sf::Vector2i t_vec);
    sf::Vector2f Normalise(sf::Vector2f t_vec);
    sf::Vector2f Normalise(sf::Vector2i t_vec);
    sf::Vector2f Lerp(sf::Vector2f t_vec1,sf::Vector2f t_vec2,float t_ratio);
}

extern sf::RenderWindow window;
//extern sf::View view;

float getRandom(float min,float max);

float getRandom();

#endif //SFMLREVOLUTIONISLE_TOOLS_H
