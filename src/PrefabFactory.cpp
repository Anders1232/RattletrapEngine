#include "PrefabFactory.h"

#include "RectTransform.h"
#include "Grouper.h"

std::unique_ptr<GameObject> PrefabFactory::SquareGridGroup() {
	GameObject *go = new GameObject();
	
	Component *c = new RectTransform( *go, nullptr );
	go->AddComponent( c );

	c = new Grouper( *go );
	((Grouper*)c)->MakeGridGroup( Grouper::ConstraintType::SQUARE, 3, Grouper::BehaviorOnLess::CENTER);
	go->AddComponent( c );

	return std::unique_ptr<GameObject>( go );
}
