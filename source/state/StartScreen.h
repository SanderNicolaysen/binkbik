#include <ResourceLoader.h>
#include "Machine.h"
#include "State.h"

#ifndef BINKBIK_STARTSCREEN_H

class StartScreen : public State
{
public:

    StartScreen(Machine &context, sf::RenderWindow &window, ResourceLoader &rl) : State(context, window)
    {
        texts = sf::Text("Welcome to Binkbik Steambang!\n"
                                 "\n"
                                 "In this game, you are supposed to mine asteroids and\n"
                                 "gather resources to sell at the store in your spacestation.\n"
                                 "There are many dangers in space, so be careful.\n"
                                 "Make sure to buy upgrades for your ship to be\n"
                                 "better prepared for the challenges you will meet!\n"
                                 "If you run out of fuel, or if your ship gets destroyed,\n"
                                 "game over. Fly safe!\n"
                                 "\n"
                                 "Press enter to start the game", *rl.font, 20);

        midX = window.getSize().x / 2;
        midY = window.getSize().y / 2;

        shadow.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
    }
    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

    void update();

    void draw();

protected:
    int midX, midY;

    sf::Font font;

    sf::Text texts;

    sf::RectangleShape shadow;
};

#endif //BINKBIK_STARTSCREEN_H