#ifndef BINKBIK_LOADING_H
#define BINKBIK_LOADING_H

#include <thread>
#include <random>
#include "ResourceLoader.h"
#include "State.h"
#include "Machine.h"

class Loading
{
public:
    Loading(sf::RenderWindow &window, ResourceLoader &rl) : window(window), rl(rl)
    {
        rl.loadFont();

        // Create thread to load resources, allows loading visuals to operate separately
        std::thread loadingThread(&ResourceLoader::load, &rl);

        // Feel free to add loading messages. the more, the merrier.
        std::vector<std::string> messages =
                {
                        "Loading...",
                        "Duct-taping vaccum seals...",
                        "Inflating planets...",
                        "Writing coherent story...",
                        "Acting busy...",
                        "Filling void...",
                        "Populating galaxy...",
                        "Refueling spaceship...",
                        "Unraveling conspiracies...",
                        "Inventing space-propellers...",
                        "Feeding the space kraken...",
                        "Investing space bucks...",
                        "Locating Starlord...",
                        "Setting phasers to stun..."
                };

        // RNG to select random loading message
        long long seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> randomMessage(0, int(messages.size()) - 1);

        int threefourths = window.getSize().y * 3 / 4;
        int max = window.getSize().x;
        float targetlength = 0;

        sf::Text header("BINKBIK", *rl.font_Medium, 70);
        header.setPosition(window.getSize().x/2 - header.getLocalBounds().width/2, window.getSize().y/3);
        header.setFillColor(sf::Color(255, 255, 255, 0));

        sf::RectangleShape bar(sf::Vector2f(0, 3));
        bar.setPosition(0, threefourths);
        bar.setFillColor(sf::Color(200, 225, 255));

        sf::Text loadingMessage(messages[randomMessage(generator)], *rl.font, 24);
        loadingMessage.setPosition(window.getSize().x/2 - loadingMessage.getGlobalBounds().width/2, threefourths + 30);

        sf::Clock clock;

        while(!rl.done || bar.getSize().x < max){
            sf::Event event{};

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    // TODO: Handle close window event during loading screen
                }
            }
            targetlength = max*rl.loaded/rl.total;
            float difference = targetlength - bar.getSize().x;
            bar.setSize(sf::Vector2f(float(bar.getSize().x + (difference / 8) + 0.1), 3));
            header.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(std::min(int(clock.getElapsedTime().asSeconds() / 1.5 * 255), 255))));
            window.clear(sf::Color(0, 10, 30));
            window.draw(header);
            window.draw(bar);
            window.draw(loadingMessage);
            window.display();
        }
        loadingThread.join();
    }

    bool exit = false;
protected:
    ResourceLoader &rl;
    sf::RenderWindow &window;
};

#endif //BINKBIK_LOADING_H
