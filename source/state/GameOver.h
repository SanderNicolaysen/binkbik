#include <ResourceLoader.h>
#include "Machine.h"
#include "State.h"

#ifndef BINKBIK_STARTSCREEN_H

class GameOver : public State
{
public:

    GameOver(Machine &context, sf::RenderWindow &window, ResourceLoader &rl) : State(context, window)
    {
        choices[0] = sf::Text("New Game", *rl.font, 20);
        choices[1] = sf::Text("Return to main menu", *rl.font, 20);
        choices[2] = sf::Text("Exit", *rl.font, 20);
    }
    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

    void update();

    void draw();

protected:
    SoundEffect* soundEffect = SoundEffect::Instance();

    int midX, midY;
    int selection{};
    sf::Text choices[3];
};

#endif //BINKBIK_GAMEOVER_H