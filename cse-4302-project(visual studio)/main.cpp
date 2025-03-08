#include <raylib.h>
#include <bits/stdc++.h>
#include "mainmenusprite.h"
#include "button.h"
#include "sound.h"
#include "playersprite.h"
#include "chimneysmoke.h"
#include "enemysprite.h"
#include "scoremanager.h"
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
Rectangle destrect = { 224, 287, 128, 128 };
Rectangle fishing = { 290, 428, 128, 128 };
Rectangle destrect2 = { 350,287,128,128 };
Rectangle enemy_temp = { 350,287,32,32};


// health bar for player----------------------------> make a class if possible Jehad / Labib, not Auvro
Rectangle carrot = { 793,35, 20, 20};
Rectangle carrot2 = { 480,490, 20, 20 };

// treasure__________________________________________
vector<Rectangle>treasures= { { 246, 131, 32, 32 } , { 600, 480, 32, 32 } ,{ 1138, 460, 32, 32 } ,{ 924, 139, 32, 32 } ,{ 1223, 281, 32, 32 } ,{ 1224,28, 32, 32 } ,{ 503, 178, 32, 32 } };
//vector<Rectangle>treasures = { { 246, 131, 32, 32 } };

//treasure carry recs

//Rectangle carry1_res = { 500, 299, 128, 128 };



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

// healthbars
// Health Bar Dimensions
const float maxHealth = 100.0f;
float playerHealth = maxHealth;
float enemyHealth = maxHealth;

// Function to draw the health bar
void DrawHealthBar(Rectangle position, float health, float maxHealth) {
	float healthBarWidth = (health / maxHealth) * (position.width - 30);
	DrawRectangle(position.x, position.y - 35, healthBarWidth, position.height - 120, RED);
	DrawRectangleLines(position.x, position.y - 35, position.width - 30, position.height - 120, DARKGRAY);
}

// Function to save the score to file
void SaveScore(float timeSpent) {
	ScoreManager::LoadScoresFromFile();
	ScoreManager::AddScore(timeSpent);
	ScoreManager::SaveScoresToFile();
}

// Check if all treasures are collected
bool allTreasuresCollected(vector<bool> treasureflag) {
	for (bool collected : treasureflag) {
		if (collected == true) {
			return false;
		}
	}
	return true;
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

	playersprite player("characters/gamecharacters/idle-S.png", 10, 10.0f, { 0, 0 });
	enemysprite enemy("characters/gamecharacters/enemywalking.png", 8, 20.0f, { 0, 0 });

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
	//playersprite currentIdleSprite = idleSpriteS;

	// Define map boundaries based on the map dimensions
	const float mapWidth = 1408.0f;
	const float mapHeight = 512.0f;
	const float player_widht = destrect.width;
	const float player_height = destrect.height;


	// health bar for health
	bool carrot1flag = true;
	bool carrot2flag = true;

	//bool is carring the treasure

	bool carryingtreasure = false;

	// health bar for treasure
	vector<bool>treasureflag(7, true);

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
				player.assignTexture("north", 4, 10.0f);
				//currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				playerMoving = playermovingdown = true;
				destrect.y += playerSpeed;
				player.assignTexture("south", 4, 10.0f);
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				//currentIdleSprite.Reset();
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				playerMoving = playermovingleft = true;
				destrect.x -= playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth);
				player.assignTexture("west", 4, 10.0f);
				//currentIdleSprite.Reset();
				isclickedright = false;
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				playerMoving = playermovingright = true;
				destrect.x += playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth - 32);
				player.assignTexture("east", 4, 10.0f);
				//currentIdleSprite.Reset();
			}

			float deltaTime = GetFrameTime();
			playerHealth -= isEnemyAttacking ? 10 * deltaTime : 0; // Example damage from enemy
			enemyHealth -= IsKeyDown(KEY_F) ? 10 * deltaTime : 0; // Example damage to enemy on attack

			//Check for Game Over (if player's health reaches zero)
			if (playerHealth <= 0) {
				SaveScore(deltaTime + 10);
				currentScreen = GameScreen::MainMenu;
				// Call game over logic here
			}
			// Winning condition: Check if all treasures are collected and the enemy health is zero
			if (allTreasuresCollected(treasureflag) && enemyHealth <= 0 && carryingtreasure == false) {
				SaveScore(deltaTime + 10);
				currentScreen = GameScreen::MainMenu; // End the game and go back to the main menu
			}    

			if (enemyHealth <= 0) {
				enemy.setAlive(false);  // Stop enemy actions when health is 0
			}

			collsionrect = { destrect.x + 15, destrect.y - 16, 32, 32 };

			//collision with buildings and trees
			ResolvePlayerBuildingCollision(collsionrect, builiding_rect);

			enemy_temp.x = destrect2.x;
			enemy_temp.y = destrect2.y;
			collsionrect.x -= 15;
			collsionrect.y += 16;

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
			if (!CheckCollisionRecs(collsionrect, carrot) and carrot1flag)
			{
				DrawTexturePro(health, { 0.0f, 0.0f, (float)health.width, (float)health.height }, carrot, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				carrot1flag = false;
			}
			if (!CheckCollisionRecs(collsionrect, carrot2) and carrot2flag)
			{
				DrawTexturePro(health, { 0.0f, 0.0f, (float)health.width, (float)health.height }, carrot2, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				carrot2flag = false;
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

			// Draw Health Bars
			DrawHealthBar(destrect, playerHealth, maxHealth); // Player health bar
			if (enemy.getAlive()) DrawHealthBar(destrect2, enemyHealth, maxHealth); // Enemy health bar


			if (CheckCollisionRecs(collsionrect, statue))
			{
				carryingtreasure = false;
			}

			if (IsKeyDown(KEY_F) && !carryingtreasure)
			{
				int player_difference = destrect.x - destrect2.x;
				if (player_difference > 0)
				{
					player.assignTexture("attack", 10, 25.0f);
				}
				else
				{
					player.assignTexture("attackLeft", 10, 25.0f);
				}
			}
			else if (carryingtreasure)
			{
				carryingtreasure = true;

				if (playerMoving)
				{
					if (playermovingleft)
					{
						player.assignTexture("carryWest", 8, 30.0f);
					}
					else
					{
						player.assignTexture("carryEast", 8, 30.0f);
					}
				}
			}
			else if (playerMoving)
			{
				if (playermovingup)
				{
					player.assignTexture("walkNorth", 8, 30.0f);
				}
				else if (playermovingright)
				{
					player.assignTexture("walkEast", 8, 30.0f);
				}
				else if (playermovingleft)
				{
					player.assignTexture("walkWest", 8, 30.0f);
				}
				else
				{
					player.assignTexture("walkSouth", 8, 30.0f);
				}
			}
			
			
			DrawRectangleLines(destrect.x + 15, destrect.y - 16, 32, 32, RED);

			if (enemy.getAlive()) {
				//checking collision with the enemy----------------------------------------------------
				enemyCollision(collsionrect, enemy_temp);
				ResolvePlayerBuildingCollision(enemy_temp, builiding_rect);
				destrect.x = collsionrect.x;
				destrect.y = collsionrect.y;

				DrawRectangleLines(destrect2.x + 15, destrect2.y - 16, 32, 32, RED);
				Follow({ destrect.x,destrect.y });

				if (isEnemyAttacking)
				{
					int difference = destrect2.x - destrect.x;
					if (difference > 0)
					{
						enemy.assignTexture("attackLeft", 8, 17.0f);
					}
					else
					{
						enemy.assignTexture("attackRight", 8, 17.0f);
					}
				}
				else
				{
					int difference = destrect2.x - destrect.x;
					if (difference > 0)
					{
						enemy.assignTexture("left", 8, 30.0f);
					}
					else
					{
						enemy.assignTexture("right", 8, 30.0f);
					}
				}

				enemy.Update();
				enemy.Draw(destrect2);
			}

			player.Update();
			player.Draw(destrect);
			fishing_left.Update();
			fishing_left.Draw(fishing);


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
