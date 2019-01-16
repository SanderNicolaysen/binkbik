#include "EndGame.h"
#include "Machine.h"


void EndGame::enter()
{
    LOGD << "Entered End Game screen";

    // Find horizontal and vertical center of screen/window
    int midX = window.getSize().x / 2;
    int midY = window.getSize().y / 2;

    text1.setPosition(midX - text1.getLocalBounds().width / 2, midY - 210);
    text2.setPosition(midX - text2.getLocalBounds().width / 2, midY - 150);

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

void EndGame::run()
{
    handleEvents();

    // Set all choices to white
    for (auto &choice : choices)
        choice.setFillColor(sf::Color::White);

    // Set selection to red
    choices[selection].setFillColor(sf::Color::Red);

    window.clear();

    window.draw(text1);
    window.draw(text2);

    for (auto &choice : choices)
        window.draw(choice);

    window.display();
}

void EndGame::exit()
{

}

void EndGame::handleEvents()
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
                        context.setState(context.GAME);
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
}