#include "DragAndDrop.h"

#include "Camera.h"
#include "Error.h"
#include "InputManager.h"
#include "TileMap.h"

DragAndDrop::DragAndDrop(TileMap &map,Vec2 associatedInitialPos, GameObject &associated, bool redrag, bool dragOnActionHold)
			: dragOnHold(dragOnActionHold)
			, associatedInitialPos(associatedInitialPos)
			, tileMap(map)
			, redrag(redrag)
			, associated(associated)
			, dragNDrop("audio/Acoes/Consertando1.wav") {
}

void DragAndDrop::Update(float dt) {
	InputManager &inputManager= InputManager::GetInstance();
	if(inputManager.MouseRelease(RIGHT_MOUSE_BUTTON)) {
		bool success = false;
		if(redrag) {
			success = tileMap.InsertGO(&associated, associatedInitialPos);
		} else {
			success = tileMap.InsertGO(&associated);
		}
		if(success) { dragNDrop.Play(1); }
		associated.RemoveComponent(DRAG_AND_DROP);
	} else if(inputManager.IsMouseDown(RIGHT_MOUSE_BUTTON) || !dragOnHold) {
		Vec2 mousePos = Camera::ScreenToWorld(inputManager.GetMousePos() );
		associated.box = mousePos-Vec2(associated.box.w/2, associated.box.h/2);
	} 
}

bool DragAndDrop::Is(ComponentType type) const {
	return ComponentType::DRAG_AND_DROP == type;
}
