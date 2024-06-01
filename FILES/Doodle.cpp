#include "Doodle.h"
#include "PlatformTypes.h"

Doodle::Doodle() : dimension_x(50), dimension_y(100), gravity(0.5), jump(0), x(275), y(800), baseJumpStrength(-16), highestPoint(800) {
    rect.setSize(sf::Vector2f(dimension_x, dimension_y));
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(x, y);
}

void Doodle::draw(sf::RenderWindow &window) const {
    window.draw(rect);
}

void Doodle::update(std::deque<std::unique_ptr<BasePlatform>> &platforms, HighScore &highScore) {
    jump += gravity;
    y += jump;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        x += 15;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        x -= 15;
    }

    for (auto &platform : platforms) {
        platform->move();
        if (checkCollision(*platform)) {
            handlePlatformCollision(platform);
            break;
        }
    }

    if (y < highestPoint) {
        highestPoint = y;
        highScore.incrementScore(1);
    }

    rect.setPosition(x, y);
}

bool Doodle::checkCollision(const BasePlatform &platform) const {
    sf::FloatRect doodleBounds = rect.getGlobalBounds();
    sf::FloatRect platformBounds = platform.getBounds();
    return jump > 0 && doodleBounds.intersects(platformBounds);
}

double Doodle::getX() const {
    return x;
}

double Doodle::getY() const {
    return y;
}

void Doodle::handlePlatformCollision(std::unique_ptr<BasePlatform> &platform) {
    if (dynamic_cast<DestructiblePlatform*>(platform.get())) {
        platform->destroy();
    }
    double multiplier = platform->propulsiveJumpMultiplier();
    jump = baseJumpStrength * multiplier;
}

std::ostream& operator<<(std::ostream& os,const Doodle& d) {
    os << "Doodle [x: " << d.x << ", y: " << d.y << ", jump: " << d.jump << "]";
    return os;
}
