#include "hello.h"

void hello() {
    auto window = sf::RenderWindow(sf::VideoMode(800, 600), "SFML Window");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        sf::Font font;

        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error loading font\n";
            return;
        }
        sf::Text text("hello, world!", font, 30);
        text.setFillColor(sf::Color::White);
        text.setPosition(100, 100);
        window.draw(text);
        window.display();
    }
}