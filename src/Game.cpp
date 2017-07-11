#include "Game.h"

#include <stdlib.h>
#include <time.h>

#include "Error.h"
#include "Resources.h"

Game* Game::instance = nullptr;

Game::Game(std::string title,int width, int height)
		: dt(0.0)
		, inputManager(InputManager::GetInstance()) {
		//, frameStart(SDL_GetTicks()) {
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("Compiled against SDL version %d.%d.%d " END_LINE, compiled.major, compiled.minor, compiled.patch);
	printf("Linked against SDL version %d.%d.%d" END_LINE, linked.major, linked.minor, linked.patch);

	frameStart = SDL_GetTicks();
	srand(time(NULL));
	
	if(nullptr != Game::instance) {
		Error("Second instantion of the game!");
	}
	Game::instance = this;

	if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)) {
		Error(SDL_GetError());
	}
	int result = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	
	if(0 == result) {
		Error("Loading IMG_Init failed: " << IMG_GetError());
	}
	
	if(0 == (result & IMG_INIT_JPG)) {
		Error("Loading IMG_INIT_JPG failed: " << IMG_GetError());
	}

	if(0 == (result & IMG_INIT_PNG)) {
		Error("Loading IMG_INIT_PNG failed: " << IMG_GetError());
	}

	if(0 == (result & IMG_INIT_TIF)) {
		Error("Loading IMG_INIT_TIF failed: " << IMG_GetError());
	}

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
	if(nullptr == window) {
		Error(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(nullptr == renderer) {
		Error(SDL_GetError());
	}

	result = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	if(0 == result) {
		Error("Loading Mix_Init failed: " << Mix_GetError());
	}

/*	if(0 == (result & MIX_INIT_MP3 ) ) {
		Error("Loading MIX_INIT_MP3 failed: " << Mix_GetError());
	}*/
	
	if(0 == (result & MIX_INIT_OGG)) {
		Error("Loading MIX_INIT_OGG failed: " << Mix_GetError());
	}

	if(0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, MIXER_CHUCK_SIZE)) {
		Error("Loading Mix_OpenAudio failed: " << Mix_GetError());
	}

	int numAllocatedChannels = Mix_AllocateChannels(1024);
	REPORT_DEBUG2(1, " Succesfully allocated " << numAllocatedChannels << " out of 1024 tried channels.");

	if(0 != TTF_Init()) {
		Error("Loading TTF_Init failed: " << TTF_GetError());
	}
	
	REPORT_I_WAS_HERE;
	storedState = nullptr;
	REPORT_I_WAS_HERE;
	capFramerate = true;
	maxFramerate = INITIAL_FRAMERATE;
	frameDuration = 1000.0/INITIAL_FRAMERATE;
}

Game::~Game() {
	IMG_Quit();
	if(nullptr != storedState) {
		delete storedState;
	}
	while(!stateStack.empty()) {
		stateStack.pop();
	}
	Resources::ClearResources();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

Game& Game::GetInstance(void) {
	return *(Game::instance);
}

State& Game::GetCurrentState(void) const {
	return *stateStack.top();
}

SDL_Renderer* Game::GetRenderer(void) const {
	return renderer;
}

void Game::Push(State* state) {
	if(nullptr != storedState) {
		std::cout << WHERE << "[WARNING]Não era para ter um state aqui...\n";
	}
	storedState = state;
	storedState->LoadAssets();
}

void Game::Run(void) {
	if(nullptr != storedState) {
		stateStack.push(std::unique_ptr<State>(storedState));
		storedState= nullptr;
	} else {
		// Jogo terminou
		return;
	}

	while(!stateStack.empty()) {
		if (capFramerate) {
			float timeRemaining = frameDuration - (SDL_GetTicks() - frameStart);
			if (0 < timeRemaining) {
				SDL_Delay(timeRemaining);
			}
		}
	
		if(stateStack.top()->QuitRequested()) {
			break;
		}
	
		CalculateDeltaTime();
		inputManager.Update();
		stateStack.top()->Update(GetDeltaTime());
		if(-1 == SDL_SetRenderDrawColor(renderer, CLEAR_COLOR)) {
			Error(SDL_GetError());
		}
		SDL_RenderClear(renderer);
		stateStack.top()->Render();
		SDL_RenderPresent(renderer);
		UpdateStack();
	}

	while(!stateStack.empty()) {
		stateStack.pop();
	}
	
	Resources::ClearResources();
}

void Game::CalculateDeltaTime(void) {
	u_int32_t newTick = SDL_GetTicks();
	// Converter de milissegundos para segundos
	dt = ((float)(newTick-frameStart))/1000.0;
	frameStart = newTick;
}

float Game::GetDeltaTime(void) const {
	return dt;
}

void Game::UpdateStack(void) {
	bool popped = false;
	if(stateStack.top()->PopRequested()) {
		stateStack.pop();
		Resources::ClearResources();
		popped = true;
	}

	if(nullptr != storedState) {
		if(!stateStack.empty()) {
			stateStack.top()->Pause();
		}
		stateStack.push(std::unique_ptr<State>(storedState));
		storedState = nullptr;
	}

	if(!stateStack.empty() && popped) {
		stateStack.top()->Resume();
	}
}

Vec2 Game::GetWindowDimensions(void) const {
	Vec2 ret;
	int x = 0;
	int y = 0;
	SDL_GetWindowSize(window, &x, &y);
	ret.x = x;
	ret.y = y;
	return ret;
}

void Game::SetMaxFramerate(signed long int newMaxFramerate) {
	REPORT_DEBUG("\tnewMaxFramerate= " << newMaxFramerate);
	if (newMaxFramerate < 1) {
		maxFramerate = 1;
	} else {
		maxFramerate = newMaxFramerate;
	}
	frameDuration = 1000.0/maxFramerate;
}

unsigned int Game::GetMaxFramerate(void) const {
	return maxFramerate;
}

float Game::GetCurrentFramerate(void) const {
	return 1.0/dt;
}

void Game::LimitFramerate(bool limit) {
	capFramerate = limit;
}

bool Game::IsFramerateLimited(void) const {
	return capFramerate;
}

SDL_Window* Game::GetWindow(void) const {
	return window;
}

void Game::SetWindowDimensions(Vec2 size){
	SDL_SetWindowSize(window, size.x, size.y);
	SetWindowCentered();
}

void Game::SetWindowFullscreen(bool isFullScreen){
	SDL_SetWindowFullscreen(window, isFullScreen ? SDL_WINDOW_FULLSCREEN : 0);
	SetWindowCentered();
}

void Game::SetWindowMaximized(void){
	SDL_MaximizeWindow(window);
	SetWindowCentered();
}

void Game::SetWindowBorderless(bool isBorderless){
	SDL_SetWindowBordered(window, isBorderless ? SDL_FALSE : SDL_TRUE);
	SetWindowCentered();
}

void Game::SetWindowCentered(void){
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool Game::GetWindowFullscreen(void) const{
	return SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
}

bool Game::GetWindowMaximized(void) const{
	return SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;
}

bool Game::GetWindowBorderless(void) const{
	return SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS;
}

unsigned int Game::GetTicks(void){
	return frameStart;
}
