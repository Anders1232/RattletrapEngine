#ifndef UIGRIDGROUP_H
#define UIGRIDGROUP_H

#include "UIelement.h"
#include "UIcanvas.h"
#include "Rect.h"
#include "Vec2.h"

#include <vector>
#include <string>

class UIgridGroup : public UIcanvas {
	public:
		enum ConstraintType {
			FIXED_N_ROWS,
			FIXED_N_COLS
		};
		enum BehaviorOnLess {
			CENTER,
			STRETCH,
			NORMAL
		};
		UIgridGroup(UIgridGroup::ConstraintType constraintType, int number, UIgridGroup::BehaviorOnLess behaviorOnLess = UIgridGroup::BehaviorOnLess::NORMAL, UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH);
		virtual void Update(float dt, Rect parentCanvas);
		void SetConstraint(UIgridGroup::ConstraintType newConstraint, int value);
		bool Is(std::string UItype) const;
		std::vector<UIelement*> groupedElements;
		Vec2 padding;
	private:
		UIgridGroup::ConstraintType constraint;
		int number;
		UIgridGroup::BehaviorOnLess behaviorOnLess;
};

#endif // UIGRIDGROUP_H
