#include "Button.h"

#include "RectTransform.h"
#include "InputManager.h"
#include "Vec2.h"

#include "Error.h"

Button::Callback::Callback( ButtonCallbackFunc callbackFunc, void* caller)
						: callbackFunc( callbackFunc )
						, caller(caller) {}

void Button::Callback::Call() {
	if( nullptr != callbackFunc ) {
		callbackFunc(caller);
	}
}

Button::Button( GameObject& associated, Button::State initialState, bool interactOnBoundingBox )
			: Component(associated)
			, interactOnBoundingBox(interactOnBoundingBox)
			, actualState(initialState)
{
    DEBUG_CONSTRUCTOR("inicio");
    DEBUG_CONSTRUCTOR("fim");
}

Button::~Button() {}

void Button::EarlyUpdate( float dt ) {
    if(associated.Released()){
        Notify();
        //DEBUG_PRINT("associated.box: (" << associated.box.w << "," << associated.box.h << ")" );
    }
    /*
    if( Button::State::DISABLED != actualState ) {
		Vec2 mousePos = INPUT_MANAGER.GetMousePos();
		bool mouseIsInside = mousePos.IsInRect( associated.box );
		if( !mouseIsInside && interactOnBoundingBox ) {
			RectTransform& rect = dynamic_cast<RectTransform&>( associated.GetComponent( ComponentType::RECT_TRANSFORM ) );
			mouseIsInside = mousePos.IsInRect( rect.GetBoundingBox() );
		}
		DEBUG_UPDATE("associated.box:{" <<
                      associated.box.x << ", " <<
                      associated.box.y << ", " <<
                      associated.box.w << ", " <<
                      associated.box.h << "}");

		DEBUG_UPDATE("mouseIsInside: " << mouseIsInside);
		if( mouseIsInside ) {
			if( INPUT_MANAGER.IsMouseDown( LEFT_MOUSE_BUTTON ) ) {
				SetState( Button::State::PRESSED );
			} else {
				SetState( Button::State::HIGHLIGHTED );
			}
			if( INPUT_MANAGER.MouseRelease( LEFT_MOUSE_BUTTON ) ) {
				Click();
			}
		} else if( Button::State::ENABLED != actualState ) {
			SetState( Button::State::ENABLED );
		}
	}
	*/
}

void Button::Update( float dt ) {
}

void Button::LateUpdate( float dt ) {}

void Button::Render() const {}

bool Button::Is( ComponentType type ) const {
	return ComponentType::BUTTON == type;
}

void Button::SetCallback( Button::State stateToSet, Button::Callback calldata ) {
}

void Button::SetReleaseCallback( Button::Callback calldata ) {
}

void Button::SetState( Button::State newState ) {
	actualState = newState;
	switch(actualState) {
		case Button::State::DISABLED: {
			disabled.Call();
			break;
		}
		case Button::State::ENABLED: {
			enabled.Call();
			break;
		}
		case Button::State::HIGHLIGHTED: {
			highlighted.Call();
			break;
		}
		case Button::State::PRESSED: {
			pressed.Call();
			break;
		}
	}
}

Button::State Button::GetState() const {
	return actualState;
}

void Button::Click() {
	released.Call();
}

bool Button::AddObserver(Component* observer){
    vector<Component*>::iterator it = find(observers.begin(), observers.end(), observer);
    if(it != observers.end()){
        return false;
    }else{
        observers.push_back(observer);
        return true;
    }
}

bool Button::Notify(){
    for(uint i = 0; i < observers.size(); i++){
        observers[i]->ButtonObserver(this);
    }
    return (observers.size() > 0);
}

#include "Error_footer.h"
