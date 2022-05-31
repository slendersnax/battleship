#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "Entity.h"

class Field {
	private:
		int nRows, nCols;

	public:
		std::vector<Entity> levelEntities;
		Field(int _nRows, int _nCols, int _nOffsetX, int _nOffsetY, int entityWidth);
		void setShip(int nStartRow, int nStartCol, int nEndRow, int nEndCol);
};

#endif
