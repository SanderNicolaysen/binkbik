#include <iostream>
#include "Pause.h"
#include "NestedMachine.h"
#include "GameSaveHandler.h"
#include "state/Game.h"

Pause::Pause(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window) : NestedState(nestedContext, context, window)
{
    choices[0] = sf::Text("Resume", *context.rl.font, characterSize);
    choices[1] = sf::Text("Save", *context.rl.font, characterSize);
    choices[2] = sf::Text("Main Menu", *context.rl.font, characterSize);
    choices[3] = sf::Text("Exit", *context.rl.font, characterSize);

    shadow.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    shadow.setFillColor(sf::Color(0, 0, 0, 50));

    arraySize = sizeof(choices) / sizeof(choices[0]);
}

void Pause::enter()
{
    LOGD << "Entered pause menu";

    shadow.setPosition(0, 0);

    // Always select first choice as default (Resume)
    selection = 0;
}

void Pause::run()
{
    if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::THRUST].setVolume(0);

    if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Playing)
        soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setVolume(0);

    handleEvents();
    update();
    draw();
}

void Pause::exit()
{

}

void Pause::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            context.setState(context.EXIT);

        // Add color to current choice
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                nestedContext.setRunning(false);

            if (event.key.code == sf::Keyboard::Up)
            {
                soundEffect->sounds[SoundEffect::SELECT].play();

                if (selection <= 0)
                    selection = arraySize - 1;
                else
                    selection--;
            }

            if (event.key.code == sf::Keyboard::Down)
            {
                soundEffect->sounds[SoundEffect::SELECT].play();

                if (selection >= arraySize - 1)
                    selection = 0;
                else
                    selection++;
            }
        }

        // Changes state according to selection
        if (event.key.code == sf::Keyboard::Return)
        {
            switch (selection)
            {
                case 0:
                    nestedContext.setRunning(false);
                    break;
                case 1:
                    {
                        // SAVE
                        GameSaveHandler gsh;
                        GameSave save = nestedContext.game.getSave();
                        gsh.saveToFile("save.json", save);
                    }
                    break;
                case 2:
                    context.setState(context.MAIN_MENU);
                    break;
                case 3:
                    nestedContext.setRunning(false);
                    context.setState(context.EXIT);
                    return;

                default:
                    break;
            }

            soundEffect->sounds[SoundEffect::ENTER].play();
        }
    }
}

void Pause::update()
{
    midX = window.getSize().x / 2;
    midY = window.getSize().y / 2;

    // Offset variables to put each choice below the previous
    int offsetY = 0;

    // Total height of pause menu
    float totalY = characterSize * arraySize + space * (arraySize - 1);

    // Set position for the menu
    for (auto &choice : choices)
    {
        choice.setPosition(window.getSize().x / 4, midY + offsetY - totalY / 2);
        offsetY += space + characterSize;
    }

    // Set view to default view so we can draw menu at correct position
    window.setView(sf::View(sf::FloatRect(0,0, window.getSize().x,
                                          window.getSize().y)));

    // Set all choices to white
    for (auto &choice : choices) {
        choice.setFillColor(sf::Color::White);
    }

    choices[selection].setFillColor(sf::Color::Blue);
}

void Pause::draw()
{
    window.draw(shadow);

    for (auto &choice : choices)
    {
        window.draw(choice);
    }
}
