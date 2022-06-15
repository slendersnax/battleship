#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Colours.h"

class Entity {
	private:
		int nRow, nCol;
		int nWidth;
		bool bHit;
		bool bIsShip;
		Colour colour;

		sf::RectangleShape entity;
	public:
		Entity(int _nRow, int _nCol, int _nWidth, int _nOffsetX, int _nOffsetY, Colour _colour);
		void setIsHit(bool isHit);
		bool getIsHit();
		void setIsShip(bool isShip);
		bool getIsShip();
		void setColour(Colour _colour);
		int getRow();
		int getCol();
		sf::RectangleShape getEntity();
};

#endif