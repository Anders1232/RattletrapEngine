#include "RectTransform.h"

#include "Camera.h"
#include "Error.h"
#include "Game.h"

RectTransform::RectTransform( GameObject &associated, GameObject *parentGO ) : Component( associated ) {
	this->parentGO = parentGO;
	debugRender = false;
	SetAnchors( {0.5, 0.5}, {0.5, 0.5} );
	SetOffsets( 0, 0, 0, 0 );
	SetCenterPin();
	SetKernelSize( {1., 1.} );
	SetMinScale();
	SetMaxScale();
	SetBehaviorType( BehaviorType::STRETCH );
}

RectTransform::~RectTransform() {}

void RectTransform::EarlyUpdate( float dt ) {}

void RectTransform::Update( float dt ) {
	Rect parentCanvas;
	if( nullptr == parentGO ) {
		parentCanvas = {0., 0., Game::GetInstance().GetWindowDimensions().x, Game::GetInstance().GetWindowDimensions().y};
	} else {
		parentCanvas = parentGO->box;
	}
	boundingBox = ComputeBoundingbox(parentCanvas);
	associated.box = ComputeBox();
	boundingBox.x += parentCanvas.x;
	boundingBox.y += parentCanvas.y;
	associated.box.x += parentCanvas.x;
	associated.box.y += parentCanvas.y;
}

void RectTransform::LateUpdate( float dt ) {}

void RectTransform::Render() const {
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
}

bool RectTransform::Is( ComponentType type ) const {
	return ComponentType::RECT_TRANSFORM == type;
}

void RectTransform::SetAnchors( Vec2 topLeft, Vec2 bottomRight ) {
	static_assert( topLeft.x >= 0. && topLeft.x <= 1.
				&& topLeft.y >= 0. && topLeft.y <= 1.
				, "topLeft must have coordiantes between 0 and 1" );
	
	static_assert( bottomRight.x >= 0. && bottomRight.x <= 1.
				&& bottomRight.y >= 0. && bottomRight.y <= 1.
				, "bottomRight must have coordiantes between 0 and 1" );
	
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
	offsets = Rect( {left, up}, {right, down} );
}

void RectTransform::SetCenterPin( Vec2 center ) {
	static_assert( center.x >= 0. && center.x <= 1.
				&& center.y >= 0. && center.y <= 1.
				, "center must have coordiantes between 0 and 1" );
	centerPin.x = ( center.x < 0 ) ? 0 : ( ( center.x > 1 ) ? 1 : center.x );
	centerPin.y = ( center.y < 0 ) ? 0 : ( ( center.y > 1 ) ? 1 : center.y );
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

Rect RectTransform::GetBoundingBox() {
	return boundingBox;
}

Rect RectTransform::ComputeBoundingBox( Rect ParentCanvas ) {
	Rect boundingBox;
	boundingBox.x = parentCanvas.w*anchors.x + offsets.x;
	boundingBox.y = parentCanvas.h*anchors.y + offsets.y;
	boundingBox.w = parentCanvas.w*anchors.w + offsets.w - boundingBox.x;
	boundingBox.h = parentCanvas.h*anchors.h + offsets.h - boundingBox.y;
	return boundingBox;
}

Rect RectTransform::ComputeBox( Rect boundingBox ) {
	if( -1 == boundingBox.x && -1 == boundingBox.y && -1 == boundingBox.w && -1 == boundingBox.h ) {
		boundingBox = this->boundingBox;
	}
	Rect box;
	box.w = kernelSize.x;
	box.h = kernelSize.y;

	Vec2 multiplier;
	float Mx = boundingBox.w/box.w;
	float My = boundingBox.h/box.h;

	Mx = ( Mx > maxScale.x ? maxScale.x : ( Mx < minScale.x ? minScale.x : Mx ) );
	My = ( My > maxScale.y ? maxScale.y : ( My < minScale.y ? minScale.y : My ) );

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
	
	box.w = multiplier.x*(box.w);
	box.h = multiplier.y*(box.h);
	box.x = boundingBox.x + (boundingBox.w - box.w)*centerPin.x;
	box.y = boundingBox.y + (boundingBox.h - box.h)*centerPin.y;

	return box;
}