#ifndef ASTAR_H
#define ASTAR_H

#include "Vec2.h"

class AStarHeuristic{
//	virtual AStarHeuristic(){};
	public:
		virtual int operator()(Vec2 originTile, Vec2 desTile) = 0;
		virtual ~AStarHeuristic(void){};
};

#endif
