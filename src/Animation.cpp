#include "Animation.h"

#include "Camera.h"

Animation::Animation(
		float x, float y, float rotation,
		string sprite, int frameCount,
		float frameTime, bool ends
		) : GameObject(), endTimer(), timeLimit(frameCount*frameTime)
		, oneTimeOnly(ends), sp(sprite,false, frameTime,frameCount ) {
	box= Vec2(x - sp.GetWidth()/2, y - sp.GetHeight()/2);
	this->rotation = rotation;
}

void Animation::Update(float dt) {
	sp.Update(dt);
	endTimer.Update(dt);
}

void Animation::Render(void) {
	sp.Render(box, rotation);
}

Rect Animation::GetWorldRenderedRect(void) const {
	return Camera::WorldToScreen(box);
}

bool Animation::IsDead(void) {
	if(oneTimeOnly) {
		if(endTimer.Get() > timeLimit) {
			return true;
		}
	}
	return false;
}

void Animation::NotifyCollision(GameObject &other) {}

bool Animation::Is(string type) {
	return "Animation" == type;
}

void Animation::RequestDelete(void) {
	if(!oneTimeOnly) {
		oneTimeOnly = true;
	}
	endTimer.Update(timeLimit);
}
