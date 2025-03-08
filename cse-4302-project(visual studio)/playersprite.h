#pragma once
#include "charactersprite.h"

class playersprite : public charactersprite {
public:
    playersprite(const char* imagePath, int totalFrames, float fps, Vector2 pos)
        : charactersprite(imagePath, totalFrames, fps, pos) {}

    // Additional player-specific logic can go here
};
