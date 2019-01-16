#ifndef BINKBIK_FLASHINGTEXT_H
#define BINKBIK_FLASHINGTEXT_H


#include <SFML/Graphics.hpp>
#include <ResourceLoader.h>

class PopupText
{
public:
    explicit PopupText(ResourceLoader &rl);

    void draw(sf::RenderWindow &window);

    void setText(std::string text);

protected:
    sf::Font* font;

    sf::Text text;

    sf::Clock clock;
};


#endif //BINKBIK_FLASHINGTEXT_H
