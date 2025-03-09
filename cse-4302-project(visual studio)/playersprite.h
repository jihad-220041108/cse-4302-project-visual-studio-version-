#pragma once
#include "charactersprite.h"

class playersprite : public charactersprite {
private:
	Texture2D south = LoadTexture("characters/gamecharacters/idle-S.png");
	Texture2D east = LoadTexture("characters/gamecharacters/idle-se.png");
	Texture2D west = LoadTexture("characters/gamecharacters/idle-west.png");
	Texture2D north = LoadTexture("characters/gamecharacters/idle-n.png");
	Texture2D walkNorth = LoadTexture("characters/gamecharacters/walk-n.png");
	Texture2D walkSouth = LoadTexture("characters/gamecharacters/walk-s.png");
	Texture2D walkEast = LoadTexture("characters/gamecharacters/walk.png");
	Texture2D walkWest = LoadTexture("characters/gamecharacters/walk-west.png");
	Texture2D westAxe = LoadTexture("characters/gamecharacters/axe-west.png");
	Texture2D attack = LoadTexture("characters/gamecharacters/attack.png");
	Texture2D attackLeft = LoadTexture("characters/gamecharacters/attack-left.png");
	Texture2D carryEast = LoadTexture("characters/gamecharacters/carry-east.png");
	Texture2D carryWest = LoadTexture("characters/gamecharacters/carry-west.png");
	Texture2D carryIdle = LoadTexture("characters/gamecharacters/carry-idle.png");
	Texture2D carryIdleEast = LoadTexture("characters/gamecharacters/carry-idle-east.png");

public:
    playersprite(const char* imagePath, int totalFrames, float fps, Vector2 pos)
        : charactersprite(imagePath, totalFrames, fps, pos) {}

    // Additional player-specific logic can go here
	void assignTexture(string dir, float totalFrame, float fps) {
		frameCount = totalFrame;
		frameTime = 1.0f / fps;

		if (dir == "south") {
			spriteSheet = south;
		}
		else if (dir == "east") {
			spriteSheet = east;
		}
		else if (dir == "west") {
			spriteSheet = west;
		}
		else if (dir == "north") {
			spriteSheet = north;
		}
		else if (dir == "walkNorth") {
			spriteSheet = walkNorth;
		}
		else if (dir == "walkSouth") {
			spriteSheet = walkSouth;
		}
		else if (dir == "walkEast") {
			spriteSheet = walkEast;
		}
		else if (dir == "walkWest") {
			spriteSheet = walkWest;
		}
		else if (dir == "westAxe") {
			spriteSheet = westAxe;
		}
		else if (dir == "attack") {
			spriteSheet = attackLeft;
		}
		else if (dir == "attackLeft") {
			spriteSheet = attack;
		}
		else if (dir == "carryEast") {
			spriteSheet = carryEast;
		}
		else if (dir == "carryWest") {
			spriteSheet = carryWest;
		}
		else if (dir == "carryIdle") {
			spriteSheet = carryIdle;
		}
		else if (dir == "carryIdleEast") {
			spriteSheet = carryIdleEast;
		}
	}
};
