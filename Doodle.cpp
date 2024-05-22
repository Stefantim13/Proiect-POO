#include "Doodle.h"

Doodle::Doodle() : dimension_x(50), dimension_y(100), gravity(0.5), jump(0), x(275), y(800), baseJumpStrength(-16) {
    rect.setSize(sf::Vector2f(dimension_x, dimension_y));
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(x, y);
}

void Doodle::draw(sf::RenderWindow &window) {
    window.draw(rect);
}

void Doodle::update(std::vector<Platform> &platforms) {
    jump += gravity;
    y += jump;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        x += 15;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        x -= 15;
    }

    for (auto &platform : platforms) {
        platform.move();
        if (checkCollision(platform) && !platform.isDestroyed_()) {
            if (platform.isDestructible()) {
                platform.destroy();
            }
            double multiplier = platform.propulsiveJumpMultiplier();
            jump = baseJumpStrength * multiplier;
            break;
        }
    }

    rect.setPosition(x, y);
}

bool Doodle::checkCollision(const Platform &platform) {
    sf::FloatRect doodleBounds = rect.getGlobalBounds();
    sf::FloatRect platformBounds = platform.getBounds();
    return jump > 0 && doodleBounds.intersects(platformBounds);
}

double Doodle::getX() {
    return x;
}

double Doodle::getY() {
    return y;
}

std::ostream& operator<<(std::ostream& os, const Doodle& d) {
    os << "Doodle [x: " << d.x << ", y: " << d.y << ", jump: " << d.jump << "]";
    return os;
}
