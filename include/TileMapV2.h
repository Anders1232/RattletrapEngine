#ifndef TILEMAPV2_H
#define TILEMAPV2_H

#include <vector>
#include "NearestGOFinder.h"
//#include "Tile.h"
#include "Component.h"

template<class T>
class TileMapV2 : public Component, NearestGOFinder{
	public:
		TileMapV2(void);
		void EarlyUpdate(float dt=0);
		void Update(float dt=0);
		void LateUpdate(float dt =0);
		void Render(void) const;
		bool Is(ComponentType type) const;
		T& At(int x, int y, int z=0) const;
		T& AtLayer(int index2D, int layer) const;
		int GetWidth(void);
		int GetHeight(void);
		int GetDepth(void);
		int Get CoordTilePos(Vec2 const &mousePos, bool affecteedByZoom, int layer)const;
	private:
		int mapWidth;
		int mapHeight;
		int mapDepth;
		std::vector<T> tileMatrix;
		std::vector<float> parallaxWeight;
};

#endif // TILEMAPV2_H
