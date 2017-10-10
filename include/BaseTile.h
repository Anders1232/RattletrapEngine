#ifndef BASETILE_H
#define BASETILE_H

#include "Error.h"

class BaseTile{
	public:
		BaseTile(uint tileSetIndex);
		uint GetTileSetIndex(void) const;
	private:
		uint tileSetIndex;
		
}

#endif // BASETILE_H
