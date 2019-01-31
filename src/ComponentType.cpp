#include <stdio.h>

#include "ComponentType.h"
#include "Sprite.h"
#include "TileMap.h"
#include "BaseTile.h"
#include "Button.h"
#include "Grouper.h"
#include "Text.h"
#include "State.h"

void RattletrapEngine::Components::ShowComponentsSize(void)
{
//	int biggestComponent;
//	int smalledComponent;
	printf("%lu\t is the size of RattletrapEngine::GameObject\n", sizeof(RattletrapEngine::GameObject) );
	printf("%lu\t is the size of RattletrapEngine::State\n", sizeof(RattletrapEngine::State) );
	printf("Size of the engine components\n");
	printf("%lu\t is the size of ComponentType::SPRITE\n", sizeof(RattletrapEngine::Sprite) );
	printf("%lu\t is the size of ComponentType::TILEMAP<BaseTile>\n", sizeof(RattletrapEngine::TileMap<BaseTile>) );
	printf("%lu\t is the size of ComponentType::TILESET\n", sizeof(RattletrapEngine::TileSet) );
	printf("%lu\t is the size of ComponentType::RECT_TRANSFORM\n", sizeof(RattletrapEngine::RectTransform) );
	printf("%lu\t is the size of ComponentType::BUTTON\n", sizeof(RattletrapEngine::Button) );
	printf("%lu\t is the size of ComponentType::GROUPER\n", sizeof(RattletrapEngine::Grouper) );
	printf("%lu\t is the size of ComponentType::TEXT\n", sizeof(RattletrapEngine::Text) );
	
}
