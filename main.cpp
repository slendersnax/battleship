#include <SFML/Graphics.hpp>
#include "classes/Field.h"
#include "classes/Ship.h"
#include "classes/GameStates.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

bool checkAreaHasShip(Field adversary, int size,  int row, int col, int direction, int nCols) {
    // check horizontally
    if (direction) {
        for (int i = col; i < size; i ++) {
            if (adversary.levelEntities[row * nCols + i].getIsShip()) {
                return true;
            }
        }
    }
    // check vertically
    else {
        for (int i = row; i < size; i ++) {
            if (adversary.levelEntities[i * nCols + col].getIsShip()) {
                return true;
            }
        }
    }

    return false;
}

// create a separate class for current Events?
void moveCurrentEventsBack(std::vector<sf::Text>& currentEvents, const int nMaxEvents) {
    for(int i = 0; i < nMaxEvents - 1; i ++) {
        currentEvents[i].setString(currentEvents[i + 1].getString());
    }
}

void emptyCurrentEvents(std::vector<sf::Text>& currentEvents, const int nMaxEvents, int& nEvents) {
    for(int i = 0; i < nMaxEvents; i ++) {
        currentEvents[i].setString("");
    }

    nEvents = 0;
}

void updateCurrentEvents(std::vector<sf::Text>& currentEvents, const int nMaxEvents, int& nEvents, std::string sMessage) {
    if(nEvents >= nMaxEvents) {
        moveCurrentEventsBack(currentEvents, nMaxEvents);
        nEvents = nMaxEvents - 1;
    }

    currentEvents[nEvents].setString(sMessage);
    nEvents ++;
}

// choosing a field for the adversary to shoot
int playerAreaToShoot(std::vector<int>& viableAreas) {
    int iArea, areaToShoot; // inde of Area

    // v1: random

    iArea = rand() % viableAreas.size();
    areaToShoot = viableAreas[iArea];

    viableAreas.erase(viableAreas.begin() + iArea);

    return areaToShoot;
}

int main() {
    srand(time(NULL));
    // game and entity values
    const int nWindowWidth = 1200, nWindowHeight = 900;

    const int nStandardWidth = 30;
    const int nRows = nWindowWidth / 80, nCols = nWindowWidth / 80;
    const int nOffsetX = nWindowWidth / 16, nOffsetY = 50;
    const int nAdversaryOffsetX = nOffsetX * 3 + nStandardWidth * nCols;
    bool bShipsSelected = false;

    bool bShipVertical = true;
    int nCurrentShip = 0;

    bool bMouseLeftReleased = false;
    bool bMouseRightReleased = false;
    bool bMouseMoved = false;

    sf::Font timesNewRoman;
    sf::Text gameStateText;
    const int nMaxEvents = 5;
    int nEvents = 0;
    std::vector<sf::Text> currentEvents(5); // to show what has recently happened
    GameState gameState;
    sf::RenderWindow window(sf::VideoMode(nWindowWidth, nWindowHeight), "BattleShip!");

    if (!timesNewRoman.loadFromFile("times-new-roman.ttf")) {
        std::cerr << "Couldn't load font." << std::endl;
        exit(1);
    }

    gameStateText.setFont(timesNewRoman);
    gameStateText.setFillColor(getSzin(Colour::White));
    gameStateText.setCharacterSize(nStandardWidth * 0.8f);
    gameStateText.setPosition(nOffsetX, nOffsetY / 4);

    for(int i = 0; i < nMaxEvents; i ++) {
        currentEvents[i].setFont(timesNewRoman);
        currentEvents[i].setFillColor(getSzin(Colour::White));
        currentEvents[i].setCharacterSize(nStandardWidth * 0.7f);
        currentEvents[i].setPosition(nOffsetX, 510 + i * 30);
        currentEvents[i].setString("");
    }

    gameState = GameState::PlayerShipSelection;
    
    window.setFramerateLimit(60);
    
    // entities
    Field player(nRows, nCols, nOffsetX, nOffsetY, nStandardWidth);
    Field adversary(nRows, nCols, nAdversaryOffsetX, nOffsetY, nStandardWidth);

    std::vector<Ship> playerShips;
    std::vector<Ship> adversaryShips;

    std::vector<int> playerShootableAreas(player.levelEntities.size());

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

    // this is all the areas that the enemy will be able to shoot
    // we will further filter these based on a few criteria
    // see playerAreaToShoot()
    for (unsigned int i = 0; i < player.levelEntities.size(); i ++) {
        playerShootableAreas[i] = i;
    }

    // main loop
    while (window.isOpen()) {
        // event handling
        sf::Event event;
        bMouseLeftReleased = false;
        bMouseRightReleased = false;
        bMouseMoved = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // note! if you press the mouse while moving it it may not work
            // problem with SFML?
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    bMouseLeftReleased = true;
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    bMouseRightReleased = true;
                    bShipVertical = !bShipVertical;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                bMouseMoved = true;
            }
        }

        // game logic
        switch (gameState) {
            case GameState::PlayerShipSelection: {
                bool selectionOver = nCurrentShip < playerShips.size() ? false : true;

                // we re-render things if the mouse has moved or if the ship's direction has changed (right click release)
                if (bMouseMoved || bMouseRightReleased) {
                    int mouseX, mouseY;
                    
                    if(bMouseMoved) {
                        mouseX = static_cast<int>(event.mouseMove.x);
                        mouseY = static_cast<int>(event.mouseMove.y);
                    }
                    else {
                        mouseX = static_cast<int>(event.mouseButton.x);
                        mouseY = static_cast<int>(event.mouseButton.y);
                    }

                    int row = (mouseY - nOffsetY) / nStandardWidth;
                    int col = (mouseX - nOffsetX) / nStandardWidth;

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
                if (bMouseLeftReleased) {
                    int row = (static_cast<int>(event.mouseButton.y) - nOffsetY) / nStandardWidth;
                    int col = (static_cast<int>(event.mouseButton.x) - nOffsetX) / nStandardWidth;
                    
                    bool bValidSelection = false;
                    std::string sCorrection = "";
                    std::string sMessage = "";

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
                            sCorrection = "POSITION INTERSECTS WITH ANOTHER SHIP.";
                        }
                    }
                    else {
                        bValidSelection = false;
                        sCorrection = "SELECT A POSITION WITHIN BOUNDARIES.";
                    }

                    if (bValidSelection) {
                        sMessage = "Player has selected position for ship " + std::to_string(nCurrentShip + 1);
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
                        sMessage = "INVALID SELECTION. " + sCorrection;
                    }

                    updateCurrentEvents(currentEvents, nMaxEvents, nEvents, sMessage);
                }

                if (selectionOver) {
                    for (int i = 0; i < nRows; i++) {
                        for (int j = 0; j < nCols; j++) {
                            player.levelEntities[i * nRows + j].setColour(player.levelEntities[i * nRows + j].getIsShip() ? Colour::Grey : Colour::Blue);
                        }
                    }

                    gameState = GameState::AdversaryShipSelection;
                    emptyCurrentEvents(currentEvents, nMaxEvents, nEvents);
                    updateCurrentEvents(currentEvents, nMaxEvents, nEvents, "Player has selected ship positions.");
                }
            }
                break;
            case GameState::AdversaryShipSelection: {
                for (unsigned int i = 0; i < adversaryShips.size(); i ++) {
                    int row;
                    int col;
                    int direction = rand() % 2;

                    do {
                        row = rand() % (nRows - adversaryShips[i].getSize());
                        col = rand() % (nCols - adversaryShips[i].getSize());
                    } while (checkAreaHasShip(adversary, adversaryShips[i].getSize(), row, col, direction, nCols)); // potential bug here? ships sometimes overlap

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
                
                std::cout << std::flush;
                usleep(1000);

                updateCurrentEvents(currentEvents, nMaxEvents, nEvents, "Adversary has selected ship positions.");
                gameState = GameState::PlayerTurn;
            }
                break;
            case GameState::PlayerTurn:
                if (bMouseLeftReleased) {
                    int row = ((int)event.mouseButton.y - nOffsetY) / nStandardWidth;
                    int col = ((int)event.mouseButton.x - nAdversaryOffsetX) / nStandardWidth;
                    std::string sMessage = "";
                    bool bHitSomething = false;
                    
                    // here we check if our shot hit an enemy ship or not
                    // could be a bit fine-tuned
                    if (row >= 0 && row < nRows && col >= 0 && col < nCols) {
                        std::cout << "PLAYER SHOT: " << row << " " << col << std::endl;
                        sMessage = "Player shot: " + std::to_string(row) + " " + std::to_string(col);
                        
                        if (!adversary.levelEntities[row * nRows + col].getIsHit()) {
                            std::cout << "PLAYER HIT: " << row << " " << col << std::endl;
                            adversary.levelEntities[row * nRows + col].setIsHit(true);
                            bHitSomething = true;
                        }
                        else {
                            std::cout << "POSITION ALREADY SHOT PREVIOUSLY. SHOOT AGAIN" << std::endl;
                            sMessage = "POSITION ALREADY SHOT PREVIOUSLY. SHOOT AGAIN";
                        }
                        
                        if (adversary.levelEntities[row * nRows + col].getIsShip()) {
                            adversary.levelEntities[row * nRows + col].setColour(Colour::Red);
                        }
                        else {
                            adversary.levelEntities[row * nRows + col].setColour(Colour::DarkBlue);
                        }
                    }
                    else {
                        std::cout << "INVALID SHOT POSITION. PLEASE SHOOT AGAIN" << std::endl;
                        sMessage = "INVALID SHOT POSITION. PLEASE SHOOT AGAIN";
                    }

                    updateCurrentEvents(currentEvents, nMaxEvents, nEvents, sMessage);

                    if(bHitSomething) {
                        gameState = GameState::AdversaryTurn;
                    }
                }

                break;
            case GameState::AdversaryTurn: {
                int positionToShoot = playerAreaToShoot(playerShootableAreas);
                int row = (positionToShoot / nCols) + 1;
                int col = (positionToShoot % nCols) + 1;
                std::string sMessage;
                
                if (!player.levelEntities[positionToShoot].getIsHit()) {
                    player.levelEntities[positionToShoot].setIsHit(true);
                    std::cout << "ENEMY SHOT: " << positionToShoot << " rc: " << row << " " << col << std::endl;
                    sMessage = "Enemy shot: " + std::to_string(row) + " " + std::to_string(col);

                    if (player.levelEntities[positionToShoot].getIsShip()) {
                        std::cout << "ENEMY HIT: " << row << " " << col << std::endl;
                        player.levelEntities[positionToShoot].setColour(Colour::Red);
                    }
                    else {
                        player.levelEntities[positionToShoot].setColour(Colour::DarkBlue);
                    }
                    
                    std::cout << std::flush;
                    usleep(1000);

                    updateCurrentEvents(currentEvents, nMaxEvents, nEvents, sMessage);
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
            for (unsigned int i = 0; i < playerShips.size(); i++) {
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
            for (unsigned int i = 0; i < adversaryShips.size(); i++) {
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
            
            if(playerShips.size() == 0) {
                updateCurrentEvents(currentEvents, nMaxEvents, nEvents, "Player lost!");
            }
            else {
                updateCurrentEvents(currentEvents, nMaxEvents, nEvents, "Player won!");
            }
        }
        
        // changing the text to fit the game state
        gameStateText.setString(getGameState(gameState));

        // drawing
        window.clear();

        for (unsigned int i = 0; i < player.levelEntities.size(); i ++) {
            window.draw(player.levelEntities[i].getEntity());
        }

        for (unsigned int i = 0; i < adversary.levelEntities.size(); i ++) {
            window.draw(adversary.levelEntities[i].getEntity());
        }

        window.draw(gameStateText);

        for(int i = 0; i < nMaxEvents; i ++) {
            window.draw(currentEvents[i]);
        }

        window.display();
    }

    return 0;
}