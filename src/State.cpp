#include "State.h"
#include "Error.h"
#include "Camera.h"

#define DEBUG
#ifdef DEBUG
    #define DEBUG_PRINT(x) do{std::cout << x << std::endl;}while(0)
#else
    #define DEBUG_PRINT(x)
#endif // DEBUG

State::State(void) : popRequested(false), quitRequested(false) {
	Camera::pos = Vec2(0,0);
}

State::~State(void) {
	objectArray.clear();
}

void State::AddObject(GameObject *object) {
    DEBUG_PRINT("Adicionado objeto ao vetor");
	objectArray.push_back(std::unique_ptr<GameObject>(object));
}

bool State::PopRequested(void) {
	return popRequested;
}

bool State::QuitRequested(void) {
	return quitRequested;
}

void State::UpdateArray(float dt) {
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
		objectArray.at(cont)->Update(dt);
		if(objectArray.at(cont)->IsDead()) {
			objectArray.erase(objectArray.begin()+cont);
			cont--;
		}
	}
}

void State::RenderArray(void) const {
/*
#ifdef RENDER_FOWARD
	for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
#else
*/
    for(unsigned int cont = 0; cont < objectArray.size(); cont++) {
    //for(int64_t cont = ((int64_t)objectArray.size()) -1; 0 <= cont ; cont--) {
//#endif
		objectArray[cont]->Render();
	}
}


#ifdef DEBUG
    #undef DEBUG
#endif // DEBUG
