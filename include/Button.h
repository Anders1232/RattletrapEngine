#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "InputManager.h"

#include "Error.h"
template<typename T>
class Button : public Component {
  public:
	enum State : int {
		DISABLED = 0,
		ENABLED,
		HIGHLIGHTED,
		PRESSED
	};
	typedef void (*ButtonCallbackFunc) (void*);
	struct Callback {
		Callback(void(T::*callbackFunc)(void*) = nullptr, void* caller = nullptr ):
                 callbackFunc( callbackFunc ),caller(caller) {}

		void(T::*callbackFunc)(void*);
		void* caller;
		T* obj;
		void Call(){
            if( nullptr != callbackFunc ) {
                (obj->*callbackFunc)(caller);
            }
		}
	};

	Button(GameObject& associated, Button::State initialState = Button::State::ENABLED, bool interactOnBoundingBox = true)
    			: Component(associated)
                , interactOnBoundingBox(interactOnBoundingBox)
                , actualState(initialState){}
	~Button(){}

	void EarlyUpdate( float dt ){
	    DEBUG_UPDATE("inicio");
        if( Button::State::DISABLED != actualState ) {
            Vec2 mousePos = INPUT_MANAGER.GetMousePos();
            bool mouseIsInside = mousePos.IsInRect( associated.box );
            if( !mouseIsInside && interactOnBoundingBox ) {
                RectTransform& rect = dynamic_cast<RectTransform&>( associated.GetComponent( ComponentType::RECT_TRANSFORM ) );
                mouseIsInside = mousePos.IsInRect( rect.GetBoundingBox() );
            }
            if( mouseIsInside ) {
                DEBUG_UPDATE("mousePos: " << mousePos.x << ", " << mousePos.y);
                DEBUG_UPDATE("associated.box: " << associated.box.x << ", " << associated.box.y << ", "
                                               << associated.box.w << ", " << associated.box.h);
                if( INPUT_MANAGER.IsMouseDown( LEFT_MOUSE_BUTTON ) ) {
                    DEBUG_UPDATE("Pressed");
                    SetState( Button::State::PRESSED );
                } else {
                    DEBUG_UPDATE("Highlighted");
                    SetState( Button::State::HIGHLIGHTED );
                }
                if( INPUT_MANAGER.MouseRelease( LEFT_MOUSE_BUTTON ) ) {
                    Click();
                }
            } else if( Button::State::ENABLED != actualState ) {
                SetState( Button::State::ENABLED );
            }
        }
        DEBUG_UPDATE("fim");
	}


	void Update( float dt ){}
	void LateUpdate( float dt ){}
	void Render() const{}
	bool Is( ComponentType type ) const {return ComponentType::BUTTON == type;}

	void SetCallback(Button::State stateToSet, Button::Callback calldata){
	    actualState = stateToSet;
	    switch(stateToSet) {
		case Button::State::DISABLED: {
			disabled = calldata;
			break;
		}
		case Button::State::ENABLED: {
			enabled = calldata;
			break;
		}
		case Button::State::HIGHLIGHTED: {
			highlighted = calldata;
			break;
		}
		case Button::State::PRESSED: {
			pressed = calldata;
			break;
		}
        }
	}

	void SetReleaseCallback(Button::Callback calldata){
	    released = calldata;
	}
	void SetState(Button::State newState){
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

	Button::State GetState(void) const{return actualState;}
	void Click(){
	    DEBUG_PRINT("Chamou click");
	    released.Call();
    }

	bool interactOnBoundingBox;

  private:
	Button::State actualState;
	Callback disabled;
	Callback enabled;
	Callback highlighted;
	Callback pressed;
	Callback released;
};

#endif // BUTTON_H
