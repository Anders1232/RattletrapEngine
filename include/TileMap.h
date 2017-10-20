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
		TileMap(GameObject &associated, std::string const &file, TileSet *tileSet);
		TileMap(GameObject &associated, std::string const &file, std::vector<TileSet*> &tileSet);
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
		void SetLayerVisibility(int layer, bool visibility);
		bool IsLayerVisible(int layer);
	private:
		int mapWidth;
		int mapHeight;
		int mapDepth;
		std::vector<T> tileMatrix;
		std::vector<float> parallaxWeight;
		std::vector<TileSet*> tileSets;
		std::vector<TileMapObserver*> observers;
		std::vector<bool> layersVisibility;
		int currentTileSet;
		Vec2 CalculateParallaxScrolling(Vec2 num, Rect& pos, float weigth) const;

		void Load(std::string const &file);
		void RenderLayer(int layer);
};

//implementação

#include <limits>
#include "Error.h"
#include "Camera.h"


template<class T>
void TileMap<T>::Load(std::string const &file) {
	FILE *arq = fopen(file.c_str(), "r");
	ASSERT(NULL != arq);
	fscanf(arq, "%d,%d,%d,", &mapWidth, &mapHeight, &mapDepth);

	int numbersToRead = mapWidth*mapHeight*mapDepth;
	tileMatrix.resize(numbersToRead);// Assim ele não desperdiça memória nem muda de tamanho no for abaixo
	int aux;
	for(int count = 0; count < numbersToRead; count++) {
		fscanf(arq, " %d,", &aux);
		tileMatrix[count] = aux;
	}
	layersVisibility= std::vector<bool>(mapDepth, true);
}

template<class T>
TileMap<T>::TileMap(GameObject &associated, std::string const &file, TileSet *tileSet): Component(associated), currentTileSet(0){
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
	tileSets.push_back(tileSet);
}

template<class T>
TileMap<T>::TileMap(GameObject &associated, std::string const &file, std::vector<TileSet*> &tileSet): Component(associated), tileSets(tileSet){
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
}

template<class T>
void TileMap<T>::EarlyUpdate(float dt){}

template<class T>
void TileMap<T>::Update(float dt){}

template<class T>
void TileMap<T>::LateUpdate(float dt){}

template<class T>
void TileMap<T>::Render(void) const {
	for(int count = 0; count < mapDepth; count++) {
		if(layersVisibility[count]) {
			RenderLayer(count);
		}
	}
}

template<class T>
void TileMap<T>::RenderLayer(int layer) {
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			int index = At(x, y, layer).GetTileSetIndex();
			if (0 <= index) {
				Rect destination;
				Vec2 tileSize= (tileSets[currentTileSet])->GetTileSize();
				Vec2 tilePos(tileSize);
				destination = CalculateParallaxScrolling(tilePos, associated.box, parallaxWeight[layer]);
				Rect tile(destination.x, destination.y, tileSize.x, tileSize.y);
				tileSets[currentTileSet]->Render(index, destination);
			}
		}
	}
}

template<class T>
Vec2 TileMap<T>::CalculateParallaxScrolling(Vec2 num, Rect& pos, float weigth) const {
	return num-(pos+Camera::pos)*(weigth+1);
	return num-(pos+Camera::pos)*((weigth+1)/(double)mapDepth);
	return num*(1.0+(double)weigth/(double)mapDepth);
	return num*(1.0-(double)weigth/(double)mapDepth);
}

template<class T>
bool TileMap<T>::Is(ComponentType type) const{
	return type == ComponentType::TILEMAP;
}

//mudanaça: o método abaixo pode lançar exceção
template<class T>
T& TileMap<T>::At(int x, int y, int z) const{
	int index = z*mapWidth*mapHeight + y*mapWidth + x;
	return (tileMatrix.at(index) );
}

template<class T>
T& TileMap<T>::AtLayer(int index2D, int layer) const{
	return tileMatrix.at(index2D + layer * mapWidth * mapHeight);
}

template<class T>
int TileMap<T>::GetWidth(void) const{
	return mapWidth;
}

template<class T>
int TileMap<T>::GetHeight(void) const{
	return mapHeight;
}

template<class T>
int TileMap<T>::GetDepth(void) const{
	return mapDepth;
}

template<class T>
int TileMap<T>::GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer) const{
	Vec2 position = coordPos;
	int x, xDir = mapWidth-1, xEsq = 0;
	Vec2 tileSize = CalculateParallaxScrolling( Vec2(tileSets[currentTileSet]->GetTileSize()), Vec2(0, 0), layer);
	tileSize = tileSize - CalculateParallaxScrolling( Vec2(0,0), Vec2(0,0), layer);
	int tileWidth = tileSize.x;
	int tileHeight = tileSize.y;
	if(position.x < 0){
		std::cerr << WHERE << "Devo lançar exceção aqui?(-1)" << endl;
		return -1;
	}
	if(position.y < 0){
		std::cerr << WHERE << "Devo lançar exceção aqui?(-2)" << endl;
		return -2;
	}
	if(position.x >= (GetWidth()-1)* tileWidth ){
		std::cerr << WHERE << "Devo lançar exceção aqui?(-3)" << endl;
		return -3;
	}
	if(position.y >= (GetHeight()-1)* tileHeight ){
		std::cerr << WHERE << "Devo lançar exceção aqui?(-4)" << endl;
		return -4;
	}
	while(1){//uma simplesBusca binária
		x = (xEsq+xDir)/2;
		if(x*tileWidth <= position.x){
			if(position.x < (x+1)*tileWidth) {
				break;
			}
			else {
				//x está pra direita
				xEsq = x;
			}
		}
		else {
			//x está pra esquerda
			xDir = x;
		}
	}
	int y, yDir = mapHeight-1, yEsq = 0;
	while(1){//uma simplesBusca binária
		y = (yEsq+yDir)/2;
		if(y*tileHeight <= position.y) {
			if(position.y < (y+1)*tileHeight) {
				break;
			} else {
				//y está pra direita
				yEsq = y;
			}
		}
		else {
			//y está pra esquerda
			yDir = y;
		}
	}
	return y*mapWidth+x;
}

//template<class T>
//void TileMap<T>::Parallax(bool parallax);

template<class T>
void TileMap<T>::SetParallaxLayerIntensity(int layer, float intensity){
	parallaxWeight[layer]= intensity;
}

template<class T>
T* TileMap<T>::FindNearest(Vec2 origin, Finder<T> finder, float range) const{
	T* chosen= nullptr;
	float chosenTillNow= range;
	for(int i=0; i < tileMatrix.size();i++){
		float tempRes= finder(tileMatrix[i]);
		if(tempRes < chosenTillNow){
			chosen= &(tileMatrix[i]);
			chosenTillNow= tempRes;
		}
	}
	return chosen;
}

template<class T>
std::vector<T*>* TileMap<T>::FindNearests(Vec2 origin, Finder<T> finder, float range) const{
	std::vector<T*> *chosen= new std::vector<T*>();
	for(int i=0; i < tileMatrix.size();i++){
		if(finder(tileMatrix[i]) < range){
			chosen->push_back( &(tileMatrix[i]) );
		}
	}
	return chosen;
}

template<class T>
void TileMap<T>::ObserveMapChanges(TileMapObserver *obs){
	observers.emplace_back(obs);
}

template<class T>
void TileMap<T>::RemoveObserver(TileMapObserver *obs){
	for(uint i=0; i < observers.size(); i++){
		if(observers[i] == obs){
			observers.erase(observers.begin()+i);
			return;
		}
	}
	Error("\tTileMap observer not found!");
}

template<class T>
void TileMap<T>::ReportChanges(int tileChanged){
	for(uint i=0; i< observers.size(); i++){
		observers[i]->NotifyTileMapChanged(tileChanged);
	}
}

template<class T>
void TileMap<T>::SetLayerVisibility(int layer, bool visibility){
	ELEMENT_ACESS(layersVisibility, layer) = visibility;
}

template<class T>
bool TileMap<T>::IsLayerVisible(int layer){
	return ELEMENT_ACESS(layersVisibility, layer);
}



#endif // TILEMAPV2_H
