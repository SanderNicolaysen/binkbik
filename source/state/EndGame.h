#ifndef BINKBIK_ENDGAME_H
#define BINKBIK_ENDGAME_H


#include "State.h"
#include "ResourceLoader.h"
#include "Machine.h"

class EndGame : public State
{
public:

    EndGame(Machine &context, sf::RenderWindow &window) : State(context, window)
    {
        choices[0] = sf::Text("Continue playing", *context.rl.font, 20);
        choices[1] = sf::Text("Return to main menu", *context.rl.font, 20);
        choices[2] = sf::Text("Exit", *context.rl.font, 20);

        text1 = sf::Text("Congratulations!", *context.rl.font, 45);
        text2 = sf::Text("You have explored all the planets known to man!", *context.rl.font, 20);
    }
    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

protected:
    SoundEffect* soundEffect = SoundEffect::Instance();

    int selection{};
    sf::Text choices[3];

    sf::Text text1;
    sf::Text text2;
};


#endif //BINKBIK_ENDGAME_H
