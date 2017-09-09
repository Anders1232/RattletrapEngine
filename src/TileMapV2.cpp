#include "TileMapV2.h"
#include "Error.h"

template<class T>
void TileMapV2<T>::Load(std::string const &file) {
	FILE *arq = fopen(file.c_str(), "r");
	ASSERT(NULL != arq);
	int mWidth, mHeight, mDepth;
	fscanf(arq, "%d,%d,%d,", &mapWidth, &mapHeight, &mapDepth);

	int numbersToRead = mapWidth*mapHeight*mapDepth;
	tileMatrix.resize(numbersToRead);// Assim ele não desperdiça memória nem muda de tamanho no for abaixo
	int aux;
	for(int count = 0; count < numbersToRead; count++) {
		fscanf(arq, " %d,", &aux);
		tileMatrix[count] = aux;
	}
}

template<class T>
TileMapV2<T>::TileMapV2(GameObject &associated, std::string const &file, TileSetV2 *tileSet): currentTileSet(0), associated(associated), displayCollisionInfo(false){
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
	tileSets.push_back(tileSet);
}

template<class T>
TileMapV2<T>::TileMapV2(GameObject &associated, std::string &file, std::vector<TileSetV2*> &tileSet): tileSets(tileSet), associated(associated), displayCollisionInfo(false){
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
}

template<class T>
void TileMapV2<T>::EarlyUpdate(float dt){}

template<class T>
void TileMapV2<T>::Update(float dt){}

template<class T>
void TileMapV2<T>::LateUpdate(float dt){}

template<class T>
void TileMapV2<T>::Render(void) const {
	for(int count = 0; count < mapDepth; count++) {
		if(COLLISION_LAYER == count && !displayCollisionInfo) {
			continue;
		}
		RenderLayer(count);
	}
}

template<class T>
void TileMapV2<T>::RenderLayer(int layer) {
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			int index = At(x, y, layer);
			if (0 <= index) {
				Vec2 destination;
				Vec2 tileSize= (tileSets[currentTileSet])->GetTileSize();
				if (parallax) {
					Vec2 tilePos(tileSize);
					destination = CalculateParallaxScrolling(tilePos, pos, layer);
				}
				else {
					destination = pos + Vec2(x*tileSize.x, y*tileSize.y);
				}
				Rect tile(destination.x, destination.y, tileSize.x, tileSize.y);
				tileSets[currentTileSet]->Render(index, destination);
			}
		}
	}
}


template<class T>
bool TileMapV2<T>::Is(ComponentType type) const;

template<class T>
T& TileMapV2<T>::At(int x, int y, int z=0) const;

template<class T>
T& TileMapV2<T>::AtLayer(int index2D, int layer) const;

template<class T>
int TileMapV2<T>::GetWidth(void);

template<class T>
int TileMapV2<T>::GetHeight(void);

template<class T>
int TileMapV2<T>::GetDepth(void);

template<class T>
int TileMapV2<T>::GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer)const;

template<class T>
void TileMapV2<T>::Parallax(bool parallax);

template<class T>
void TileMapV2<T>::SetParallaxLayerIntensity(int layer, float intensity);

template<class T>
T* TileMapV2<T>::FindNearest(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max());

template<class T>
std::vector<T*>* TileMapV2<T>::FindNearests(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max());


