#include <SFML/Window/Event.hpp>
#include "MainMenu.h"
#include "Game.h"
#include "GameSaveHandler.h"

MainMenu::MainMenu(Machine &context, sf::RenderWindow &window) : State(context, window)
{
    buttons.emplace_back(Button("New Game", context.rl, [this](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        this->context.setState(Machine::START);
    }));
    buttons.emplace_back(Button("Load Game", context.rl, [this](){
        GameSaveHandler gsh;
        GameSave gs = gsh.loadFromFile("save.json");
        if (gsh.isError())
        {
            LOGE << "Error loading GameSave file.";
            return;
        }
        this->context.resetState(Machine::GAME, new Game(this->context, this->window, this->context.rl, gs));
        this->context.setState(Machine::GAME);
    }));
    buttons.emplace_back(Button("Options", context.rl, [this](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        this->context.setState(Machine::OPTIONS);
    }));
    buttons.emplace_back(Button("Exit", context.rl, [this](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        this->context.setState(Machine::EXIT);
    }));

    header.setFont(*context.rl.font_Medium);
    header.setString("BINKBIK");
    header.setCharacterSize(200);
}

void MainMenu::enter()
{
    selected = buttons.begin();

    window.setView(sf::View(sf::FloatRect(0,0, window.getSize().x,
                                          window.getSize().y)));

    // Position header
    header.setOrigin(header.getGlobalBounds().width/2, header.getGlobalBounds().height/2);
    header.setPosition(window.getSize().x/2, 150);

    // Position buttons
    sf::Vector2f pos(window.getSize().x/2, 350);
    for (Button &btn: buttons)
    {
        pos.y += btn.background.getGlobalBounds().height + 15;
        btn.text.setPosition(pos);
        btn.background.setPosition(pos);
    }

    window.setMouseCursorVisible(true);
}

void MainMenu::run()
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed: context.setState(Machine::EXIT); break;

            case sf::Event::MouseButtonPressed:
            for (auto &btn : buttons)
                if (btn.background.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    btn.action();

            case sf::Event::MouseMoved:
            for (auto it = buttons.begin(); it != buttons.end(); ++it)
                if (it->background.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                {
                    if (selected != it) soundEffect->sounds[SoundEffect::SELECT].play();
                    selected = it;
                }

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Up:
                        soundEffect->sounds[SoundEffect::SELECT].play();
                        selected--;
                        break;
                    case sf::Keyboard::Down:
                        soundEffect->sounds[SoundEffect::SELECT].play();
                        selected++;
                        break;
                    case sf::Keyboard::Return:
                        selected->action();
                        break;
                    default: break;
                }
                break;

            default: break;
        }
    }

    // Enable circular 'navigation'/scrolling
    if (selected < buttons.begin()) selected = --buttons.end();
    if (selected >= buttons.end())  selected = buttons.begin();

    window.clear(sf::Color(0, 10, 15));

    // Draw header
    window.draw(header);

    // Draw buttons
    for (Button &btn: buttons)
    {
        if (&(*selected) == &btn)
            btn.background.setTextureRect(sf::IntRect(0, 64, 276, 64)); // Highlight
        else
            btn.background.setTextureRect(sf::IntRect(0, 0, 276, 64)); // Normal

        window.draw(btn.background);
        window.draw(btn.text);
    }

    window.display();
}

void MainMenu::exit()
{

}
