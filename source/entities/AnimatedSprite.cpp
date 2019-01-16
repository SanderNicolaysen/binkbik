#include <cmath>
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(Game &game, sf::Texture* texture_sheet, const sf::IntRect &startRect, unsigned int frames)
        : Entity(game),
          startRect(startRect),
          frameCount(frames)
{
    texture = texture_sheet;
    sprite.setTexture(*texture);

    textureRect = startRect;
    sprite.setTextureRect(textureRect);
}

void AnimatedSprite::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void AnimatedSprite::update(float delta)
{
    if (stopped) return;

    counter += delta; // Count milliseconds

    if (counter >= interval)
    {
        frame = frame + 1 >= frameCount ? 0 : frame + 1;

        textureRect.left = startRect.width * frame;

        sprite.setTextureRect(textureRect);

        counter = 0;
    }
}

void AnimatedSprite::start()
{
    stopped = false;
}

void AnimatedSprite::stop()
{
    stopped = true;
}

void AnimatedSprite::setFrameDuration(unsigned int milliseconds)
{
    interval = milliseconds / 1000.0f;
}

void AnimatedSprite::setStartRect(sf::IntRect startRect)
{
    this->startRect = startRect;
    textureRect = startRect;
}

void AnimatedSprite::reset()
{
    textureRect = startRect;
    sprite.setTextureRect(startRect);
}

void AnimatedSprite::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

void AnimatedSprite::setRotation(float angle)
{
    sprite.setRotation(angle);
}

void AnimatedSprite::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void AnimatedSprite::setOrigin(sf::Vector2f origin)
{
    sprite.setOrigin(origin);
}

bool AnimatedSprite::isStopped() const {
    return stopped;
}

const sf::IntRect &AnimatedSprite::getStartRect() const {
    return startRect;
}

void AnimatedSprite::setFrame(unsigned int frame) {
    AnimatedSprite::frame = frame;
}

unsigned int AnimatedSprite::getFrame() const {
    return frame;
}