#include "Game.h"
#include "PlatformException.h"
#include "PlatformTypes.h"
#include "BasePlatform.h"
#include <random>

Game::Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize()), verticalSpacing(200), state(Playing) {
    generateInitialPlatforms();

    if (!font.loadFromFile("Arial.ttf")) {
        throw FontLoadException();
    }

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("You Lost!\nPress R to Restart\nPress Q to Quit");
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f);
}

void Game::generateInitialPlatforms() {
    platforms.emplace_back(std::make_unique<InfinitePlatform>(250, 950));
}

std::unique_ptr<BasePlatform> Game::generateRandomPlatform(double y) {
    double x = randomX();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> type_dist(0, 4); 
    switch (type_dist(gen)) {
        case 0:
            return std::make_unique<InfinitePlatform>(x, y);
        case 1:
            return std::make_unique<DestructiblePlatform>(x, y);
        case 2:
            return std::make_unique<MovingPlatform>(x, y);
        case 3:
            return std::make_unique<PropulsivePlatform>(x, y);
        case 4:
            return std::make_unique<InvisiblePlatform>(x, y);
        default:
            throw InvalidPlatformTypeException();
    }
}

double Game::randomX() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, window.getSize().x - 100);
    return dis(gen);
}

void Game::drawPlatforms() const {
    for (const auto &platform : platforms) {
        platform->draw(window);
    }
}

void Game::maintainPlatformCount() {
    while (platforms.back()->getY() > doodle.getY() - 600) {
        platforms.push_back(generateRandomPlatform(platforms.back()->getY() - verticalSpacing));
    }
    while (!platforms.empty() && platforms.front()->getY() > doodle.getY() + 600) {
        platforms.pop_front();
    }
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameOver) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        restartGame();
                    } else if (event.key.code == sf::Keyboard::Q) {
                        window.close();
                    }
                }
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                window.close();
            }
        }

        if (state == Playing) {
            doodle.update(platforms, highScore);
            camera.update(sf::Vector2f(doodle.getX(), doodle.getY()));

            platforms.erase(
                std::remove_if(platforms.begin(), platforms.end(), [](const std::unique_ptr<BasePlatform> &platform) {
                    return platform->getIsDestroyed();
                }),
                platforms.end());

            maintainPlatformCount();

            if (doodle.getY() > camera.getCurrentY() + 600) {
                highScore.saveHighScore();
                state = GameOver;
            }
        }

        window.clear();
        window.setView(camera.getView());
        drawPlatforms();
        doodle.draw(window);
        highScore.draw(window);

        if (state == GameOver) {
            window.setView(window.getDefaultView());
            window.draw(gameOverText);
        }

        window.display();
    }
}

Game::Game(const Game& other) : doodle(other.doodle), window(other.window), camera(other.camera), platforms(), highScore(other.highScore),
                          verticalSpacing(other.verticalSpacing), state(other.state) {
    for (const auto& platform : other.platforms) {
        platforms.push_back(platform->clone());
    }
}

Game& Game::operator=(Game other) {
    swap(*this, other);
    return *this;
}

void swap(Game& first, Game& second) {
    using std::swap;
    swap(first.doodle, second.doodle);
    swap(first.camera, second.camera);
    swap(first.platforms, second.platforms);
    swap(first.highScore, second.highScore);
    swap(first.verticalSpacing, second.verticalSpacing);
    swap(first.state, second.state);
    swap(first.gameOverText, second.gameOverText);
    swap(first.font, second.font);
}

void Game::restartGame() {
    doodle = Doodle();
    camera = Camera(window.getSize());
    platforms.clear();
    generateInitialPlatforms();
    highScore.resetCurrentScore();
    state = Playing;
}

Game::~Game() {
    std::cout << "Game over\n";
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Total number of Platforms generated: " << BasePlatform::getTotalPlatforms() << "\n";
    os << "Current Score: " << game.highScore.getCurrentScore() << "\n";
    return os;
}
