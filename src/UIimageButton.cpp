#include "UIimageButton.h"

#include "Error.h"

UIimageButton::UIimageButton(UIbutton::State initialState, UIelement::BehaviorType behavior)
				: UIimage(behavior)
				, UIbutton(initialState, false)
				, kernelDirty(false)
				, disabledSprite(nullptr)
				, enabledSprite(nullptr)
				, highlightedSprite(nullptr)
				, pressedSprite(nullptr) {}

UIimageButton::~UIimageButton() {
	sp = nullptr;
	if(nullptr != disabledSprite) {
		delete disabledSprite;
	}
	if(nullptr != enabledSprite) {
		delete enabledSprite;
	}
	if(nullptr != highlightedSprite) {
		delete highlightedSprite;
	}
	if(nullptr != pressedSprite) {
		delete pressedSprite;
	}
}

void UIimageButton::Update(float dt, Rect parentCanvas) {
	if(kernelDirty) {
		UpdateKernel();
	}
	UIimage::Update(dt, parentCanvas);
	UIbutton::Update(dt, INPUT_MANAGER.GetMousePos().IsInRect( interactOnBoundingBox ? boundingBox : box));
}

void UIimageButton::SetUIbuttonState(UIbutton::State newState) {
	switch(newState) {
		case UIbutton::State::DISABLED : {
			if(nullptr != disabledSprite) {
				sp = disabledSprite;
			} else {
				REPORT_DEBUG2(true, " Sprite Disabled não configurada.");
			}
			break;
		}
		case UIbutton::State::ENABLED : {
			if(nullptr != enabledSprite) {
				sp = enabledSprite;
			} else {
				REPORT_DEBUG2(true, " Sprite Enabled não configurada.");
			}
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			if(nullptr != highlightedSprite) {
				sp = highlightedSprite;
			} else {
				REPORT_DEBUG2(true, " Sprite Highlighted não configurada.");
			}
			break;
		}
		case UIbutton::State::PRESSED : {
			if(nullptr != pressedSprite) {
				sp = pressedSprite;
			} else {
				REPORT_DEBUG2(true, " Sprite Pressed não configurada.");
			}
			break;
		}
	}
	UpdateKernel();
	UIbutton::SetUIbuttonState(newState);
}

void UIimageButton::SetStateSprite(UIbutton::State state, Sprite* sprite) {
	switch(state) {
		case UIbutton::State::DISABLED : {
			if(nullptr != disabledSprite) {
				delete disabledSprite;
			}
			disabledSprite = sprite;
			break;
		}
		case UIbutton::State::ENABLED : {
			if(nullptr != enabledSprite) {
				delete enabledSprite;
			}
			enabledSprite = sprite;
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			if(nullptr != highlightedSprite) {
				delete highlightedSprite;
			}
			highlightedSprite = sprite;
			break;
		}
		case UIbutton::State::PRESSED : {
			if(nullptr != pressedSprite) {
				delete pressedSprite;
			}
			pressedSprite = sprite;
			break;
		}
	}
	if(state == actualState) {
		sp = sprite;
		UpdateKernel();
	}
}

Sprite& UIimageButton::GetStateSprite(UIbutton::State state) const {
	kernelDirty = true;
	switch(state) {
		case UIbutton::State::DISABLED : {
			ASSERT(nullptr != disabledSprite);
			return *disabledSprite;
			break;
		}
		case UIbutton::State::ENABLED : {
			ASSERT(nullptr != enabledSprite);
			return *enabledSprite;
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			ASSERT(nullptr != highlightedSprite);
			return *highlightedSprite;
			break;
		}
		case UIbutton::State::PRESSED : {
			ASSERT(nullptr != pressedSprite);
			return *pressedSprite;
			break;
		}
	}
	Error("Não deveria chegar até aqui");
	return *disabledSprite; // Qualquer coisa, nao vai chegar aqui mesmo... Error faz o programa sair.
}

bool UIimageButton::Is(std::string UItype) const {
	return "UIimageButton" == UItype || UIimage::Is(UItype) || UIbutton::Is(UItype);
}

void UIimageButton::UpdateKernel(void) {
	if(nullptr != sp) {
		kernelSize = Vec2(sp->GetWidth(), sp->GetHeight());
	}
	kernelDirty = false;
}
