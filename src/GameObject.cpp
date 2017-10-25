#include "GameObject.h"
#include "Game.h"
#include "Camera.h"

#include "Error.h"

GameObject::GameObject(void):
    rotation(0.),
    dead(false),
    parent(nullptr),
    debug(false),
    showOnScreen(true){
}


GameObject::~GameObject(){
	REPORT_I_WAS_HERE;
	for(uint i = 0; i < components.size(); i++) {
		delete components[i];
	}
	components.clear();
}

void GameObject::EarlyUpdate(float dt){
	for(uint i=0; i < components.size(); i++){
		components[i]->EarlyUpdate(dt);
	}
}

void GameObject::Update(float dt){
	for(uint i=0; i < components.size(); i++){
		components[i]->Update(dt);
	}
}

void GameObject::Render(void){
    DEBUG_RENDER("inicio");
    if(showOnScreen){
        for(unsigned int i = 0; i < components.size(); i++){
                components[i]->Render();
        }
    }
	if(debug){
        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, 255);
        SDL_Rect rect = {box.x, box.y, box.w, box.h};
        SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &rect);
	}
	DEBUG_RENDER("fim");
}

bool GameObject::IsDead(void){
	return dead;
}

void GameObject::RequestDelete(void){
	dead= true;
}

void GameObject::NotifyCollision(GameObject &other){
	REPORT_DEBUG("\t GameObject::NotifyCollision called!");
}

bool GameObject::Is(string type){
	REPORT_DEBUG("\t GameObject::Is called!");
	return ("GameObject" == type);
}

Rect GameObject::GetWorldRenderedRect(void) const{
	return Camera::WorldToScreen(box);
}

void GameObject::AddComponent(Component* component){
	components.emplace_back(component);
}

void GameObject::RemoveComponent(ComponentType type){
	for(uint i = 0; i < components.size();i++){
		if(components[i]->Is(type)){
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}
	}
}

Component& GameObject::GetComponent(ComponentType type) const{
	for(uint i = 0; i < components.size();i++){
		if(components[i]->Is(type)){
			return *(components[i]);
		}
	}
	Error("Component not found!");
}

#include "Error_footer.h"
