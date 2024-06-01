#include <iostream>
#include <fstream>
#include <deque>
#include <random>
#include <SFML/Graphics.hpp>
#include <compare>
#include <chrono>
#include <memory>
#include <utility>
#include <exception>
#include <algorithm>

class PlatformException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Platform exception occurred.";
    }
};

class InvalidPlatformTypeException : public PlatformException {
public:
    const char* what() const noexcept override {
        return "Invalid platform type.";
    }
};

class FontLoadException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error loading font";
    }
};

class HighScore {
    int currentScore = 0;
    int highScore = 0;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;

public:
    HighScore() {
        loadHighScore();

        if (!font.loadFromFile("Arial.ttf")) { 
            throw FontLoadException();
        }
        
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);

        highScoreText.setFont(font);
        highScoreText.setCharacterSize(24);
        highScoreText.setFillColor(sf::Color::White);
    }

    void incrementScore(int points) {
        currentScore += points;
        if (currentScore > highScore) {
            highScore = currentScore;
        }
    }

    void resetCurrentScore() {
        currentScore = 0;
    }

    void saveHighScore() {
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    void loadHighScore() {
        std::ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::View view = window.getView();
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f viewSize = view.getSize();
        
        scoreText.setString("Score: " + std::to_string(currentScore));
        scoreText.setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2 + 10);

        highScoreText.setString("High Score: " + std::to_string(highScore));
        highScoreText.setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2 + 40);

        window.draw(scoreText);
        window.draw(highScoreText);
    }

    int getCurrentScore() const {
        return currentScore;
    }
};

class Camera {
    sf::View view;
    double currentY = 0;

public:
    explicit Camera(const sf::Vector2u &windowSize) {
        view.setSize(static_cast<double>(windowSize.x), static_cast<double>(windowSize.y));
        view.setCenter(view.getSize() / 2.0f);
    }

    void update(const sf::Vector2f &playerPosition) {
        currentY = std::min(playerPosition.y, view.getCenter().y);
        view.setCenter(view.getCenter().x, currentY);
    }

    const sf::View &getView() const {
        return view;
    }

    double getCurrentY() const {
        return currentY;
    }

    friend std::ostream& operator<<(std::ostream& os, const Camera& cam) {
        os << "Camera [Center: (" << cam.view.getCenter().x << ", " << cam.view.getCenter().y << ")]";
        return os;
    }
};

class BasePlatform {
protected:
    double length = 100, width = 20;
    double x, y;
    sf::RectangleShape plat;
    bool isDestroyed = false;
    double velocity = 0;

public:
    BasePlatform(double x_, double y_) : x(x_), y(y_) {
        plat.setSize(sf::Vector2f(length, width));
        plat.setPosition(x, y);
        plat.setFillColor(sf::Color::Green);
    }

    virtual ~BasePlatform() = default;

    virtual void move() = 0;
    virtual void destroy() = 0;
    virtual double propulsiveJumpMultiplier() const = 0;
    virtual std::unique_ptr<BasePlatform> clone() const = 0;
    
    void draw(sf::RenderWindow &window) const {
        if (!isDestroyed) {
            window.draw(plat);
        }
    }

    bool getIsDestroyed() const { return isDestroyed; }
    double getX() const { return x; }
    double getY() const { return y; }
    sf::FloatRect getBounds() const { return plat.getGlobalBounds(); }

    friend std::ostream& operator<<(std::ostream& os ,const BasePlatform& p) {
        os << "Platform [x: " << p.x << ", y: " << p.y << "]";
        return os;
    }

    static int totalPlatforms;
    static int getTotalPlatforms() {
        return totalPlatforms;
    }
};

int BasePlatform::totalPlatforms = 0;

class InfinitePlatform : public BasePlatform {
public:
    InfinitePlatform(double x, double y) : BasePlatform(x, y) {
        totalPlatforms++;
    }

    void move() override {}
    void destroy() override {}
    double propulsiveJumpMultiplier() const override { return 1.0; }


    std::unique_ptr<BasePlatform> clone() const override {
        return std::make_unique<InfinitePlatform>(*this);
    }
};

class DestructiblePlatform : public BasePlatform {
public:
    DestructiblePlatform(double x, double y) : BasePlatform(x, y) {
        plat.setFillColor(sf::Color::White);
        totalPlatforms++;
    }

    void move() override {}
    void destroy() override { isDestroyed = true; plat.setSize(sf::Vector2f(0, 0)); }
    double propulsiveJumpMultiplier() const override { return 1.0; }


    std::unique_ptr<BasePlatform> clone() const override {
        return std::make_unique<DestructiblePlatform>(*this);
    }
};

class MovingPlatform : public BasePlatform {
public:
    MovingPlatform(double x, double y) : BasePlatform(x, y) {
        velocity = 5;
        plat.setFillColor(sf::Color::Yellow);
        totalPlatforms++;
    }

    void move() override {
        x += velocity;
        if (x < 0 || x + length > 800) {
            velocity = -velocity;
        }
        plat.setPosition(x, y);
    }

    void destroy() override {}
    double propulsiveJumpMultiplier() const override { return 1.0; }

    std::unique_ptr<BasePlatform> clone() const override {
        return std::make_unique<MovingPlatform>(*this);
    }
};

class PropulsivePlatform : public BasePlatform {
public:
    PropulsivePlatform(double x, double y) : BasePlatform(x, y) {
        plat.setFillColor(sf::Color::Blue);
        totalPlatforms++;
    }

    void move() override {}
    void destroy() override {}
    double propulsiveJumpMultiplier() const override { return 2.0; }
    std::unique_ptr<BasePlatform> clone() const override {
        return std::make_unique<PropulsivePlatform>(*this);
    }
};

class Doodle {
    double dimension_x = 50, dimension_y = 100;
    double gravity = 0.5;
    double jump = 0;
    double x = 275, y = 800;
    double baseJumpStrength = -16;
    sf::RectangleShape rect;
    double highestPoint = 800;

public:
    Doodle() {
        rect.setSize(sf::Vector2f(dimension_x, dimension_y));
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(x, y);
    }

    void draw(sf::RenderWindow &window) const {
        window.draw(rect);
    }

    void update(std::deque<std::unique_ptr<BasePlatform>> &platforms, HighScore &highScore) {
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

    bool checkCollision(const BasePlatform &platform) const {
        sf::FloatRect doodleBounds = rect.getGlobalBounds();
        sf::FloatRect platformBounds = platform.getBounds();
        return jump > 0 && doodleBounds.intersects(platformBounds);
    }

    double getX() const { return x; }
    double getY() const { return y; }

    void handlePlatformCollision(std::unique_ptr<BasePlatform> &platform) {
        if (dynamic_cast<DestructiblePlatform*>(platform.get())) {
            platform->destroy();
        }
        double multiplier = platform->propulsiveJumpMultiplier();
        jump = baseJumpStrength * multiplier;
    }

    friend std::ostream& operator<<(std::ostream& os,const Doodle& d) {
        os << "Doodle [x: " << d.x << ", y: " << d.y << ", jump: " << d.jump << "]";
        return os;
    }
};

class Game {
    Doodle doodle;
    sf::RenderWindow &window;
    Camera camera;
    std::deque<std::unique_ptr<BasePlatform>> platforms;
    HighScore highScore;
    int verticalSpacing = 200;

    enum GameState { Playing, GameOver };
    GameState state;
    sf::Text gameOverText;
    sf::Font font;

public:
    explicit Game(sf::RenderWindow &window_) : window(window_), camera(window_.getSize()), state(Playing) {
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

    void generateInitialPlatforms() {
        platforms.emplace_back(std::make_unique<InfinitePlatform>(250, 950));
    }

    std::unique_ptr<BasePlatform> generateRandomPlatform(double y) {
        double x = randomX();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> type_dist(0, 3); 
        switch (type_dist(gen)) {
            case 0:
                return std::make_unique<InfinitePlatform>(x, y);
            case 1:
                return std::make_unique<DestructiblePlatform>(x, y);
            case 2:
                return std::make_unique<MovingPlatform>(x, y);
            case 3:
                return std::make_unique<PropulsivePlatform>(x, y);
            default:
                throw InvalidPlatformTypeException();
        }
    }

    double randomX() const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, window.getSize().x - 100);
        return dis(gen);
    }

    void drawPlatforms() const {
        for (const auto &platform : platforms) {
            platform->draw(window);
        }
    }

    void maintainPlatformCount() {
        while (platforms.back()->getY() > doodle.getY() - 600) {
            platforms.push_back(generateRandomPlatform(platforms.back()->getY() - verticalSpacing));
        }
        while (!platforms.empty() && platforms.front()->getY() > doodle.getY() + 600) {
            platforms.pop_front();
        }
    }

    void run() {
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

    Game(const Game& other) : doodle(other.doodle), window(other.window), camera(other.camera), platforms(), highScore(other.highScore),
                              verticalSpacing(other.verticalSpacing), state(other.state) {
        for (const auto& platform : other.platforms) {
            platforms.push_back(platform->clone());
        }
    }

    Game& operator=(Game other) {
        swap(*this, other);
        return *this;
    }

    friend void swap(Game& first, Game& second) {
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
    

    void restartGame() {
        doodle = Doodle();
        camera = Camera(window.getSize());
        platforms.clear();
        generateInitialPlatforms();
        highScore.resetCurrentScore();
        state = Playing;
    }

    ~Game() { std::cout << "Game over\n"; }

    friend std::ostream& operator<<(std::ostream& os, const Game& game) {
        os << "Total number of Platforms generated: " << BasePlatform::getTotalPlatforms() << "\n";
        os << "Current Score: " << game.highScore.getCurrentScore() << "\n";
        return os;
    }
};

int main() {
    try {
        sf::RenderWindow window;
        window.create(sf::VideoMode({800, 1200}), "Doodle Jump", sf::Style::Default);
        window.setFramerateLimit(60);
        Game game(window);
        game.run();
        std::cout << game;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    
    return 0;
}
