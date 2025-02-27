#include "button.h"
Button::Button(const char* imagePath, Vector2 imagePosition, float scale)
{
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = imagePosition;
    swipe = LoadSound("sounds/button_swipe.mp3");
    SetSoundVolume(swipe, 3.0f);
}

Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw()
{
    Vector2 mousePos = GetMousePosition();
    Color drawColor = isMouseOver(mousePos) ? GRAY : WHITE;
    DrawTextureV(texture, position, drawColor);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle buttonRect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    return CheckCollisionPointRec(mousePos, buttonRect) && mousePressed;
}

bool Button::isMouseOver(Vector2 mousePos)
{
    Rectangle buttonRect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    if (CheckCollisionPointRec(mousePos, buttonRect))
    {
        if (playSound)
        {
            playSound = false;
            PlaySound(swipe);
        }
    }
    else
    {
        playSound = true;
    }

    return CheckCollisionPointRec(mousePos, buttonRect);
}

void Button::stopSound()
{
    StopSound(swipe);
}