#include "State.h"
#include "Error.h"
#include "Camera.h"

State::State(void) : popRequested(false), quitRequested(false) {
	Camera::pos = Vec2(0,0);
}

State::~State(void) {
	objectArray.clear();
}

void State::AddObject(GameObject *object) {
	objectArray.push_back(std::unique_ptr<GameObject>(object));
}

bool State::PopRequested(void) {
	return popRequested;
}

bool State::QuitRequested(void) {
	return quitRequested;
}

void State::UpdateActive() {
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		objectArray[cont]->UpdateActive();
	}
}

void State::EarlyUpdate(float dt) {
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->EarlyUpdate(dt);
		}
	}
}

void State::Update(float dt) {
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->Update(dt);
		}
	}
}

void State::LateUpdate(float dt) {
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->LateUpdate(dt);
		}
	}
}

void State::Render(void) const {
	REPORT_I_WAS_HERE;
#ifdef RENDER_FOWARD
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
#else
	for(int64_t cont = ((int64_t)objectArray.size()) -1; 0 <= cont ; cont--) {
#endif
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->Render();
		}
	}
}

void State::DeleteRequested(void){
	//loop deletando os objetos
	for(int64_t cont = ((int64_t)objectArray.size()) -1; 0 <= cont ; cont--) {
		if(objectArray[cont]->IsDead()) {
			objectArray.erase(objectArray.begin()+cont);
		}
	}
}
