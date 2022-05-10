#include <SFML/Graphics.hpp>
#include "Entity.h"

void mainLoop() {

}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Shooty Shooty Bang Bang");
    const int nStandardWidth = 40;

    Entity entits(100.f, 100.f, 50, 50, Colour::Blue);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(entits.getEntity());
        window.display();
    }

    return 0;
}