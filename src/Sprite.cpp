#include "Sprite.h"

#include "Camera.h"
#include "Error.h"
#include "Game.h"
#include "Resources.h"

//Alterar esses valores altera a parte da textura que será renderizada
#define SPRITE_OPEN_X (0)
#define SPRITE_OPEN_Y (0)
#define HIGHLIGHT 30

Sprite::Sprite(void): Sprite("", false, 0, 1) {}

Sprite::Sprite(std::string file, bool highlighted, float frameTime, int frameCount)
		: colorMultiplier(255, 255, 255), blendMode(ALPHA_BLEND)
		, frameCount(frameCount)
		, currentFrame(0), timeElapsed(0)
		, frameTime(frameTime), clipRect()
		, scaleX(1.), scaleY(1.)
		, highlightable(highlighted) {
	if(highlightable) {
		colorMultiplier = Color(255-HIGHLIGHT, 255-HIGHLIGHT, 255-HIGHLIGHT);
	}
	REPORT_I_WAS_HERE;
	if(file.empty()) {
		texture = nullptr;
		REPORT_I_WAS_HERE;
	} else {
		Open(file);
	}
	REPORT_I_WAS_HERE;
}

Sprite::~Sprite() {}

void Sprite::Open(std::string file) {
	REPORT_I_WAS_HERE;
	texture = Resources::GetImage(file);
	REPORT_I_WAS_HERE;
	if(nullptr == texture) {
		Error(SDL_GetError());
	}
	REPORT_I_WAS_HERE;
	// Verificar se houve erro na chamada
	if(SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height)) {
		Error(SDL_GetError());
	}
	REPORT_I_WAS_HERE;
	SetClip(SPRITE_OPEN_X, SPRITE_OPEN_Y, width/frameCount, height);
	REPORT_I_WAS_HERE;
}

void Sprite::SetClip(int x, int y, int w, int h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
}

void Sprite::Render(Rect world, float angle, bool isCoordOnWorld) const {
	Game& game = Game::GetInstance();
	
	if(0 >= world.w || 0 >= world.h) {
		world.w = GetWidth();
		world.h = GetHeight();
	}

	if(isCoordOnWorld) {
		world = Camera::WorldToScreen(world);
	}

	{// Se todas as coordenadas do Rect estão fora da tela, não precisa renderizar
		Vec2 screenSize = game.GetWindowDimensions();
		float points[4] = {world.x, world.y, world.x+world.w, world.y+world.h};
		
		bool isOutOfBounds = true;
		isOutOfBounds = isOutOfBounds && (0 > points[0] || screenSize.x < points[0]);
		isOutOfBounds = isOutOfBounds && (0 > points[1] || screenSize.y < points[1]);
		isOutOfBounds = isOutOfBounds && (0 > points[2] || screenSize.x < points[2]);
		isOutOfBounds = isOutOfBounds && (0 > points[3] || screenSize.y < points[3]);

		if(isOutOfBounds) return;
	}

	if( -1 == SDL_SetTextureAlphaMod( texture.get(), colorMultiplier.a ) ) {
		CHECK_SDL_ERROR;
	}

	if( -1 == SDL_SetTextureBlendMode( texture.get(), blendMode ) ) {
		CHECK_SDL_ERROR;
	}

	if( -1 == SDL_SetTextureColorMod( texture.get(), colorMultiplier.r, colorMultiplier.g, colorMultiplier.b ) ) {
		CHECK_SDL_ERROR;
	}

	SDL_Rect dst = world;
	if(highlightable && InputManager::GetInstance().GetMousePos().IsInRect(dst)){
		Color colorHighlighted(	(colorMultiplier.r + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.r + HIGHLIGHT),
								(colorMultiplier.g + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.g + HIGHLIGHT),
								(colorMultiplier.b + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.b + HIGHLIGHT) );
		if ( -1 == SDL_SetTextureColorMod( texture.get(), colorHighlighted.r, colorHighlighted.g, colorHighlighted.b) ) {
			CHECK_SDL_ERROR;
		}
	}
	if(SDL_RenderCopyEx(game.GetRenderer(), texture.get(), &clipRect, &dst, angle, NULL, SDL_FLIP_NONE) ){//verifica se haverá erro
		// Verifica se haverá erro
		Error(SDL_GetError());
	}
}

int Sprite::GetHeight(void) const {
	return height*scaleY;
}

int Sprite::GetWidth(void) const {
	return width/frameCount*scaleX;
}

bool Sprite::IsOpen(void) const {
	return (nullptr != texture);
}

/*void Sprite::Rotate(double angle) {
	this->angle= angle;
}
*/
std::shared_ptr<SDL_Texture> Sprite::GetTexture(void) const {
	return texture;
}

void Sprite::SetScaleX(float scale) {
	scaleX = scale;
}

void Sprite::SetScaleY(float scale) {
	scaleY = scale;
}

void Sprite::SetScale(float scale) {
	scaleX = scale;
	scaleY = scale;
}

void Sprite::ScaleX(float scale) {
	scaleX *= scale;
}

void Sprite::ScaleY(float scale) {
	scaleY *= scale;
}

void Sprite::Scale(float scale) {
	scaleX *= scale;
	scaleY *= scale;
}

void Sprite::Update(float dt) {
	timeElapsed += dt;
	if(timeElapsed > frameTime) {
		timeElapsed -= frameTime;
		currentFrame = (currentFrame+1)%frameCount;
		clipRect.x = currentFrame*(width/frameCount);
	}
}

void Sprite::SetFrame(int frame) {
	currentFrame = frame%frameCount;
	int newXRect = currentFrame*(width/frameCount);
	clipRect.x = newXRect;
}

void Sprite::SetFrameCount(int frameCount) {
	this->frameCount = frameCount;
	clipRect.w = width/frameCount;
}
void Sprite::SetFrameTime(float frameTime) {
	this->frameTime=frameTime;
}
