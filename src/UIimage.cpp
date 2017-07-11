#include "UIimage.h"

#include "Error.h"

UIimage::UIimage(std::string file, UIelement::BehaviorType behavior)
		 : UIelement(behavior) {
	sp = new Sprite(file);
	kernelSize = Vec2(sp->GetWidth(), sp->GetHeight());
	angle = 0;
}

UIimage::UIimage(std::string file, float frameTime, int frameCount, UIelement::BehaviorType behavior)
		 : UIelement(behavior) {
	sp = new Sprite(file, false, frameTime, frameCount);
	kernelSize = Vec2(sp->GetWidth(), sp->GetHeight());
	angle = 0;
}

UIimage::UIimage(UIelement::BehaviorType behavior) : UIelement(behavior), angle(0), sp(nullptr) {}

UIimage::~UIimage() {
	if(nullptr != sp) {
		delete sp;
	}
}

void UIimage::Update(float dt, Rect parentCanvas) {
	UIelement::Update(dt, parentCanvas);
	sp->Update(dt);
}

void UIimage::Render(bool debugRender) const {
	UIelement::Render(debugRender);

	if(nullptr != sp) {

		if(box.w > 0 && box.h > 0) {
			sp->Render(box, angle, false);
		}
	} else {
		Error("Alguma classe filha de UIimage nao foi corretamente inicializada.")
	}
}

Sprite& UIimage::GetSprite(void) {
	return *sp;
}

bool UIimage::Is(std::string UItype) const {
	return "UIimage" == UItype || UIelement::Is(UItype);
}
