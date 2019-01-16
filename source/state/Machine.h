#ifndef BINKBIK_MACHINE_H
#define BINKBIK_MACHINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include "ResourceLoader.h"
#include "State.h"

class Machine
{
public:
    enum StateID { MAIN_MENU, GAME, EXIT, OPTIONS, START, END, GAME_FINISHED };

    explicit  Machine(sf::RenderWindow &window, ResourceLoader& rl);

    void go();

    void setState(StateID state);

    void resetState(StateID state, State* stateobj);

    ResourceLoader &rl;
protected:

    std::map<StateID, State*> states;

    StateID state;

    sf::RenderWindow &window;
};


#endif //BINKBIK_MACHINE_H
