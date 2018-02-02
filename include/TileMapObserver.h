#ifndef TILEMAPOBSERVER_H
#define TILEMAPOBSERVER_H

namespace RattletrapEngine {

	class TileMapObserver{
		public:
			virtual void NotifyTileMapChanged(int tileChanged)=0;
	};

}

#endif // TILEMAPOBSERVER_H
