#ifndef BINKBIK_OPTIONS_H
#define BINKBIK_OPTIONS_H

#include <functional>
#include <utility>
#include "State.h"
#include "Machine.h"
#include "SoundEffect.h"

class Options : public State
{
public:
    class Button
    {
    public:
        Button(const std::string &string, ResourceLoader &rl, std::function<void()> action)
                : text(string, *rl.font, 15), background(*rl.btn1, sf::IntRect(0,0,276,64)), action(std::move(action))
        {
            text.setOrigin(text.getLocalBounds().width/2, 24 - text.getLocalBounds().height/2);
            background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
        }

        sf::Text text;
        sf::Sprite background;
        std::function<void()> action;
    };

    Options(Machine &context, sf::RenderWindow &window);

    void enter() override;

    void run() override;

    void exit() override;

    void positionHeaderAndButtons();

    sf::Text header;

    std::vector<Button> buttons;

    std::vector<Button>::iterator selected;

    SoundEffect* soundEffect = SoundEffect::Instance();

    bool modified = false;

    std::string resolutions[6] =
            {"1024x768",
             "1200x800",
             "1280x1024",
             "1368x768",
             "1600x900",
             "1920x1080"
            };

    int resolutionSelected = 1;

    int volumeMusicSelected = 100, volumeSoundsSelected = 100;

    std::string volumeMusicSelectedString = std::to_string(volumeMusicSelected);
    std::string volumeSoundsSelectedString = std::to_string(volumeSoundsSelected);
};

#endif //BINKBIK_OPTIONS_H
