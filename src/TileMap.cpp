#include <limits>

<<<<<<< HEAD
#include "TileMapV2.h"
#include "Error.h"
#include "Camera.h"


template<class T>
void TileMapV2<T>::Load(std::string const &file) {
=======
#include "TileMap.h"
#include "Error.h"
#include "Camera.h"


template<class T>
void TileMap<T>::Load(std::string const &file) {
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
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
<<<<<<< HEAD
TileMapV2<T>::TileMapV2(GameObject &associated, std::string const &file, TileSetV2 *tileSet): Component(associated), currentTileSet(0), displayCollisionInfo(false){
=======
TileMap<T>::TileMap(GameObject &associated, std::string const &file, TileSetV2 *tileSet): Component(associated), currentTileSet(0), displayCollisionInfo(false){
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
	tileSets.push_back(tileSet);
}

template<class T>
<<<<<<< HEAD
TileMapV2<T>::TileMapV2(GameObject &associated, std::string &file, std::vector<TileSetV2*> &tileSet): Component(associated), tileSets(tileSet), displayCollisionInfo(false){
=======
TileMap<T>::TileMap(GameObject &associated, std::string &file, std::vector<TileSetV2*> &tileSet): Component(associated), tileSets(tileSet), displayCollisionInfo(false){
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	Load(file);
	parallaxWeight.resize(mapDepth, 1);
}

template<class T>
<<<<<<< HEAD
void TileMapV2<T>::EarlyUpdate(float dt){}

template<class T>
void TileMapV2<T>::Update(float dt){}

template<class T>
void TileMapV2<T>::LateUpdate(float dt){}

template<class T>
void TileMapV2<T>::Render(void) const {
=======
void TileMap<T>::EarlyUpdate(float dt){}

template<class T>
void TileMap<T>::Update(float dt){}

template<class T>
void TileMap<T>::LateUpdate(float dt){}

template<class T>
void TileMap<T>::Render(void) const {
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	for(int count = 0; count < mapDepth; count++) {
		if(COLLISION_LAYER == count && !displayCollisionInfo) {
			continue;
		}
		RenderLayer(count);
	}
}

template<class T>
<<<<<<< HEAD
void TileMapV2<T>::RenderLayer(int layer) {
=======
void TileMap<T>::RenderLayer(int layer) {
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			int index = At(x, y, layer);
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
<<<<<<< HEAD
Vec2 TileMapV2<T>::CalculateParallaxScrolling(Vec2 num, Rect& pos, float weigth) const {
=======
Vec2 TileMap<T>::CalculateParallaxScrolling(Vec2 num, Rect& pos, float weigth) const {
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return num-(pos+Camera::pos)*(weigth+1);
	return num-(pos+Camera::pos)*((weigth+1)/(double)mapDepth);
	return num*(1.0+(double)weigth/(double)mapDepth);
	return num*(1.0-(double)weigth/(double)mapDepth);
}

template<class T>
<<<<<<< HEAD
bool TileMapV2<T>::Is(ComponentType type) const{
=======
bool TileMap<T>::Is(ComponentType type) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return type == ComponentType::TILEMAP;
}

//mudanaça: o método abaixo pode lançar exceção
template<class T>
<<<<<<< HEAD
T& TileMapV2<T>::At(int x, int y, int z) const{
=======
T& TileMap<T>::At(int x, int y, int z) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	int index = z*mapWidth*mapHeight + y*mapWidth + x;
	return (tileMatrix.at(index) );
}

template<class T>
<<<<<<< HEAD
T& TileMapV2<T>::AtLayer(int index2D, int layer) const{
=======
T& TileMap<T>::AtLayer(int index2D, int layer) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return tileMatrix.at(index2D + layer * mapWidth * mapHeight);
}

template<class T>
<<<<<<< HEAD
int TileMapV2<T>::GetWidth(void) const{
=======
int TileMap<T>::GetWidth(void) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return mapWidth;
}

template<class T>
<<<<<<< HEAD
int TileMapV2<T>::GetHeight(void) const{
=======
int TileMap<T>::GetHeight(void) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return mapHeight;
}

template<class T>
<<<<<<< HEAD
int TileMapV2<T>::GetDepth(void) const{
=======
int TileMap<T>::GetDepth(void) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	return mapDepth;
}

template<class T>
<<<<<<< HEAD
int TileMapV2<T>::GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer) const{
=======
int TileMap<T>::GetCoordTilePos(Vec2 const &coordPos, bool affecteedByZoom, int layer) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
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
<<<<<<< HEAD
//void TileMapV2<T>::Parallax(bool parallax);

template<class T>
void TileMapV2<T>::SetParallaxLayerIntensity(int layer, float intensity){
=======
//void TileMap<T>::Parallax(bool parallax);

template<class T>
void TileMap<T>::SetParallaxLayerIntensity(int layer, float intensity){
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	parallaxWeight[layer]= intensity;
}

template<class T>
<<<<<<< HEAD
T* TileMapV2<T>::FindNearest(Vec2 origin, Finder<T> finder, float range) const{
=======
T* TileMap<T>::FindNearest(Vec2 origin, Finder<T> finder, float range) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
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
<<<<<<< HEAD
std::vector<T*>* TileMapV2<T>::FindNearests(Vec2 origin, Finder<T> finder, float range) const{
=======
std::vector<T*>* TileMap<T>::FindNearests(Vec2 origin, Finder<T> finder, float range) const{
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
	std::vector<T*> *chosen= new std::vector<T*>();
	for(int i=0; i < tileMatrix.size();i++){
		if(finder(tileMatrix[i]) < range){
			chosen->push_back( &(tileMatrix[i]) );
		}
	}
	return chosen;
}

<<<<<<< HEAD
void TileMap::ObserveMapChanges(TileMapObserver *obs){
        observers.emplace_back(obs);
}

void TileMap::RemoveObserver(TileMapObserver *obs){
        for(uint i=0; i < observers.size(); i++){
                if(observers[i] == obs){
                        observers.erase(observers.begin()+i);
                        return;
                }
        }
        Error("\tTileMap observer not found!");
}

void TileMap::ReportChanges(int tileChanged){
        for(uint i=0; i< observers.size(); i++){
                observers[i]->NotifyTileMapChanged(tileChanged);
        }
=======
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
>>>>>>> f688457c02dec9d0bdfbfa10736d653ae78d72ca
}


