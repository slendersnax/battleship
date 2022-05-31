#include <SFML/Graphics.hpp>
#include "classes/Field.h"
#include "classes/Ship.h"
#include "classes/GameStates.h"
#include <iostream>

int main() {
    // game and entity values
    const int nWindowWidth = 800, nWindowHeight = 600;

    const int nStandardWidth = 30;
    const int nRows = 10, nCols = 10;
    const int nOffsetX = 50, nOffsetY = 50;
    bool bEventTextUpdated = false;

    sf::Font timesNewRoman;
    sf::Text gameStateText;
    std::vector<sf::Text> currentEvents;    // to show what has recently happened
    GameState gameState;
    sf::RenderWindow window(sf::VideoMode(nWindowWidth, nWindowHeight), "BattleShip!");

    if (!timesNewRoman.loadFromFile("times-new-roman.ttf")) {
        std::cout << "font loading error" << std::endl;
    }

    gameStateText.setFont(timesNewRoman);
    gameStateText.setFillColor(getSzin(Colour::White));
    gameStateText.setCharacterSize(nStandardWidth * 0.8f);
    gameStateText.setPosition(nOffsetX, nOffsetY / 4);

    gameState = GameState::PlayerShipSelection;
    
    window.setFramerateLimit(120);
    
    // entities
    Field player(nRows, nCols, nOffsetX, nOffsetY, nStandardWidth);
    Field adversary(nRows, nCols, nOffsetX * 3 + nStandardWidth * nCols, nOffsetY, nStandardWidth);

    std::vector<Ship> playerShips;
    std::vector<Ship> enemyShips;

    playerShips.push_back(Ship(1, 2));
    playerShips.push_back(Ship(2, 3));
    playerShips.push_back(Ship(3, 5));
    playerShips.push_back(Ship(4, 3));
    playerShips.push_back(Ship(5, 4));

    enemyShips.push_back(Ship(6, 2));
    enemyShips.push_back(Ship(7, 3));
    enemyShips.push_back(Ship(8, 5));
    enemyShips.push_back(Ship(9, 3));
    enemyShips.push_back(Ship(10, 4));

    // main loop
    while (window.isOpen()) {
        // event handling
        sf::Event event;
        bool bMousePressed = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    bMousePressed = true;
                    //std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    //std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                }
            }
        }

        // game logic
        switch (gameState) {
            case GameState::PlayerShipSelection: {
                bool selectionOver = true;
                int currentShip = -1;

                for (int i = 0; i < playerShips.size(); i++) {
                    if (playerShips[i].shipFields.size() < playerShips[i].getSize()) {
                        selectionOver = false;
                        currentShip = i;

                        break;
                    }

                    for (int j = 0; j < playerShips[i].shipFields.size(); j++) {
                        playerShips[i].shipFields[j]->setColour(Colour::Grey);
                    }
                }

                if (bMousePressed) {
                    int col = (event.mouseButton.x - nOffsetX) / nStandardWidth;
                    int row = (event.mouseButton.y - nOffsetY) / nStandardWidth;
                    std::cout << currentShip << std::endl;
                    playerShips[currentShip].shipFields.push_back(&player.levelEntities[row * nRows + col]);
                }

                if (selectionOver) {
                    gameState = GameState::AdversaryShipSelection;
                }
            }
                break;
            case GameState::AdversaryShipSelection: {
                std::cout << "adversary selection" << std::endl;
            }
                break;
            case GameState::PlayerTurn:
                break;
            case GameState::AdversaryTurn:
                break;
        }

        gameStateText.setString(getGameState(gameState));

        // drawing
        window.clear();

        for (int i = 0; i < player.levelEntities.size(); i ++) {
            window.draw(player.levelEntities[i].getEntity());
        }

        for (int i = 0; i < adversary.levelEntities.size(); i ++) {
            window.draw(adversary.levelEntities[i].getEntity());
        }

        window.draw(gameStateText);

        window.display();
    }

    return 0;
}