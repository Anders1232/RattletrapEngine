#ifndef TILEMAPV2_H
#define TILEMAPV2_H

#include <vector>
#include "NearestGOFinder.h"
//#include "Tile.h"
#include "Component.h"
#include "Tileset.h"

template<class T>
class TileMapV2 : public Component{
	public:
		TileMapV2(GameObject &associated, std::string &file, TileSet *tileSet);
		TileMapV2(GameObject &associated, std::string &file, std::vector<TileSet*> &tileSet);
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
		int GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer)const;
		void Parallax(bool parallax);
		void SetParallaxLayerIntensity(int layer, floar intensity);
	private:
		int mapWidth;
		int mapHeight;
		int mapDepth;
		std::vector<T> tileMatrix;
		std::vector<float> parallaxWeight;
		std::vector<TileSet*> tileSets;
		GameObject &associated;
};

#endif // TILEMAPV2_H
