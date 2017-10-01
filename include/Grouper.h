#ifndef GROUPER_H
#define GROUPER_H

#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"
#include "Vec2.h"

class Grouper : public Component {
  public:
	enum ConstraintType {
		FIXED_N_ROWS,
		FIXED_N_COLS
	};
	enum BehaviorOnLess {
		RIGHT_ALIGN,
		LEFT_ALIGN,
		STRETCH,
		CENTER
	};

	Vec2 padding;
	std::vector<GameObject*> groupedElements;

	Grouper( GameObject& associated );
	~Grouper();

	void EarlyUpdate( float dt );
	void Update( float dt );
	void LateUpdate( float dt );

	bool Is( ComponentType type ) const;

	void MakeGridGroup( ConstraintType ctype, int num, BehaviorOnLess bol );
	void MakeHorizontalGroup();
	void MakeVerticalGroup();
private:
	Grouper::ConstraintType constraint;
	int number;
	Grouper::BehaviorOnLess behaviorOnLess;
};

#endif // GROUPER_H
