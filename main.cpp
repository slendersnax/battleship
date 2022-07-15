#include <SFML/Graphics.hpp>
#include "classes/Field.h"
#include "classes/Ship.h"
#include "classes/GameStates.h"
#include <iostream>
#include <time.h>
#include <Windows.h>

bool checkAreaHasShip(Field adversary, int size,  int row, int col, int direction, int nRows, int nCols) {
    // check horizontally
    if (direction) {
        for (int i = col; i < size; i ++) {
            if (adversary.levelEntities[row * nRows + i].getIsShip()) {
                return true;
            }
        }
    }
    // check vertically
    else {
        for (int i = row; i < size; i++) {
            if (adversary.levelEntities[i * nRows + col].getIsShip()) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    srand(time(NULL));
    // game and entity values
    const int nWindowWidth = 1200, nWindowHeight = 900;

    const int nStandardWidth = 30;
    const int nRows = nWindowWidth / 80, nCols = nWindowWidth / 80;
    const int nOffsetX = nWindowWidth / 16, nOffsetY = 50;
    const int nAdversaryOffsetX = nOffsetX * 3 + nStandardWidth * nCols;
    bool bEventTextUpdated = false;
    bool bShipsSelected = false;
    bool bDoneShooting = false;

    bool bShipVertical = true;
    int nCurrentShip = 0;
    int nLastRow, nLastCol;

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
    
    window.setFramerateLimit(60);
    
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
        bool bMouseMoved = false;
        bool bSpacePressed = false;

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
            if (event.type == sf::Event::MouseMoved) {
                bMouseMoved = true;
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    bSpacePressed = true;
                    bShipVertical = !bShipVertical;
                }
            }
        }

        // game logic
        switch (gameState) {
            case GameState::PlayerShipSelection: {
                bool selectionOver = nCurrentShip < playerShips.size() ? false : true;

                if (bMouseMoved || bSpacePressed) {
                    // when we check for space pressed we DON'T HAVE A MOUSE MOVE POSITION SO 
                    // IT CAN'T RENDER IT
                    // STUPID
                    int row = (static_cast<int>(event.mouseMove.y) - nOffsetY) / nStandardWidth;
                    int col = (static_cast<int>(event.mouseMove.x) - nOffsetX) / nStandardWidth;

                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        for (int i = 0; i < nRows; i++) {
                            for (int j = 0; j < nCols; j++) {
                                player.levelEntities[i * nRows + j].setColour(player.levelEntities[i * nRows + j].getIsShip() ? Colour::Grey : Colour::Blue);
                            }
                        }

                        int startingRow = row < nRows - playerShips[nCurrentShip].getSize() ? row : nRows - playerShips[nCurrentShip].getSize();
                        int startingCol = col < nCols - playerShips[nCurrentShip].getSize() ? col : nCols - playerShips[nCurrentShip].getSize();

                        if (bShipVertical) {
                            for (int i = startingRow; i < startingRow + playerShips[nCurrentShip].getSize(); i++) {
                                player.levelEntities[i * nRows + col].setColour(Colour::Red);
                            }
                        }
                        else {
                            for (int i = startingCol; i < startingCol + playerShips[nCurrentShip].getSize(); i++) {
                                player.levelEntities[row * nRows + i].setColour(Colour::Red);
                            }
                        }
                    }
                }

                // this is where the ship's positions are set
                if (bMousePressed) {
                    int row = (static_cast<int>(event.mouseButton.y) - nOffsetY) / nStandardWidth;
                    int col = (static_cast<int>(event.mouseButton.x) - nOffsetX) / nStandardWidth;
                    
                    bool bValidSelection = false;
                    std::string correction;

                    // a metric fuckton of checks
                    // first if we're withing boundaries
                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        // if we are, if the starting position is not good, we must change it
                        // this we actually do in the background invisibly
                        // we must only change the row or column, since the ship is either in a vertical or horizontal position
                        
                        // vertically
                        if (bShipVertical) {
                            row = row < nRows - playerShips[nCurrentShip].getSize() ? row : nRows - playerShips[nCurrentShip].getSize();
                        }
                        // horizontally
                        else {
                            col = col < nCols - playerShips[nCurrentShip].getSize() ? col : nCols - playerShips[nCurrentShip].getSize();
                        }

                        // then we check for intersections
                        bool bIntersectsShips = false;

                        // vertically
                        if (bShipVertical) {
                            for (int i = row; i < row + playerShips[nCurrentShip].getSize(); i++) {
                                if (player.levelEntities[i * nRows + col].getIsShip()) {
                                    bIntersectsShips = true;
                                    break;
                                }
                            }
                        }
                        // horizontally
                        else {
                            for (int i = col; i < col + playerShips[nCurrentShip].getSize(); i++) {
                                if (player.levelEntities[row * nRows + i].getIsShip()) {
                                    bIntersectsShips = true;
                                    break;
                                }
                            }
                        }
                        // if it doesn't intersect, we're set
                        if (!bIntersectsShips) {
                            bValidSelection = true;
                        }
                        else {
                            bValidSelection = false;
                            correction = "POSITION INTERSECTS ANOTHER SHIP.";
                        }
                    }
                    else {
                        bValidSelection = false;
                        correction = "SELECT A POSITION WITHIN BOUNDARIES.";
                    }

                    if (bValidSelection) {
                        if (bShipVertical) {
                            for (int i = row; i < row + playerShips[nCurrentShip].getSize(); i ++) {
                                std::cout << "PLAYER SELECTED: Ship " << nCurrentShip << ": " << i << " " << col << std::endl;

                                playerShips[nCurrentShip].shipFields.push_back(&player.levelEntities[i * nRows + col]);
                                playerShips[nCurrentShip].shipFields.back()->setColour(Colour::Grey);
                                player.levelEntities[i * nRows + col].setIsShip(true);
                            }
                        }
                        else {
                            for (int i = col; i < col + playerShips[nCurrentShip].getSize(); i ++) {
                                std::cout << "PLAYER SELECTED: Ship " << nCurrentShip << ": " << row << " " << i << std::endl;

                                playerShips[nCurrentShip].shipFields.push_back(&player.levelEntities[row * nRows + i]);
                                playerShips[nCurrentShip].shipFields.back()->setColour(Colour::Grey);
                                player.levelEntities[row * nRows + i].setIsShip(true);
                            }
                        }

                        nCurrentShip ++;
                    }
                    else {
                        std::cout << "INVALID SELECTION. " << correction << std::endl;
                    }
                }

                if (selectionOver) {
                    for (int i = 0; i < nRows; i++) {
                        for (int j = 0; j < nCols; j++) {
                            player.levelEntities[i * nRows + j].setColour(player.levelEntities[i * nRows + j].getIsShip() ? Colour::Grey : Colour::Blue);
                        }
                    }

                    gameState = GameState::AdversaryShipSelection;
                }
            }
                break;
            case GameState::AdversaryShipSelection: {
                // to do: smarter shelling
                for (int i = 0; i < adversaryShips.size(); i ++) {
                    int row;
                    int col;
                    int direction = rand() % 2;

                    do {
                        row = rand() % (nRows - adversaryShips[i].getSize());
                        col = rand() % (nCols - adversaryShips[i].getSize());
                    } while (checkAreaHasShip(adversary, adversaryShips[i].getSize(), row, col, direction, nRows, nCols));

                    // setting the direction of the enemy's ships
                    // 1 is horizontal
                    if (direction) {
                        for (int j = col; j < col + adversaryShips[i].getSize(); j ++) {
                            adversaryShips[i].shipFields.push_back(&adversary.levelEntities[row * nRows + j]);
                            adversary.levelEntities[row * nRows + j].setIsShip(true);
                        }
                    }
                    // 0 is vertical
                    else {
                        for (int j = row; j < row + adversaryShips[i].getSize(); j ++) {
                            adversaryShips[i].shipFields.push_back(&adversary.levelEntities[j * nRows + col]);
                            adversary.levelEntities[j * nRows + col].setIsShip(true);
                        }
                    }
                }

                bShipsSelected = true;
                gameState = GameState::PlayerTurn;
                Sleep(1000);
            }
                break;
            case GameState::PlayerTurn:
                // maybe later
                if (bMouseMoved) {
                    int row = (static_cast<int>(event.mouseMove.y) - nOffsetY) / nStandardWidth;
                    int col = (static_cast<int>(event.mouseMove.x) - nAdversaryOffsetX) / nStandardWidth;
                }
                if (bMousePressed) {
                    int row = ((int)event.mouseButton.y - nOffsetY) / nStandardWidth;
                    int col = ((int)event.mouseButton.x - nAdversaryOffsetX) / nStandardWidth;
                    
                    // here we check if our shot hit an enemy ship or not
                    // could be a bit fine-tuned
                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        std::cout << "PLAYER SHOT: " << row << " " << col << std::endl;
                        
                        if (!adversary.levelEntities[row * nRows + col].getIsHit()) {
                            std::cout << "PLAYER HIT: " << row << " " << col << std::endl;
                            adversary.levelEntities[row * nRows + col].setIsHit(true);
                            gameState = GameState::AdversaryTurn;
                        }
                        else {
                            std::cout << "POSITION ALREADY SHOT PREVIOUSLY. SHOOT AGAIN" << std::endl;
                        }
                        
                        if (adversary.levelEntities[row * nRows + col].getIsShip()) {
                            adversary.levelEntities[row * nRows + col].setColour(Colour::Red);
                        }
                        else {
                            adversary.levelEntities[row * nRows + col].setColour(Colour::White);
                        }
                    }
                    else {
                        std::cout << "INVALID SHOT POSITION. PLEASE SHOOT AGAIN" << std::endl;
                    }
                }

                break;
            case GameState::AdversaryTurn: {
                int row = rand() % nRows;
                int col = rand() % nCols;
                
                if (!player.levelEntities[row * nRows + col].getIsHit()) {
                    player.levelEntities[row * nRows + col].setIsHit(true);
                    std::cout << "ENEMY SHOT: " << row << " " << col << std::endl;

                    if (player.levelEntities[row * nRows + col].getIsShip()) {
                        std::cout << "ENEMY HIT: " << row << " " << col << std::endl;
                        player.levelEntities[row * nRows + col].setColour(Colour::Red);
                    }
                    else {
                        player.levelEntities[row * nRows + col].setColour(Colour::White);
                    }
                    
                    Sleep(1000);

                    gameState = GameState::PlayerTurn;
                }
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