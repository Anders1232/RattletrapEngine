#include "UIbutton.h"

#include "InputManager.h"

UIbutton::UIbutton(UIbutton::State initialState, bool interactOnBoundingBox)
				: interactOnBoundingBox(interactOnBoundingBox)
				, actualState(initialState)
				, disabledCallback(nullptr)
				, enabledCallback(nullptr)
				, highlightedCallback(nullptr)
				, pressedCallback(nullptr)
				, clickedCallback(nullptr) {}

void UIbutton::Update(float dt, bool mouseIsInside) {
	if(actualState != UIbutton::State::DISABLED) {
		if(mouseIsInside) {
			if(INPUT_MANAGER.IsMouseDown(LEFT_MOUSE_BUTTON)) {
				SetUIbuttonState(UIbutton::State::PRESSED);
			} else {
				SetUIbuttonState(UIbutton::State::HIGHLIGHTED);
			}
			if(INPUT_MANAGER.MouseRelease(LEFT_MOUSE_BUTTON)) {
				Click();
			}
		} else if (UIbutton::State::ENABLED != actualState) {
			SetUIbuttonState(UIbutton::State::ENABLED);
		}
	}
}

void UIbutton::SetCallback(UIbutton::State stateToSet, void* caller, ButtonCallback callback) {
	switch(stateToSet) {
		case UIbutton::State::DISABLED: {
			disabledCallback = callback;
			disableCaller = caller;
			break;
		}
		case UIbutton::State::ENABLED: {
			enabledCallback = callback;
			enabledCaller = caller;
			break;
		}
		case UIbutton::State::HIGHLIGHTED: {
			highlightedCallback = callback;
			highlightedCaller = caller;
			break;
		}
		case UIbutton::State::PRESSED: {
			pressedCallback = callback;
			pressedCaller = caller;
			break;
		}
	}
}

void UIbutton::SetClickCallback(void* caller, ButtonCallback callback) {
	clickedCallback = callback;
	clickCaller = caller;
}

void UIbutton::SetUIbuttonState(UIbutton::State newState) {
	actualState = newState;
	switch(actualState) {
		case UIbutton::State::DISABLED: {
			if(nullptr != disabledCallback) {
				disabledCallback(disableCaller);
			}
			break;
		}
		case UIbutton::State::ENABLED: {
			if(nullptr != enabledCallback) {
				enabledCallback(enabledCaller);
			}
			break;
		}
		case UIbutton::State::HIGHLIGHTED: {
			if(nullptr != highlightedCallback) {
				highlightedCallback(highlightedCaller);
			}
			break;
		}
		case UIbutton::State::PRESSED: {
			if(nullptr != pressedCallback) {
				pressedCallback(pressedCaller);
			}
			break;
		}
	}
}

UIbutton::State UIbutton::GetUIbuttonState(void) const {
	return actualState;
}

void UIbutton::Click() {
	if(nullptr != clickedCallback) {
		clickedCallback(clickCaller);
	}
}

bool UIbutton::Is(std::string UItype) const {
	return "UIbutton" == UItype;
}
