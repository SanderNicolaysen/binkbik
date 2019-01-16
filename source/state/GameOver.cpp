#include <iostream>
#include "GameOver.h"
#include "entities/Spaceship.h"


void GameOver::enter()
{
    LOGD << "Entered game over screen";

    // Find horizontal and vertical center of screen/window
    int midX = window.getSize().x / 2;
    int midY = window.getSize().y / 2;

    // Offset variable to put each choice below the previous
    int offsetY = 0;

    // Iterate through all choices
    for (auto &choice : choices) {
        choice.setFillColor(sf::Color::White);
        choice.setPosition(midX - choice.getGlobalBounds().width / 2, midY - choice.getGlobalBounds().height / 2 + offsetY);
        offsetY += 50;
    }

    // Always select New Game by default
    selection = 0;
}

void GameOver::run()
{
    handleEvents();
    update();
    draw();
}

void GameOver::exit()
{

}

void GameOver::handleEvents()
{
    sf::Event event{};

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            context.setState(context.EXIT);

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up) {
                soundEffect->sounds[SoundEffect::SELECT].play();
                if (selection < 1 ){
                    selection = 2;
                } else {
                    selection--;
                }
            }
            if (event.key.code == sf::Keyboard::Down) {
                soundEffect->sounds[SoundEffect::SELECT].play();

                selection++;

                if (selection > 2 )
                    selection = 0;
            }

            if (event.key.code == sf::Keyboard::Return) {
                switch (selection) {
                    case 0:
                        context.setState(context.START);
                        break;
                    case 1:
                        context.setState(context.MAIN_MENU);
                        break;
                    default:
                        context.setState(context.EXIT);
                        return;
                }
                soundEffect->sounds[SoundEffect::ENTER].play();
            }
        }
    }

    // Set all choices to white
    for (auto &choice : choices) {
        choice.setFillColor(sf::Color::White);
    }
    // Set selection to red
    choices[selection].setFillColor(sf::Color::Red);

    window.clear();
    for (auto &choice : choices)
    {
        window.draw(choice);
    }
    window.display();
}

void GameOver::update()
{
}

void GameOver::draw()
{
}