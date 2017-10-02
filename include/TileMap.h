#ifndef TILEMAPV2_H
#define TILEMAPV2_H

#include <vector>
#include "NearestFinder.h"
//#include "Tile.h"
#include "Component.h"
#include "Tileset.h"
#include "TileMapObserver.h"

/**
	\todo Que o define da camada de colisão se torne um inteiro ou lista de inteiros, que é lida do arquivo
*/
#define COLLISION_LAYER (1)

template<class T>
class TileMap : public Component, NearestFinder<T>{
	public:
		TileMap(GameObject &associated, const std::__cxx11::string &file, TileSet *tileSet);
		TileMap(GameObject &associated, std::string &file, std::vector<TileSet*> &tileSet);
		void EarlyUpdate(float dt=0);
		void Update(float dt=0);
		void LateUpdate(float dt =0);
		void Render(void) const;
		bool Is(ComponentType type) const;
		T& At(int x, int y, int z=0) const;
		T& AtLayer(int index2D, int layer) const;
		int GetWidth(void) const;
		int GetHeight(void) const;
		int GetDepth(void) const;
		int GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer) const;
//		void Parallax(bool parallax);
		void SetParallaxLayerIntensity(int layer, float intensity);
		T* FindNearest(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max()) const;
		std::vector<T*>* FindNearests(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max())  const;
		void ReportChanges(int tileChanged);
		void ObserveMapChanges(TileMapObserver *);
		void RemoveObserver(TileMapObserver *);

	private:
		int mapWidth;
		int mapHeight;
		int mapDepth;
		std::vector<T> tileMatrix;
		std::vector<float> parallaxWeight;
		std::vector<TileSet*> tileSets;
		std::vector<TileMapObserver*> observers;
		int currentTileSet;
		bool displayCollisionInfo;
		Vec2 CalculateParallaxScrolling(Vec2 num, Rect& pos, float weigth) const;

		void Load(std::string const &file);
		void RenderLayer(int layer);
};

#endif // TILEMAPV2_H
