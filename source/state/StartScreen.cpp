#include <iostream>
#include "StartScreen.h"
#include "Game.h"

void StartScreen::enter()
{
    LOGD << "Entered start screen";

    shadow.setPosition(0, 0);
    texts.setPosition(midX /2 - 200, midY /2);

    window.setMouseCursorVisible(false);
}

void StartScreen::run()
{
    handleEvents();
    update();

    window.clear();
    draw();
    window.display();
}

void StartScreen::exit()
{

}

void StartScreen::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            context.setState(context.EXIT);
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                context.resetState(context.GAME, new Game(context, window, context.rl));
                context.setState(context.GAME);
            }
        }
    }
}

void StartScreen::update()
{

}

void StartScreen::draw()
{
    window.draw(shadow);
    window.draw(texts);
}