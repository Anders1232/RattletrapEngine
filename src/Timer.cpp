#include "Timer.h"

Timer::Timer(void) {
	time=0;
}

void Timer::Update(float dt) {
	time+=dt;
}

void Timer::Restart(void) {
	time=0;
}

float Timer::Get(void) const {
	return time;
}


