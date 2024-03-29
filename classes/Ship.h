#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"
#include <vector>

class Ship {
	private:
		int nID;
		int nSize;
		bool bDestroyed;
	public:
		std::vector<Entity*> shipFields;
		Ship(int _nID, int _nSize);
		int getSize();
};

#endif
