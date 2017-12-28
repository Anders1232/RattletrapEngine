#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include "Error.h"
#include "Sprite.h"
#include "Vec2.h"
#include "Rect.h"
#include "ComponentType.h"

class TileSet:public Component{
	public:
		TileSet(std::string file, GameObject &associated);
		void Update(float dt);
		void EarlyUpdate(float dt);
		void LateUpdate(float dt);
		void Render() const;
		void Render(int index, Rect pos) const;
		uint GetSize(void) const;
		Vec2 GetTileSize(void) const;
		bool Is(uint type) const;
	private:
		std::vector<Sprite> sprites;
		Vec2 tileSize;
};

/*
[largura]x[altura]
[quantidade de entradas]
[[indice(ordenado)]\t[arquivo]\\t[quantas sprites]\t[tempo entre troca de imagens]\n]*
*/

#endif // TILESET_H
