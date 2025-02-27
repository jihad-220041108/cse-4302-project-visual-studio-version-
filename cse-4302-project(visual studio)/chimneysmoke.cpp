#include "chimneysmoke.h"

chimneysmoke::chimneysmoke(const char* imagePath, int totalFrames, float image_width, float image_height, int fps, float config, Vector2 pos)
{
	spriteSheet = LoadTexture(imagePath);
	frameCount = totalFrames;
	currentFrame = 0;
	elapsedTime = 0.0f;
	position = pos;
	this->image_width = image_width;
	this->image_height = image_height;
	frameTime = 1.0f / fps;
	sourceRect = { 0.0f, 0.0f, (float)image_width, (float)image_height };
	destRect = { position.x, position.y, (float)image_width*config, (float)image_height*config};
}


void chimneysmoke::Update()
{
	elapsedTime += GetFrameTime();
	if (elapsedTime >= frameTime)
	{
		currentFrame = (currentFrame + 1) % frameCount;
		sourceRect.x = currentFrame * (float)image_width;
		elapsedTime = 0.0f;
	}
}

void chimneysmoke::Draw()const
{
	DrawTexturePro(spriteSheet, sourceRect, destRect, { 0,0 }, 0, WHITE);
}

chimneysmoke::~chimneysmoke()
{
	UnloadTexture(spriteSheet);
}
