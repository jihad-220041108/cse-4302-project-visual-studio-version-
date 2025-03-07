#include "sound.h"

sound::sound(const char* path)
{
    sound_texture = LoadSound(path);
    SetSoundVolume(sound_texture, 1.5);
}

sound::sound() {}

void sound::setSound(const char* path, float soundVolume)
{
	sound_texture = LoadSound(path);
	SetSoundVolume(sound_texture, soundVolume);
}

void sound::playSound()
{
    PlaySound(sound_texture);
}

sound::~sound()
{
    UnloadSound(sound_texture);
}