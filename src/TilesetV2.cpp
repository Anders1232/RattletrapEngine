#include <stdio.h>
#include "TilesetV2.h"
#include "Error.h"

#define FORBIDDEN_CHAR ">"
#define MAX_SPRITE_FILENAME_SIZE 100

TileSetV2::TileSetV2(std::string fileName, GameObject &associated){
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

void TileSetV2::Update(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].Update(dt);
	}
}

void TileSetV2::EarlyUpdate(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].EarlyUpdate(dt);
	}
}

void TileSetV2::LateUpdate(float dt){
	for(uint i=0; i< sprites.size(); i++){
		sprites[i].LateUpdate(dt);
	}
}

void TileSetV2::Render() const{
	printf("[WARNING] TileSet::Render(void) should not be called.\n");
}

void TileSetV2::Render(int index, Rect pos) const{
	sprites[index].Render(pos);
}

uint TileSetV2::GetSize(void) const{
	return sprites.size();
}

Vec2 TileSetV2::GetTileSize(void) const{
	return tileSize;
}



