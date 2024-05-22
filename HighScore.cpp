#include "HighScore.h"

HighScore::HighScore() : currentScore(0), highScore(0) {
    loadHighScore();

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font\n";
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::White);
}

void HighScore::incrementScore(int points) {
    if (points > currentScore) {
        currentScore = points;
    }
    if (currentScore > highScore) {
        highScore = currentScore;
    }
}

void HighScore::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void HighScore::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void HighScore::draw(sf::RenderWindow& window) {
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

int HighScore::getCurrentScore() const {
    return currentScore;
}
