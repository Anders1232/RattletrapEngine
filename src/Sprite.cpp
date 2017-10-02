#include "Sprite.h"

#include "Camera.h"
#include "Error.h"
#include "Game.h"
#include "Resources.h"

//Alterar esses valores altera a parte da textura que será renderizada
#define SPRITE_OPEN_X (0)
#define SPRITE_OPEN_Y (0)
#define HIGHLIGHT 30

#define DEBUG 0
#ifdef DEBUG
    #define DEBUG_PRINT(x) do{ std::cout << x <<  std::endl; }while(0);
#else
    #define DEBUG_PRINT(x)
#endif // DEBUG


//Sprite::Sprite(void): Sprite("", false, 0, 1) {}

Sprite::Sprite(GameObject &associated, std::string file, float frameTime, int frameCount, float angle, bool isCoordOnWorld)
		:associated(associated),
        colorMultiplier(255, 255, 255),
		blendMode(ALPHA_BLEND),
		frameCount(frameCount),
		currentFrame(0),
		timeElapsed(0),
		frameTime(frameTime),
		clipRect(),
		scaleX(1.), scaleY(1.),
		path(file),
		animationLines(1),
		isCoordOnWorld(isCoordOnWorld){
	DEBUG_PRINT("Construtor do sprite");
//	if(file.empty()) {
//		texture = nullptr;
//		REPORT_I_WAS_HERE;
//	} else {
		texture = Resources::GetImage(file);
		if(nullptr == texture) {
			Error(SDL_GetError());
		}
		// Verificar se houve erro na chamada
		if(SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height)) {
			Error(SDL_GetError());
		}
		SetClip(SPRITE_OPEN_X, SPRITE_OPEN_Y, width/frameCount, height);
		SetScreenRect(associated.box.x, associated.box.y, width/frameCount, height/animationLines);

		DEBUG_PRINT("fim do construtor");
//	}
}

Sprite::~Sprite() {}

void Sprite::SetClip(int x, int y, int w, int h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
}

void Sprite::SetScreenRect(int x, int y, int w, int h){
    onScreenRect.x = x;
	onScreenRect.y = y;
	onScreenRect.w = w;
	onScreenRect.h = h;
}

void Sprite::Render() {//const{
	Game& game = Game::GetInstance();

	//{// Se todas as coordenadas do Rect estão fora da tela, não precisa renderizar
		Vec2 screenSize = game.GetWindowDimensions();
		float points[4] = {onScreenRect.x, onScreenRect.y,
                           onScreenRect.x + onScreenRect.w,
                           onScreenRect.y + onScreenRect.h};

		bool isOutOfBounds = true;
		isOutOfBounds = isOutOfBounds && (0 > points[0] || screenSize.x < points[0]);
		isOutOfBounds = isOutOfBounds && (0 > points[1] || screenSize.y < points[1]);
		isOutOfBounds = isOutOfBounds && (0 > points[2] || screenSize.x < points[2]);
		isOutOfBounds = isOutOfBounds && (0 > points[3] || screenSize.y < points[3]);

		if(isOutOfBounds) return;
	//}

	if( -1 == SDL_SetTextureAlphaMod( texture.get(), colorMultiplier.a ) ) {
		CHECK_SDL_ERROR;
	}

	if( -1 == SDL_SetTextureBlendMode( texture.get(), blendMode ) ) {
		CHECK_SDL_ERROR;
	}

	if( -1 == SDL_SetTextureColorMod( texture.get(), colorMultiplier.r, colorMultiplier.g, colorMultiplier.b ) ) {
		CHECK_SDL_ERROR;
	}

	if(SDL_RenderCopyEx(game.GetRenderer(), texture.get(), &clipRect, &onScreenRect, associated.rotation, NULL, SDL_FLIP_NONE) ){//verifica se haverá erro
		// Verifica se haverá erro
		Error(SDL_GetError());
	}
}


int Sprite::GetHeight(void) const {
	return height/animationLines*scaleY;
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

void Sprite::SetAnimationLines(int animationLines){
    this->animationLines = animationLines;
    onScreenRect.h /= animationLines;
    clipRect.h /= animationLines;
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

bool Sprite::Is(ComponentType type) const{
	return ComponentType::SPRITE == type;
}

void Sprite::EarlyUpdate(float dt){}

void Sprite::LateUpdate(float dt){}

void Sprite::SetPosition(int x, int y){
    onScreenRect.x = x;
    onScreenRect.y = y;
}


#ifdef DEBUG
    #undef DEBUG
#endif
