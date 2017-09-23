#ifndef RECT_TRANSFORM_H
#define RECT_TRANSFORM_H

#include <cfloat>

#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"
#include "Rect.h"
#include "Vec2.h"

class RectTransform : public Component {
  public:
	enum BehaviorType {
		FIT,
		FILL,
		STRETCH
	};

	RectTransform( GameObject &associated, GameObject *parentGO );
	~RectTransform();

	void EarlyUpdate( float dt );
	void Update( float dt );
	void LateUpdate( float dt );
	void Render() const;
	bool Is( ComponentType type ) const;

	bool debugRender;

	void SetAnchors( Vec2 topLeft, Vec2 bottomRight );
	void SetOffsets( float up, float right, float down, float left );
	void SetCenterPin( Vec2 center = {0.5, 0.5} );
	void SetKernelSize( Vec2 kernelSize );
	void SetMinScale( Vec2 minScale = {-FLT_MAX, -FLT_MAX} );
	void SetMaxScale( Vec2 maxScale = {FLT_MAX, FLT_MAX} );
	void SetBehaviorType( BehaviorType behavior );

  protected:
	Rect ComputeBoundingBox( Rect parentCanvas );
	Rect ComputeBox( Rect boundingBox = {-1., -1., -1., -1.} );

  private:
	GameObject *parentGO;
	Rect boundingBox;
	Rect anchors;
	Rect offsets;
	Vec2 centerPin;
	Vec2 kernelSize;
	Vec2 minScale;
	Vec2 maxScale;
	BehaviorType behavior;
};

#endif // RECT_TRANSFORM_H
