#include <SFML/Graphics.hpp>
#include "classes/Field.h"
#include "classes/Ship.h"

enum GameStates { PlayerShipSelection, AdversaryShipSelection, PlayerTurn, AdversaryTurn, GameOver };

int main() {
    // game and entity values
    const int nWindowWidth = 800, nWindowHeight = 600;

    const int nStandardWidth = 30;
    const int nRows = 10, nCols = 10;
    const int nOffsetX = 50, nOffsetY = 50;
    
    // window and entities
    sf::RenderWindow window(sf::VideoMode(nWindowWidth, nWindowHeight), "BattleShip!");
    
    Field player(nRows, nCols, nOffsetX, nOffsetY, nStandardWidth);
    Field adversary(nRows, nCols, nOffsetX * 3 + nStandardWidth * nCols, nOffsetY, nStandardWidth);
    


    window.setFramerateLimit(60);

    // main loop
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // drawing
        window.clear();

        for (int i = 0; i < player.levelEntities.size(); i ++) {
            window.draw(player.levelEntities[i].getEntity());
        }

        for (int i = 0; i < adversary.levelEntities.size(); i ++) {
            window.draw(adversary.levelEntities[i].getEntity());
        }

        window.display();
    }

    return 0;
}