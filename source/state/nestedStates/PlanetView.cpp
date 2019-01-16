#include "PlanetView.h"
#include "NestedMachine.h"

PlanetView::PlanetView(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window, int planetID)
        : NestedState(nestedContext, context, window)
{
    // TODO: Get texture/font through ResourceLoader
    if (!texture.loadFromFile("textures/panel.png"))
        std::cout << "Error loading 'panel.png'" << std::endl;
    if (!font.loadFromFile("font/CamingoCode-Regular.ttf"))
        std::cout << "Error loading 'font/CamingoCode-Regular.ttf'" << std::endl;

    background.setTexture(texture);
    background.setPosition(window.getSize().x/2 - background.getLocalBounds().width/2,
                           window.getSize().y/2 - background.getLocalBounds().height/2);

    text.setFont(font);
    text.setString(planetTexts[planetID - 1]);
    text.setCharacterSize(18);
    text.setPosition(background.getPosition() + sf::Vector2f(55, 80));
}

void PlanetView::enter()
{

}

void PlanetView::run()
{
    if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::THRUST].setVolume(0);

    if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setVolume(0);

    sf::Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                context.setState(context.EXIT);
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        nestedContext.setRunning(false);
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }


    window.draw(background);
    window.draw(text);
}

void PlanetView::exit()
{

}
