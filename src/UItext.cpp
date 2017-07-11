#include "UItext.h"

#include "Resources.h"
#include "Game.h"
#include "Error.h"

UItext::UItext( string fontFile,
				int fontSize,
				TextStyle style,
				SDL_Color color,
				string text,
				UIelement::BehaviorType behavior,
				bool isStrobing )
		: UIelement(behavior)
		, font(Resources::GetFont(fontFile, fontSize))
		, texture(nullptr), text(text), style(style)
		, fontSize(fontSize), color(color), textTime()
		, fontFile(fontFile), isStrobe(isStrobing)
		, strobeFrequency(TEXT_FREQUENCY)
		, timeShown(MIN_TIME_SHOWN) {
	RemakeTexture();
}

UItext::~UItext() {
	if(nullptr != texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

void UItext::Update(float dt, Rect parentCanvas) {
	UIelement::Update(dt, parentCanvas);
	textTime.Update(dt);
	if(textTime.Get() >= strobeFrequency) {
		textTime.Restart();
	}
}

void UItext::Render(bool debugRender) const {
	UIelement::Render(debugRender);
	if(box.w > 0 && box.h > 0) {
		if(isStrobe ? textTime.Get() < timeShown : true) {
			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = kernelSize.x;
			srcRect.h = kernelSize.y;
			SDL_Rect destRect = box;
			if(0 != SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &srcRect, &destRect) ) {
				Error("Render error: " << SDL_GetError());
			}
		}
	}
}

void UItext::SetText(string text) {
	this->text= text;
	RemakeTexture();
}

void UItext::SetColor(SDL_Color color) {
	this->color= color;
	RemakeTexture();
}

void UItext::SetStyle(TextStyle style) {
	this->style= style;
	RemakeTexture();
}

void UItext::SetFontSize(int fontSize) {
	this->fontSize= fontSize;
	RemakeTexture();
}

void UItext::RemakeTexture(void) {
	if(nullptr != texture) {
		SDL_DestroyTexture(texture);
	}
	font = Resources::GetFont(fontFile, fontSize);
	SDL_Surface *temp = nullptr;
	if(TextStyle::SOLID == style) {
		temp = TTF_RenderText_Solid(font.get(), text.c_str(), color);
	}
	else if(TextStyle::SHARED == style) {
		SDL_Color bgColor;
		bgColor= {0, 0, 0, 0};//preto
		temp = TTF_RenderText_Shaded(font.get(), text.c_str(), color, bgColor);
	}
	else if(TextStyle::BLENDED == style) {
		temp = TTF_RenderText_Blended(font.get(), text.c_str(), color);
	}
	if (nullptr == temp) {
		REPORT_DEBUG2(true, " " << TTF_GetError());
	}

	texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), temp);
	if (nullptr == texture) {
		REPORT_DEBUG2(true, " Cuidado! Nao foi possivel se criar uma textura de texto:\t" << SDL_GetError() << "\n\t\t\t\t\t Se crashar, esse talvez seja o motivo...");
	}
	
	SDL_FreeSurface(temp);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	kernelSize = Vec2(w, h);
}

Vec2 UItext::GetSize(void) const {
	return kernelSize;
}

void UItext::SetTimeShown(float newTime) {
	timeShown = newTime;
}

void UItext::SetStrobeFrequency(float fullTime) {
	strobeFrequency = fullTime;
}

bool UItext::Is(std::string UItype) const {
	return "UItext" == UItype || UIelement::Is(UItype);
}
