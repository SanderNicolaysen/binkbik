#include <iostream>
#include <plog/include/plog/Log.h>
#include "SoundEffect.h"


void SoundEffect::openMusicFromFile(sf::Music &music, std::string filename)
{
    if (!music.openFromFile(filename))
    {
        std::cout << "Failed to open music from file" << std::endl;
        LOGE << "Failed to open music from file";
    }
}

void SoundEffect::smallImpact(std::default_random_engine &generator)
{
    int rindex = rng(generator);
    smallImpacts[rindex].play();
}

SoundEffect* SoundEffect::instance = nullptr;

void SoundEffect::decreaseThrustSound(float delta)
{
    if (sounds[SoundEffect::THRUST].getVolume() > 0)
        sounds[SoundEffect::THRUST].setVolume(std::max(sounds[SoundEffect::THRUST].getVolume() - delta * 100.0f, 0.0f));

    if (sounds[SoundEffect::THRUST].getVolume() < 1.6f)
        sounds[SoundEffect::THRUST].stop();
}

void SoundEffect::stopSfx()
{
    if (sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Playing)
        sounds[SoundEffect::THRUST].setVolume(0);

    if (sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Playing)
        sounds[SoundEffect::INSIDEGASCLOUD].setVolume(0);

    if (enemyNearby.getStatus() == enemyNearby.Playing)
    {
        enemyNearby.pause();

        exploration.play();
    }

    sounds[SoundEffect::PROPELLER].setPitch(0);
}