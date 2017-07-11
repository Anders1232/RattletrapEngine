#include "UIcanvas.h"

UIcanvas::UIcanvas(UIelement::BehaviorType behavior) : UIelement(behavior) {}

UIcanvas::UIcanvas(Vec2 aspectRatio, UIelement::BehaviorType behavior) : UIelement(behavior) {
	kernelSize = aspectRatio;
}

void UIcanvas::Update(float dt, Rect parentCanvas) {
	UIelement::Update(dt, parentCanvas);
}

void UIcanvas::SetAspectRatio(Vec2 aspectRatio) {
	this->kernelSize = aspectRatio;
}

bool UIcanvas::Is(std::string UItype) const {
	return "UIcanvas" == UItype || UIelement::Is(UItype);
}
