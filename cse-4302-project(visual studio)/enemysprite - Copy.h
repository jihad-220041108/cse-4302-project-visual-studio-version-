#pragma once
#include "charactersprite.h"

class enemysprite : public charactersprite {
public:
    enemysprite(const char* imagePath, int totalFrames, float fps, Vector2 pos)
        : charactersprite(imagePath, totalFrames, fps, pos) {}

    // Additional enemy-specific logic can go here
};
