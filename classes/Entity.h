#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Colours.h"

class Entity {
	private:
		int nRow, nCol;
		int nWidth;
		bool bShip;
		Colour colour;

		sf::RectangleShape entity;
	public:
		Entity(int _nRow, int _nCol, int _nWidth, Colour _colour);
		void draw(sf::RenderWindow renderWindow);
		sf::RectangleShape getEntity();
};

#endif