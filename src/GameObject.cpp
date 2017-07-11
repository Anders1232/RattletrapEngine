#include "GameObject.h"
#include "Error.h"
#include "Camera.h"

GameObject::GameObject(void): rotation(0.), dead(false){
}


GameObject::~GameObject(){
	REPORT_I_WAS_HERE;
	for(uint i = 0; i < components.size(); i++) {
		delete components[i];
	}
	components.clear();
}

void GameObject::Update(float dt){
	for(uint i=0; i < components.size(); i++){
		components[i]->Update(dt);
	}
}

void GameObject::Render(void){
	REPORT_DEBUG("\t GameObject::Render called!");
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

Component& GameObject::GetComponent(ComponentType type){
	for(uint i = 0; i < components.size();i++){
		if(components[i]->Is(type)){
			return *(components[i]);
		}
	}
	Error("Component not found!");
}

