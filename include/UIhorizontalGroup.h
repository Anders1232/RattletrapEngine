#ifndef UIHORIZONTALGROUP_H
#define UIHORIZONTALGROUP_H

#include "UIelement.h"
#include "UIcanvas.h"
#include "Rect.h"

#include <vector>
#include <string>

class UIhorizontalGroup : public UIcanvas {
	public:
		UIhorizontalGroup(UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH, float padding = 0);
		virtual void Update(float dt, Rect parentCanvas);
		bool Is(std::string UItype) const;
		std::vector<UIelement*> groupedElements;
		float padding;
};

#endif // UIHORIZONTALGROUP_H
