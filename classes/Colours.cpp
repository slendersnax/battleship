#include "Colours.h"

sf::Color getSzin(Colour colour) {
	switch (colour) {
		case Colour::White:
			return sf::Color(255, 255, 255);
			break;
		case Colour::Black:
			return sf::Color(0, 0, 0);
			break;
		case Colour::Red:
			return sf::Color(255, 0, 0);
			break;
		case Colour::Green:
			return sf::Color(0, 255, 0);
			break;
		case Colour::Blue:
			return sf::Color(0, 0, 255);
			break;
		default:
			return sf::Color(255, 255, 255);
			break;
	}
}