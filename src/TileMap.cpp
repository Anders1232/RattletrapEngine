#include "TileMap.h"

#include <algorithm>
#include <cstdio>
#include <exception>
#include <iostream>

#include "Camera.h"
#include "Error.h"
#include "InputManager.h"

#define PAREDE 0

TileMap::TileMap(string file, TileSet *tileSet)
		: tileSet(tileSet)
		, gameObjectMatrix()
		, displayCollisionInfo(false) {
	Load(file, tileMatrix, true);
	gameObjectMatrix.resize(GetWidth()*GetHeight()*GetDepth());
	for(unsigned int count = 0; count < gameObjectMatrix.size(); count++) {
		gameObjectMatrix[count] = nullptr;
	}
}

void TileMap::Load(string file, std::vector<int> &target, bool setOfficialSize) {
	FILE *arq = fopen(file.c_str(), "r");
	ASSERT(NULL != arq);
	int mWidth, mHeight, mDepth;
	fscanf(arq, "%d,%d,%d,", &mWidth, &mHeight, &mDepth);
	if(setOfficialSize){
		mapWidth = mWidth;
		mapHeight = mHeight;
		mapDepth = mDepth;
	}

	int numbersToRead = mWidth*mHeight*mDepth;
	target.resize(numbersToRead);// Assim ele não desperdiça memória nem muda de tamanho no for abaixo
	int aux;
	for(int count = 0; count < numbersToRead; count++) {
		fscanf(arq, " %d,", &aux);
		target[count] = aux;
	}
}
 
void TileMap::SetTileSet(TileSet *tileSet) {
	ASSERT(this->tileSet->GetTileHeight() <= tileSet->GetTileHeight());
	ASSERT(this->tileSet->GetTileWidth() <= tileSet->GetTileWidth());
	this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z) const {
	int index = z*mapWidth*mapHeight + y*mapWidth + x;
	try {
		return ( (int&)tileMatrix.at(index) );
	}
	catch(...) {
		static const int m1=-1;
		return (int&)m1;
	}
}
 
void TileMap::Render(Vec2 pos, bool parallax, Vec2 mouse) const {
	for(int count = 0; count < mapDepth; count++) {
		if(COLLISION_LAYER == count && !displayCollisionInfo) {
			continue;
		}
		RenderLayer(count, pos, parallax, mouse);
	}
}
 
void TileMap::RenderLayer(int layer, Vec2 pos, bool parallax, Vec2 mouse) const {
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			int index = At(x, y, layer);
			if (0 <= index) {
				Vec2 destination;
				if (parallax) {
					Vec2 tilePos(x*tileSet->GetTileWidth(), y*tileSet->GetTileHeight());
					destination = CalculateParallaxScrolling(tilePos, pos, layer);
				}
				else {
					destination = pos + Vec2(x*tileSet->GetTileWidth(), y*tileSet->GetTileHeight());
				}
				Rect tile(destination.x, destination.y, tileSet->GetTileWidth(), tileSet->GetTileHeight());
				tileSet->Render(index, destination, mouse.IsInRect(tile));
			}
		}
	}
}

Vec2 TileMap::CalculateParallaxScrolling(Vec2 num, Vec2 pos, int layer) const {
	return num-(pos+Camera::pos)*(layer+1);
	return num-(pos+Camera::pos)*((layer+1)/(double)mapDepth);
	return num*(1.0+(double)layer/(double)mapDepth);
	return num*(1.0-(double)layer/(double)mapDepth);
}

int TileMap::GetWidth(void) const {
	return mapWidth;
}

int TileMap::GetHeight(void) const {
	return mapHeight;
}

int TileMap::GetDepth(void) const {
	return mapDepth;
}

int TileMap::GetCoordTilePos(Vec2 const &mousePos, bool affecteedByZoom, int layer) const {
	Vec2 position = mousePos;
	int x, xDir = mapWidth-1, xEsq = 0;
	Vec2 tileSize = CalculateParallaxScrolling( Vec2(tileSet->GetTileWidth(), tileSet->GetTileHeight()), Vec2(0, 0), layer);
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

bool TileMap::InsertGO(GameObject* obj, bool checkCollision) {
	Vec2 mousePos = Camera::ScreenToWorld(InputManager::GetInstance().GetMousePos());
	int position = GetCoordTilePos(mousePos, false, 0);
	REPORT_DEBUG("\t position = " << position << "\t of " << mapHeight*mapWidth << " tiles.");
	if(0 > position) {
		std::cout << WHERE << "[ERROR] Tried to put the gameObject on an invalid tileMap position." << END_LINE;
		obj->RequestDelete();
		return false;
	}
	if(checkCollision){
		if(-1 == AtLayer(position, COLLISION_LAYER)) {
			REPORT_DEBUG("\tInserting the gameObject at position " << position);
			gameObjectMatrix[position] = obj;
			tileMatrix[position+(COLLISION_LAYER*mapWidth*mapHeight)] = PAREDE;
			int line = position / GetWidth();
			int column = position % GetWidth();
			obj->box.x = column*tileSet->GetTileWidth();
			obj->box.y = line*tileSet->GetTileHeight();
			//TODO: aqui ajudar a box para ficar exatamente no tileMap
			ReportChanges(position);
		}
		else if(0 > AtLayer(position, COLLISION_LAYER)) {
			REPORT_DEBUG("\ttentado inserir objeto em posição inválida, pois nela está" << tileMatrix[position+(COLLISION_LAYER * mapWidth*mapHeight)]);
			obj->RequestDelete();
			return false;
		}
		else {
			REPORT_DEBUG("\ttentado inserir objeto em posição já ocupada!");
			obj->RequestDelete();
			return false;
		}
	}
	else{
		int tilePos= GetCoordTilePos(obj->box.Center(), false, 0);
		REPORT_DEBUG("\tInserting the gameObject at position " << tilePos);
		gameObjectMatrix[tilePos] = obj;
		int line = tilePos / GetWidth();
		int column = tilePos % GetWidth();
		obj->box.x = column*tileSet->GetTileWidth();
		obj->box.y = line*tileSet->GetTileHeight();
		//TODO: aqui ajudar a box para ficar exatamente no tileMap
	}
	return true;
}

bool TileMap::InsertGO(GameObject* obj,Vec2 initialPos) {
	Vec2 mousePos = Camera::ScreenToWorld(InputManager::GetInstance().GetMousePos());
	int position = GetCoordTilePos(mousePos, false, 0);
	REPORT_DEBUG("\t position = " << position << "\t of " << mapHeight*mapWidth << " tiles.");
	if(0 > position) {
		std::cout << WHERE << "[ERROR] Tried to put the gameObject on an invalid tileMap position." << END_LINE;
		//obj->box = initialPos;
		return false;
	}
	int initialTile = GetCoordTilePos(initialPos, false, 0);
	if(-1 == AtLayer(position, COLLISION_LAYER)) {
		REPORT_DEBUG("\tInserting the gameObject at position " << position);
		gameObjectMatrix[position] = obj;
		tileMatrix[position+(COLLISION_LAYER*mapWidth*mapHeight)] = PAREDE;
		int line = position / GetWidth();
		int column = position % GetWidth();
		obj->box.x = column*tileSet->GetTileWidth();
		obj->box.y = line*tileSet->GetTileHeight();
		RemoveGO(initialTile);
		//TODO: aqui ajudar a box para ficar exatamente no tileMap
	}
	else {
		int line = initialTile / GetWidth();
		int column = initialTile % GetWidth();
		obj->box.x = column*tileSet->GetTileWidth();
		obj->box.y = line*tileSet->GetTileHeight();
		return false;
	}
	return true;
}

void TileMap::RemoveGO(int position){
	REPORT_I_WAS_HERE;
	if(0 == AtLayer(position, COLLISION_LAYER)){
		if(nullptr == gameObjectMatrix[position]){
			REPORT_DEBUG("\t Trying to remove a gameObject where there is none");
		}
		REPORT_DEBUG("\tRemoving the gameObject at position " << position);
		gameObjectMatrix[position] = nullptr;
		tileMatrix[position + (COLLISION_LAYER * mapWidth * mapHeight)] = -1;
		ReportChanges(position);
	}
	else{
		REPORT_DEBUG("\ttentado remover objeto de posicao inválida" << std::endl);
	}
}

void TileMap::RemoveGO(void){
	Vec2 mousePos = Camera::ScreenToWorld(InputManager::GetInstance().GetMousePos());
	int position = GetCoordTilePos(mousePos, false, COLLISION_LAYER);
	RemoveGO(position);
}

void TileMap::ShowCollisionInfo(bool show) {
	displayCollisionInfo = show;
}

bool TileMap::IsShowingCollisionInfo() {
	return displayCollisionInfo;
}

int& TileMap::AtLayer(int index2D, int layer) const {
	return (int&)tileMatrix.at(index2D + layer * mapWidth * mapHeight);
}

vector<vector<int>>* TileMap::GetTileGroups(int tileType) const {
	vector<vector<int>> *tilePoints = new vector<vector<int>>();
	vector<int> foundTilePoints;
	uint countLimit = GetWidth()*GetHeight();
	int base = countLimit*COLLISION_LAYER;
	REPORT_I_WAS_HERE;

	for(uint i = 0; i < countLimit; i++) {
		int positionToBeSearch = base+i;
		if(tileType == tileMatrix[positionToBeSearch]) {
			foundTilePoints.push_back(positionToBeSearch%(GetWidth()*GetHeight()));
		}
	}
	REPORT_I_WAS_HERE;
	// Agora que tenho todos os spawn points vou agrupá-los de acordo com suas adjacências.
	if(foundTilePoints.empty()) {
		Error("Não foi encontrado spawn points!");
	}

	tilePoints->emplace_back();
	(*tilePoints)[0].push_back(foundTilePoints[0]);
	foundTilePoints.erase(foundTilePoints.begin());
	while(!foundTilePoints.empty()) {
		bool neighborFound = false;
		for(unsigned int i = 0; i < tilePoints->size(); i++) {
			vector<int> &vec = (*tilePoints)[i];
			if(
					(std::find(vec.begin(), vec.end(), foundTilePoints[0]+1) != vec.end() )//posição à direita
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]-1) != vec.end() )//posição à esquerda
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]+GetWidth()) != vec.end() )// posição em cima
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]-GetWidth()) != vec.end() )//posição em baixo
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]-GetWidth()-1) != vec.end() )//diagonal supeior esquerda
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]-GetWidth()+1) != vec.end() )//diagonal supeior direita
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]+GetWidth()-1) != vec.end() )//diagonal inferior esquerda
					|| (std::find(vec.begin(), vec.end(), foundTilePoints[0]+GetWidth()+1) != vec.end() )//diagonal inferior direita
			){
				vec.push_back(foundTilePoints[0]);
				foundTilePoints.erase(foundTilePoints.begin());
				neighborFound = true;
				break;
			}
		}
		if(!neighborFound) {
			tilePoints->emplace_back();
			(*tilePoints)[tilePoints->size()-1].push_back(foundTilePoints[0]);
			foundTilePoints.erase(foundTilePoints.begin());
		}
		REPORT_I_WAS_HERE;
	}
#if DEBUG
	std::cout << WHERE << "\tNumero de tile groups achados: " << (*tilePoints).size() << END_LINE;
	for(uint i = 0; i < (*tilePoints).size(); i++) {
		std::cout << WHERE << "\tTile groups " << i <<" tem tamanho " << (*tilePoints)[i].size() << END_LINE;
		std::cout << WHERE << "\tTileWidth= " << GetWidth() << END_LINE;
		for(uint i2 = 0; i2 < (*tilePoints)[i].size(); i2++) {
			std::cout << WHERE << "\tSpawn point: " << (*tilePoints)[i][i2] << END_LINE;
		}
	}
#endif
	return tilePoints;
}

Vec2 TileMap::GetTileSize(void) const{
	return Vec2(tileSet->GetTileWidth(), tileSet->GetTileHeight());
}

bool TileMap::Traversable(int index) const{
	int tile = AtLayer(index,COLLISION_LAYER);
	return(TILE_VAZIO == tile || END_POINT == tile || SPAWN_POINT == tile);
}

std::vector<int>* TileMap::GetNeighbors(int tileIndex) const{
	//[i-1][j-1] -> soma-se -(width+1) da posicao atual
	//[i-1][j] -> soma-se -width da posicao atual
	//[i-1][j+1] -> soma-se (1 - width) da posicao atual
	//[i][j-1] -> soma-se (-1) da posicao atual
	//[i][j + 1] -> soma-se 1 da posicao atual
	//[i+1][j-1] -> soma-se width - 1 da posicao atual
	//[i+1][j] -> soma-se width da posicao atual
	//[i+1][j+1] -> soma-se (width+1) da posicao atual

	std::vector<int> *neighbors= new std::vector<int>();
	//Se não está no limite lateral direito nem esquerdo
	if(0 != (tileIndex % mapWidth) && (tileIndex % mapWidth != mapWidth - 1)){
		//Se não está no limite superior
		if(0 <=  tileIndex - mapWidth - 1){
			neighbors->push_back(tileIndex - mapWidth - 1);
			neighbors->push_back(tileIndex - mapWidth);
			neighbors->push_back(tileIndex - mapWidth + 1);
			neighbors->push_back(tileIndex - 1);
		}
		else{
			neighbors->push_back(tileIndex - 1);
		}
		//Se não está no limite inferior
		if(tileIndex + mapWidth + 1 < (int)tileMatrix.size()){
			neighbors->push_back(tileIndex + 1);
			neighbors->push_back(tileIndex + mapWidth-1);
			neighbors->push_back(tileIndex + mapWidth);
			neighbors->push_back(tileIndex + mapWidth + 1);
		}
		else{
			neighbors->push_back(tileIndex + 1);
		}
	}
	//Se está no limite direito
	else if(0 != (tileIndex % mapWidth)){
		//Se não está no limite superior
		if(0 <=  tileIndex - mapWidth){
			neighbors->push_back(tileIndex - mapWidth - 1);
			neighbors->push_back(tileIndex - mapWidth);
			neighbors->push_back(tileIndex - 1);
		}
		else{
			neighbors->push_back(tileIndex - 1);
		}
		//Se não está no limite inferior
		if(tileIndex + mapWidth < (int)tileMatrix.size()){
			neighbors->push_back(tileIndex + mapWidth - 1);
			neighbors->push_back(tileIndex + mapWidth);
		}
	}
	//Se esta no limite esquerdo
	else if(tileIndex % mapWidth != mapWidth - 1){
		//Se não está no limite superior
		if(0 <=  tileIndex - mapWidth - 1){
			neighbors->push_back(tileIndex - mapWidth);
			neighbors->push_back(tileIndex - mapWidth + 1);
			neighbors->push_back(tileIndex + 1);
		}
		else{
			neighbors->push_back(tileIndex + 1);
		}
		//Se não está no limite inferior
		if(tileIndex + mapWidth < (int)tileMatrix.size()){
			neighbors->push_back(tileIndex + mapWidth);
			neighbors->push_back(tileIndex + mapWidth + 1);
		}
	}
	return(neighbors);
}

std::list<int> *TileMap::AStar(int originTile,int destTile,AStarHeuristic* heuristic,std::map<int, double> weightMap){
	typedef std::priority_queue<std::pair<double,int>,std::vector<std::pair<double,int> >,LessThanByHeuristic> mypqType;
	std::list<int> *path= new std::list<int>();//caminho final a ser retornado
	double weight;//Auxiliar para peso associado a cada tile
	std::pair<double,int> current;//Auxiliar para tile atual que está sendo processado
	//lista  de caminhos <destino,<anterior,custo>>);
	std::list<std::pair<int ,std::pair<int,double> > > paths;
	//Define functor a ser usado no ordenamento do vetor de tiles
	mypqType processHeap(LessThanByHeuristic(destTile,heuristic,mapWidth,true));
	//inicia o vetor de distâncias e de visited
	std::vector<double> dist(tileMatrix.size(),std::numeric_limits<double>::max());
	std::vector<bool> visited(tileMatrix.size(),false);
	//a distância de orig para orig é 0
	dist[originTile] = 0.0;
	//Inicializa listas de processamento e de caminhos
	processHeap.emplace(std::make_pair(dist.at(originTile),originTile));
	paths.push_back(std::make_pair(originTile,std::make_pair(originTile,dist.at(originTile))));
	//Loop de processamento do Djkistra
	while(!processHeap.empty()){
		//Seleciona o nó com menor custo fazendo assim uma busca guiada(A*)
		current = processHeap.top();
		if(current.second == destTile){
			break;
		}
		processHeap.pop();//remove topo da heap
		//Obtém todos os vizinhos de "current"
		std::vector<int> *neighbors = GetNeighbors(current.second);
		//Se chegou ao destino sai do loop
		// percorre os vértices "v" adjacentes de "current"
		for(unsigned int j = 0 ; j < neighbors->size();j ++){
			int neighborInAnalisis= neighbors->at(j);
			//Se o vértice já foi visitado ou não é atingível passa-se para o proximo
			if (visited.at(neighborInAnalisis) || !Traversable(neighborInAnalisis)){
				continue;
			}
			try{
				weight = weightMap.at(AtLayer(neighborInAnalisis,WALKABLE_LAYER));
			}
			catch(const std::out_of_range& oor){
				continue;
			}
			//Verifica se o custo do caminho a partir de current é menor que o registrado no vizinho
			if((dist.at(neighborInAnalisis) > dist.at(current.second) + weight)){
				//Caso o vizinho já tenha sido processado em alguma iteração
				if(dist.at(neighborInAnalisis) != std::numeric_limits<double>::max()){
					//Remove o custo e o caminho associado ao vizinho das listas visto que novos serão inseridos
					paths.remove(std::make_pair(neighborInAnalisis,std::make_pair(current.second,weight)));
				}
				//atualiza a distância do vizinho e insere nas listas
				dist.at(neighborInAnalisis) = dist.at(current.second) + weight;
				paths.push_back(std::make_pair(neighborInAnalisis,std::make_pair(current.second,weight)));
				processHeap.emplace(std::make_pair(weight,neighborInAnalisis));
			}
		}
		visited.at(current.second) = true;
		delete neighbors;
	}
	//Deducao do caminho
	std::list<std::pair<int ,std::pair<int,double> > >::iterator it;
	int actual_node;
	if(current.second == destTile){
		actual_node = current.second;
	}
	//Caso o destino não esteja possível procura o caminho com destino mais proximo do destino o qual se quer
	else{
		double closerNode = std::numeric_limits<double>::max();
		for(it = paths.begin(); it != paths.end(); ++ it){
			double auxCloser = (*heuristic)(Vec2(it->first / mapWidth,it->first % mapWidth),Vec2(destTile / mapWidth,destTile % mapWidth));
			if(auxCloser < closerNode){
				actual_node = it->first;
				closerNode = auxCloser;
			}
		}
	}
	while(actual_node != originTile){
		for(it = paths.begin(); it != paths.end(); ++ it){
			if(it->first == actual_node){
				path->push_front(actual_node);
				actual_node = it->second.first;
				break;
			}
		}
	}
	return(path);
}

void TileMap::ShowPath(std::shared_ptr<std::vector<int> > path){
	for(uint i = 0; i <path->size(); ++i){
		tileMatrix[i + (WALKABLE_LAYER*mapWidth*mapHeight)] = END_POINT;
	}
}

GameObject* TileMap::FindNearestGO(Vec2 origin, std::string objectDestType, float range){
	GameObject* closerObj = nullptr;
	double closerObjDistance = std::numeric_limits<double>::max();
	for(unsigned int i = 0; i < gameObjectMatrix.size(); i ++){
		GameObject *gameObjectInAnalisis= gameObjectMatrix[i];
		if(nullptr != gameObjectInAnalisis){
			if(gameObjectInAnalisis->Is(objectDestType)){
				double distance = origin.VecDistance(gameObjectInAnalisis->box.Center()).Magnitude();
				if(distance < closerObjDistance && distance <= range){
					closerObjDistance = distance;
					closerObj = gameObjectInAnalisis;
				}
			}
		}
	}
	return(closerObj);
}

std::vector<GameObject*>* TileMap::FindNearestGOs(Vec2 origin, std::string targetType, float range){
	vector<GameObject*> *objectsInRange= new vector<GameObject*>();
	for(unsigned int i = 0; i < gameObjectMatrix.size(); i ++){
		GameObject *gameObjectInAnalisis= gameObjectMatrix[i];
		if(nullptr != gameObjectInAnalisis){
			if(gameObjectInAnalisis->Is(targetType)){
				double distance = origin.VecDistance(gameObjectInAnalisis->box.Center()).Magnitude();
				if(distance <= range){
					objectsInRange->push_back(gameObjectInAnalisis);
				}
			}
		}
	}
	return(objectsInRange);
}


GameObject* TileMap::GetGO(int index){
	return gameObjectMatrix.at(index);
}

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
}

int64_t TileMap::Have(GameObject *obj){
	for(int64_t i=0; i < (int64_t)gameObjectMatrix.size(); i++){
		if(gameObjectMatrix[i] == obj){
			return i;
		}
	}
	return -1;
}

