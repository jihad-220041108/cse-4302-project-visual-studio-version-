#include "charactersprite.h"

playersprite::playersprite(const char* imagePath, int totalFrames, float fps, Vector2 pos)
    : frameCount(totalFrames), currentFrame(0), elapsedTime(0.0f), position(pos)
{
    spriteSheet = LoadTexture(imagePath);
    frameTime = 1.0f / fps;

    sourceRect = { 0.0f, 0.0f, 64.0f, 64.0f };
}

void playersprite::Update()
{
    elapsedTime += GetFrameTime();

    if (elapsedTime >= frameTime)
    {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;

        sourceRect.x = static_cast<float>(currentFrame) * 96.0f; 
    }
}
//modified_here just delete the vec
void playersprite::Draw(Rectangle destRect)
{
    Vector2 origin = { destRect.width / 2, destRect.height / 2 };
    DrawTexturePro(spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);
    
}

void playersprite::Reset()
{
    currentFrame = 0;
    elapsedTime = 0.0f;
    sourceRect.x = 0.0f;
}

playersprite::~playersprite()
{
    UnloadTexture(spriteSheet);
}
