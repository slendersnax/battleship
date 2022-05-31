#include "Ship.h"

Ship::Ship(int _nID, int _nSize) : nID(_nID), nSize(_nSize) {
	this->bDestroyed = false;
}

int Ship::getSize() {
	return this->nSize;
}