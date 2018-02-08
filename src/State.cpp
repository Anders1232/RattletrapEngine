#include "State.h"
#include "Error.h"
#include "Camera.h"
//#include "Threading.h"

//#define USE_THREADING


#include "Error.h"
State::State(void) : popRequested(false), quitRequested(false) {
	Camera::pos = Vec2(0,0);
}

State::~State(void) {
	objectArray.clear();
}

void State::AddObject(GameObject *object) {
    DEBUG_PRINT(object->GetTag() << " adicionado");
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
#ifdef USE_THREADING
	Rattletrap::Threading::EarlyUpdate(objectArray);
#else
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->EarlyUpdate(dt);
		}
	}
#endif
}

void State::Update(float dt) {
#ifdef USE_THREADING
	Rattletrap::Threading::Update(objectArray);
#else
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->Update(dt);
		}
	}
#endif
}

void State::LateUpdate(float dt) {
#ifdef USE_THREADING
	Rattletrap::Threading::LateUpdate(objectArray);
#else
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->LateUpdate(dt);
		}
	}
#endif
}

void State::Render(void) const {
#ifndef RENDER_BACKWARD
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
#else
	for(int64_t cont = ((int64_t)objectArray.size()) -1; 0 <= cont ; cont--) {
#endif
        DEBUG_RENDER("Renderizando objeto " << cont);
		if(objectArray[cont]->IsActive()){
			objectArray[cont]->Render();
		}
	}
}

void State::DeleteRequested(void){
	//loop deletando os objetos
	for(int64_t cont = ((int64_t)objectArray.size()) -1; 0 <= cont ; cont--) {
		if(objectArray[cont]->IsDead()) {
            DEBUG_PRINT("Object " << cont << " apagado");
			objectArray.erase(objectArray.begin()+cont);
		}
	}
}

#include "Error_footer.h"
