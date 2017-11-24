#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <algorithm>
#include "NearestFinder.h"
//#include "Tile.h"
#include "Component.h"
#include "Tileset.h"
#include "TileMapObserver.h"

class AStarHeuristic{
//	virtual AStarHeuristic(){};
	public:
		virtual int operator()(Vec2 originTile, Vec2 desTile) = 0;
		virtual ~AStarHeuristic(void){};
};

//---------------------------

template<class T>
class AStarWeight{
//	virtual AStarWeight(){};
	public:
		virtual float CalculateCost(T& tile) = 0;
		virtual bool IsTraversable(T& tile)=0;
		virtual ~AStarWeight(void){};
};

//---------------------------


/**
	\todo Que o define da camada de colisão se torne um inteiro ou lista de inteiros, que é lida do arquivo
*/
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
		std::vector<T*>* FindNearests(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max()) const;
		void ReportChanges(int tileChanged);
		void ObserveMapChanges(TileMapObserver *);
		void RemoveObserver(TileMapObserver *);
		std::list<int>* AStar(int originTile,int destTile, AStarHeuristic* heuristic, AStarWeight<T> weightMap);
		inline Vec2 GetVec2Coord(int pos);
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
		inline std::vector<int64_t> GetNeighbours(int64_t tile);
		
		void Load(std::string const &file);
		void RenderLayer(int layer);
};

//------------------------

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
		tileMatrix[count] = T(aux);
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
Vec2 TileMap<T>::GetVec2Coord(int pos){
	return Vec2(pos / GetWidth(), pos % GetWidth());
}

template<class T>
std::vector<int64_t> TileMap<T>::GetNeighbours(int64_t tile){//TODO: otimizar
	std::vector<int64_t> ret;
	int64_t tileMapSize= GetWidth()*GetHeight();
	int64_t temp= GetWidth()+1;
	if(tile > temp){
		ret.push_back(tile-temp);
	}
	temp--;
	if(tile > temp){
		ret.push_back(tile-temp);
	}
	temp--;
	if(tile > temp){
		ret.push_back(tile-temp);
	}
	if(0 < tile){
		ret.push_back(tile--);
	}
	if(tile < (tileMapSize-1)){
		ret.push_back(tile++);
	}
	temp= tile+GetWidth()-1;
	if( temp < tileMapSize ){
		ret.push_back(temp);
	}
	temp++;
	if( temp < tileMapSize ){
		ret.push_back(temp);
	}
	temp++;
	if( temp < tileMapSize ){
		ret.push_back(temp);
	}
	return ret;
}


inline void OrderedInsertion(std::vector<int64_t>& v, int64_t num)
{
	auto index= std::lower_bound(v.begin(), v.end(), num);
	if(index != v.end())
	{
		if((*index) != num)
		{
			v.insert(index, num);
			return;
		}
	}
	else
	{
		v.push_back(num);
		return;
	}
}

template<class T>
class AStarCompare{
	public:
		AStarCompare(std::vector<int64_t> const &guessedCost, std::vector<int64_t> const &accumulatedCost):
			accumulatedCost(accumulatedCost),
			guessedCost(guessedCost){}
		bool operator()(int64_t a, int64_t b){
			if(ELEMENT_ACESS(guessedCost, a) < ELEMENT_ACESS(guessedCost, b) ){
				return true;
			}
			else if(ELEMENT_ACESS(guessedCost, a) == ELEMENT_ACESS(guessedCost, b)){
				return ELEMENT_ACESS(accumulatedCost, b) < ELEMENT_ACESS(accumulatedCost, a);
			}
			else{
				return false;
			}
		}
	private:
		std::vector<int64_t> const &guessedCost;
		std::vector<int64_t> const &accumulatedCost;
};

inline std::list<int>* MakePath(int origin, int dest, std::vector<int64_t> const &antecessor){
	std::list<int> *ret= new std::list<int>();
	int aux= dest;
	while(aux != origin){
		ret->push_front(aux);
		aux= antecessor[aux];
	}
	ret->push_front(origin);
	return ret;
}

template<class T>
std::list<int>* TileMap<T>::AStar(int originTile,int destTile, AStarHeuristic* heuristic, AStarWeight<T> weightMap){
	std::vector<int64_t> closedSet;
	std::vector<int64_t> openSet;//esse vetor deve ser ordenado
	openSet.push_back(originTile);
	std::vector<int64_t> antecessor(GetWidth() * GetHeight(), -1);
	std::vector<float> accumulatedCost(GetWidth() * GetHeight(), std::numeric_limits<float>::max() );
	accumulatedCost[originTile]=0;
	std::vector<float> guessedCost(GetWidth() * GetHeight(), std::numeric_limits<float>::max() );
	guessedCost[originTile]= (*heuristic)(GetVec2Coord(originTile), GetVec2Coord(destTile) );
	while(openSet.size() > 0){
		int current= openSet[0];
		if(current == destTile){
			return MakePath(originTile, destTile, antecessor);
		}
		openSet.erase(openSet.begin());
		OrderedInsertion(closedSet,current);//verificar se aqui precisa de adição ordenada
		std::vector<int64_t> neightbors= GetNeighbours(current);
		for(int i=0; i < neightbors.size(); i++){
			int64_t currentNeightbor= neightbors[i];
			if(!weightMap.IsTraversable(ELEMENT_ACESS(tileMatrix,current) ) ){
				continue;
			}
			if(std::binary_search(closedSet.begin(), closedSet.end(), currentNeightbor) ){
				continue;
			}
			if(!std::binary_search(openSet.begin(), openSet.end(), currentNeightbor) ){
				openSet.push_back(currentNeightbor);
				std::sort(openSet.begin(), openSet.end(), AStarCompare<T>(guessedCost, accumulatedCost));
			}
			float tentativeDistance= ELEMENT_ACESS(accumulatedCost, current) + weightMap.CalculateCost(currentNeightbor);
			if(tentativeDistance < accumulatedCost[currentNeightbor ] ){
				antecessor[currentNeightbor]= current;
				accumulatedCost[currentNeightbor]= tentativeDistance;
				guessedCost[currentNeightbor]= accumulatedCost[currentNeightbor] + (*heuristic)(GetVec2Coord(currentNeightbor), GetVec2Coord(destTile));
			}
		}
	}
	//se chegar aqui é pq não achou um caminho da origem ao destino
	std::vector<int64_t> bestPaths;
	float minimumCostFound= std::numeric_limits<float>::max();
	for(int i=0; i< guessedCost.size(); i++){
		if(guessedCost[i] < minimumCostFound){
			bestPaths.clear();
			bestPaths.push_back(i);
			minimumCostFound= guessedCost[i];
		}
		else if(guessedCost[i] == minimumCostFound){
			bestPaths.push_back(i);
		}
	}
	int chosenPath=0;
	for(int i=1; i< bestPaths.size(); i++){
		if(accumulatedCost[i] > accumulatedCost[chosenPath]){
			chosenPath= i;
		}
	}
	return MakePath(originTile, chosenPath, antecessor);
}

template<class T>
void TileMap<T>::SetLayerVisibility(int layer, bool visibility){
	ELEMENT_ACESS(layersVisibility, layer) = visibility;
}

template<class T>
bool TileMap<T>::IsLayerVisible(int layer){
	return ELEMENT_ACESS(layersVisibility, layer);
}



#endif // TILEMAP_H
