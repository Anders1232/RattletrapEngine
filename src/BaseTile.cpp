#include "BaseTile.h"

BaseTile::BaseTile(int index): tileSetIndex(index){
}
BaseTile::~BaseTile(){
}

int BaseTile::GetTileSetIndex(void) const{
	return tileSetIndex;
}
