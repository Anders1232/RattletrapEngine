#include "Component.h"

Component::Component(GameObject &associated) : associated(associated), enabled(true), newEnabled(true) {}

void Component::SetEnable(bool newValue){
	newEnabled= newValue;
}

bool Component::IsEnabled(void) const{
	return enabled;
}

void Component::UpdateEnable(void){
	enabled= newEnabled;
}
