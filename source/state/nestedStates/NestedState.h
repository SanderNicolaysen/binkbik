#ifndef BINKBIK_NESTEDSTATE_H
#define BINKBIK_NESTEDSTATE_H

#include <plog/include/plog/Log.h>
#include <SFML/Graphics.hpp>

#include "state/Machine.h"

class NestedMachine; // Forward declaration of NestedMachine to prevent circular include

class NestedState
{
public:
    explicit NestedState(NestedMachine& nestedContext, Machine &context, sf::RenderWindow &window)
            :  context(context), window(window), nestedContext(nestedContext) {};

    virtual void enter() = 0;

    virtual void run() = 0;

    virtual void exit() = 0;

protected:
    Machine &context;

    NestedMachine &nestedContext;

    sf::RenderWindow &window;
};

#endif //BINKBIK_NESTEDSTATE_H
