#ifndef TILESETV2_H
#define TILESETV2_H

#include <string>
#include <vector>
#include "Error.h"
#include "Sprite.h"
#include "Vec2.h"
#include "Rect.h"

class TileSetV2:public Component{
	public:
		TileSetV2(std::string file, GameObject &associated);
		void Update(float dt);
		void EarlyUpdate(float dt);
		void LateUpdate(float dt);
		void Render() const;
		void Render(int index, Rect pos) const;
		uint GetSize(void) const;
		Vec2 GetTileSize(void) const;
	private:
		std::vector<Sprite> sprites;
		Vec2 tileSize;
};

/*
[largura]x[altura]
[quantidade de entradas]
[[indice(ordenado)]\t[arquivo]\\t[quantas sprites]\t[tempo entre troca de imagens]\n]*
*/

#endif // TILESETV2_H
