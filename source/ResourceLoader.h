#ifndef BINKBIK_RESOURCELOADER_H
#define BINKBIK_RESOURCELOADER_H

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <plog/include/plog/Log.h>
#include <SFML/Graphics/Font.hpp>
#include "SoundEffect.h"

/***
 * Class for preloading and storing game resources (textures, audio etc.)
 */
class ResourceLoader {
public:
    int total =
    44 +
    SoundEffect::SOUND_COUNT +  // sfx
    12                          // sfx small_impact
    ;
    
    int loaded = 0;

    bool done = false;

    std::vector<sf::Texture*> tex_planets;
    sf::Texture* tex_asteroid1;
    sf::Texture* tex_asteroid2;
    sf::Texture* tex_asteroid3;
    sf::Texture* tex_asteroid4;
    sf::Texture* tex_asteroid5;
    sf::Texture* tex_asteroid6;
    sf::Texture* tex_asteroid7;
    sf::Texture* tex_asteroid8;
    sf::Texture* tex_resources;
    sf::Texture* tex_spaceship;
    sf::Texture* tex_drill;
    sf::Texture* tex_propeller;
    sf::Texture* tex_exhaust;
    sf::Texture* tex_spacestation;
    sf::Texture* tex_background;
    sf::Texture* tex_fuelcan;
    sf::Texture* tex_healthHeart;
    sf::Texture* tex_energy;
    sf::Texture* tex_enemy;
    sf::Texture* tex_beam;
    sf::Texture* tex_gasCloud;
    sf::Texture* tex_spacerock;
    sf::Texture* tex_compass;
    sf::Texture* tex_panelBg;
    sf::Texture* tex_vignette;
    sf::Texture* tex_shield;
    sf::Texture* tex_guidedmissile;
    sf::Texture* tex_missile;

    sf::Texture* btn1;

    sf::Font* font = new sf::Font;
    sf::Font* font_Black = new sf::Font;
    sf::Font* font_Bold = new sf::Font;
    sf::Font* font_Medium = new sf::Font;

    void load();
    void loadFont();
protected:
    sf::Texture* loadfile(std::string filename);

    sf::Font* loadFontFromFile(std::string filename);

    sf::SoundBuffer loadSfx(std::string filename);

    SoundEffect* soundEffect = SoundEffect::Instance();
};

#endif //BINKBIK_RESOURCELOADER_H