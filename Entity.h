#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Colours.h"

class Entity {
	private:
		double fX, fY;
		int nWidth, nHeight;
		Colour colour;

		sf::RectangleShape entity; // entity is a rectangle
	public:
		Entity(double _fX, double _fY, int _nWidth, int _nHeight, Colour _colour);
		void draw(sf::RenderWindow renderWindow);
		sf::RectangleShape getEntity();
};

#endif