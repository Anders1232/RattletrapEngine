#ifndef GAMEOBJECTFINDER_H
#define GAMEOBJECTFINDER_H

#include <limits>
#include <vector>
#include "Vec2.h"
#include "GameObject.h"

class NearestGOFinder{
	public:
		virtual GameObject* FindNearestGO(Vec2 origin, std::string targetType, float range= std::numeric_limits<float>::max()) = 0;
		virtual std::vector<GameObject*>* FindNearestGOs(Vec2 origin, std::string targetType, float range= std::numeric_limits<float>::max()) = 0;
		//lembrando que é responsabilidade do chamador a desalocação
		virtual ~NearestGOFinder(){};
};

#endif // GAMEOBJECTFINDER_H
