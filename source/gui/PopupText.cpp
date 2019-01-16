#include "PopupText.h"

PopupText::PopupText(ResourceLoader &rl)
{
    font = rl.font;
    text.setFont(*font);
    text.setCharacterSize(24);
}

void PopupText::setText(std::string text)
{
    this->text.setString(text);
    clock.restart();
}

void PopupText::draw(sf::RenderWindow &window)
{
    if (clock.getElapsedTime().asSeconds() > 1.5f || text.getString() == "")
        return;

    text.setPosition(window.getSize().x/2 - text.getGlobalBounds().width/2,
                     window.getSize().y - text.getGlobalBounds().height - 25);
    text.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 255 - clock.getElapsedTime().asMilliseconds()*0.17f));
    window.draw(text);
}
