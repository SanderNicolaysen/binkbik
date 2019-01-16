#include "Menu.h"

void Menu::enter()
{
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

    // TODO: Select New Game by default if no save files are available?
    // Always select Load Game by default
    selection = 1;
}

void Menu::run()
// TODO: Make Load Game unselectable (or un-choosable) if no saves are available?
// TODO: Add Mouse input support
{
    sf::Event event{};

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            context.setState(context.EXIT);

        if (event.type == sf::Event::KeyPressed)
        {
            // Keypresses are handled, Menu loops so that pressing down from the bottom choice puts the selection back to the top
            // TODO: Get rid of these literals
            if (event.key.code == sf::Keyboard::Up) {
                soundEffect->sounds[SoundEffect::SELECT].play();
                if (selection < 1 ){
                    selection = 3;
                } else {
                    selection--;
                }
            }
            if (event.key.code == sf::Keyboard::Down) {
                soundEffect->sounds[SoundEffect::SELECT].play();
                if (selection > 2 ){
                    selection = 0;
                } else {
                    selection++;
                }
            }

            // TODO: I'm sure this could be more efficient if the choice array was a map where each sf::Text would have an associated StateID
            // Changes state according to selection
            if (event.key.code == sf::Keyboard::Return) {
                switch (selection) {
                    case 0:
                        context.setState(context.START);
                        break;
                    case 1:
                        // STATE ID NOT YET AVAILIBLE
                        // context.setState(context.LOAD);
                        break;
                    case 2:
                        // STATE ID NOT YET AVAILIBLE
                        // context.setState(context.SETTINGS);
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
    // Set selection to green
    choices[selection].setFillColor(sf::Color::Blue);

    window.clear();
    for (auto &choice : choices)
    {
        window.draw(choice);
    }
    window.display();
}

void Menu::exit()
{

}
