#include "Entity.h"

Entity::Entity(double _fX, double _fY, int _nWidth, int _nHeight, Colour _colour) : fX(_fX), fY(_fY), nWidth(_nWidth), nHeight(_nHeight), colour(_colour) {
	this->entity.setSize(sf::Vector2f(_nWidth, _nHeight));
	this->entity.setPosition(_fX, _fY);
	this->entity.setFillColor(getSzin(_colour));
}

void Entity::draw(sf::RenderWindow renderWindow) {

}

sf::RectangleShape Entity::getEntity() {
	return this->entity;
}

