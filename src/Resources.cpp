#include "Resources.h"
#include "Error.h"
#include "Game.h"

std::unordered_map<string, std::shared_ptr<SDL_Texture>> Resources::imageTable;
std::unordered_map<string, std::shared_ptr<Mix_Music>> Resources::musicTable;
std::unordered_map<string, std::shared_ptr<Mix_Chunk>> Resources::soundTable;
std::unordered_map<string, std::shared_ptr<TTF_Font>> Resources::fontTable;
int Resources::musicVolume = 64;
int Resources::soundVolume = 16;

#define VOLUME_BOUND_ADJUST(x){\
		if(128<x){\
			x=128;\
		}\
		else if(0>x){\
			x=0;\
		}\
	}

std::shared_ptr<SDL_Texture> Resources::GetImage(string file) {
	SDL_Texture* ret;
	if(imageTable.end() == imageTable.find(file)) {
		ret=IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
		if(nullptr == ret) {
			Error(IMG_GetError());
		}
//		ASSERT(nullptr != ret);
		imageTable[file]= std::shared_ptr<SDL_Texture>
				(
					ret,
					[](SDL_Texture *texture) {
						SDL_DestroyTexture(texture);
					}
				);
	}
	return imageTable[file];
}

void Resources::ClearImages(void) {
	std::unordered_map<string, std::shared_ptr<SDL_Texture>>::iterator i= imageTable.begin();
	while(i != imageTable.end()) {
		if((*i).second.unique()) {
			i= imageTable.erase(i);
		}
		else {
			i++;
		}
	}
}

std::shared_ptr<Mix_Music> Resources::GetMusic(string file) {
	Mix_Music* ret;
	if(musicTable.end() == musicTable.find(file)) {
		ret=Mix_LoadMUS(file.c_str());
		if(nullptr == ret) {
			Error(Mix_GetError());
		}
		musicTable[file]= std::shared_ptr<Mix_Music>
				(
					ret,
					[](Mix_Music *music) {
						Mix_FreeMusic(music);
					}
				);
	}
	return musicTable[file];
}

void Resources::ClearResources(void) {
	ClearImages();
	ClearMusic();
	ClearSound();
	ClearFonts();
}

void Resources::ClearMusic(void) {
	std::unordered_map<string, std::shared_ptr<Mix_Music>>::iterator i= musicTable.begin();
	while(i != musicTable.end()) {
		if((*i).second.unique()) {
			i= musicTable.erase(i);
		}
		else {
			i++;
		}
	}
}

std::shared_ptr<Mix_Chunk> Resources::GetSound(string file) {
	Mix_Chunk* ret;
	if(soundTable.end() == soundTable.find(file)) {
		ret= Mix_LoadWAV(file.c_str());
		if(nullptr == ret) {
			Error(Mix_GetError());
		}
//		ASSERT(nullptr != ret);
		soundTable[file]= std::shared_ptr<Mix_Chunk>
				(
					ret,
					[](Mix_Chunk *chunck) {
						Mix_FreeChunk(chunck);
					}
				);
	}
	return soundTable[file];
}

std::shared_ptr<TTF_Font> Resources::GetFont(string file, int fontSize) {
	TTF_Font* ret;
	if(fontTable.end() == fontTable.find(file+std::to_string(fontSize))) {
		ret= TTF_OpenFont (file.c_str(), fontSize);
		if(nullptr == ret) {
			Error(TTF_GetError());
		}
		fontTable[file+std::to_string(fontSize)]= std::shared_ptr<TTF_Font>
				(
					ret,
					[](TTF_Font *font) {
						TTF_CloseFont (font);
					}
				);
	}
	return fontTable[file+std::to_string(fontSize)];
}

void Resources::ClearSound(void) {
	std::unordered_map<string, std::shared_ptr<Mix_Chunk>>::iterator i= soundTable.begin();
	while(i != soundTable.end()) {
		if((*i).second.unique()) {
			i= soundTable.erase(i);
		}
		else {
			i++;
		}
	}
}

void Resources::ClearFonts(void) {
	std::unordered_map<string, std::shared_ptr<TTF_Font>>::iterator i= fontTable.begin();
	while(i != fontTable.end()) {
		if((*i).second.unique()) {
			i= fontTable.erase(i);
		}
		else {
			i++;
		}
	}
}

void Resources::ChangeMusicVolume(int deltaVolume){
	musicVolume+= deltaVolume;
	VOLUME_BOUND_ADJUST(musicVolume);
	Mix_VolumeMusic(musicVolume);
}

void Resources::ChangeSoundVolume(int deltaVolume){
	soundVolume+= deltaVolume;
	REPORT_DEBUG("\tsoundVolume= " << soundVolume);
	VOLUME_BOUND_ADJUST(soundVolume);
	REPORT_DEBUG("\tsoundVolume= " << soundVolume);
	std::unordered_map<string, std::shared_ptr<Mix_Chunk>>::iterator i= soundTable.begin();
	while(i != soundTable.end()) {
		Mix_VolumeChunk(i->second.get(), soundVolume);
		i++;
	}
}

void Resources::ForceMusicVolume(int volume){
	REPORT_DEBUG("\tmusicVolume= " << musicVolume);
	musicVolume= volume;
	REPORT_DEBUG("\tmusicVolume= " << musicVolume);
	VOLUME_BOUND_ADJUST(musicVolume);
	REPORT_DEBUG("\tmusicVolume= " << musicVolume);
	Mix_VolumeMusic(musicVolume);
}

void Resources::ForceSoundVolume(int volume){
	REPORT_DEBUG("\tsoundVolume= " << soundVolume);
	soundVolume= volume;
	REPORT_DEBUG("\tsoundVolume= " << soundVolume);
	VOLUME_BOUND_ADJUST(soundVolume);
	REPORT_DEBUG("\tsoundVolume= " << soundVolume);
	std::unordered_map<string, std::shared_ptr<Mix_Chunk>>::iterator i= soundTable.begin();
	while(i != soundTable.end()) {
		Mix_VolumeChunk(i->second.get(), soundVolume);
		i++;
	}
}
/*
int Resources::GetMusicVolume(){
	return musicVolume;
}

int Resources::GetSoundVolume(){
	return soundVolume;
}
*/
