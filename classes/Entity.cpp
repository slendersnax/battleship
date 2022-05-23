#include "Entity.h"

Entity::Entity(int _nRow, int _nCol, int _nWidth, Colour _colour) : nRow(_nRow), nCol(_nCol), nWidth(_nWidth), colour(_colour) {
	this->entity.setSize(sf::Vector2f(_nWidth, _nWidth));
	this->entity.setPosition(50, 50);
	this->entity.setFillColor(getSzin(_colour));
}

void Entity::draw(sf::RenderWindow renderWindow) {

}

sf::RectangleShape Entity::getEntity() {
	return this->entity;
}

