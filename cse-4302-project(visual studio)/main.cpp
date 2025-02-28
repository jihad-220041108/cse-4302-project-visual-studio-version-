#include <raylib.h>
#include <bits/stdc++.h>
#include "mainmenuspirit.h"
#include "button.h"
#include "sound.h"
#include "charactersprite.h"
#include "chimneysmoke.h"
#include "enemysprite.h"
using namespace std;

enum class GameScreen
{
	MainMenu,
	Game,
	GameOver
};

float playerSpeed = 3.0f;
Camera2D gamecamera;
int framcount = 0;
int playerframe = 0;
Rectangle sourcerectidle = { 0, 0, 64, 64 };
Rectangle sourcerect = { 96, 0, 64, 64 };
Rectangle destrect = { 224, 287, 128, 128 };
Rectangle destrect2 = { 350,287,128,128 };
Rectangle enemy_temp = { 350,287,32,32};
Rectangle collsionrect;
vector<Rectangle> builiding_rect{ {40, 15, 45, 65},{140, 40, 45, 65} };// for building rect
template <typename T>
T Clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
// Function to handle collision between player and buildings
void ResolvePlayerBuildingCollision(Rectangle& destrect, const vector<Rectangle>& buildings)
{
	// Loop through all buildings to check for collision
	for (const auto& building : buildings)
	{
		// Calculate the overlap between player and building
		float overlapX = (destrect.x + destrect.width / 2) - (building.x + building.width / 2);
		float overlapY = (destrect.y + destrect.height / 2) - (building.y + building.height / 2);

		// Half-widths and half-heights of the player and building
		float halfWidthSum = (destrect.width + building.width) / 2;
		float halfHeightSum = (destrect.height + building.height) / 2;

		// Check for overlap (collision detection)
		if (fabs(overlapX) < halfWidthSum && fabs(overlapY) < halfHeightSum)
		{
			// Calculate penetration distances
			float penetrationX = halfWidthSum - fabs(overlapX);
			float penetrationY = halfHeightSum - fabs(overlapY);

			// Resolve collision by moving player back based on smaller penetration distance
			if (penetrationX < penetrationY)
			{
				// Resolve X-axis collision
				if (overlapX > 0)
				{
					destrect.x += penetrationX;  // Move player to the right
				}
				else
				{
					destrect.x -= penetrationX;  // Move player to the left
				}
			}
			else
			{
				// Resolve Y-axis collision
				if (overlapY > 0)
				{
					destrect.y += penetrationY;  // Move player down
				}
				else
				{
					destrect.y -= penetrationY;  // Move player up
				}
			}
		}
	}
}
void enemyCollision(Rectangle& destrect, Rectangle building)
{
	// Loop through all buildings to check for collision
		// Calculate the overlap between player and building
	float overlapX = (destrect.x + destrect.width / 2) - (building.x + building.width / 2);
	float overlapY = (destrect.y + destrect.height / 2) - (building.y + building.height / 2);

	// Half-widths and half-heights of the player and building
	float halfWidthSum = (destrect.width + building.width) / 2;
	float halfHeightSum = (destrect.height + building.height) / 2;

	// Check for overlap (collision detection)
	if (fabs(overlapX) < halfWidthSum && fabs(overlapY) < halfHeightSum)
	{
		// Calculate penetration distances
		float penetrationX = halfWidthSum - fabs(overlapX);
		float penetrationY = halfHeightSum - fabs(overlapY);

		// Resolve collision by moving player back based on smaller penetration distance
		if (penetrationX < penetrationY)
		{
			// Resolve X-axis collision
			if (overlapX > 0)
			{
				destrect.x += penetrationX;  // Move player to the right
			}
			else
			{
				destrect.x -= penetrationX;  // Move player to the left
			}
		}
		else
		{
			// Resolve Y-axis collision
			if (overlapY > 0)
			{
				destrect.y += penetrationY;  // Move player down
			}
			else
			{
				destrect.y -= penetrationY;  // Move player up
			}
		}
	}
}
bool CanMove(Rectangle newPos, Rectangle enemy) {
	return !CheckCollisionRecs(newPos, enemy); // Returns true if no collision, false otherwise
}
void Follow(Vector2 heroPos) {
	float dx = heroPos.x - destrect2.x;
	float dy = heroPos.y - destrect2.y;
	float distance = sqrt(dx * dx + dy * dy);

	if (distance > 40.0f) {
		destrect2.x += (dx / distance) * 1.5;
		destrect2.y += (dy / distance) * 1.5;
		/*collisionRect.x = position.x;
		collisionRect.y = position.y;*/
	}
}
int main()
{

	GameScreen currentScreen = GameScreen::MainMenu;
	bool exit = false;
	const int windowWidth = GetScreenWidth();
	const int windowHeight = GetScreenHeight();

	InitWindow(windowWidth, windowHeight, "OOP Sprite Animation");
	ToggleFullscreen();

	playersprite idleSpriteS("characters/gamecharacters/idle-S.png", 4, 20.0f, { 0, 0 });
	playersprite idleSpriteE("characters/gamecharacters/idle-se.png", 4, 20.0f, { 0, 0 });
	playersprite idleSpriteW("characters/gamecharacters/idle-west.png", 8, 20.0f, { 0, 0 });
	playersprite idleSpriteN("characters/gamecharacters/idle-n.png", 4, 20.0f, { 0, 0 });
	playersprite walkSpriteN("characters/gamecharacters/walk-n.png", 8, 30.0f, { 0, 0 });
	playersprite walkSpriteS("characters/gamecharacters/walk-s.png", 8, 25.0f, { 0, 0 });
	playersprite walkSpriteE("characters/gamecharacters/walk.png", 8, 30.0f, { 0, 0 });
	playersprite walkSpriteW("characters/gamecharacters/walk-west.png", 8, 30.0f, { 0, 0 });
	playersprite westaxe("characters/gamecharacters/axe-west.png", 10, 30.0f, { 0, 0 });
	playersprite axe1("characters/gamecharacters/axe1.png", 10, 30.0f, { 0, 0 });
	playersprite axe2("characters/gamecharacters/axe2.png", 10, 30.0f, { 0, 0 });
	playersprite axe3("characters/gamecharacters/axe3.png", 10, 30.0f, { 0, 0 });
	playersprite axe4("characters/gamecharacters/axe4.png", 10, 30.0f, { 0, 0 });
	playersprite attack("characters/gamecharacters/attack.png", 10, 30.0f, { 0, 0 });
	playersprite attack_left("characters/gamecharacters/attack-left.png", 10, 30.0f, { 0, 0 });
	//playersprite enemy("characters/gamecharacters/enemywalking.png", 8, 30.0f, { 0,0 });
	enemysprite enemy("characters/gamecharacters/enemywalking.png", 8, 30.0f, { 0,0 });
	Texture  mapTexturelayer2;
	chimneysmoke smoke1("characters/map/smoke1.png", 30, 15, 37, 25, 1.0, { 775, 177 });
	chimneysmoke smoke2("characters/map/smoke2.png", 30, 10, 30, 25, 1.0, { 845, 300 });
	chimneysmoke smoke3("characters/map/smoke3.png", 30, 18, 21, 25, 1.0, { 1125, 320 });
	chimneysmoke smoke4("characters/map/smoke4.png", 30, 20, 32, 25, 1.0, { 1185, 110 });
	mapTexturelayer2 = LoadTexture("characters/map/game.png");

	SpriteAnimation menuBackground("images/new2.png", 38, 38.0f, { 0, 1 });
	InitAudioDevice();
	Button startButton{ "buttons/start.png", {0, 500}, 0.65 };
	Button exitButton{ "buttons/exit.png", {0, 650}, 0.65 };
	sound click("sounds/button.mp3");
	SetTargetFPS(60);
	playersprite currentIdleSprite = idleSpriteS;

	// Define map boundaries based on the map dimensions
	const float mapWidth = 1408.0f;
	const float mapHeight = 512.0f;
	const float player_widht = destrect.width;
	const float player_height = destrect.height;
	while (!WindowShouldClose() && exit == false)
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (currentScreen == GameScreen::MainMenu)
		{
			menuBackground.Update();
			menuBackground.Draw();
			startButton.Draw();
			exitButton.Draw();
			if (startButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
			{
				click.playSound();
				currentScreen = GameScreen::Game;
			}
			if (exitButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
			{
				click.playSound();
				exit = true;
			}
		}
		else if (currentScreen == GameScreen::Game)
		{
			bool playerMoving = false;
			bool isclickedright = true;
			bool playermovingup = false, playermovingdown = false, playermovingleft = false, playermovingright = false;
			if (IsKeyDown(KEY_W))
			{
				playerMoving = playermovingup = true;
				destrect.y -= playerSpeed;
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				currentIdleSprite = idleSpriteN;
				currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_S))
			{
				playerMoving = playermovingdown = true;
				destrect.y += playerSpeed;
				currentIdleSprite = idleSpriteS;
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_A))
			{
				playerMoving = playermovingleft = true;
				destrect.x -= playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth);
				currentIdleSprite = idleSpriteW;
				currentIdleSprite.Reset();
				isclickedright = false;
			}
			else if (IsKeyDown(KEY_D))
			{
				playerMoving = playermovingright = true;
				destrect.x += playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth - 32);
				currentIdleSprite = idleSpriteE;
				currentIdleSprite.Reset();
			}
			collsionrect = { destrect.x + 15, destrect.y - 16,32, 32 };
			ResolvePlayerBuildingCollision(collsionrect, builiding_rect);
			enemy_temp.x = destrect2.x;
			enemy_temp.y = destrect2.y;
			collsionrect.x -= 15;
			collsionrect.y += 16;
			enemyCollision(collsionrect,enemy_temp);
			destrect.x = collsionrect.x;
			destrect.y = collsionrect.y;
			if (IsKeyDown(KEY_M))
				currentScreen = GameScreen::MainMenu;

			// Update camera position based on player position
			gamecamera.target = { destrect.x + destrect.width / 2, destrect.y + destrect.height / 2 };
			gamecamera.offset = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) };
			gamecamera.rotation = 0.0f;
			gamecamera.zoom = 3.0f;

			// Calculate camera bounds to restrict movement
			float halfScreenWidth = (GetScreenWidth() / 2) / gamecamera.zoom;
			float halfScreenHeight = (GetScreenHeight() / 2) / gamecamera.zoom;

			// Clamp camera target to stay within map boundaries
			gamecamera.target.x = Clamp(gamecamera.target.x, halfScreenWidth, mapWidth - halfScreenWidth);
			gamecamera.target.y = Clamp(gamecamera.target.y, halfScreenHeight, mapHeight - halfScreenHeight);

			BeginMode2D(gamecamera);
			DrawTexture(mapTexturelayer2, 0, 0, WHITE);
			DrawRectangleLines(40, 15, 45, 65, RED);
			DrawRectangleLines(140, 40, 45, 65, RED);
			if (IsKeyDown(KEY_F))
			{
				if (!playerMoving)
				{
					if (IsKeyPressed(KEY_A))
					{
						attack_left.Update();
						attack_left.Draw(destrect);
					}
					else
					{
						attack.Update();
						attack.Draw(destrect);
					}
				}
				else 
				{
					if (playermovingleft)
					{
						attack_left.Update();
						attack_left.Draw(destrect);
					}
					else
					{
						attack.Update();
						attack.Draw(destrect);
					}
				}
			}
			else if (playerMoving)
			{
				if (playermovingup)
				{

					walkSpriteN.Update();
					walkSpriteN.Draw(destrect);
				}
				else if (playermovingright)
				{

					walkSpriteE.Update();
					walkSpriteE.Draw(destrect);
				}
				else if (playermovingleft)
				{

					walkSpriteW.Update();
					walkSpriteW.Draw(destrect);
				}
				else
				{

					walkSpriteS.Update();
					walkSpriteS.Draw(destrect);
				}
			}
			else
			{

				currentIdleSprite.Update();
				currentIdleSprite.Draw(destrect);
			}
			Follow({ destrect.x,destrect.y });
			DrawRectangleLines(destrect.x + 15, destrect.y - 16, 32, 32, RED);
			DrawRectangleLines(destrect2.x + 15, destrect2.y - 16, 32, 32, RED);
			//DrawRectangle(destrect2.x, destrect2.y, 128, 128, RED);

			enemy.Update();
			enemy.Draw(destrect2);
			smoke1.Update();
			smoke1.Draw();
			smoke2.Update();
			smoke2.Draw();
			smoke3.Update();
			smoke3.Draw();
			smoke4.Update();
			smoke4.Draw();
			cout << "X: " << destrect.x << " Y: " << destrect.y << endl;
			EndMode2D();
		}

		EndDrawing();
	}
	CloseWindow();

	return 0;
}
