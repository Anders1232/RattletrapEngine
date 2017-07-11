#include "ManhattanDistance.h"

ManhattanDistance::ManhattanDistance(){
	
}

int ManhattanDistance::operator()(Vec2 origin, Vec2 destiny){
	int distanceX= origin.x - destiny.x;
	int distanceY= origin.y-destiny.y;
	if(0 > distanceX){
		distanceX= -distanceX;
	}
	if(0 > distanceY){
		distanceY= -distanceY;
	}
	return distanceX+distanceY;
}

