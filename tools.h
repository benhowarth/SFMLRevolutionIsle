//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_TOOLS_H
#define SFMLREVOLUTIONISLE_TOOLS_H


// x range
extern int const colNo;
//y range
extern int const rowNo;



extern int const screenWidth;
extern int const screenHeight;


namespace vec {
    float Length(sf::Vector2f _vec);
    sf::Vector2f Normalise(sf::Vector2f _vec);
}

extern sf::RenderWindow window;
extern sf::View view;


#endif //SFMLREVOLUTIONISLE_TOOLS_H
