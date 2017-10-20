#ifndef BASETILE_H
#define BASETILE_H

#include "Error.h"

class BaseTile{
	public:
		BaseTile(int tileSetIndex);
		int GetTileSetIndex(void) const;
	protected:
		int tileSetIndex;
}


#endif // BASETILE_H
