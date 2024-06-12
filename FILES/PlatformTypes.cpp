#include "PlatformTypes.h"
#include "BasePlatform.h"
InfinitePlatform::InfinitePlatform(double x, double y) : BasePlatform(x, y) {
    totalPlatforms++;
}

void InfinitePlatform::move() {}

void InfinitePlatform::destroy() {}

double InfinitePlatform::propulsiveJumpMultiplier() const {
    return 1.0;
}

std::unique_ptr<BasePlatform> InfinitePlatform::clone() const {
    return std::make_unique<InfinitePlatform>(*this);
}

DestructiblePlatform::DestructiblePlatform(double x, double y) : BasePlatform(x, y) {
    plat.setFillColor(sf::Color::White);
    totalPlatforms++;
}

void DestructiblePlatform::move() {}

void DestructiblePlatform::destroy() {
    isDestroyed = true;
    plat.setSize(sf::Vector2f(0, 0));
}

double DestructiblePlatform::propulsiveJumpMultiplier() const {
    return 1.0;
}

std::unique_ptr<BasePlatform> DestructiblePlatform::clone() const {
    return std::make_unique<DestructiblePlatform>(*this);
}

MovingPlatform::MovingPlatform(double x, double y) : BasePlatform(x, y) {
    velocity = 5;
    plat.setFillColor(sf::Color::Yellow);
    totalPlatforms++;
}

void MovingPlatform::move() {
    x += velocity;
    if (x < 0 || x + length > 800) {
        velocity = -velocity;
    }
    plat.setPosition(x, y);
}

void MovingPlatform::destroy() {}

double MovingPlatform::propulsiveJumpMultiplier() const {
    return 1.0;
}

std::unique_ptr<BasePlatform> MovingPlatform::clone() const {
    return std::make_unique<MovingPlatform>(*this);
}

PropulsivePlatform::PropulsivePlatform(double x, double y) : BasePlatform(x, y) {
    plat.setFillColor(sf::Color::Blue);
    totalPlatforms++;
}

void PropulsivePlatform::move() {}

void PropulsivePlatform::destroy() {}

double PropulsivePlatform::propulsiveJumpMultiplier() const {
    return 2.0;
}

std::unique_ptr<BasePlatform> PropulsivePlatform::clone() const {
    return std::make_unique<PropulsivePlatform>(*this);
}

InvisiblePlatform::InvisiblePlatform(double x, double y) : BasePlatform(x, y) {
    plat.setFillColor(sf::Color::Transparent); 
}

void InvisiblePlatform::move() {
    if (clock.getElapsedTime().asSeconds() >= 0.5f) { 
        isVisible = !isVisible; 
        plat.setFillColor(isVisible ? sf::Color::Cyan : sf::Color::Transparent); 
        clock.restart(); 
    }
}

void InvisiblePlatform::destroy() {}

double InvisiblePlatform::propulsiveJumpMultiplier() const {
    return 1.0;
}

std::unique_ptr<BasePlatform> InvisiblePlatform::clone() const {
    return std::make_unique<InvisiblePlatform>(*this); 
}