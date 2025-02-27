#pragma once
#include <raylib.h>
#include "charactersprite.h"
class enemysprite : public playersprite{
public:
	enemysprite(const char* imagePath, int totalFrames, float fps, Vector2 pos) : playersprite(imagePath, totalFrames, fps, pos) {}
	/*void Follow(Vector2 heroPos);*/
};