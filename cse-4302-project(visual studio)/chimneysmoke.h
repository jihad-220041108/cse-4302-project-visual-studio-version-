#pragma once
#include <raylib.h>
class chimneysmoke
{
public:
	Texture2D spriteSheet;
	Rectangle sourceRect;     // Rectangle for selecting frame from sprite sheet
	Rectangle destRect;       // Rectangle for drawing the sprite
	Vector2 position;         // Position where the sprite will be drawn
	int frameCount;           // Total number of frames in the animation
	int currentFrame;         // Current frame being displayed
	int image_width;
	int image_height;
	float frameTime;          // Time between frames
	float elapsedTime;        // Time accumulator for switching frames
	float image_widner;

	chimneysmoke(const char* imagePath, int totalFrames, float image_width,float image_height,int fps,float config, Vector2 pos);
	void Update();
	void Draw()const;                
	~chimneysmoke();
};

