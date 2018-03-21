#include "Grouper.h"

#include "Error.h"
#include "Rect.h"
#include "RectTransform.h"

namespace RattletrapEngine {

	Grouper::Grouper( GameObject& associated )
				: Component( associated )
				, constraint( Grouper::ConstraintType::SQUARE )
				, number( 3 )
				, behaviorOnLess( Grouper::BehaviorOnLess::CENTER ) {}

	Grouper::~Grouper() {}

	void Grouper::EarlyUpdate( float dt ) {}

	void Grouper::Update( float dt ) {
		const unsigned int n = groupedElements.size();
		if( n > 0 ) {
			Vec2 pad = Vec2( padding.x/associated.box.w, padding.y/associated.box.h );
			int numRows, numCols;
			if( Grouper::ConstraintType::SQUARE == constraint ) {
				float sqr = std::sqrt(n);
				numRows = std::floor(sqr);
				numCols = std::ceil(sqr);
			} else {
				numRows = (Grouper::ConstraintType::FIXED_N_ROWS == constraint) ? number : std::ceil((float)n/number);
				numCols = (Grouper::ConstraintType::FIXED_N_COLS == constraint) ? number : std::ceil((float)n/number);
			}
			Vec2 delta = Vec2( ( 1. - pad.x*(numCols-1) ) / numCols, ( 1. - pad.y*(numRows-1) ) / numRows);

			float y = 0;
			int index = -1;
			for( int j = 0; j < numRows; j++, y+=delta.y+pad.y ) {
				float x = 0;
				int rem = (n-1) - index;

				if( rem < numCols ) {
					switch( behaviorOnLess ) {
						case Grouper::BehaviorOnLess::LEFT_ALIGN: {
							x = 0;
							break;
						}
						case Grouper::BehaviorOnLess::RIGHT_ALIGN: {
							x = (numCols-rem)*delta.x;
							break;
						}
						case Grouper::BehaviorOnLess::CENTER: {
							x = (numCols-rem)*delta.x/2.;
							break;
						}
						case Grouper::BehaviorOnLess::STRETCH: {
							delta.x = 1./rem;
							break;
						}
					}
				}

				for( int i = 0; i < numCols; i++, x+=delta.x+pad.x ) {
					index = i+j*numCols;
					if((unsigned int)index >= n) return;
					RectTransform* rt = dynamic_cast<RectTransform*>( groupedElements[index]->GetComponent( ComponentType::RECT_TRANSFORM ) );
					rt->SetAnchors( {x, y},
								{x+delta.x, y+delta.y} );
				}
			}
		}
	}

	void Grouper::LateUpdate( float dt ) {}

	bool Grouper::Is( int componentType ) const {
		return ComponentType::GROUPER == componentType;
	}

	void Grouper::MakeGridGroup( Grouper::ConstraintType ctype, int num, Grouper::BehaviorOnLess bol ) {
		ASSERT2( 0 < num, "Num deve ser maior que zero." );
		constraint = ctype;
		number = num;
		behaviorOnLess = bol;
	}

	void Grouper::MakeHorizontalGroup() {
		MakeGridGroup( Grouper::ConstraintType::FIXED_N_ROWS, 1, Grouper::BehaviorOnLess::STRETCH );
	}

	void Grouper::MakeVerticalGroup() {
		MakeGridGroup( Grouper::ConstraintType::FIXED_N_COLS, 1, Grouper::BehaviorOnLess::STRETCH );
	}

}
