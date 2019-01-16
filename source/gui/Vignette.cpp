#include "Vignette.h"


Vignette::Vignette(ResourceLoader &rl)
{
    vignette.setTexture(rl.tex_vignette);
    color.a = 0; // Make sure we don't accidentally flash when the game starts
}

void Vignette::draw(sf::RenderWindow &window)
{
    if (color.a <= 0)
        return;

    vignette.setFillColor(color);
    vignette.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

    // Fade out - takes 1 second on fully opaque colors
    int a = color.a - int(255 * timer.restart().asSeconds());
    color.a = static_cast<sf::Uint8>(a < 0 ? 0 : a);

    window.draw(vignette);
}

void Vignette::setColor(sf::Color color)
{
    this->color = color;
    timer.restart();
}
