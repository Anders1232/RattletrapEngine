#include "UIhorizontalGroup.h"

UIhorizontalGroup::UIhorizontalGroup(UIelement::BehaviorType behavior, float padding)
		: UIcanvas(behavior)
		, padding(padding) {}

void UIhorizontalGroup::Update(float dt, Rect parentCanvas) {
	UIcanvas::Update(dt, parentCanvas);
	const unsigned int n = groupedElements.size();
	if(n > 0) {
		float pad = padding/box.w;
		float delta = ( 1. - pad*(n-1) ) / n;
		float j = 0;
		for(unsigned int i = 0; i < n; i++, j+=delta+pad) {
			groupedElements[i]->SetAnchors( {j, 0.},
											{j+delta, 1.}
			);
		}
	}
}

bool UIhorizontalGroup::Is(std::string UItype) const {
	return "UIhorizontalGroup" == UItype || UIcanvas::Is(UItype);
}
