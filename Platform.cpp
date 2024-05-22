#include "Platform.h"

Platform::Platform(double x_, double y_, int type_) : length(100), width(20), x(x_), y(y_), type(type_), isDestroyed(false), velocity(0) {
    plat.setSize(sf::Vector2f(length, width));
    plat.setPosition(x, y);
    plat.setFillColor(sf::Color::Green);

    if (type == 2) {
        velocity = 5;
        plat.setFillColor(sf::Color::Yellow);
    }
    if (type == 3) {
        plat.setFillColor(sf::Color::Blue);
    }
    if (type == 1) {
        plat.setFillColor(sf::Color::White);
    }
}

void Platform::move() {
    if (type == 2) {
        x += velocity;
        if (x < 0 || x + length > 800) {
            velocity = -velocity;
        }
        plat.setPosition(x, y);
    }
}

void Platform::destroy() {
    if (type == 1) {
        isDestroyed = true;
    }
}

bool Platform::isDestroyed_() {
    return isDestroyed;
}

void Platform::draw(sf::RenderWindow &window) {
    if (!isDestroyed) {
        window.draw(plat);
    }
}

bool Platform::isDestructible() const {
    return type == 1;
}

double Platform::propulsiveJumpMultiplier() const {
    return type == 3 ? 2.0 : 1.0;
}

double Platform::getX() {
    return x;
}

double Platform::getY() {
    return y;
}

sf::FloatRect Platform::getBounds() const {
    return plat.getGlobalBounds();
}

std::ostream& operator<<(std::ostream& os, const Platform& p) {
    os << "Platform [x: " << p.x << ", y: " << p.y << ", type: " << p.type << ", isDestroyed: " << p.isDestroyed << "]";
    return os;
}
