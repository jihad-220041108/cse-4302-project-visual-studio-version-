#pragma once
#include "charactersprite.h"

class enemysprite : public charactersprite {
private:
    Texture2D right = LoadTexture("characters/gamecharacters/enemywalking.png");
    Texture2D left = LoadTexture("characters/gamecharacters/enemywalking_left.png");
    Texture2D attackRight = LoadTexture("characters/gamecharacters/enemyattack.png");
    Texture2D attackLeft = LoadTexture("characters/gamecharacters/enemyattack_left.png");

    bool isAlive = true;  // Track whether the enemy is alive or not



public:
    enemysprite(const char* imagePath, int totalFrames, float fps, Vector2 pos)
        : charactersprite(imagePath, totalFrames, fps, pos) {}

    void assignTexture(string dir, float totalFrame, float fps) {
        frameCount = totalFrame;
        frameTime = 1.0f / fps;

        if (dir == "right") {
            spriteSheet = right;
        }
        else if (dir == "left") {
            spriteSheet = left;
        }
        else if (dir == "attackRight") {
            spriteSheet = attackRight;
        }
        else if (dir == "attackLeft") {
            spriteSheet = attackLeft;
        }
    }

    void setAlive(bool alive) {
        isAlive = alive;
    }

    bool getAlive() const {
        return isAlive;
    }
};
