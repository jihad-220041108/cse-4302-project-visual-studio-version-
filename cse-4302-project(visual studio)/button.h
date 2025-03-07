#pragma once
#include "raylib.h"
#include "sound.h"

class Button {
public:
    Button(const char* imagePath, Vector2 imagePosition, float scale);
    ~Button();
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);
    bool isMouseOver(Vector2 mousePos);
    bool playSound = true;
    sound swipe, click;
    void stopSound();
private:
    Texture2D texture;
    Vector2 position;
};
