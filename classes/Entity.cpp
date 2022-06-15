#include "Entity.h"

Entity::Entity(int _nRow, int _nCol, int _nWidth, int _nOffsetX, int _nOffsetY, Colour _colour) : nRow(_nRow), nCol(_nCol), nWidth(_nWidth), colour(_colour) {
	this->entity.setSize(sf::Vector2f(_nWidth, _nWidth));
	this->entity.setPosition(nCol * nWidth + _nOffsetX, nRow * nWidth + _nOffsetY);
	this->entity.setFillColor(getSzin(_colour));
	this->entity.setOutlineColor(getSzin(Colour::Black));
	this->entity.setOutlineThickness(1.f);
	this->bHit = false;
	this->bIsShip = false;
}

// get-set for hits
void Entity::setIsHit(bool isHit) {
	this->bHit = isHit;
}

bool Entity::getIsHit() {
	return this->bHit;
}

// get-set for ship status

void Entity::setIsShip(bool isShip) {
	this->bIsShip = isShip;
}

bool Entity::getIsShip() {
	return this->bIsShip;
}

void Entity::setColour(Colour _colour) {
	this->entity.setFillColor(getSzin(_colour));
}

// positions
int Entity::getRow() {
	return this->nRow;
}

int Entity::getCol() {
	return this->nCol;
}

sf::RectangleShape Entity::getEntity() {
	return this->entity;
}

