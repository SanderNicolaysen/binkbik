#include "NestedMachine.h"
#include "Pause.h"
#include "Store.h"
#include "PlanetView.h"

NestedMachine::NestedMachine(Machine &context,
                             sf::RenderWindow &window,
                             Game& game)
                                         : context(context),
                                           window(window),
                                           game(game)
{
    running = false;

    states.emplace(StateID::PAUSE, new Pause(*this, context, window));
    states.emplace(StateID::STORE, new Store(*this, context, window));
    states.emplace(StateID::PLANETVIEW, new PlanetView(*this, context, window, 1));
    state = StateID::NONE;
}

void NestedMachine::go()
{
    states[state]->run();
}

void NestedMachine::setState(StateID state)
{
    // Don't crash on first setState
    if (this->state != StateID::NONE)
        states[state]->exit();

    this->state = state;
    states[this->state]->enter();
}

void NestedMachine::resetState(NestedMachine::StateID state, NestedState *stateObj)
{
    delete states[state];
    states[state] = stateObj;
}
