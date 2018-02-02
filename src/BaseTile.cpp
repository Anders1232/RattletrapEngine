#include "BaseTile.h"

namespace RattletrapEngine {

	BaseTile::BaseTile(int index): tileSetIndex(index){
	}

	int BaseTile::GetTileSetIndex(void) const{
		return tileSetIndex;
	}

}
