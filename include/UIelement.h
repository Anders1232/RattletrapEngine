#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Sprite.h"
#include "Rect.h"
#include "Vec2.h"

#include <string>

class UIelement {
	public:
		enum BehaviorType {
				FIT,
				FILL,
				STRETCH
		};
		void SetCenter(Vec2 center);
		void SetAnchors(Vec2 min, Vec2 max);
		void SetOffsets(Vec2 min, Vec2 max);
		void SetBehavior(BehaviorType type);
		Rect GetAnchors(void) const;
		Rect GetOffsets(void) const;
		virtual void Update(float dt, Rect parentCanvas);
		virtual void Render(bool debugRender = false) const;
		virtual bool Is(std::string UItype) const;
		Rect GetBoundingBox(void) const;
		Rect ComputeBoundingbox(Rect parentCanvas);
		Rect ComputeBox(Rect boundingBox = {-1.,-1.,-1.,-1.});
		operator Rect() const;
	protected:
		UIelement(BehaviorType behavior = BehaviorType::STRETCH, Vec2 center = Vec2(0.5, 0.5));
		Rect boundingBox;
		Rect box;
		Vec2 kernelSize;
		Vec2 center;
	private:
		Rect anchors; // Coordenadas W e H sao absolutas, e nao relativas como comumente usadas em Rects
		Rect offsets; // Coordenadas W e H sao absolutas, e nao relativas como comumente usadas em Rects
		BehaviorType behavior;
};

#endif // UIELEMENT_H
