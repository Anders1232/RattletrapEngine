#include "UItextButton.h"

#include "InputManager.h"
#include "Error.h"

UItextButton::UItextButton( string fontFile,
							int fontSize,
							UItext::TextStyle style,
							SDL_Color color,
							string text,
							UIbutton::State startingState,
							UIelement::BehaviorType behavior,
							bool isStrobing)
		: UItext(fontFile, fontSize, style, color, text, behavior, isStrobing)
		, UIbutton(startingState)
		, disabledColor(color)
		, enabledColor(color)
		, highlightedColor(color)
		, pressedColor(color) {}

void UItextButton::Update(float dt, Rect parentCanvas) {
	UItext::Update(dt, parentCanvas);
	UIbutton::Update(dt, INPUT_MANAGER.GetMousePos().IsInRect(interactOnBoundingBox ? boundingBox : box));
	UItext::Update(dt, parentCanvas);	
}

void UItextButton::ConfigColors(SDL_Color disabled, SDL_Color enabled, SDL_Color highlighted, SDL_Color pressed) {
	disabledColor = disabled;
	enabledColor = enabled;
	highlightedColor = highlighted;
	pressedColor = pressed;
	SetColor(GetStateColor(actualState));
}

void UItextButton::SetUIbuttonState(UIbutton::State newState) {

	if(newState == actualState) return;

	switch(newState) {
		case UIbutton::State::DISABLED : {
			SetColor(disabledColor);
			break;
		}
		case UIbutton::State::ENABLED : {
			SetColor(enabledColor);
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			SetColor(highlightedColor);
			break;
		}
		case UIbutton::State::PRESSED : {
			SetColor(pressedColor);
			break;
		}
	}
	UIbutton::SetUIbuttonState(newState);
}

void UItextButton::SetStateColor(UIbutton::State state, SDL_Color color) {
	switch(state) {
		case UIbutton::State::DISABLED : {
			disabledColor = color;
			break;
		}
		case UIbutton::State::ENABLED : {
			enabledColor = color;
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			highlightedColor = color;
			break;
		}
		case UIbutton::State::PRESSED : {
			pressedColor = color;
			break;
		}
	}
	if(state == actualState) {
		SetColor(color);
	}
}

SDL_Color UItextButton::GetStateColor(UIbutton::State state) const {
	switch(state) {
		case UIbutton::State::DISABLED : {
			return disabledColor;
			break;
		}
		case UIbutton::State::ENABLED : {
			return enabledColor;
			break;
		}
		case UIbutton::State::HIGHLIGHTED : {
			return highlightedColor;
			break;
		}
		case UIbutton::State::PRESSED : {
			return pressedColor;
			break;
		}
	}
	Error("Não deveria chegar até aqui");
	return {255,255,255,255};
}

bool UItextButton::Is(std::string UItype) const {
	return "UItextButton" == UItype || UItext::Is(UItype) || UIbutton::Is(UItype);
}
