#include "Field.h"
#include <fstream>

Field::Field(int _nRows, int _nCols, int _nOffsetX, int _nOffsetY, int entityWidth) : nRows(_nRows), nCols(_nCols) {
	for (int i = 0; i < nRows; i ++) {
		for (int j = 0; j < nCols; j ++) {
			levelEntities.push_back(Entity(i, j, entityWidth, _nOffsetX, _nOffsetY, Colour::Blue));
		}
	}
}