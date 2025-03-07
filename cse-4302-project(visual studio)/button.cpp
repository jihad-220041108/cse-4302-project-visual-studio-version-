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
    swipe.setSound("sounds/button_swipe.mp3", 3.0f);
	click.setSound("sounds/button.mp3", 3.0f);
    //SetSoundVolume(swipe, 3.0f);
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
    bool x =  CheckCollisionPointRec(mousePos, buttonRect) && mousePressed;
	if (x)
	{
		click.playSound();
        return true;
	}

    return false;
}

bool Button::isMouseOver(Vector2 mousePos)
{
    Rectangle buttonRect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    if (CheckCollisionPointRec(mousePos, buttonRect))
    {
        if (playSound)
        {
            playSound = false;
            swipe.playSound();
        }
    }
    else
    {
        playSound = true;
    }

    return CheckCollisionPointRec(mousePos, buttonRect);
}