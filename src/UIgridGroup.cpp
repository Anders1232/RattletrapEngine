#include "UIgridGroup.h"

UIgridGroup::UIgridGroup(UIgridGroup::ConstraintType constraintType, int number, UIgridGroup::BehaviorOnLess behaviorOnLess, UIelement::BehaviorType behavior)
		: UIcanvas(behavior)
		, constraint(constraintType)
		, number(number)
		, behaviorOnLess(behaviorOnLess) {
	number = (number > 1.) ? number : 1.;
	padding = Vec2(0., 0.);
}

void UIgridGroup::SetConstraint(UIgridGroup::ConstraintType newConstraint, int value) {
	constraint = newConstraint;
	number = (number > 1.) ? number : 1.;
}

void UIgridGroup::Update(float dt, Rect parentCanvas) {
	UIcanvas::Update(dt, parentCanvas);
	const unsigned int n = groupedElements.size();
	if(n > 0) {
		Vec2 pad = Vec2( padding.x/box.w, padding.y/box.h );
		int numRows = (ConstraintType::FIXED_N_ROWS == constraint) ? number : std::ceil((float)n/number);
		int numCols = (ConstraintType::FIXED_N_COLS == constraint) ? number : std::ceil((float)n/number);
		Vec2 delta = Vec2( ( 1. - pad.x*(numCols-1) ) / numCols, ( 1. - pad.y*(numRows-1) ) / numRows);
		float y = 0;
		unsigned int index = 0;
		for(int j = 0; j < numRows; j++, y+=delta.y+pad.y) {
			float x = 0;
			int rem = (n - 1) - index;
			if(rem < numCols) {
				if(UIgridGroup::BehaviorOnLess::STRETCH == behaviorOnLess) {
					delta.x = 1./rem;
				} else if(UIgridGroup::BehaviorOnLess::CENTER == behaviorOnLess) {
					x = (numCols-rem)*delta.x/2.;
				}
			}
			for(int i = 0; i < numCols; i++, x+=delta.x+pad.x) {
				index = i+j*numCols;
				if(index >= n) return;
				groupedElements[index]->SetAnchors( {x, y},
													{x+delta.x, y+delta.y} );
			}
		}
	}
}

bool UIgridGroup::Is(std::string UItype) const {
	return "UIgridGroup" == UItype || UIcanvas::Is(UItype);
}
