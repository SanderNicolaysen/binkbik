#ifndef BINKBIK_NESTED_MACHINE_H
#define BINKBIK_NESTED_MACHINE_H

#include "state/Machine.h"
#include "NestedState.h"

// A separate state-machine for states that need to implement nested states (states within a state)

class Game;

class NestedMachine
{
public:
    enum StateID { NONE, PAUSE, STORE, PLANETVIEW };

    explicit NestedMachine(Machine &context,
                           sf::RenderWindow &window,
                           Game &game);

    void go();

    void setState(StateID state);

    void resetState(StateID state, NestedState* stateObj);

    bool getRunning() const { return running; }

    void setRunning(bool running) { this->running = running; }

    Game& game;

protected:
    std::map<StateID, NestedState*> states;
    bool running;

    StateID state;

    Machine& context;
    sf::RenderWindow &window;
};

#endif //BINKBIK_NESTED_MACHINE_H
