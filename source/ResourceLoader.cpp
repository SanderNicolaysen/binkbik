#include <SFML/System/Time.hpp>
#include <SFML/Window.hpp>
#include "ResourceLoader.h"

void ResourceLoader::loadFont()
{
    // LOAD FONT FIRST, LOADING SCREEN DOES NOT APPEAR UNTIL FONT IS LOADED
    font = loadFontFromFile("font/CamingoCode-Regular.ttf");
    font = loadFontFromFile("font/Orbitron-Regular.ttf");
    font_Black = loadFontFromFile("font/Orbitron-Black.ttf");
    font_Bold = loadFontFromFile("font/Orbitron-Bold.ttf");
    font_Medium = loadFontFromFile("font/Orbitron-Medium.ttf");
}

void ResourceLoader::load()
{
    // LOAD BEAM
    tex_beam = loadfile("textures/beam.png");

    // LOAD GAS CLOUD
    tex_gasCloud = loadfile("textures/gasCloud.png");

    // LOAD ENEMY
    tex_enemy = loadfile("textures/enemy.png");

    // LOAD FUEL CAN
    tex_fuelcan = loadfile("textures/fuelCan.png");

    // LOAD HEALTH HEART
    tex_healthHeart = loadfile("textures/healthHeart.png");

    tex_energy = loadfile("textures/lightning.png");

    // Load the background image
    tex_background = loadfile("textures/background.png");
    tex_background->setRepeated(true);

    // LOAD SPACE STATION
    tex_spacestation = loadfile("textures/spacestation.png");

    // Load "spacerock" texture
    tex_spacerock = loadfile("textures/spacerock.png");

    // LOAD SPACESHIP
    // TODO Change spaceship texture to something more awesome/relevant
    tex_spaceship = loadfile("textures/ship.png");
    tex_drill = loadfile("textures/drill.png");
    tex_propeller = loadfile("textures/space_propeller.png");
    tex_exhaust = loadfile("textures/exhaust.png");

    // LOAD ASTEROID
    tex_asteroid1 = loadfile("textures/asteroid_1.png");
    tex_asteroid2 = loadfile("textures/asteroid_2.png");
    tex_asteroid3 = loadfile("textures/asteroid_3.png");
    tex_asteroid4 = loadfile("textures/asteroid_4.png");
    tex_asteroid5 = loadfile("textures/asteroid_5.png");
    tex_asteroid6 = loadfile("textures/asteroid_6.png");
    tex_asteroid7 = loadfile("textures/asteroid_7.png");
    tex_asteroid8 = loadfile("textures/asteroid_8.png");

    // LOAD PANEL
    tex_panelBg = loadfile("textures/panel.png");

    // LOAD VIGNETTE
    tex_vignette = loadfile("textures/vignette.png");

    // LOAD SHIELD
    tex_shield = loadfile("textures/spr_shield.png");

    // LOAD GEM
    tex_resources = loadfile("textures/resources.png");

    // LOAD BUTTONS FOR MENU
    btn1 = loadfile("textures/btn1.png");

    // LOAD MISSILES
    tex_guidedmissile = loadfile("textures/guidedmissile.png");
    tex_missile = loadfile("textures/missile.png");

    // LOAD PLANETS
    for (int i = 0; i < 13; i++)
    {
        std::stringstream filename;
        filename << "textures/planet" << std::setfill('0') << std::setw(2) << i + 1 << ".png";


        // TODO: maybe also resample planets to a smaller resolution
        tex_planets.emplace_back(loadfile(filename.str()));
    }

    tex_compass = loadfile("textures/compass.png");
    tex_compass->setSmooth(true);

    // SOUND
    soundEffect->soundBuffers[SoundEffect::ENTER] = loadSfx("audio/sounds/enter.wav");
    soundEffect->soundBuffers[SoundEffect::SELECT] = loadSfx("audio/sounds/select.wav");
    soundEffect->soundBuffers[SoundEffect::BEAMSHOT] = loadSfx("audio/sounds/beamShot.wav");
    soundEffect->soundBuffers[SoundEffect::HITBYBEAM] = loadSfx("audio/sounds/hitByBeam.wav");
    soundEffect->soundBuffers[SoundEffect::INSIDEGASCLOUD] = loadSfx("audio/sounds/insideGasCloud.wav");
    soundEffect->soundBuffers[SoundEffect::THRUST] = loadSfx("audio/sounds/move.wav");
    soundEffect->soundBuffers[SoundEffect::PROPELLER] = loadSfx("audio/sounds/spacePropeller.wav");
    soundEffect->soundBuffers[SoundEffect::ACTIVATE_SHIELD] = loadSfx("audio/sounds/activate-shield.wav");
    soundEffect->soundBuffers[SoundEffect::LEFTMISSILE] = loadSfx("audio/sounds/missile.wav");
    soundEffect->soundBuffers[SoundEffect::RIGHTMISSILE] = loadSfx("audio/sounds/missile.wav");
    soundEffect->soundBuffers[SoundEffect::HITBYMISSILE] = loadSfx("audio/sounds/hitByMissile.wav");

    for (int k = 0; k < SoundEffect::SOUND_COUNT; ++k)
    {
        soundEffect->sounds[k].setBuffer(soundEffect->soundBuffers[k]);
        soundEffect->sounds[k].setVolume(soundEffect->antiClipMaxVol);
    }

    soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setLoop(true);
    soundEffect->sounds[SoundEffect::THRUST].setLoop(true);
    soundEffect->sounds[SoundEffect::PROPELLER].setLoop(true);
    soundEffect->sounds[SoundEffect::PROPELLER].setPitch(0);

    for (int j = 0; j < 12; ++j)
    {
        std::stringstream filename;
        filename << "audio/small_impact_" << j << ".wav";

        soundEffect->smallImpactBuffers[j] = loadSfx(filename.str());
        soundEffect->smallImpacts[j].setBuffer(soundEffect->smallImpactBuffers[j]);
    }

    done = true;
}

sf::Texture* ResourceLoader::loadfile(std::string filename)
{
    sf::Texture* t = new sf::Texture();
    if (!t->loadFromFile(filename))
    {
        std::cout << "Failed to load texture" << filename << std::endl;
        LOGE << "Failed to load texture" << filename << std::endl;
    }
    loaded++;
    return t;
}

sf::Font* ResourceLoader::loadFontFromFile(std::string filename)
{
    sf::Font* f = new sf::Font();

    if (!f->loadFromFile(filename))
    {
        std::cout << "Failed to load font" << filename << std::endl;
        LOGE << "Failed to load font" << filename << std::endl;
    }

    loaded++;

    return f;
}

sf::SoundBuffer ResourceLoader::loadSfx(std::string filename)
{
    sf::SoundBuffer s;

    if (!s.loadFromFile(filename))
    {
        std::cout << "Failed to load sound effect" << filename << std::endl;
        LOGE << "Failed to load sound effect" << filename << std::endl;
    }
    loaded++;

    return s;
}


