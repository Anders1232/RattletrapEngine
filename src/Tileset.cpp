#include "Tileset.h"
#include "Error.h"
#include "Game.h"
#include "Camera.h"

#define HIGHLIGHT 30

TileSet::TileSet(int tileWidth, int tileHeight, string file)
		: tileSet(file, true)
		, tileWidth(tileWidth)
		, tileHeight(tileHeight) {

	REPORT_I_WAS_HERE;
	rows = tileSet.GetHeight()/tileHeight;
	columns = tileSet.GetWidth()/tileWidth;
}

void TileSet::Render(unsigned int index, Vec2 pos, bool highlighted) {
	ASSERT2((unsigned int)rows*columns > index, "\tReceived "<<index<<" but tileSet has size " << (unsigned int)rows*columns );
	unsigned int desiredLine, desiredColumn;
	desiredLine = index/columns;
	desiredColumn = index%columns;
	SDL_Rect wantedSubSprite;

	wantedSubSprite.x = desiredColumn*tileWidth;
	wantedSubSprite.y = desiredLine*tileHeight;
	wantedSubSprite.w = tileWidth;
	wantedSubSprite.h = tileHeight;
	Rect destinyRect(pos.x, pos.y, tileWidth, tileHeight);
	SDL_Rect dst = Camera::WorldToScreen(destinyRect);

	Color color = Color(tileSet.colorMultiplier.r, tileSet.colorMultiplier.g, tileSet.colorMultiplier.b);
	if(highlighted){
		color.r = (tileSet.colorMultiplier.r + HIGHLIGHT) > 255 ? 255 : (tileSet.colorMultiplier.r + HIGHLIGHT);
		color.g = (tileSet.colorMultiplier.g + HIGHLIGHT) > 255 ? 255 : (tileSet.colorMultiplier.g + HIGHLIGHT);
		color.b = (tileSet.colorMultiplier.b + HIGHLIGHT) > 255 ? 255 : (tileSet.colorMultiplier.b + HIGHLIGHT);
	}
	if ( -1 == SDL_SetTextureColorMod( tileSet.GetTexture().get(), color.r, color.g, color.b) ) {
		CHECK_SDL_ERROR;
	}
	SDL_RenderCopy(Game::GetInstance().GetRenderer(), tileSet.GetTexture().get(), &wantedSubSprite, &dst);
//	REPORT_I_WAS_HERE;
}

int TileSet::GetTileHeight(void) {
	return tileHeight;
}

int TileSet::GetTileWidth(void) {
	return tileWidth;
}
