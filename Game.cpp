#include "Game.h"

Game::Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize()), state(Playing), verticalSpacing(200), platformCount(1000), type(0) {
    generateInitialPlatforms();

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font\n";
    }

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("You Lost!\nPress R to Restart");
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f);
}

void Game::generateInitialPlatforms() {
    platforms.emplace_back(250, 950, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> type_dist(0, 3);

    for (int i = 1; i < platformCount; ++i) {
        double Platform_x = randomX();
        double Platform_y = platforms.back().getY() - verticalSpacing;

        switch (type_dist(gen)) {
            case 0: type = 0; break;
            case 1: type = 1; break;
            case 2: type = 2; break;
            case 3: type = 3; break;
        }

        platforms.emplace_back(Platform_x, Platform_y, type);
    }
}

double Game::randomX() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, window.getSize().x - 100);
    return dis(gen);
}

void Game::drawPlatforms() {
    for (auto &platform : platforms) {
        platform.draw(window);
    }
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                restartGame();
            }
        }

        if (state == Playing) {
            doodle.update(platforms);
            camera.update(sf::Vector2f(doodle.getX(), doodle.getY()));

            if (doodle.getY() > camera.getCurrentY() + 600) {
                highScore.saveHighScore();
                state = GameOver;
            }
        }

        window.clear();
        window.setView(camera.getView());
        drawPlatforms();
        doodle.draw(window);
        highScore.incrementScore(800 - doodle.getY());
        highScore.draw(window);

        if (state == GameOver) {
            window.setView(window.getDefaultView());
            window.draw(gameOverText);
        }

        window.display();
    }
}

Game::Game(const Game& other) : doodle(other.doodle), window(other.window), camera(other.camera), platforms(other.platforms),
verticalSpacing(other.verticalSpacing), platformCount(other.platformCount), type(other.type) {}

Game& Game::operator=(const Game& other) {
    if (this != &other) {
        doodle = other.doodle;
        camera = other.camera;
        platforms = other.platforms;
        verticalSpacing = other.verticalSpacing;
        platformCount = other.platformCount;
        type = other.type;
    }
    return *this;
}

void Game::restartGame() {
    doodle = Doodle();
    camera = Camera(window.getSize());
    platforms.clear();
    generateInitialPlatforms();
    highScore = HighScore();
    state = Playing;
}

Game::~Game() {
    std::cout << "Game over.\n";
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game [Number of Platforms: " << game.platforms.size() << ", Current Type: " << game.type << "]";
    return os;
}
