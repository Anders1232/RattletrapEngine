#include "UIverticalGroup.h"

UIverticalGroup::UIverticalGroup(UIelement::BehaviorType behavior, float padding)
		: UIcanvas(behavior)
		, padding(padding) {}

void UIverticalGroup::Update(float dt, Rect parentCanvas) {
	UIcanvas::Update(dt, parentCanvas);
	const unsigned int n = groupedElements.size();
	if(n > 0) {

		float pad = padding/box.h;
		float delta = ( 1. - pad*(n-1) ) / n;
		float j = 0;
		for(unsigned int i = 0; i < n; i++, j+=delta+pad) {
			groupedElements[i]->SetAnchors( {0., j},
											{1., j+delta}
			);
		}
	}
}

bool UIverticalGroup::Is(std::string UItype) const {
	return "UIverticalGroup" == UItype || UIcanvas::Is(UItype);
}
