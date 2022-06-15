#include <SFML/Graphics.hpp>
#include "classes/Field.h"
#include "classes/Ship.h"
#include "classes/GameStates.h"
#include <iostream>
#include <time.h>
#include <Windows.h>

int main() {
    srand(time(NULL));
    // game and entity values
    const int nWindowWidth = 800, nWindowHeight = 600;

    const int nStandardWidth = 30;
    const int nRows = 10, nCols = 10;
    const int nOffsetX = 50, nOffsetY = 50;
    const int nAdversaryOffsetX = nOffsetX * 3 + nStandardWidth * nCols;
    bool bEventTextUpdated = false;
    bool bShipsSelected = false;

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
    Field adversary(nRows, nCols, nAdversaryOffsetX, nOffsetY, nStandardWidth);

    std::vector<Ship> playerShips;
    std::vector<Ship> adversaryShips;

    playerShips.push_back(Ship(1, 2));
    playerShips.push_back(Ship(2, 3));
    playerShips.push_back(Ship(3, 5));
    playerShips.push_back(Ship(4, 3));
    playerShips.push_back(Ship(5, 4));

    adversaryShips.push_back(Ship(6, 2));
    adversaryShips.push_back(Ship(7, 3));
    adversaryShips.push_back(Ship(8, 5));
    adversaryShips.push_back(Ship(9, 3));
    adversaryShips.push_back(Ship(10, 4));

    // main loop
    while (window.isOpen()) {
        // event handling
        sf::Event event;
        bool bMousePressed = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // note! if you press the mouse while moving it it may not work
            // problem with SFML?
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    bMousePressed = true;
                }
            }
        }

        // game logic
        switch (gameState) {
            case GameState::PlayerShipSelection: {
                bool selectionOver = true;
                int currentShip = -1;

                // we look through our ships to see if their positions are all full
                for (int i = 0; i < playerShips.size(); i++) {
                    // if one ship still has an empty position, we select that one
                    if (playerShips[i].shipFields.size() < playerShips[i].getSize()) {
                        selectionOver = false;
                        currentShip = i;

                        break;
                    }
                }

                // this needs a lot of checks
                // this is where the ship's positions are set
                if (bMousePressed) {
                    int col = ((int)event.mouseButton.x - nOffsetX) / nStandardWidth;
                    int row = ((int)event.mouseButton.y - nOffsetY) / nStandardWidth;
 
                    std::cout << "PLAYER SELECTED: " << currentShip << " " << row << ": " << (int)event.mouseButton.y << " - " << nOffsetY << ", " << col << std::endl;
                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        playerShips[currentShip].shipFields.push_back(&player.levelEntities[row * nRows + col]);
                        playerShips[currentShip].shipFields.back()->setColour(Colour::Grey);
                    }
                }

                if (selectionOver) {
                    gameState = GameState::AdversaryShipSelection;
                }
            }
                break;
            case GameState::AdversaryShipSelection: {
                for (int i = 0; i < adversaryShips.size(); i ++) {
                    for (int j = 0; j < adversaryShips[i].getSize(); j ++) {
                        adversaryShips[i].shipFields.push_back(&adversary.levelEntities[i * nRows + j]);
                        adversaryShips[i].shipFields.back()->setColour(Colour::Green);
                    }
                }

                bShipsSelected = true;
                gameState = GameState::PlayerTurn;
                Sleep(1000);
            }
                break;
            case GameState::PlayerTurn:
                if (bMousePressed) {
                    int col = ((int)event.mouseButton.x - nAdversaryOffsetX) / nStandardWidth;
                    int row = ((int)event.mouseButton.y - nOffsetY) / nStandardWidth;

                    std::cout << "PLAYER SHOT: " << row << ": " << (int)event.mouseButton.y << " - " << nOffsetY << ", " << col << std::endl;
                    
                    // here we check if our shot hit an enemy ship or not
                    // could be a bit fine-tuned
                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        for (int i = 0; i < adversaryShips.size(); i ++) {
                            for (int j = 0; j < adversaryShips[i].getSize(); j ++) {
                                int shipPartRow = adversaryShips[i].shipFields[j]->getRow();
                                int shipPartCol = adversaryShips[i].shipFields[j]->getCol();

                                if (row == shipPartRow && col == shipPartCol) {
                                    adversaryShips[i].shipFields[j]->setIsHit(true);
                                    adversaryShips[i].shipFields[j]->setColour(Colour::Red);
                                }
                            }
                        }
                    }

                    gameState = GameState::AdversaryTurn;
                }

                break;
            case GameState::AdversaryTurn: {
                int row = rand() % nRows;
                int col = rand() % nCols;

                for (int i = 0; i < playerShips.size(); i++) {
                    for (int j = 0; j < playerShips[i].getSize(); j++) {
                        int shipPartRow = playerShips[i].shipFields[j]->getRow();
                        int shipPartCol = playerShips[i].shipFields[j]->getCol();

                        if (row == shipPartRow && col == shipPartCol) {
                            playerShips[i].shipFields[j]->setIsHit(true);
                            playerShips[i].shipFields[j]->setColour(Colour::Red);
                        }
                    }
                }
                Sleep(1000);

                gameState = GameState::PlayerTurn;
            }

                break;
            case GameState::GameOver:
                break;
        }

        // checking which ships are completely destroyed
        if (bShipsSelected) {
            // checking player ships
            for (int i = 0; i < playerShips.size(); i++) {
                bool bDestroyed = true;
                // checking if every part of a ship is hit, if so, it's considered DESTROYED
                for (int j = 0; j < playerShips[i].getSize(); j++) {
                    if (!playerShips[i].shipFields[j]->getIsHit()) {
                        bDestroyed = false;
                        break;
                    }
                }

                if (bDestroyed) {
                    playerShips.erase(playerShips.begin() + i);
                }
            }

            // checking enemy ships
            for (int i = 0; i < adversaryShips.size(); i++) {
                bool bDestroyed = true;
                // checking if every part of a ship is hit, if so, it's considered DESTROYED
                for (int j = 0; j < adversaryShips[i].getSize(); j++) {
                    if (!adversaryShips[i].shipFields[j]->getIsHit()) {
                        bDestroyed = false;
                        break;
                    }
                }

                if (bDestroyed) {
                    adversaryShips.erase(adversaryShips.begin() + i);
                }
            }
        }

        // checking if either the player or enemy has any ships left
        // if one doesn't, the other one wins

        if (playerShips.size() == 0 || adversaryShips.size() == 0) {
            gameState = GameState::GameOver;
        }
        
        // changing the text to fit the game state
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