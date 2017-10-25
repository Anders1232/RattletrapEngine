#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "InputManager.h"

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
        if( Button::State::DISABLED != actualState ) {
            Vec2 mousePos = INPUT_MANAGER.GetMousePos();
            bool mouseIsInside = mousePos.IsInRect( associated.box );
            if( !mouseIsInside && interactOnBoundingBox ) {
                RectTransform& rect = dynamic_cast<RectTransform&>( associated.GetComponent( ComponentType::RECT_TRANSFORM ) );
                mouseIsInside = mousePos.IsInRect( rect.GetBoundingBox() );
            }
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
	}


	void Update( float dt ){}
	void LateUpdate( float dt ){}
	void Render() const{}
	bool Is( ComponentType type ) const {return ComponentType::BUTTON == type;}

	void SetCallback(Button::State stateToSet, Button::Callback calldata){
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
	void Click(){	released.Call();}

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
