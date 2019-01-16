#ifndef BINKBIK_VIGNETTE_H
#define BINKBIK_VIGNETTE_H


#include <SFML/Graphics.hpp>
#include "ResourceLoader.h"

class Vignette
{
public:
    explicit Vignette(ResourceLoader &rl);

    void draw(sf::RenderWindow &window);

    void setColor(sf::Color color);

protected:
    sf::RectangleShape vignette;

    sf::Color color;

    sf::Clock timer;
};


#endif //BINKBIK_VIGNETTE_H
