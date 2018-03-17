#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ComponentType.h"
#include "GameObject.h"

namespace RattletrapEngine {

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
		
			Button(GameObject& associated, Button::State initialState = Button::State::ENABLED, bool interactOnBoundingBox = true);
			~Button();
			
			void EarlyUpdate( float dt );
			void Update( float dt );
			void LateUpdate( float dt );
            void Render();
			bool Is( int componentType ) const;
		
			void SetCallback(Button::State stateToSet, Button::Callback calldata);
			void SetReleaseCallback(Button::Callback calldata);
			void SetState(Button::State newState);
			Button::State GetState(void) const;
			void Click();
		
			bool interactOnBoundingBox;
		
		private:
			Button::State actualState;
			Callback disabled;
			Callback enabled;
			Callback highlighted;
			Callback pressed;
			Callback released;
	};

}

#endif // BUTTON_H
