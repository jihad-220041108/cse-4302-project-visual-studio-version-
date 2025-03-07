#pragma once
#include <raylib.h>
#include <bits/stdc++.h>
using namespace std;

class charactersprite {
public:
    Texture2D spriteSheet;
    Rectangle sourceRect;    // Rectangle for selecting frame from sprite sheet
    Vector2 position;        // Position where the sprite will be drawn
    int frameCount;          // Total number of frames in the animation
    int currentFrame;        // Current frame being displayed
    float frameTime;         // Time between frames
    float elapsedTime;       // Time accumulator for switching frames

    charactersprite(const char* imagePath, int totalFrames, float fps, Vector2 pos);
    virtual void Update();
    virtual void Draw(Rectangle destRect);  // Draw with the specified destination rectangle
    void Reset();  // Reset animation to the start
    virtual ~charactersprite();
};
