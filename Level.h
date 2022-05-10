#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include "Entity.h"

class Level {
	private:
		std::vector<Entity> levelEntities;

	public:
		Level();
		init(std::string levelName);
};

#endif
