#ifndef BINKBIK_ANIMATEDSPRITE_H
#define BINKBIK_ANIMATEDSPRITE_H


#include <SFML/Graphics.hpp>
#include "Entity.h"

class AnimatedSprite : protected Entity
{
public:
    AnimatedSprite(Game &game, sf::Texture *texture_sheet, const sf::IntRect &startRect, unsigned int frames);

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    void start();

    void stop();

    void setFrameDuration(unsigned int milliseconds);

    void setStartRect(sf::IntRect startRect);

    void reset();

    sf::Sprite const getSprite() { return sprite; }

    void setScale(sf::Vector2f scale);

    void setRotation(float angle);

    void setPosition(sf::Vector2f position);

    void setOrigin(sf::Vector2f origin);

protected:
    sf::IntRect startRect;
public:
    const sf::IntRect &getStartRect() const;

protected:

    sf::IntRect textureRect;

    float counter = 0;

    float interval = 0.25f;

    unsigned int frame = 0;
public:
    unsigned int getFrame() const;

public:
    void setFrame(unsigned int frame);

protected:

    unsigned int frameCount = 0;

    bool stopped = true;
public:
    bool isStopped() const;
};


#endif //BINKBIK_ANIMATEDSPRITE_H
