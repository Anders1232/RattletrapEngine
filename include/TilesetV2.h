#ifndef TILESETV2_H
#define TILESETV2_H

#include <string>
#include <vector>
#include "Sprite.h"
#include "Vec2.h"
#include "Rect.h"

class TileSetV2:public Component{
	public:
		TileSetV2(string file);
		void Update(float dt);
		void EarlyUpdate(float dt);
		void LateUpdate(float dt);
		void Render();
		void Render(int index, Rect pos);
		int GetSize(void) const;
		Vec2 GetTileSize(void) const;
	private:
		std::vector<Sprite> sprites;
		Vec2 tileSize;
		int tileWidth;/**< Espessura de um tile.*/
		int tileHeight;/**< Altura de um tile.*/
}

/*
[largura]x[altura]
[quantidade de entradas]
[[indice(ordenado)][arquivo][quantas sprites][tempo entre troca de imagens]]*
*/

#endif // TILESETV2_H
