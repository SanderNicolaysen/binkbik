#include <iostream>
#include <plog/Log.h>
#include <SFML/Graphics.hpp>
#include <state/Loading.h>
#include "state/Machine.h"

// argc = no. of arguments
// argv = array of character arrays? arguments
int main(int  argc, char *argv[])
{
    plog::init(plog::debug, "binkbik.log");

    ResourceLoader rl;

    {
        // Create a small splash-screen window while loading
        sf::RenderWindow window(sf::VideoMode(500, 350), "Binkbik Steambang", sf::Style::None);
        window.setVerticalSyncEnabled(true);
        Loading loading(window, rl);
        window.close();
    }

    // Sleeping a bit before activating the main window prevents it from losing focus...
    sf::sleep(sf::milliseconds(20));

    // Setup main window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Binkbik Steambang", sf::Style::Default);
    window.setFramerateLimit(60);
    window.requestFocus();

    Machine machine(window, rl);

    // Iterate through all arguments and perform argument actions
    for (int i = 1; i < argc; i++)
    {
        if ((std::string)argv[i] == "skipmenu")
            machine.setState(Machine::GAME);
        else
            LOG_INFO << "Program argument \"" << argv[i] << "\" not recognized!";
    }

    machine.go();

    return 0;
}