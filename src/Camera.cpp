#include "Camera.h"

#include "Error.h"
#include "Game.h"
#include "InputManager.h"

#include <cmath>

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos = Vec2(0,0);
float Camera::minSpeed = CAMERA_DEFAULT_MIN_SPEED;
float Camera::maxSpeed = CAMERA_DEFAULT_MAX_SPEED;
float Camera::currentSpeed = CAMERA_DEFAULT_MIN_SPEED;
float Camera::currentLogZoom = 0.0;
float Camera::minLogZoom = CAMERA_DEFAULT_MIN_LOG_ZOOM;
float Camera::maxLogZoom = CAMERA_DEFAULT_MAX_LOG_ZOOM;
float Camera::logZoomSpeed = CAMERA_DEFAULT_LOG_ZOOM_SPEED;
bool Camera::zoomFixed = !CAMERA_DEFAULT_ZOOMABLE;

void Camera::Follow(GameObject* newFocus) {
	focus = newFocus;
}

void Camera::Unfollow(void) {
	focus = nullptr;
}

void Camera::Update(float dt) {
	if(nullptr != focus) {
		// Centrar o foco no centro da tela
		pos = ScreenToWorld(WorldToScreen((focus->box).Center()) - Game::GetInstance().GetWindowDimensions()*0.5);
	} else {
		// Normaliza o nível de zoom atual
		float zoomLevel = (currentLogZoom-minLogZoom)/(maxLogZoom-minLogZoom);
		// Interpola linearmente entre min e max baseado no nível de zoom
		float speed = zoomLevel*minSpeed + (1-zoomLevel)*maxSpeed;
		if(ActionManager::LeftArrowAction()) {
			pos.x -= speed*dt;
		}
		if(ActionManager::RightArrowAction()) {
			pos.x += speed*dt;
		}
		if(ActionManager::DownArrowAction()) {
			pos.y += speed*dt;
		}
		if(ActionManager::UpArrowAction()) {
			pos.y -= speed*dt;
		}
	}
	if(INPUT_MANAGER.IsMouseScrolling()){
		Camera::Zoom( (float)INPUT_MANAGER.MouseScroll().y );
	}
	if(INPUT_MANAGER.KeyPress('k')) {
		REPORT_DEBUG2(true," CameraPos("<<pos.x<<","<<pos.y<<") with log zoom of "<<currentLogZoom);
	}
}

void Camera::ForceLinearZoom(float newZoom) {
	currentLogZoom = std::log(newZoom)/std::log(CAMERA_LOG_ZOOM_BASE);
}

void Camera::ForceLogZoom(float newLogZoom) {
	currentLogZoom = newLogZoom;
}

void Camera::SetZoomable(bool zoomable) {
	zoomFixed = !zoomable;
}

void Camera::Zoom(float deltaZoom) {
	if(!zoomFixed) {
		Vec2 oldMousePos = ScreenToWorld(INPUT_MANAGER.GetMousePos());
		currentLogZoom += deltaZoom*logZoomSpeed;
		if(maxLogZoom < currentLogZoom) {
			currentLogZoom = maxLogZoom;
		} else if(minLogZoom > currentLogZoom) {
			currentLogZoom = minLogZoom;
		}
		if(nullptr == focus) {
			Vec2 newMousePos = ScreenToWorld(INPUT_MANAGER.GetMousePos());
			pos = pos + (oldMousePos - newMousePos);
		}
	}
}

void Camera::SetZoomLimits(float minZoom, float maxZoom) {
	Camera::minLogZoom = minZoom;
	Camera::maxLogZoom = maxZoom;
}

float Camera::GetLinearZoom(void) {
	return std::pow(CAMERA_LOG_ZOOM_BASE, currentLogZoom);
}

float Camera::GetLogZoom(void) {
	return currentLogZoom;
}

void Camera::SetZoomSpeed(float newZoomSpeed) {
	logZoomSpeed = newZoomSpeed;
}

Vec2 Camera::WorldToScreen(Vec2 world) {
	Vec2 screen = world-pos;
	float currentZoom = GetLinearZoom();
	screen.x *= currentZoom;
	screen.y *= currentZoom;
	return screen;
}

Rect Camera::WorldToScreen(Rect world) {
	Rect screen;
	float currentZoom = GetLinearZoom();
	screen.x = (world.x-pos.x)*currentZoom;
	screen.y = (world.y-pos.y)*currentZoom;
	screen.w = world.w*currentZoom;
	screen.h = world.h*currentZoom;
	return screen;
}

Vec2 Camera::ScreenToWorld(Vec2 screen) {
	Vec2 world;
	float currentZoom = GetLinearZoom();
	world.x = screen.x/currentZoom;
	world.y = screen.y/currentZoom;
	world = world+pos;
	return world;
}

Rect Camera::ScreenToWorld(Rect screen) {
	Rect world;
	float currentZoom = GetLinearZoom();
	world.x = (screen.x/currentZoom)+pos.x;
	world.y = (screen.y/currentZoom)+pos.y;
	world.w = screen.w/currentZoom;
	world.h = screen.h/currentZoom;
	return world;
}
	
void Camera::SetSpeedLimits(float minSpeed, float maxSpeed) {
	Camera::minSpeed = minSpeed;
	Camera::maxSpeed = maxSpeed;	
}

float Camera::GetMinSpeed(void) {
	return minSpeed;
}

float Camera::GetMaxSpeed(void) {
	return maxSpeed;
}
