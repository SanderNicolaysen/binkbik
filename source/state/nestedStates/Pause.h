#ifndef BINKBIK_PAUSE_H
#define BINKBIK_PAUSE_H

#include "NestedState.h"
#include "ResourceLoader.h"
#include "SoundEffect.h"

class Pause : public NestedState
{
public:
    Pause(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window);

    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

    void update();

    void draw();

protected:
    int selection = 0;
    int midX, midY;
    int arraySize;

    unsigned int characterSize = 30;
    int space = 30;

    sf::Text choices[4];
    sf::RectangleShape shadow;

    SoundEffect* soundEffect = SoundEffect::Instance();
};

#endif //BINKBIK_PAUSE_H
