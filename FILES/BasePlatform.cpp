#include "BasePlatform.h"


int BasePlatform::totalPlatforms = 0;

BasePlatform::BasePlatform(double x_, double y_) : length(100), width(20), x(x_), y(y_), isDestroyed(false), velocity(0) {
    plat.setSize(sf::Vector2f(length, width));
    plat.setPosition(x, y);
    plat.setFillColor(sf::Color::Green);
}

void BasePlatform::draw(sf::RenderWindow &window) const {
    if (!isDestroyed) {
        window.draw(plat);
    }
}

bool BasePlatform::getIsDestroyed() const {
    return isDestroyed;
}

double BasePlatform::getX() const {
    return x;
}

double BasePlatform::getY() const {
    return y;
}

sf::FloatRect BasePlatform::getBounds() const {
    return plat.getGlobalBounds();
}

int BasePlatform::getTotalPlatforms() {
    return totalPlatforms;
}

std::ostream& operator<<(std::ostream& os ,const BasePlatform& p) {
    os << "Platform [x: " << p.x << ", y: " << p.y << "]";
    return os;
}
