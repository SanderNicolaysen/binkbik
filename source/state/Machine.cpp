#include "Machine.h"
#include "Menu.h"
#include "Loading.h"
#include "StartScreen.h"
#include "GameOver.h"
#include "MainMenu.h"
#include "Options.h"
#include "EndGame.h"

Machine::Machine(sf::RenderWindow &window, ResourceLoader& rl) : window(window), rl(rl)
{
    states.emplace(StateID::MAIN_MENU, new MainMenu(*this, window));
    states.emplace(StateID::GAME, new Game(*this, window, rl));
    states.emplace(StateID::START, new StartScreen(*this, window, rl));
    states.emplace(StateID::END, new GameOver(*this, window, rl));
    states.emplace(StateID::OPTIONS, new Options(*this, window));
    states.emplace(StateID::GAME_FINISHED, new EndGame(*this, window));

    // Set starting state
    state = StateID::MAIN_MENU;
}

void Machine::go()
{
    while(state != StateID::EXIT)
    {
        State *running = states[state];
        running->enter();

        // Keep running as long as the state is unchanged
        do running->run();
        while (states[state] == running);
        running->exit();
    }

    if (window.isOpen())
    {
        window.close();
    }
}

void Machine::setState(StateID state)
{
    Machine::state = state;
}

void Machine::resetState(StateID state, State* stateobj)
{
    delete states[state];
    states[state] = stateobj;
}