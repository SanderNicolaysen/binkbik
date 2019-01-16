#include <iostream>
#include <plog/Log.h>

#include "Store.h"
#include "NestedMachine.h"
#include "entities/Shield.h"

Store::Store(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window) : NestedState(nestedContext, context, window)
{
    leftButtons.emplace_back(Button(
            "100% fuel!",
            "Fuel", 2, context.rl, [this](){

            if (spaceship->fuel < spaceship->maxFuel)
            {
                if (spaceship->inventory.spendCash(leftSelected->price))
                    spaceship->fuel = spaceship->maxFuel;
            }
    }));

    leftButtons.emplace_back(Button(
            "100% health!",
            "Health", 2, context.rl, [this](){

                if (spaceship->health < spaceship->maxHealth)
                {
                    if (spaceship->inventory.spendCash(leftSelected->price))
                        spaceship->health = spaceship->maxHealth;
                }
            }));

    leftButtons.emplace_back(Button(
            "1 Powerful but slow missile\n\n"
            "Press 'M' to fire missile",
            "Missile", 1, context.rl, [this](){
                if (spaceship->inventory.spendCash(leftSelected->price))
                    spaceship->missilecount++;
            }));

    leftButtons.emplace_back(Button(
            "1 Homing missile that will\n"
            "guide itself toward enemies\n\n"
            "Press 'G' to fire homing missile",
            "Homing Missile", 2, context.rl, [this](){
                if (spaceship->inventory.spendCash(leftSelected->price))
                    spaceship->guidedmissilecount++;
            }));

    leftButtons.emplace_back(Button(
            "Increase fuel by 10%",
            "Fuel Upgrade", 5, context.rl, [this](){

        if (spaceship->inventory.spendCash(leftSelected->price))
        {
            spaceship->maxFuel += spaceship->maxFuel * 0.1f;
            spaceship->fuel = spaceship->maxFuel;
        }
    }));

    leftButtons.emplace_back(Button(
            "Increase health by 10%",
            "Health Upgrade", 5, context.rl, [this](){

        if (spaceship->inventory.spendCash(leftSelected->price))
        {
            // Increase health by 10% of maximum health
            spaceship->maxHealth += spaceship->maxFuel * 0.1f;
            spaceship->health = spaceship->maxHealth;
        }
    }));

    leftButtons.emplace_back(Button(
                    "Blue arrow  ->  Space-station\n"
                    "Green Arrow ->  Planet\n"
                    "Red Arrow   ->  Enemy",
                    "compass", 10, context.rl, [this](){ upgradeCompass();
            }));

    leftButtons.emplace_back(Button(
            "ENERGY LEVELS\n"
            "    <50%, 100%] ->  Blue\n"
            "    <20%, 50%]  ->  Orange\n"
            "    <0%, 20%]    ->  Red\n\n"
            "Press 'S' to activate shield",
            "Shield", 10, context.rl, [this](){

        if(!shield->upgrade)
        {
            if (spaceship->inventory.spendCash(leftSelected->price))
                shield->upgrade = true;
        }
    }));

    leftButtons.emplace_back(Button(
            "Increase shield energy by 5%",
            "Shield energy", 5, context.rl, [this](){

                if(shield->upgrade)
                {
                    if (spaceship->inventory.spendCash(leftSelected->price))
                    {
                        shield->maxHealth += shield->maxHealth * 0.05f;
                        shield->health = shield->maxHealth;
                    }
                }
            }));

    leftButtons.emplace_back(Button(
            "Upgrades your ship with\n"
                    "rocket thrusters!",
            "Engine upgrade", 115, context.rl, [this](){
                if (!spaceship->rocketUpgrade && spaceship->inventory.spendCash(leftSelected->price))
                {
                    spaceship->rocketUpgrade = true;
                    spaceship->speedLimit = 500;
                    spaceship->acceleration = 400;
                }
    }));

    drillUpgradeButton = leftButtons.end();
    leftButtons.emplace_back(Button(
            "Steel drill:  $40\n"
            "Titanium drill:  $100\n"
            "Gold drill:  $200\n",
            "Drill upgrade", 40, context.rl, [this](){
                if (spaceship->getDrill() == Spaceship::IRON && spaceship->inventory.spendCash(leftSelected->price))
                {
                    spaceship->setDrill(Spaceship::STEEL);
                    leftSelected->price = 100; // increase price
                }
                else if (spaceship->getDrill() == Spaceship::STEEL && spaceship->inventory.spendCash(leftSelected->price))
                {
                    spaceship->setDrill(Spaceship::TITANIUM);
                    leftSelected->price = 200; // increase price
                }
                else if (spaceship->getDrill() == Spaceship::TITANIUM && spaceship->inventory.spendCash(leftSelected->price))
                {
                    spaceship->setDrill(Spaceship::GOLD);
                    leftSelected->price = 0;
                }
    }));

    // RIGHT BUTTONS
    rightButtons.emplace_back(Button("", "Buy", 0, context.rl, [this](){
        leftSelected->action();
    }));
    rightButtons.emplace_back(Button("", "Sell Resources", 0, context.rl, [this](){
        spaceship->inventory.sellResources();
    }));

    // HEADER
    header.setFont(*context.rl.font);
    header.setString("UPGRADES");
    header.setCharacterSize(70);

    // STORE BACKGROUND
    panelBg.setTexture(context.rl.tex_panelBg);
    panelBg.setSize(sf::Vector2f(800.f, 600.f));

    // PRICE IN TEXT
    priceInText.setFont(*context.rl.font);
    priceInText.setCharacterSize(24);
}

void Store::enter()
{
    window.setView(sf::View(sf::FloatRect(0,0, window.getSize().x,
                                          window.getSize().y)));

    panelBg.setPosition(window.getSize().x / 2 - panelBg.getSize().x / 2, window.getSize().y / 2 - panelBg.getSize().y / 2 + 20);

    spaceship = nestedContext.game.getEntityCollection().getSpaceship();
    shield = std::dynamic_pointer_cast<Shield>(nestedContext.game.getEntityCollection().getEntityRange(Entity::SHIELD).begin()->second);

    // Set correct price on drill (needed after a loaded game)
    if (spaceship->getDrill() == Spaceship::STEEL)
        drillUpgradeButton->price = 100;
    else if (spaceship->getDrill() == Spaceship::TITANIUM)
        drillUpgradeButton->price = 200;
    else if (spaceship->getDrill() == Spaceship::GOLD)
        drillUpgradeButton->price = 0;

    leftSide = true;
    leftSelected = leftButtons.begin();
    rightSelected = rightButtons.begin();

    // Position header
    header.setOrigin(header.getLocalBounds().width / 2, header.getLocalBounds().height / 2);
    header.setPosition(panelBg.getPosition().x + panelBg.getLocalBounds().width / 2, panelBg.getPosition().y + 50);

    // scale buttons
    for (auto& btn : leftButtons)
        btn.background.setScale(1.1f, 1.1f);
    for (auto& btn : rightButtons)
        btn.background.setScale(1.1f, 1.1f);


}

void Store::run()
{
    if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::THRUST].setVolume(0);

    if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setVolume(0);

    handleEvents();
    update();
    draw();
}

void Store::exit()
{

}
void Store::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                context.setState(context.EXIT);
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        nestedContext.setRunning(false);
                        break;
                    case sf::Keyboard::Left:
                        soundEffect->sounds[SoundEffect::SELECT].play();
                        leftSide = true;
                        rightSelected = rightButtons.begin();
                        break;
                    case sf::Keyboard::Right:
                        soundEffect->sounds[SoundEffect::SELECT].play();
                        leftSide = false;
                        break;
                    case sf::Keyboard::Up:
                        if (leftSide)
                        {
                            soundEffect->sounds[SoundEffect::SELECT].play();
                            leftSelected--;
                        } else
                        {
                            soundEffect->sounds[SoundEffect::SELECT].play();
                            rightSelected--;
                        }
                        break;
                    case sf::Keyboard::Down:
                        if (leftSide)
                        {
                            soundEffect->sounds[SoundEffect::SELECT].play();
                            leftSelected++;
                        } else
                        {
                            soundEffect->sounds[SoundEffect::SELECT].play();
                            rightSelected++;
                        }
                        break;
                    case sf::Keyboard::Return:
                        if (leftSide)
                        {
                            soundEffect->sounds[SoundEffect::SELECT].play();
                            leftSide = false;
                        }
                        else
                        {
                            soundEffect->sounds[SoundEffect::ENTER].play();
                            rightSelected->action();
                        }

                    default: break;
                }

            default: break;
        }
    }

    if (leftSelected < leftButtons.begin()) leftSelected = leftButtons.begin();
    if (leftSelected >= leftButtons.end()) leftSelected = leftButtons.end() - 1;

    // Circular scrolling
    if (rightSelected < rightButtons.begin()) rightSelected = --rightButtons.end();
    if (rightSelected >= rightButtons.end()) rightSelected = rightButtons.begin();
}

void Store::update()
{
    // Position leftButtons
    sf::Vector2f posLeft(panelBg.getPosition().x + panelBg.getLocalBounds().width / 2 - 180, panelBg.getPosition().y + 100);
    if (leftSelected >= leftButtons.begin() + 4)
    {
        for (int i = 3; i >= 0; i--)
        {
            posLeft.y += (leftSelected - i)->background.getGlobalBounds().height + 30;
            (leftSelected - i)->background.setPosition(posLeft);
            (leftSelected - i)->text.setPosition(posLeft);
        }
    }
    else
    {
        for (auto btn = leftButtons.begin(); btn < leftButtons.begin() + 4; btn++)
        {
            posLeft.y += btn->background.getGlobalBounds().height + 30;
            btn->background.setPosition(posLeft);
            btn->text.setPosition(posLeft);
        }
    }
    // Position rightButtons
    sf::Vector2f posRight(panelBg.getPosition().x + panelBg.getLocalBounds().width - 180, leftButtons.begin()->background.getPosition().y + leftSelected->background.getGlobalBounds().height + 30);
    for (auto& btn : rightButtons)
    {
        posRight.y += btn.background.getGlobalBounds().height + 30;
        btn.background.setPosition(posRight);
        btn.text.setPosition(posRight);
    }

    // Highlight selected leftButton
    for (Button &btn : leftButtons)
    {
        if (&(*leftSelected) == &btn)
            btn.background.setTextureRect(sf::IntRect(0,64,276,64));
        else
            btn.background.setTextureRect(sf::IntRect(0,0,276,64));
    }

    // Highlight selected rightButton
    for (Button& btn : rightButtons)
    {
        if (&(*rightSelected) == &btn && !leftSide)
            btn.background.setTextureRect(sf::IntRect(0,64,276,64));
        else
            btn.background.setTextureRect(sf::IntRect(0,0,276,64));
    }

    // Position info
    leftSelected->info.setPosition(rightSelected->background.getPosition().x - rightSelected->background.getGlobalBounds().width / 2 + 10, leftButtons.begin()->background.getPosition().y - leftButtons.begin()->background.getGlobalBounds().height / 2);
    priceInText.setString("$ price " + std::to_string(leftSelected->price));
    priceInText.setOrigin(0, priceInText.getLocalBounds().height / 2);
    priceInText.setPosition(rightButtons.begin()->background.getPosition().x - rightButtons.begin()->background.getGlobalBounds().width / 2 + 10, rightButtons.begin()->background.getPosition().y - rightButtons.begin()->background.getGlobalBounds().height / 2 - 40);

    // Decrease character size if width of text is larger than panel
    while (leftSelected->info.getPosition().x + leftSelected->info.getGlobalBounds().width > panelBg.getPosition().x + panelBg.getLocalBounds().width - 12)
        leftSelected->info.setCharacterSize(leftSelected->info.getCharacterSize() - 1);
}

void Store::draw()
{
    window.draw(panelBg);
    window.draw(header);

    // Draw left buttons
    if (leftSelected >= leftButtons.begin() + 4)
    {
        for (int i = 3; i >= 0; i--)
        {
            window.draw((leftSelected - i)->background);
            window.draw((leftSelected - i)->text);
        }
    }
    else
    {
        for (auto btn = leftButtons.begin(); btn < leftButtons.begin() + 4; btn++)
        {
            window.draw(btn->background);
            window.draw(btn->text);
        }
    }

    // Draw right buttons
    for (Button& btn : rightButtons)
    {
        window.draw(btn.background);
        window.draw(btn.text);
    }

    // Draw info
    window.draw(leftSelected->info);
    window.draw(priceInText);
}

void Store::upgradeCompass()
{
    if (spaceship->homeCompass == nullptr && spaceship->inventory.spendCash(leftSelected->price))
    {
        spaceship->homeCompass = new Compass(nestedContext.game, nestedContext.game.getEntityCollection().getSpacestation(), sf::Color(122,122,255,80));
        spaceship->planetCompass = new Compass(nestedContext.game, Entity::PLANET, sf::Color(122,255,122,80));
        spaceship->enemyCompass = new Compass(nestedContext.game, Entity::ENEMY, sf::Color(255,122,122,80));
    }
}
