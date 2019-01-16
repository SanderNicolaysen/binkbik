#ifndef BINKBIK_MAINMENU_H
#define BINKBIK_MAINMENU_H

#include <functional>
#include <utility>
#include "State.h"
#include "Machine.h"
#include "SoundEffect.h"

class MainMenu : public State
{
public:
    class Button
    {
    public:
        Button(const std::string &string, ResourceLoader &rl, std::function<void()> action)
                : text(string, *rl.font, 24), background(*rl.btn1, sf::IntRect(0,0,276,64)), action(std::move(action))
        {
            text.setOrigin(text.getLocalBounds().width/2, 24 - text.getLocalBounds().height/2);
            background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
        }

        sf::Text text;
        sf::Sprite background;
        std::function<void()> action;
    };

    MainMenu(Machine &context, sf::RenderWindow &window);

    void enter() override;

    void run() override;

    void exit() override;

    sf::Text header;

    std::vector<Button> buttons;

    std::vector<Button>::iterator selected;

    SoundEffect* soundEffect = SoundEffect::Instance();
};


#endif //BINKBIK_MAINMENU_H
