#include <stdio.h>
#include "Tileset.h"
#include "Error.h"

#define FORBIDDEN_CHAR ">"
#define MAX_SPRITE_FILENAME_SIZE 100

TileSet::TileSet(std::string fileName, GameObject &associated):Component(associated){
	FILE *file= fopen(fileName.c_str(), "r");
	
	ASSERT2(file != NULL, "Could not open(or find) file "<< fileName);
	uint vecSize;
	fscanf(file, " %fx%f %u", &(tileSize.x), &(tileSize.y), &vecSize);
	sprites.reserve(vecSize);
	char spriteName[MAX_SPRITE_FILENAME_SIZE];
	int numFrames;
	float durationPerFrame;
	for(; vecSize >0; vecSize--){
		ASSERT(3 == fscanf(file, " %[^" FORBIDDEN_CHAR "]" FORBIDDEN_CHAR " %d %f", spriteName, &numFrames, &durationPerFrame) );
		sprites.emplace_back(std::string(spriteName), associated, durationPerFrame, numFrames);
	}
}

void TileSet::Update(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].Update(dt);
	}
}

void TileSet::EarlyUpdate(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].EarlyUpdate(dt);
	}
}

void TileSet::LateUpdate(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].LateUpdate(dt);
	}
}

void TileSet::Render() const{
	printf("[WARNING] TileSet::Render(void) should not be called.\n");
}

void TileSet::Render(int index, Rect pos) const{
	sprites[index].Render(pos);
}

uint TileSet::GetSize(void) const{
	return sprites.size();
}

Vec2 TileSet::GetTileSize(void) const{
	return tileSize;
}



