#include "GameObject.h"
#include "Error.h"
#include "Camera.h"

namespace RattletrapEngine {

	GameObject::GameObject(void): rotation(0.), dead(false), active(true), newActive(true){
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
			if(components[i]->IsEnabled() ){
				components[i]->Update(dt);
			}
		}
	}

	void GameObject::EarlyUpdate(float dt){
		for(uint i=0; i < components.size(); i++){
			if(components[i]->IsEnabled() ){
				components[i]->EarlyUpdate(dt);
			}
		}
	}

	void GameObject::LateUpdate(float dt){
		for(uint i=0; i < components.size(); i++){
			if(components[i]->IsEnabled() ){
				components[i]->LateUpdate(dt);
			}
		}
	}

	void GameObject::Render(void){
		REPORT_DEBUG("\t GameObject::Render called!");
		for(uint i=0; i < components.size(); i++){
			if(components[i]->IsEnabled() ){
				components[i]->Render();
			}
		}
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

	void GameObject::RemoveComponent(int componentType){
		for(uint i = 0; i < components.size();i++){
			if(components[i]->Is(componentType)){
				delete components[i];
				components.erase(components.begin() + i);
				return;
			}
		}
	}

	void GameObject::RemoveComponent(Component* component){
		for(uint i = 0; i < components.size();i++){
			if(components[i] == component){
				delete components[i];
				components.erase(components.begin() + i);
				return;
			}
		}
	}

    Component* GameObject::GetComponent(int componentType) const{
		for(uint i = 0; i < components.size();i++){
			if(components[i]->Is(componentType)){
                return components[i];
			}
		}
	}

	std::vector<Component *> GameObject::GetComponents(int componentType) const{
		std::vector<Component *> ret;
		for(uint i = 0; i < components.size();i++){
			if(components[i]->Is(componentType)){
				ret.push_back(components[i] );
			}
		}
		return ret;
	}

	void GameObject::UpdateActive(void){
		active= newActive;//Não é necessário ter um if.
		for(uint i = 0; i < components.size();i++){
			components[i]->UpdateEnable();
		}
	}

	void GameObject::SetActive(bool newValue){
		newActive= newValue;
	}

	bool GameObject::IsActive(void) const{
		return active;
	}

}
