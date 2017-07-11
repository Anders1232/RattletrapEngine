#include "InputManager.h"
#include "string.h"
#include "Error.h"
//#include "Camera.h"

InputManager::InputManager():quitRequested(false), updateCounter(0), mouseX(0), mouseY(0), mouseScroolUpdate(0) {
	memset(mouseState, 0, 6*sizeof(bool));
	memset(mouseUpdate, 0, 6*sizeof(int));
}

InputManager::~InputManager() {
}

InputManager& InputManager::GetInstance(void) {
	static InputManager inputManager;
	return inputManager;
}

void InputManager::Update(){
	SDL_Event event;
	updateCounter ++;
	quitRequested = false;
	// Obtenha as coordenadas do mouse
	SDL_GetMouseState(&mouseX, &mouseY);
	// SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
	while (SDL_PollEvent(&event)){
		// Se o evento for quit, setar a flag para terminação
		if( SDL_QUIT == event.type){
			quitRequested = true;
		}
		// Se o evento for clique...
		else if(SDL_MOUSEBUTTONDOWN == event.type){
			mouseState[event.button.button] = true;
			mouseUpdate[event.button.button] = updateCounter;
			
		}
		else if(SDL_MOUSEBUTTONUP == event.type ) {
			
			mouseState[event.button.button] = false;
			mouseUpdate[event.button.button] = updateCounter;
		}
		else if(SDL_MOUSEWHEEL == event.type){
			mouseScroolState= Vec2(event.wheel.x, event.wheel.y);
			mouseScroolUpdate= updateCounter;
		}
		else if(SDL_KEYDOWN == event.type){
			if(!event.key.repeat){
				keyState[event.key.keysym.sym] = true;
				keyUpdate[event.key.keysym.sym] = updateCounter;
			}
		}
		else if(SDL_KEYUP == event.type ){
			keyState[event.key.keysym.sym] = false;
			keyUpdate[event.key.keysym.sym] = updateCounter;
		}
		else if(SDL_CONTROLLERDEVICEADDED == event.type ){
			std::cout <<" reconheceu controle" << std::endl;
			if( SDL_IsGameController(event.cdevice.which)){
				//Adiciona o controle
				SDL_GameController *pad = SDL_GameControllerOpen(event.cdevice.which);
				padToController[event.cdevice.which] = pad;
			}
		}
		else if(SDL_CONTROLLERDEVICEREMOVED == event.type ){
			try{
				SDL_GameController *pad = padToController.at(event.cdevice.which);
				SDL_GameControllerClose(pad);
				padToController.erase(event.cdevice.which);
			}
			catch(const std::out_of_range& oor){
				Error("Tentou retirar controle que não existe?!?");
			}
		}
		else if(SDL_CONTROLLERBUTTONDOWN == event.type){
			controllerState[event.cbutton.button] = true;
			controllerUpdate[event.cbutton.button] = updateCounter;
		}
		else if(SDL_CONTROLLERBUTTONUP == event.type ){
			controllerState[event.cbutton.button] = false;
			controllerUpdate[event.cbutton.button] = updateCounter;
		}
		else if(SDL_CONTROLLERAXISMOTION == event.type ){			
			if(SDL_CONTROLLER_AXIS_TRIGGERLEFT == event.caxis.axis ){
				controllerState[SDL_CONTROLLER_AXIS_TRIGGERLEFT] = true;
				controllerUpdate[SDL_CONTROLLER_AXIS_TRIGGERLEFT] = updateCounter;
			}
			else if(SDL_CONTROLLER_AXIS_TRIGGERRIGHT == event.caxis.axis ){
				controllerState[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] = true;
				controllerUpdate[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] = updateCounter;
			}
			else if(SDL_CONTROLLER_AXIS_INVALID != event.caxis.axis ){
				SDL_GameController *pad = padToController.at(event.cdevice.which);
				int16_t StickLeftX = SDL_GameControllerGetAxis(pad,SDL_CONTROLLER_AXIS_LEFTX);
				int16_t StickLeftY = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);
				int16_t StickRightX = SDL_GameControllerGetAxis(pad,SDL_CONTROLLER_AXIS_RIGHTX);
				int16_t StickRightY = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY);
				controllerLeftStickState = Vec2((float)StickLeftX,(float)StickLeftY);
				controllerRightStickState = Vec2((float)StickRightX,(float)StickRightY);
				controllerStickUpdate = updateCounter;
			}
		}
	}
}

bool InputManager::KeyPress(int key) const{
	try{
		return(keyUpdate.at(key) == updateCounter && keyState.at(key));
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::KeyRelease(int key) const{
	try{
		return(keyUpdate.at(key) != updateCounter && !keyState.at(key));
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::IsKeyDown(int key) const{
	try{
		return(keyState.at(key) == true);
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::IsKeyUp(int key) const{
	try{
		return(keyState.at(key) == false);
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::ButtonPress(int button) const{
	try{
		return(controllerUpdate.at(button) == updateCounter && controllerState.at(button));
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::ButtonRelease(int button) const{
	try{
		return(controllerUpdate.at(button) != updateCounter && !controllerState.at(button));
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::IsButtonDown(int button) const{
	try{
		return(controllerState.at(button) == true);
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

bool InputManager::IsButtonUp(int button) const{
	try{
		return(controllerState.at(button) == false);
	}
	catch (const std::out_of_range& oor){
		return false;
	}
}

Vec2 InputManager::GetControllerLeftStickState() const{
	return controllerLeftStickState;
}

Vec2 InputManager::GetControllerRightStickState() const{
		return controllerRightStickState;
}

bool InputManager::IsControllerSticking(void) const{
	return (controllerStickUpdate == updateCounter);
}

bool InputManager::MousePress(int button) const{
	return (mouseUpdate[button] == updateCounter && mouseState[button]);
}

bool InputManager::MouseRelease(int button) const{
	return (mouseUpdate[button] == updateCounter && !mouseState[button]);
}

bool InputManager::IsMouseDown(int button) const{
	return (true == mouseState[button] );
}

bool InputManager::IsMouseUp(int button) const{
	return (false == mouseState[button] );
}

int InputManager::GetMouseX() const{
	return(mouseX);
}

int InputManager::GetMouseY() const{
	return(mouseY);
}

bool InputManager::QuitRequested() const{
	return(quitRequested);
}

Vec2 InputManager::GetMousePos() const{
	return Vec2(mouseX, mouseY);
}

Vec2 InputManager::MouseScroll(void) const{
	return mouseScroolState;
}

bool InputManager::IsMouseScrolling(void) const{
	return (mouseScroolUpdate == updateCounter);
}
