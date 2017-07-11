#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "Color.h"
#include "Sprite.h"
#include "Rect.h"

#include "UIelement.h"

#include <string>

class UIimage : public UIelement {
	public:
		UIimage(std::string file, UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH);
		UIimage(std::string file, float frameTime, int frameCount, UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH);
		virtual ~UIimage();
		virtual void Update(float dt, Rect parentCanvas);
		virtual void Render(bool debugRender = false) const;
		Sprite& GetSprite(void);
		bool Is(std::string UItype) const;
		float angle;
	protected:
		UIimage(UIelement::BehaviorType behavior = UIelement::BehaviorType::STRETCH);
		Sprite* sp;
};

#endif // UIIMAGE_H
