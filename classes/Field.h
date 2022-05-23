#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <string>
#include "Entity.h"

class Field {
	private:
		std::vector<Entity> levelEntities;

	public:
		Field();
		void init();
};

#endif
