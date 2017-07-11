#ifndef UIVERTICALGROUP_H
#define UIVERTICALGROUP_H

#include "UIelement.h"
#include "UIcanvas.h"
#include "Rect.h"

#include <vector>
#include <string>

class UIverticalGroup : public UIcanvas {
	public:
		UIverticalGroup(UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH, float padding = 0);
		virtual void Update(float dt, Rect parentCanvas);
		bool Is(std::string UItype) const;
		std::vector<UIelement*> groupedElements;
		float padding;
};

#endif // UIVERTICALGROUP_H
