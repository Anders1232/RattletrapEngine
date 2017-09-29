#include "DragAndDrop.h"

#include "Camera.h"
#include "Error.h"
#include "InputManager.h"
#include "TileMap.h"
#include "Game.h"
#include "Animation.h"

#define STROBBING_TIME 0.5
#define TIME_TO_APPEAR 1

DragAndDrop::DragAndDrop(TileMap &map,Vec2 associatedInitialPos, GameObject &associated, bool redrag)
			: firstClick(false)
			, redrag(redrag)
			, associatedInitialPos(associatedInitialPos)
			, tileMap(map)
			, associated(associated)
			, strobe()
			, toAppear()
			, dragNDrop("audio/Acoes/Consertando1.wav")
			, isDropping(false)
			, dropped(false) {
}

void DragAndDrop::Update(float dt) {
	InputManager &inputManager= InputManager::GetInstance();

	if(!redrag){
		if(inputManager.MousePress(LEFT_MOUSE_BUTTON)){
			PutFirst();
		}
	}

	else{
		if(firstClick){
			strobe.Update(dt);
			if(strobe.Get() > STROBBING_TIME){
				strobe.Restart();
			}
			// faça a componente sprite piscar

			if(isDropping){
				Game::GetInstance().GetCurrentState().AddObject(new Animation(associatedInitialPos.x, associatedInitialPos.y, associated.rotation, "./img/SpriteSheets/Fumaca_spritesheet.png", 8, 0.1, true));
				toAppear.Update(dt);
				if(toAppear.Get() > TIME_TO_APPEAR){
					Game::GetInstance().GetCurrentState().AddObject(new Animation(associated.box.Center().x, associated.box.Center().y, associated.rotation, "./img/SpriteSheets/Fumaca_spritesheet.png", 8, 0.1, true));
					dropped = true;
					toAppear.Restart();
				}
			}

			if(inputManager.MouseRelease(RIGHT_MOUSE_BUTTON)) {
				isDropping = tileMap.InsertGO(&associated, associatedInitialPos);
				if(isDropping) {
					dragNDrop.Play(1);
					toAppear.Restart();
				}
			}
			
			
			if(dropped){ firstClick = false; }
		} 
		
		if(!firstClick){
			if(inputManager.MouseRelease(RIGHT_MOUSE_BUTTON)) {
				bool taken = false;
				taken = tileMap.InsertGO(&associated, associatedInitialPos);
				if(taken) {
					firstClick = true;
				}
			}
		}
	}
}

bool DragAndDrop::Is(ComponentType type) const {
	return ComponentType::DRAG_AND_DROP == type;
}

bool DragAndDrop::PutFirst(void){
	if(tileMap.InsertGO(&associated)){
		redrag = true;
	}
	return redrag;
}
