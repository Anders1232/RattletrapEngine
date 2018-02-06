#include "Sprite.h"

#include "Camera.h"
#include "Game.h"
#include "Resources.h"

//Alterar esses valores altera a parte da textura que será renderizada
#define SPRITE_OPEN_X (0)
#define SPRITE_OPEN_Y (0)
#define HIGHLIGHT 30

#include "Error.h"
Sprite::Sprite(GameObject &associated, std::string file, bool highlighted, float frameTime, int frameCount, float angle, int animationLines)
		:Component(associated),
		associated(associated),
		blendMode(ALPHA_BLEND),
		frameCount(frameCount),
		currentFrame(0),
		timeElapsed(0),
		frameTime(frameTime),
		clipRect(),
		scaleX(1.),
		scaleY(1.),
		path(file),
		animationLines(1),
		highlightable(highlighted),
		autoAssociate(true),
		debug(false)
{
	DEBUG_CONSTRUCTOR("inicio");
	if(highlightable) {
		colorMultiplier = Color(255-HIGHLIGHT, 255-HIGHLIGHT, 255-HIGHLIGHT);
	}

	texture = Resources::GetImage(file);
	if(nullptr == texture) {
		Error(SDL_GetError());
	}
	// Verificar se houve erro na chamada
	if(SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height)) {
		Error(SDL_GetError());
	}
	SetClip(SPRITE_OPEN_X, SPRITE_OPEN_Y, width/frameCount, height/animationLines);
	SetScreenRect(associated.box.x, associated.box.y, width/frameCount, height/animationLines);
	associated.box.w = width/frameCount;
	associated.box.h = height/animationLines;
	DEBUG_CONSTRUCTOR("   frameCount: " << frameCount);
	DEBUG_CONSTRUCTOR("   frameTime: " << frameTime);
	DEBUG_CONSTRUCTOR("   clipRect:" <<   clipRect.x << ", " <<
								  clipRect.y << ", " <<
								  clipRect.w << ", " <<
								  clipRect.h << "::" );
	DEBUG_CONSTRUCTOR("   onScreen:" <<   onScreenRect.x << ", "
							<< onScreenRect.y << ", "
							<< onScreenRect.w << ", "
							<< onScreenRect.h << "::" );

	DEBUG_CONSTRUCTOR("fim");
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

void Sprite::Render() const{
	DEBUG_RENDER("inicio");
	DEBUG_RENDER("  Renderizando: " <<  path);
	Game& game = Game::GetInstance();

//{// Se todas as coordenadas do Rect estão fora da tela, não precisa renderizar
	Vec2 screenSize = game.GetWindowDimensions();
	float points[4] = {(float)onScreenRect.x, (float)onScreenRect.y,
					   (float)onScreenRect.x + onScreenRect.w,
					   (float)onScreenRect.y + onScreenRect.h};

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
	SDL_Rect dst;
	if(autoAssociate){
		dst = associated.box;
	}else{
		dst = onScreenRect;
	}
	dst.w = onScreenRect.w;
	dst.h = onScreenRect.h;
	if(highlightable && InputManager::GetInstance().GetMousePos().IsInRect(dst)){//onScreenRect)){
		Color colorHighlighted(	(colorMultiplier.r + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.r + HIGHLIGHT),
								(colorMultiplier.g + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.g + HIGHLIGHT),
								(colorMultiplier.b + HIGHLIGHT) > 255 ? 255 : (colorMultiplier.b + HIGHLIGHT) );
		if ( -1 == SDL_SetTextureColorMod( texture.get(), colorHighlighted.r, colorHighlighted.g, colorHighlighted.b) ) {
			CHECK_SDL_ERROR;
		}
	}
	DEBUG_RENDER("   clipRect:" <<   clipRect.x << ", " <<
								  clipRect.y << ", " <<
								  clipRect.w << ", " <<
								  clipRect.h << "::" );
	DEBUG_RENDER("   dst:" <<   dst.x << ", "
							<< dst.y << ", "
							<< dst.w << ", "
							<< dst.h << "::" );
	if(SDL_RenderCopyEx(game.GetRenderer(), texture.get(), &clipRect, &dst, associated.rotation, NULL, SDL_FLIP_NONE) ){//verifica se haverá erro
		// Verifica se haverá erro
		Error(SDL_GetError());
	}
	if(DEBUG){
        SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 255, 100);
        SDL_RenderDrawRect(game.GetRenderer(), &dst);

        SDL_SetRenderDrawColor(game.GetRenderer(), 255, 0, 0, 100);
        SDL_RenderDrawRect(game.GetRenderer(), &dst);
	}
	DEBUG_RENDER("fim");
}


int Sprite::GetHeight(void) const {
	return height/(animationLines*scaleY);
}

int Sprite::GetWidth(void) const {
	return width/(frameCount*scaleX);
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
	associated.box.h = clipRect.h /= animationLines;
}

void Sprite::SetAnimationLine(int animationLine){
	SetClip(0, animationLine*(height/animationLines), width/frameCount, height/animationLines);
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

bool Sprite::Is(uint type) const{
	return ComponentType::SPRITE == type;
}

void Sprite::EarlyUpdate(float dt){}

void Sprite::LateUpdate(float dt){}

void Sprite::SetPosition(int x, int y){
	autoAssociate = false;
	onScreenRect.x = x;
	onScreenRect.y = y;
}

void Sprite::SetPosition(Vec2 pos){
	autoAssociate = false;
	onScreenRect.x = pos.x;
	onScreenRect.y = pos.y;
}

int Sprite::GetScreenX(){
	return onScreenRect.x;
}

int Sprite::GetScreenY(){
	return onScreenRect.y;
}

SDL_Rect Sprite::GetScreenRect(){
	return onScreenRect;
}

#include "Error_footer.h"
