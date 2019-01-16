#ifndef BINKBIK_MENU_H
#define BINKBIK_MENU_H

#include "State.h"
#include "Game.h"
#include "SoundEffect.h"

////////////////////////////
//// MAIN MENU CLASS
////////////////////////////

class Menu : public State
{
public:
    Menu(Machine &context, sf::RenderWindow &window, ResourceLoader &rl) : State(context, window) {
        choices[0] = sf::Text("New Game", *rl.font, 20);
        choices[1] = sf::Text("Load Game", *rl.font, 20);
        choices[2] = sf::Text("Settings", *rl.font, 20);
        choices[3] = sf::Text("Exit", *rl.font, 20);
    }

    void enter() override;

    void run() override;

    void exit() override;

protected:
    int selection{};

    // TODO: Consider making this a map<StateID, std::tuple<sf::Text, bool selectable>> or something
    sf::Text choices[4];

    SoundEffect* soundEffect = SoundEffect::Instance();
};

#endif //BINKBIK_MENU_H