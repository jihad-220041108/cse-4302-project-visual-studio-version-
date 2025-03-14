#include <raylib.h>
#include <bits/stdc++.h>
#include "mainmenusprite.h"
#include "button.h"
#include "sound.h"
#include "playersprite.h"
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
//Rectangle sourcerectidle = { 0, 0, 64, 64 };
//Rectangle sourcerect = { 96, 0, 64, 64 };
Rectangle destrect = { 224, 287, 128, 128 };
Rectangle fishing = { 290, 428, 128, 128 };
Rectangle destrect2 = { 350,287,128,128 };
Rectangle enemy_temp = { 350,287,32,32};


// health bar for player----------------------------> make a class if possible Jehad / Labib, not Auvro
Rectangle carrot = { 793,35, 20, 20};
Rectangle carrot2 = { 480,490, 20, 20 };

// treasure__________________________________________
vector<Rectangle>treasures= { { 246, 131, 32, 32 } , { 600, 480, 32, 32 } ,{ 1138, 460, 32, 32 } ,{ 924, 139, 32, 32 } ,{ 1223, 281, 32, 32 } ,{ 1224,28, 32, 32 } ,{ 503, 178, 32, 32 } };


//treasure carry recs

Rectangle carry1_res = { 500, 299, 128, 128 };



// rectangle for statue
Rectangle statue = { 1262,280,200,150 };

Rectangle collsionrect;
vector<Rectangle> builiding_rect{ {40, 15, 45, 65},{140, 40, 45, 65},{265,45,45,60},{580, 190, 80, 60},{600, 134, 60, 60},{1210, 400, 200, 150}};// for building rect
template <typename T>
T Clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

bool CanMove(Rectangle newPos, Rectangle enemy) {
	return !CheckCollisionRecs(newPos, enemy);
}

// Function to handle collision between player and buildings
void ResolvePlayerBuildingCollision(Rectangle& destrect, const vector<Rectangle>& buildings)
{
	// Loop through all buildings to check for collision
	for (auto building : buildings)
	{
		//DrawRectangleLines(building.x, building.y, building.width, building.height, RED);

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

bool isEnemyAttacking = false;

// Modify your Follow function to include attack logic
void Follow(Vector2 heroPos) {
	float dx = heroPos.x - destrect2.x;
	float dy = heroPos.y - destrect2.y;
	float distance = sqrt(dx * dx + dy * dy);

	// Define the attack range
	const float attackRange = 40.0f;
	const float followRange = 500.0f;  // Maximum distance to follow player

	if (distance <= attackRange) {
		// Enemy is close enough to attack
		isEnemyAttacking = true;
		// You might want to stop movement while attacking
		// Or slow it down slightly
		//destrect2.x += (dx / distance) * 0.5;  // Slower movement during attack
		//destrect2.y += (dy / distance) * 0.5;
	}
	else if (distance <= followRange) {
		// Enemy is following but not close enough to attack
		isEnemyAttacking = false;
		destrect2.x += (dx / distance) * 1.5;
		destrect2.y += (dy / distance) * 1.5;
	}
	else {
		// Enemy is too far to follow or attack
		isEnemyAttacking = false;
	}
}


/// Main Function


int main()
{

	GameScreen currentScreen = GameScreen::MainMenu;
	bool exit = false;
	const int windowWidth = GetScreenWidth();
	const int windowHeight = GetScreenHeight();

	InitWindow(windowWidth, windowHeight, "OOP Sprite Animation");
	ToggleFullscreen();

	// all of the following fetch player sprite from the folder
	playersprite idleSpriteS("characters/gamecharacters/idle-S.png", 4, 10.0f, { 0, 0 });
	playersprite idleSpriteE("characters/gamecharacters/idle-se.png", 4, 10.0f, { 0, 0 });
	playersprite idleSpriteW("characters/gamecharacters/idle-west.png", 8, 20.0f, { 0, 0 });
	playersprite idleSpriteN("characters/gamecharacters/idle-n.png", 4, 10.0f, { 0, 0 });
	playersprite walkSpriteN("characters/gamecharacters/walk-n.png", 8, 30.0f, { 0, 0 });
	playersprite walkSpriteS("characters/gamecharacters/walk-s.png", 8, 25.0f, { 0, 0 });
	playersprite walkSpriteE("characters/gamecharacters/walk.png", 8, 30.0f, { 0, 0 });
	playersprite walkSpriteW("characters/gamecharacters/walk-west.png", 8, 30.0f, { 0, 0 });
	playersprite westaxe("characters/gamecharacters/axe-west.png", 10, 30.0f, { 0, 0 });
	playersprite attack("characters/gamecharacters/attack.png", 10, 25.0f, { 0, 0 });
	playersprite attack_left("characters/gamecharacters/attack-left.png", 10, 25.0f, { 0, 0 });
	// carry sprites for the treasure
	playersprite carry1("characters/gamecharacters/carry-1.png", 8, 30.0f, { 0, 0 });
	playersprite carry2("characters/gamecharacters/carry-2.png", 8, 30.0f, { 0, 0 });
	playersprite carry3("characters/gamecharacters/carry-3.png", 8, 30.0f, { 0, 0 });
	playersprite carry_west("characters/gamecharacters/carry-west.png", 8, 30.0f, { 0, 0 });
	// end of player sprite

	// all of the following fetch enemy sprite from the folder
	enemysprite enemy("characters/gamecharacters/enemywalking.png", 8, 30.0f, { 0,0 });
	enemysprite enemyleft("characters/gamecharacters/enemywalking_left.png", 8, 30.0f, { 0,0 });
	enemysprite enemyattack("characters/gamecharacters/enemyattack.png", 8, 17.0f, { 0,0 });
	enemysprite enemyattack_left("characters/gamecharacters/enemyattack_left.png", 8, 17.0f, { 0,0 });
	// end of enemy sprite

	// Other sprites
	Texture  mapTexturelayer2;
	chimneysmoke smoke1("characters/map/smoke1.png", 30, 15, 37, 25, 1.5, { 775, 156 });
	chimneysmoke smoke2("characters/map/smoke2.png", 30, 10, 30, 25, 1.5, { 843, 280 });
	chimneysmoke smoke3("characters/map/smoke3.png", 30, 18, 21, 25, 1.0, { 1125, 320 });
	chimneysmoke smoke4("characters/map/smoke4.png", 30, 20, 32, 25, 1.0, { 1185, 110 });
	playersprite fishing_left("characters/gamecharacters/casting-left.png", 17, 10.0f, { 0, 0 });


	//health carrot -----------------------------------------------------------------
	Image carrotImage = LoadImage("characters/gamecharacters/health.png");
	Texture2D health = LoadTextureFromImage(carrotImage);
	        
	// treasure ..............................................................................
	Image treasure = LoadImage("characters/gamecharacters/treasure.png");
	Texture2D treasureTexture = LoadTextureFromImage(treasure);

	mapTexturelayer2 = LoadTexture("characters/map/game.png");
	SpriteAnimation menuBackground("images/new2.png", 38, 38.0f, { 0, 1 });
	InitAudioDevice();
	Button startButton{ "buttons/start.png", {100, 480}, 0.85 };
	Button exitButton{ "buttons/exit.png", {100, 630}, 0.85 };
	SetTargetFPS(60);
	playersprite currentIdleSprite = idleSpriteS;

	// Define map boundaries based on the map dimensions
	const float mapWidth = 1408.0f;
	const float mapHeight = 512.0f;
	const float player_widht = destrect.width;
	const float player_height = destrect.height;


	// health bar for health
	bool healthbar1flag = true;
	bool healthbar2flag = true;

	//bool is carring the treasure

	bool carryingtreasure = false;

	// health bar for treasure
	vector<bool>treasureflag(7,true);

	// Main game loop
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
				currentScreen = GameScreen::Game;
			}
			if (exitButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
			{
				exit = true;
			}
		}
		else if (currentScreen == GameScreen::Game)
		{
			bool playerMoving = false;
			bool isclickedright = true;
			bool playermovingup = false, playermovingdown = false, playermovingleft = false, playermovingright = false;
			if (IsKeyDown(KEY_UP))
			{
				playerMoving = playermovingup = true;
				destrect.y -= playerSpeed;
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				currentIdleSprite = idleSpriteN;
				currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				playerMoving = playermovingdown = true;
				destrect.y += playerSpeed;
				currentIdleSprite = idleSpriteS;
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				playerMoving = playermovingleft = true;
				destrect.x -= playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth);
				currentIdleSprite = idleSpriteW;
				currentIdleSprite.Reset();
				isclickedright = false;
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				playerMoving = playermovingright = true;
				destrect.x += playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth - 32);
				currentIdleSprite = idleSpriteE;
				currentIdleSprite.Reset();
			}

			collsionrect = { destrect.x + 15, destrect.y - 16, 32, 32 };

			//collision with buildings and trees
			ResolvePlayerBuildingCollision(collsionrect, builiding_rect);

			enemy_temp.x = destrect2.x;
			enemy_temp.y = destrect2.y;
			collsionrect.x -= 15;
			collsionrect.y += 16;

			//checking collision with the enemy----------------------------------------------------
			enemyCollision(collsionrect,enemy_temp);
			ResolvePlayerBuildingCollision(enemy_temp, builiding_rect);
			destrect.x = collsionrect.x;
			destrect.y = collsionrect.y;

			if (IsKeyDown(KEY_M))
				currentScreen = GameScreen::MainMenu;

			// Update camera position based on player position
			gamecamera.target = { destrect.x + destrect.width / 2, destrect.y + destrect.height / 2 };
			gamecamera.offset = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) };
			gamecamera.rotation = 0.0f;
			gamecamera.zoom = 2.2f;

			// Calculate camera bounds to restrict movement
			float halfScreenWidth = (GetScreenWidth() / 2) / gamecamera.zoom;
			float halfScreenHeight = (GetScreenHeight() / 2) / gamecamera.zoom;

			// Clamp camera target to stay within map boundaries
			gamecamera.target.x = Clamp(gamecamera.target.x, halfScreenWidth, mapWidth - halfScreenWidth);
			gamecamera.target.y = Clamp(gamecamera.target.y, halfScreenHeight, mapHeight - halfScreenHeight);

			BeginMode2D(gamecamera);
			DrawTexture(mapTexturelayer2, 0, 0, WHITE);

			
			// ---------------------------------------> health bar for player
			if (!CheckCollisionRecs(collsionrect, carrot) and healthbar1flag)
			{
				DrawTexturePro(health, { 0.0f, 0.0f, (float)health.width, (float)health.height }, carrot, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				healthbar1flag = false;
			}
			if (!CheckCollisionRecs(collsionrect, carrot2) and healthbar2flag)
			{
				DrawTexturePro(health, { 0.0f, 0.0f, (float)health.width, (float)health.height }, carrot2, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				healthbar2flag = false;
			}
			/// ------------------------------------------->  for treasure


			for (auto& treasure_recs : treasures)
			{
				if (!CheckCollisionRecs(collsionrect, treasure_recs) and treasureflag[&treasure_recs - &treasures[0]])
				{
					DrawTexturePro(treasureTexture, { 0.0f, 0.0f, (float)treasureTexture.width, (float)treasureTexture.height }, treasure_recs, { 0, 0 }, 0.0f, WHITE);
				}
				//i guess in this part there is a problem

				else if (treasureflag[&treasure_recs - &treasures[0]] && !carryingtreasure)
				{
					
					treasureflag[&treasure_recs - &treasures[0]] = false;
					carryingtreasure = !treasureflag[&treasure_recs - &treasures[0]];
				}
				else if (carryingtreasure and CheckCollisionRecs(collsionrect,treasure_recs) and treasureflag[&treasure_recs - &treasures[0]])
				{
					DrawTexturePro(treasureTexture, { 0.0f, 0.0f, (float)treasureTexture.width, (float)treasureTexture.height }, treasure_recs, { 0, 0 }, 0.0f, WHITE);
				}
			}



			//rectangle lines for buildings
			for (auto building : builiding_rect)
			{
				DrawRectangleLines(building.x, building.y, building.width, building.height, RED);
			}


			if (CheckCollisionRecs(collsionrect, statue))
			{
				carryingtreasure = false;
			}

			if (IsKeyDown(KEY_F) && !carryingtreasure)
			{
				int player_difference = destrect.x - destrect2.x;
				if (player_difference > 0)
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
			else if (carryingtreasure)
			{
				if (playerMoving)
				{
					if (playermovingleft)
					{
						carry_west.Update();
						carry_west.Draw(destrect);
					}
					else
					{
						carry1.Update();
						carry1.Draw(destrect);
						carry2.Update();
						carry2.Draw(destrect);
						carry3.Update();
						carry3.Draw(destrect);
					}
				}
				else
				{
					currentIdleSprite.Update();
					currentIdleSprite.Draw(destrect);
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

			if (isEnemyAttacking)
			{
				int difference = destrect2.x - destrect.x;
				if(difference > 0)
				{
					enemyattack_left.Update();
					enemyattack_left.Draw(destrect2);
					
				}
				else
				{
					enemyattack.Update();
					enemyattack.Draw(destrect2);
				}
				
			}
			else
			{
				int difference = destrect2.x - destrect.x;
				if (difference > 0)
				{
					enemyleft.Update();
					enemyleft.Draw(destrect2);
				}
				else
				{
					enemy.Update();
					enemy.Draw(destrect2);
				}	
			}


			fishing_left.Update();
			fishing_left.Draw(fishing);

			// carry treasure


			smoke1.Draw();
			smoke1.Update();
			smoke2.Draw();
			smoke2.Update();
			smoke3.Update();
			smoke3.Draw();
			smoke4.Update();
			smoke4.Draw();
			cout << "X: " << destrect.x << " Y: " << destrect.y << endl;
			EndMode2D();
		}
		/*else if (currentScreen == GameScreen::GameOver)
		{
			DrawText("Game Over", 400, 200, 40, RED);
		}*/

		EndDrawing();
	}
	CloseWindow();

	return 0;
}
