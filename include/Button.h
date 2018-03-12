#ifndef BUTTON_H
#define BUTTON_H

#include <algorithm> //find()
#include <vector>
#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "InputManager.h"

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
			Callback( ButtonCallbackFunc callbackFunc = nullptr, void* caller = nullptr );
			ButtonCallbackFunc callbackFunc;
			void* caller;
			void Call();
		};

		Button(GameObject& associated, std::string name = "", Button::State initialState = Button::State::ENABLED, bool interactOnBoundingBox = true);
		~Button();

		void EarlyUpdate( float dt );
		void Update( float dt );
		void LateUpdate( float dt );
		void Render() const;
		bool Is( unsigned int type ) const;

		void SetCallback(Button::State stateToSet, Button::Callback calldata);
		void SetReleaseCallback(Button::Callback calldata);
		void SetState(Button::State newState);
		Button::State GetState(void) const;
		void Click();

		bool AddObserver(Component* observer);
		bool interactOnBoundingBox;
		std::string name;

	private:
	    bool Notify();
	    std::vector<Component*> observers;
		Button::State actualState;
		Callback disabled;
		Callback enabled;
		Callback highlighted;
		Callback pressed;
		Callback released;
};

#endif // BUTTON_H
