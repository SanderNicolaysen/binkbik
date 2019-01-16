#ifndef BINKBIK_SOUNDEFFECT_H
#define BINKBIK_SOUNDEFFECT_H

#include <SFML/Audio.hpp>
#include <random>

class SoundEffect
{
public:
    enum SoundId { ENTER, SELECT, BEAMSHOT, HITBYBEAM, INSIDEGASCLOUD, THRUST, PROPELLER, ACTIVATE_SHIELD, LEFTMISSILE, RIGHTMISSILE, HITBYMISSILE, SOUND_COUNT };

    void openMusicFromFile(sf::Music &music, std::string filename);

    // Singleton pattern
    static SoundEffect* Instance()
    {
        if (instance == nullptr)
        {
            instance = new SoundEffect();
        }

        return instance;
    }

    sf::SoundBuffer smallImpactBuffers[12];

    sf::Sound smallImpacts[12];

    sf::SoundBuffer soundBuffers[SOUND_COUNT];

    sf::Sound sounds[SOUND_COUNT];

    sf::Music exploration, enemyNearby;

    void smallImpact(std::default_random_engine &generator);

    void decreaseThrustSound(float delta);

    void stopSfx();

    float antiClipMaxVol = 60.0f;

    float insideGasCloudMaxVolume = antiClipMaxVol, thrustMaxVolume = antiClipMaxVol;

protected:

    std::uniform_int_distribution<int> rng{0,11};

    static SoundEffect* instance;
};

#endif //BINKBIK_SOUNDEFFECT_H
