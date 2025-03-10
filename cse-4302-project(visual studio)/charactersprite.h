#pragma once
#include <raylib.h>
#include <bits/stdc++.h>
#include "sound.h"
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
	Color color = WHITE;     // Tint color for the sprite
	//sound attacksound;       // Sound to play when attacking
    charactersprite(const char* imagePath, int totalFrames, float fps, Vector2 pos);
    virtual void Update();
    virtual void Draw(Rectangle destRect);  // Draw with the specified destination rectangle
    virtual void assignTexture(string dir, float totalFrame, float fps);
	void setColor(Color color);
    void Reset();  // Reset animation to the start
	void destroy();

    virtual ~charactersprite();
};
