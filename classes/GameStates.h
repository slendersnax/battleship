#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <string>

enum class GameState { PlayerShipSelection, AdversaryShipSelection, PlayerTurn, AdversaryTurn, GameOver };

std::string getGameState(GameState state);

#endif