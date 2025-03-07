#pragma once
#include<raylib.h>

class sound {
private:
    Sound sound_texture;
public:
    sound(const char* path);
    sound();
    void setSound(const char* path, float soundVolume);
    void playSound();
    ~sound();
};

