#include <SFML/Window/Event.hpp>
#include "Options.h"

Options::Options(Machine &context, sf::RenderWindow &window) : State(context, window)
{
    buttons.emplace_back(Button("Resolution x, y:\n\t" + resolutions[resolutionSelected], context.rl, [this](){

        soundEffect->sounds[SoundEffect::SELECT].play();

        if (resolutionSelected > 5)
            resolutionSelected = 5;
        else if (resolutionSelected < 0)
            resolutionSelected = 0;

        buttons[0].text.setString("Resolution x, y:\n\t" + resolutions[resolutionSelected]);

        modified = true;
    }));
    buttons.emplace_back(Button("Music Volume:\n\t\t\t" + volumeMusicSelectedString, context.rl, [this](){
        soundEffect->sounds[SoundEffect::SELECT].play();

        if (volumeMusicSelected > 100)
            volumeMusicSelected = 0;
        else if (volumeMusicSelected < 0)
            volumeMusicSelected = 100;

        volumeMusicSelectedString = std::to_string(volumeMusicSelected);

        buttons[1].text.setString("Music Volume:\n\t\t\t" + volumeMusicSelectedString);
    }));
    buttons.emplace_back(Button("Sound Effects Volume:\n\t\t\t\t\t  " + volumeSoundsSelectedString, context.rl, [this](){
        soundEffect->sounds[SoundEffect::SELECT].play();

        if (volumeSoundsSelected > 100)
            volumeSoundsSelected = 0;
        else if (volumeSoundsSelected < 0)
            volumeSoundsSelected = 100;

        volumeSoundsSelectedString = std::to_string(volumeSoundsSelected);

        buttons[2].text.setString("Sound Effects Volume:\n\t\t\t\t\t  " + volumeSoundsSelectedString);
    }));
    buttons.emplace_back(Button("Apply changes", context.rl, [this, &window](){
        soundEffect->sounds[SoundEffect::ENTER].play();

        unsigned int x = 0, y = 0;

        switch (resolutionSelected)
        {
            case 0:
                x = 1024;
                y = 768;
                break;
            case 1:
                x = 1200;
                y = 800;
                break;
            case 2:
                x = 1280;
                y = 1024;
                break;
            case 3:
                x = 1368;
                y = 768;
                break;
            case 4:
                x = 1600;
                y = 900;
                break;
            case 5:
                x = 1920;
                y = 1080;
                break;
            default:
                break;
        }

        if (modified)
        {
            window.create(sf::VideoMode(x, y), "Binkbik Steambang", sf::Style::Default);
            window.setFramerateLimit(60);

            // Put window in the center of the screen
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2,
                                            sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));

            modified = false;
        }

        // Set music volume
        soundEffect->exploration.setVolume(volumeMusicSelected * soundEffect->antiClipMaxVol / 100.0f);
        soundEffect->enemyNearby.setVolume(volumeMusicSelected * soundEffect->antiClipMaxVol / 100.0f);

        if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Playing)
            soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].stop();

        if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Playing)
            soundEffect->sounds[SoundEffect::THRUST].stop();

        // Set sound effects volume
        for (int i = 0; i < SoundEffect::SOUND_COUNT; ++i) {
            soundEffect->sounds[i].setVolume(volumeSoundsSelected * soundEffect->antiClipMaxVol / 100.0f);
        }

        for (auto &smallImpact : soundEffect->smallImpacts)
        {
            smallImpact.setVolume(volumeSoundsSelected * soundEffect->antiClipMaxVol / 100.0f);
        }

        soundEffect->insideGasCloudMaxVolume = volumeSoundsSelected * soundEffect->antiClipMaxVol / 100.0f;
        soundEffect->thrustMaxVolume = volumeSoundsSelected * soundEffect->antiClipMaxVol / 100.0f;

        positionHeaderAndButtons();
    }));
    buttons.emplace_back(Button("Fullscreen", context.rl, [this, &window](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        window.create(sf::VideoMode(1920, 1080), "Binkbik Steambang", sf::Style::Fullscreen);
        window.setFramerateLimit(60);

        positionHeaderAndButtons();

        buttons[0].text.setString("Resolution x, y:\n\t1920x1080");
        resolutionSelected = 5;
    }));
    buttons.emplace_back(Button("Windowed Mode", context.rl, [this, &window](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        window.create(sf::VideoMode(1200, 800), "Binkbik Steambang", sf::Style::Default);
        window.setFramerateLimit(60);

        positionHeaderAndButtons();

        buttons[0].text.setString("Resolution x, y:\n\t1200x800");
        resolutionSelected = 1;
    }));
    buttons.emplace_back(Button("Go back", context.rl, [this](){
        soundEffect->sounds[SoundEffect::ENTER].play();
        this->context.setState(Machine::MAIN_MENU);
    }));

    header.setFont(*context.rl.font_Medium);
    header.setString("OPTIONS");
    header.setCharacterSize(200);
}

void Options::enter()
{
    selected = buttons.begin();

    positionHeaderAndButtons();

    window.setMouseCursorVisible(true);
}

void Options::run()
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Resized:
            {
                // Correct the view after resizing the window
                window.setView(sf::View(sf::FloatRect(0,0,event.size.width,
                                                      event.size.height)));

                positionHeaderAndButtons();
            }
                break;

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
                    case sf::Keyboard::Right:
                        // Resolution
                        if (selected == buttons.begin())
                        {
                            resolutionSelected++;
                            selected->action();
                        }
                        // Music volume
                        else if (selected == buttons.begin() + 1)
                        {
                            volumeMusicSelected += 5;
                            selected->action();
                        }
                        // Sfx Volume
                        else if (selected == buttons.begin() + 2)
                        {
                            volumeSoundsSelected += 5;
                            selected->action();
                        }
                        break;
                    case sf::Keyboard::Left:
                        // Resolution
                        if (selected == buttons.begin())
                        {
                            resolutionSelected--;
                            selected->action();
                        }
                        // Music volume
                        else if (selected == buttons.begin() + 1)
                        {
                            volumeMusicSelected -= 5;
                            selected->action();
                        }
                        // Sfx Volume
                        else if (selected == buttons.begin() + 2)
                        {
                            volumeSoundsSelected -= 5;
                            selected->action();

                        }
                        break;
                    case sf::Keyboard::M:
                        // Music volume
                        if (selected == buttons.begin() + 1)
                        {
                            volumeMusicSelected = 0;
                            selected->action();
                        }
                        else if (selected == buttons.begin() + 2)
                        {
                            volumeSoundsSelected = 0;
                            selected->action();
                        }
                        break;
                    case sf::Keyboard::Return:
                        if (selected == buttons.begin())
                            resolutionSelected++;
                        else if (selected == buttons.begin() + 1)
                            volumeMusicSelected += 5;
                        else if (selected == buttons.begin() + 2)
                            volumeSoundsSelected += 5;

                        selected->action();
                        break;
                    case sf::Keyboard::Escape:
                        context.setState(Machine::MAIN_MENU);
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

void Options::exit()
{

}

void Options::positionHeaderAndButtons()
{
    // Position header
    header.setOrigin(header.getGlobalBounds().width/2, header.getGlobalBounds().height/2);
    header.setPosition(window.getSize().x/2, 50);

    // Position buttons
    sf::Vector2f pos(window.getSize().x/2, 150);
    for (Button &btn: buttons)
    {
        pos.y += btn.background.getGlobalBounds().height + 15;
        btn.text.setPosition(pos);
        btn.background.setPosition(pos);
    }
}

