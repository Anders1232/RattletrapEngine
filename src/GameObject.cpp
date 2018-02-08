#include "GameObject.h"
#include "Game.h"
#include "Camera.h"
#include "Error.h"

GameObject::GameObject(void): rotation(0.), dead(false), active(true), newActive(true), parent(nullptr), clicked(false){
}

GameObject::GameObject(string tag, State* context):
    rotation(0.), dead(false), active(true), newActive(true), parent(nullptr), clicked(false), context(context), tag(tag) {
}


GameObject::~GameObject(){
	REPORT_I_WAS_HERE;
	for(uint i = 0; i < components.size(); i++) {
		delete components[i];
	}
	components.clear();
}

void GameObject::SetParent(GameObject& parent, int xrelative, int yrelative){
    this->parent = &parent;
    parent.child[tag] = this;
    parentRelative.x = xrelative;
    parentRelative.y = yrelative;
}

void GameObject::SetTag(string tag){
    this->tag = tag;
}

string GameObject::GetTag(){
    return tag;
}

bool GameObject::IsTag(string tag){
    return this->tag == tag;
}

GameObject* GameObject::GetChildWithTag(string tag){
    //  TODO: melhorar sistema de registro de child
    GameObject* gameObject = child[tag];
    if( gameObject != nullptr){
        return gameObject;
    }
    Error("Child not found!");
    return nullptr;
}


void GameObject::EarlyUpdate(float dt){
    if(parent != nullptr){
        if(box.x - parent->box.x != parentRelative.x){
            box.x = parent->box.x + parentRelative.x;
        }
        if(box.y - parent->box.y != parentRelative.y){
            box.y = parent->box.y + parentRelative.y;
        }
    }
    SDL_Rect rct;
    rct.x = box.x;
    rct.y = box.y;
    rct.w = box.w;
    rct.h = box.h;
    if(InputManager::GetInstance().GetMousePos().IsInRect(rct) &&
       InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
        clicked = true;
    }

    if(InputManager::GetInstance().GetMousePos().IsInRect(rct) &&
       InputManager::GetInstance().MouseRelease(LEFT_MOUSE_BUTTON)){
        released = true;
    }

	for(uint i=0; i < components.size(); i++){
		components[i]->EarlyUpdate(dt);
	}
}
/*
void GameObject::EarlyUpdate(float dt){
	for(uint i=0; i < components.size(); i++){
		if(components[i]->IsEnabled() ){
			components[i]->EarlyUpdate(dt);
		}
	}
}
*/

void GameObject::Update(float dt){
	for(uint i=0; i < components.size(); i++){
		if(components[i]->IsEnabled() ){
			components[i]->Update(dt);
		}
	}
}

void GameObject::LateUpdate(float dt){
	for(uint i=0; i < components.size(); i++){
		if(components[i]->IsEnabled() ){
			components[i]->LateUpdate(dt);
		}
	}
	clicked = false;
	released = false;
}

void GameObject::Render(void){
	for(uint i = 0; i < components.size(); i++){
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
	DEBUG_PRINT("\t GameObject::NotifyCollision called!");
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

void GameObject::RemoveComponent(Component* component){
	for(uint i = 0; i < components.size();i++){
		if(components[i] == component){
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}
	}
}

Component& GameObject::GetComponent(unsigned int type) const{
    DEBUG_PRINT("type: " << type);
	for(uint i = 0; i < components.size();i++){
		if(components[i]->Is(type)){
            DEBUG_PRINT("Component found");
			return *(components[i]);
		}
	}
	Error("Component not found!");
}

std::vector<Component *> GameObject::GetComponents(ComponentType type) const{
	std::vector<Component *> ret;
	for(uint i = 0; i < components.size();i++){
		if(components[i]->Is(type)){
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
	for(unordered_map<string, GameObject*>::iterator it = child.begin(); it != child.end(); it++){
        it->second->SetActive(newValue);
	}
}

bool GameObject::IsActive(void) const{
	return active;
}

void GameObject::SetPosition(Vec2 v){
    DEBUG_PRINT(tag << ": Ignoring parent to set position");
    DEBUG_PRINT("v:" << v.x << ", " << v.y);
    box.x = v.x;
    box.y = v.y;
}
void GameObject::SetPosition(int x, int y){
    DEBUG_PRINT(tag << ": Ignoring parent to set position");
    DEBUG_PRINT("v:" << x << ", " << y);
    box.x = x;
    box.y = y;
}


void GameObject::SetRelativePosition(Vec2 v){
    if(parent == nullptr){
        DEBUG_PRINT(tag << ": No parent");
        DEBUG_PRINT("v:" << v.x << ", " << v.y);
        box.x = v.x;
        box.y = v.y;
    }else{
        DEBUG_PRINT(tag << " parent is  " << parent->GetTag() );
        DEBUG_PRINT("v:" << v.x << ", " << v.y);
        parentRelative.x = v.x;
        parentRelative.y = v.y;
        box.x = parent->box.x + parentRelative.x;
        box.y = parent->box.y + parentRelative.y;
    }
}

void GameObject::SetRelativePosition(int x, int y){
    if(parent == nullptr){
        DEBUG_PRINT(tag << ": No parent");
        box.x = x;
        box.y = y;
    }else{
        DEBUG_PRINT(tag << " parent is  " << parent->GetTag() );
        parentRelative.x = x;
        parentRelative.y = y;
        box.x = parent->box.x + parentRelative.x;
        box.y = parent->box.y + parentRelative.y;
    }
}

void GameObject::CreateNewObject(GameObject* gameObject){
    if(context != nullptr){
        context->AddObject(gameObject);
    }else{
        DEBUG_PRINT("Trying to create new object without context!");
    }
}

State* GameObject::GetContext(){
    return context;
}



#include "Error_footer.h"
