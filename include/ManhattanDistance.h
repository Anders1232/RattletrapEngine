#ifndef MANHATTANDISTANCE_H
#define MANHATTANDISTANCE_H

#include "AStarHeuristic.h"

class ManhattanDistance : public AStarHeuristic
{
	public:
		ManhattanDistance();
		int operator()(Vec2 origin, Vec2 destiny);
};

#endif // MANHATTANDISTANCE_H
