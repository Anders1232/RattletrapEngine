#include "RectTransform.h"

#include "Camera.h"
#include "Game.h"

#include "Error.h"
RectTransform::RectTransform( GameObject &associated, GameObject *GOparent ) : Component( associated ) {
	this->GOparent = GOparent;
	debugRender = false;
	SetAnchors( {0.5, 0.5}, {0.5, 0.5} );
	SetOffsets( 0, 0, 0, 0 );
	SetCenterPin();
	SetKernelSize( {1., 1.} );
	SetMinScale();
	SetMaxScale();
	SetBehaviorType( BehaviorType::STRETCH );
	DEBUG_CONSTRUCTOR("associated.box:{" <<
                      associated.box.x << ", " <<
                      associated.box.y << ", " <<
                      associated.box.w << ", " <<
                      associated.box.h << "}");

	DEBUG_CONSTRUCTOR("fim");
}

RectTransform::~RectTransform() {}

void RectTransform::EarlyUpdate( float dt ) {}

void RectTransform::Update( float dt ) {
    DEBUG_UPDATE("inicio");
    DEBUG_UPDATE("associated.box:{" <<
                      associated.box.x << ", " <<
                      associated.box.y << ", " <<
                      associated.box.w << ", " <<
                      associated.box.h << "}");

	Rect parentCanvas;
	if( nullptr == GOparent ) {
		parentCanvas = {0., 0., Game::GetInstance().GetWindowDimensions().x, Game::GetInstance().GetWindowDimensions().y};
	} else {
		parentCanvas = GOparent->box;
	}
	boundingBox = ComputeBoundingBox(parentCanvas);
	associated.box = ComputeBox();
	boundingBox.x += parentCanvas.x;
	boundingBox.y += parentCanvas.y;
	associated.box.x += parentCanvas.x;
	associated.box.y += parentCanvas.y;
	DEBUG_UPDATE("box:{" <<
                      associated.box.x << ", " <<
                      associated.box.y << ", " <<
                      associated.box.w << ", " <<
                      associated.box.h << "}");

	DEBUG_UPDATE("fim");
}

void RectTransform::LateUpdate( float dt ) {}

void RectTransform::Render() const {
    DEBUG_RENDER("inicio");
	if (debugRender) {
		SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, 255); // Anchors em Vermelho
		SDL_Rect anch = Rect( boundingBox.x - offsets.x, boundingBox.y - offsets.y, boundingBox.w - offsets.w + offsets.x, boundingBox.h - offsets.h + offsets.y );
		SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &anch);

		SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 0, 255); // BoundingBox em Amarelo
		SDL_Rect bounding = boundingBox;
		SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &bounding);

		SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 255, 0, 255); // Box em Verde
		SDL_Rect renderBox = associated.box;
		SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &renderBox);
	}
	DEBUG_RENDER("fim");
}

bool RectTransform::Is( ComponentType type ) const {
	return ComponentType::RECT_TRANSFORM == type;
}

void RectTransform::SetAnchors(int v1, int v2, int u1, int u2){
    Vec2 v(v1, v2);
    Vec2 u(u1, u2);
    SetAnchors(v, u);
}

void RectTransform::SetAnchors( Vec2 topLeft, Vec2 bottomRight ) {
	ASSERT2( topLeft.x >= 0. && topLeft.x <= 1.
				&& topLeft.y >= 0. && topLeft.y <= 1.
				, "topLeft must have coordinates between 0 and 1" );
	
	ASSERT2( bottomRight.x >= 0. && bottomRight.x <= 1.
				&& bottomRight.y >= 0. && bottomRight.y <= 1.
				, "bottomRight must have coordinates between 0 and 1" );
	
	if( topLeft.x < 0. ) topLeft.x = 0.;
	if( topLeft.y < 0. ) topLeft.y = 0.;
	if( topLeft.x > 1. ) topLeft.x = 1.;
	if( topLeft.y > 1. ) topLeft.y = 1.;

	if( bottomRight.x < 0. ) bottomRight.x = 0.;
	if( bottomRight.y < 0. ) bottomRight.y = 0.;
	if( bottomRight.x > 1. ) bottomRight.x = 1.;
	if( bottomRight.y > 1. ) bottomRight.y = 1.;

	anchors = Rect( topLeft.x, topLeft.y, bottomRight.x, bottomRight.y );
}

void RectTransform::SetOffsets( float up, float right, float down, float left ) {
	offsets = Rect(left, up, right, down);
}

void RectTransform::SetCenterPin( Vec2 center ) {
	ASSERT2( center.x >= 0. && center.x <= 1.
				&& center.y >= 0. && center.y <= 1.
				, "center must have coordinates between 0 and 1" );
	centerPin.x = ( center.x < 0 ) ? 0 : ( ( center.x > 1 ) ? 1 : center.x );
	centerPin.y = ( center.y < 0 ) ? 0 : ( ( center.y > 1 ) ? 1 : center.y );
}

void RectTransform::SetKernelSize(float w, float h){
    Vec2 v(w,h);
    SetKernelSize(v);
}
void RectTransform::SetKernelSize( Vec2 kernelSize ) {
	this->kernelSize = kernelSize;
}

void RectTransform::SetMinScale( Vec2 minScale ) {
	this->minScale = minScale;
}

void RectTransform::SetMaxScale( Vec2 maxScale ) {
	this->maxScale = maxScale;
}

void RectTransform::SetBehaviorType( BehaviorType behavior ) {
	this->behavior = behavior;
}

Rect RectTransform::GetBoundingBox() const{
	return boundingBox;
}

Rect RectTransform::GetOffsets() const{
    return offsets;
}

Rect RectTransform::GetAnchors() const{
    return anchors;
}

Rect RectTransform::ComputeBoundingBox( Rect parentCanvas ) {
	Rect boundingBox;
	DEBUG_UPDATE("parentCanvas:{" <<
                      parentCanvas.x << ", " <<
                      parentCanvas.y << ", " <<
                      parentCanvas.w << ", " <<
                      parentCanvas.h << "}");

    DEBUG_UPDATE("anchors:{" <<
                      anchors.x << ", " <<
                      anchors.y << ", " <<
                      anchors.w << ", " <<
                      anchors.h << "}");
    DEBUG_UPDATE("offsets:{" <<
                      offsets.x << ", " <<
                      offsets.y << ", " <<
                      offsets.w << ", " <<
                      offsets.h << "}");

	boundingBox.x = parentCanvas.w*anchors.x + offsets.x;
	boundingBox.y = parentCanvas.h*anchors.y + offsets.y;
	boundingBox.w = parentCanvas.w*anchors.w + offsets.w + boundingBox.x;
	boundingBox.h = parentCanvas.h*anchors.h + offsets.h + boundingBox.y;

	DEBUG_UPDATE("boundingBox:{" <<
                      boundingBox.x << ", " <<
                      boundingBox.y << ", " <<
                      boundingBox.w << ", " <<
                      boundingBox.h << "}");

	return boundingBox;
}

Rect RectTransform::ComputeBox( Rect boundingBox ) {
    DEBUG_UPDATE("inicio");
	if( -1 == boundingBox.x && -1 == boundingBox.y && -1 == boundingBox.w && -1 == boundingBox.h ) {
		boundingBox = this->boundingBox;
	}
	Rect box;
	DEBUG_UPDATE("kernelSize: " << kernelSize.x << "x" << kernelSize.y);
	box.w = kernelSize.x;
	box.h = kernelSize.y;
    DEBUG_UPDATE("box (wxh): " << box.w << "x" << box.h);

    DEBUG_UPDATE("boundingbox (x,y,w,h): (" << boundingBox.x << ","
                                                             << boundingBox.y << ","
                                                             << boundingBox.w << ","
                                                             << boundingBox.h << ")");
	Vec2 multiplier;
	float Mx = boundingBox.w/box.w;
	float My = boundingBox.h/box.h;
	DEBUG_UPDATE("Mx x MY: " << Mx << "x" << My);

	Mx = ( Mx > maxScale.x ? maxScale.x : ( Mx < minScale.x ? minScale.x : Mx ) );
	My = ( My > maxScale.y ? maxScale.y : ( My < minScale.y ? minScale.y : My ) );
    DEBUG_UPDATE("Mx x MY: " << Mx << "x" << My);

	if( BehaviorType::STRETCH == behavior ) {
		multiplier = Vec2(Mx, My);
	} else if( BehaviorType::FIT == behavior ) {
		float min = Mx < My ? Mx : My;
		multiplier = Vec2(min, min);
	} else if( BehaviorType::FILL == behavior ) {
		float max = Mx > My ? Mx : My;
		multiplier = Vec2(max, max);
	} else {
		Error("Tipo de comportamento de UI indefinido.");
	}
    DEBUG_UPDATE("multiplier (XxY): " << multiplier.x << "x" << multiplier.y);
	box.w = multiplier.x*(box.w);
	box.h = multiplier.y*(box.h);
	box.x = boundingBox.x + (boundingBox.w - box.w)*centerPin.x;
	box.y = boundingBox.y + (boundingBox.h - box.h)*centerPin.y;
    DEBUG_UPDATE("box (wxh): " << box.w << "x" << box.h);
    DEBUG_UPDATE("fim");
	return box;
}

#include "Error_footer.h"
