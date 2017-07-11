#include "ActionManager.h"


bool ActionManager::LeftArrowAction(){

	return (InputManager::GetInstance().IsKeyDown('a') 
		 || InputManager::GetInstance().IsKeyDown('A') 
		 || InputManager::GetInstance().IsKeyDown(SDLK_LEFT) 
		 || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		 || InputManager::GetInstance().GetControllerLeftStickState().x < 0);

}

bool ActionManager::RightArrowAction(){

	return (InputManager::GetInstance().IsKeyDown('d') 
		 || InputManager::GetInstance().IsKeyDown('D') 
		 || InputManager::GetInstance().IsKeyDown(SDLK_RIGHT) 
		 || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		 || InputManager::GetInstance().GetControllerLeftStickState().x > 0);

}

bool ActionManager::UpArrowAction(){

	return (InputManager::GetInstance().IsKeyDown('w') 
		 || InputManager::GetInstance().IsKeyDown('W') 
		 || InputManager::GetInstance().IsKeyDown(SDLK_UP) 
		 || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP)
		 || InputManager::GetInstance().GetControllerLeftStickState().y < 0);

}

bool ActionManager::DownArrowAction(){

	return (InputManager::GetInstance().IsKeyDown('s') 
		 || InputManager::GetInstance().IsKeyDown('S') 
		 || InputManager::GetInstance().IsKeyDown(SDLK_DOWN) 
		 || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		 || InputManager::GetInstance().GetControllerLeftStickState().y > 0);

}

bool ActionManager::EscapeAction(){

	return (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE) || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_BACK));

}

bool ActionManager::StartAction(){

	return (InputManager::GetInstance().IsKeyDown(SDLK_SPACE) || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_START));

}

bool ActionManager::RightShoulderAction(){

	return (InputManager::GetInstance().IsKeyDown(SDL_BUTTON_RIGHT) || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));

}

bool ActionManager::LeftShoulderAction(){

	return (InputManager::GetInstance().IsKeyDown(SDL_BUTTON_LEFT) || InputManager::GetInstance().IsButtonDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));

}
