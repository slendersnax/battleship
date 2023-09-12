#include "GameStates.h"

std::string getGameState(GameState state) {
	switch (state) {
		case GameState::PlayerShipSelection:
			return "Select ship positions";
			break;
		case GameState::AdversaryShipSelection:
			return "Enemy is selecting ship positions...";
			break;
		case GameState::PlayerTurn:
			return "Your turn";
			break;
		case GameState::AdversaryTurn:
			return "Enemy's turn";
			break;
		case GameState::GameOver:
			return "Game Over";
			break;
		default:
			return "what?? this isn't supposed to happen!";
			break;
	}
}