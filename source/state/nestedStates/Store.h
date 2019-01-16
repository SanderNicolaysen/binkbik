#ifndef BINKBIK_STORE_H
#define BINKBIK_STORE_H

#include <entities/Spaceship.h>
#include <map>
#include <functional>
#include <utility>

#include "NestedState.h"
#include "state/Game.h"


class Store : public NestedState
{
public:
    class Button
    {
    public:
        Button(const std::string &s1, const std::string &s2, int price, ResourceLoader &rl, std::function<void()> action)
                : info(s1, *rl.font, 24), text(s2, *rl.font, 24), price(price), background(*rl.btn1, sf::IntRect(0,0,276,64)), action(std::move(action))
        {
            text.setOrigin(text.getLocalBounds().width / 2, 24 - text.getLocalBounds().height / 2);
            background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
        }

        sf::Text info;
        sf::Text text;
        int price;
        sf::Sprite background;
        std::function<void()> action;
    };

    Store(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window);

    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

    void update();

    void draw();

    void upgradeCompass();

protected:
    sf::RectangleShape panelBg;

    std::vector<Button> leftButtons;
    std::vector<Button> rightButtons;

    std::vector<Button>::iterator leftSelected;
    std::vector<Button>::iterator rightSelected;

    sf::Text priceInText;

    sf::Text header;

    bool leftSide;

    SpaceshipPtr spaceship;
    ShieldPtr shield;
    EntityPtr enemy;

    std::vector<Button>::iterator drillUpgradeButton;

    SoundEffect* soundEffect = SoundEffect::Instance();
};

#endif //BINKBIK_STORE_H