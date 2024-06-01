#include <SFML/Graphics.hpp>
#include "Game.h"

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
