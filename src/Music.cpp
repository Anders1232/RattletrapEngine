#include "Music.h"

Music::Music() {
	music= nullptr;
}

Music::Music(string file) {
	Open(file);
//	music= Resources::GetMusic(file);
}

void Music::Play(int times) {
//	TEMP_REPORT_I_WAS_HERE;
	Mix_PlayMusic(music.get(), times-1);
}

void Music::Stop(void) {
	Mix_FadeOutMusic(MUSIC_FADE_OUT_TIME_MSEC);
}

void Music::Open(string file) {
	music= Resources::GetMusic(file);
}

bool Music::IsOpen(void)const {
	return music != nullptr;
}
