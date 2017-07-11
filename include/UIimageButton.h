#ifndef UIIMAGEBUTTON_H
#define UIIMAGEBUTTON_H

#include "Color.h"
#include "Sprite.h"
#include "Rect.h"

#include "UIimage.h"
#include "UIbutton.h"

#include <string>

class UIimageButton : public UIimage, public UIbutton {
	public:
		UIimageButton(UIbutton::State initialState = UIbutton::State::ENABLED, UIelement::BehaviorType behavior = UIelement::BehaviorType::FIT);
		virtual ~UIimageButton();
		void Update(float dt, Rect parentCanvas);
		Sprite& GetStateSprite(UIbutton::State state) const;
		void SetUIbuttonState(UIbutton::State newState);
		void SetStateSprite(UIbutton::State state, Sprite* sprite);
		bool Is(std::string UItype) const;
	protected:
		mutable bool kernelDirty;
		void UpdateKernel(void);
	private:
		Sprite* disabledSprite;
		Sprite* enabledSprite;
		Sprite* highlightedSprite;
		Sprite* pressedSprite;
};

#endif // UIIMAGEBUTTON_H
