#ifndef BINKBIK_STATE_H
#define BINKBIK_STATE_H


#include <plog/include/plog/Log.h>
#include <SFML/Graphics.hpp>

class Machine; // Forward declaration of Machine to prevent circular include

class State
{
public:
    explicit State(Machine &context, sf::RenderWindow &window);

    virtual void enter() = 0;

    virtual void run() = 0;

    virtual void exit() = 0;

protected:
    Machine &context;

    sf::RenderWindow &window;
};


#endif //BINKBIK_STATE_H
