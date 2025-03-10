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
	GetName,
	HighScore,
	Game,
	GameOver
};

float playerSpeed = 3.0f;
Camera2D gamecamera;
int framecount = 0;
int playerframe = 0;

// Character destination rectangle
Rectangle destrect = { 224, 287, 128, 128 };
Rectangle fishing = { 290, 428, 128, 128 };
Rectangle destrect2 = { 350,287,128,128 };
Rectangle enemy_temp = { 350,287,32,32};

// Enemy attack flag
bool isEnemyAttacking = false;
bool canAttack = false;

// health carrot for player
Rectangle carrot = { 793,35, 20, 20};
Rectangle carrot2 = { 480,490, 20, 20 };

// treasure__________________________________________
vector<Rectangle>treasures= { { 246, 131, 32, 32 } , { 600, 480, 32, 32 } ,{ 1138, 460, 32, 32 } ,{ 924, 139, 32, 32 } ,{ 1223, 281, 32, 32 } ,{ 1224,28, 32, 32 } ,{ 503, 178, 32, 32 } };
//vector<Rectangle>treasures = { { 246, 131, 32, 32 } };


// rectangle for statue
Rectangle statue = { 1262,280,200,150 };

Rectangle collsionrect;
vector<Rectangle> builiding_rect{ {40, 15, 45, 65},{140, 40, 45, 65},{265,45,45,60},{580, 190, 30, 20},{600, 134, 30, 30},{1210, 400, 200, 150},{36,376,377,224} ,{780,216,65,44} ,{1284,342,49,21},{941,285,35,9},{381,262,338,1},{774,351,83,45} ,{1118,362,57,37} ,{1165,122,125,73} ,{906,64,1,173},{1000,83,1,170} };// for building rect
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

// Modify your Follow function to include attack logic
void Follow(Vector2 heroPos) {
	float dx = heroPos.x - destrect2.x;
	float dy = heroPos.y - destrect2.y;
	float distance = sqrt(dx * dx + dy * dy);

	// Define the attack range
	const float attackRange = 40.0f;
	const float playerAttackRange = 70.0f;
	const float followRange = 1000.0f;  // Maximum distance to follow player

	if (distance <= playerAttackRange)
	{
		canAttack = true;
	}
	else
	{
		canAttack = false;
	}

	if (distance <= attackRange) {
		// Enemy is close enough to attack
		isEnemyAttacking = true;
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

// Check if all treasures are collected
bool allTreasuresCollected(vector<bool> treasureflag) {
	for (bool collected : treasureflag) {
		if (collected == true) {
			return false;
		}
	}
	return true;
}

// Function to save the score to file
void SaveScore(float timeSpent, string playerName) {
	ScoreManager::LoadScoresFromFile();
	ScoreManager::AddScore(timeSpent, playerName);
	ScoreManager::SaveScoresToFile();
}

// Update high scores
vector<pair<float, string>> highScores;

void UpdateHighScores() {
	highScores.clear();
	vector<pair<float, string>> scores = ScoreManager::GetScores();
	for (auto& score : scores) {
		highScores.push_back(score);
	}
}

// Main function related variables

string currentIdleSprite = "south";
int defeatedEnemies = 0;
//bool for health carrot
bool carrot1flag = true;
bool carrot2flag = true;
//bool is carring the treasure
bool carryingtreasure = false;
// bool for treasure
vector<bool>treasureflag(7, true);
int treasuresCollected = 0;
// Player name
string playerName = "";
// Time measurement
float timeSpent = 0.0;
float startTime = 0.0;

void resetGame() {
	playerHealth = maxHealth;
	enemyHealth = maxHealth;
	playerName = "";
	currentIdleSprite = "south";
	defeatedEnemies = 0;
	treasuresCollected = 0;
	carrot1flag = true;
	carrot2flag = true;
	carryingtreasure = false;
	treasureflag = { true, true, true, true, true, true, true};
	destrect = { 224, 287, 128, 128 };
	destrect2 = { 350,287,128,128 };
	enemy_temp = { 350,287,32,32 };
	carrot = { 793,35, 20, 20 };
	carrot2 = { 480,490, 20, 20 };
	isEnemyAttacking = false;
	canAttack = false;
	highScores.clear();
	timeSpent = 0.0f;
	startTime = 0.0f;
}

int main()
{
	GameScreen currentScreen = GameScreen::MainMenu;
	bool exit = false;
	const int windowWidth = GetScreenWidth();
	const int windowHeight = GetScreenHeight();

	InitWindow(windowWidth, windowHeight, "OOP Sprite Animation");
	ToggleFullscreen();
	startTime = GetTime();

	// Player
	playersprite player("characters/gamecharacters/idle-S.png", 10, 10.0f, { 0, 0 });
	// Enemies
	enemysprite enemy("characters/gamecharacters/enemywalking.png", 8, 20.0f, { 0, 0 });

	// Other sprites
	Texture  mapTexturelayer2;
	chimneysmoke smoke1("characters/map/smoke1.png", 30, 15, 37, 25, 1.5, { 775, 156 });
	chimneysmoke smoke2("characters/map/smoke2.png", 30, 10, 30, 25, 1.5, { 843, 280 });
	chimneysmoke smoke3("characters/map/smoke3.png", 30, 18, 21, 25, 1.0, { 1125, 320 });
	chimneysmoke smoke4("characters/map/smoke4.png", 30, 20, 32, 25, 1.0, { 1185, 110 });
	playersprite fishing_left("characters/gamecharacters/casting-left.png", 17, 10.0f, { 0, 0 });

	//FONT
	Font font = LoadFont("characters/gamecharacters/alagard.ttf");

	//health carrot -----------------------------------------------------------------
	Image carrotImage = LoadImage("characters/gamecharacters/health.png");
	Texture2D Carrot = LoadTextureFromImage(carrotImage);
	        
	// treasure ..............................................................................
	Image treasure = LoadImage("characters/gamecharacters/treasure.png");
	Texture2D treasureTexture = LoadTextureFromImage(treasure);

	mapTexturelayer2 = LoadTexture("characters/map/game.png");
	SpriteAnimation menuBackground("images/new2.png", 38, 38.0f, { 0, 1 });
	InitAudioDevice();
	Button startButton{ "buttons/start.png", {100, 330}, 0.85 };
	Button exitButton{ "buttons/exit.png", {100, 630}, 0.85 };
	Button highScoreButton{ "buttons/highScore.png", {90, 480}, 0.85 };
	Button backButton{ "buttons/back.png", {200, 750}, 0.85 }; // Back Button
	//player attack sound 
	sound attacksound("sounds/attack.mp3");
	SetTargetFPS(60);

	// Define map boundaries based on the map dimensions
	const float mapWidth = 1408.0f;
	const float mapHeight = 512.0f;
	const float player_widht = destrect.width;
	const float player_height = destrect.height;
	
	// Main game loop
	while (!WindowShouldClose() && exit == false)
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		timeSpent = GetTime() - startTime;

		if (currentScreen == GameScreen::MainMenu)
		{
			menuBackground.Update();
			menuBackground.Draw();
			startButton.Draw();
			exitButton.Draw();
			highScoreButton.Draw();

			if (startButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
			{
				currentScreen = GameScreen::GetName;
			}
			if (exitButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
			{
				exit = true;
			}
			if (highScoreButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
				currentScreen = GameScreen::HighScore;
			}
		}
		else if (currentScreen == GameScreen::GetName) {
			/*menuBackground.Update();
			menuBackground.Draw();*/
			DrawText("Enter your name!", 710, 400, 50, BLACK);

			if (IsKeyPressed(KEY_ENTER) && playerName.size() >= 4) {
				currentScreen = GameScreen::Game;
			}

			if (IsKeyPressed(KEY_BACKSPACE)) {
				if (!playerName.empty()) {
					playerName.pop_back();
				}
			}
			else if(playerName.size() <= 12){
				int key = GetCharPressed();
				if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
					playerName.push_back((char)key);
				}
				key = GetCharPressed(); 
			}

			// Draw the player name in the box
			Rectangle textBox = { 650, 480, 540, 60 };
			DrawRectangleRec(textBox, LIGHTGRAY);
			if (playerName.size()) {
				DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLUE);
			}
			DrawText(playerName.c_str(), textBox.x + 270 - 16*playerName.size(), textBox.y + 5, 50, BLACK);
			DrawText("Press Enter to start the game", 540, 600, 50, BLACK);
		}

		else if (currentScreen == GameScreen::Game)
		{
			bool isclickedright = true, playerMoving = false;
			bool playermovingup = false, playermovingdown = false;
			bool playermovingleft = false, playermovingright = false;

			if (IsKeyDown(KEY_UP))
			{
				playerMoving = playermovingup = true;
				destrect.y -= playerSpeed;
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
				currentIdleSprite = "north";
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				playerMoving = playermovingdown = true;
				destrect.y += playerSpeed;
				currentIdleSprite = "south";
				destrect.y = Clamp(destrect.y, 0.0f, mapHeight);
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				playerMoving = playermovingleft = true;
				destrect.x -= playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth);
				currentIdleSprite = "west";
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				playerMoving = playermovingright = true;
				destrect.x += playerSpeed;
				destrect.x = Clamp(destrect.x, 0.0f, mapWidth - 32);
				currentIdleSprite = "east";
			}

			float deltaTime = GetFrameTime();
			playerHealth -= isEnemyAttacking ? 10 * deltaTime : 0; // Example damage from enemy
			enemyHealth -= (IsKeyDown(KEY_F) && !carryingtreasure && canAttack) ? 10 * deltaTime : 0; // Example damage to enemy on attack
			
			
	
			
			if (IsKeyPressed(KEY_F) && !carryingtreasure && canAttack)
				attacksound.playSound();





			if (enemyHealth <= 0 && enemy.getAlive()) {
				defeatedEnemies++;

				if (defeatedEnemies >= 5)
					enemy.setAlive(false);  // Stop enemy actions when health is 0
				else {
					enemyHealth = maxHealth;
					destrect2.x = 560;
					destrect2.y = 389;
	
					if (defeatedEnemies % 2)
						enemy.setColor({ 255, 0, 0, 190 });
					else
						enemy.setColor(WHITE);
				}
			}

			//Check for Game Over (if player's health reaches zero)
			if (playerHealth <= 0) {
				float score = (treasuresCollected * 15) + (defeatedEnemies * 20) - ((float)timeSpent/10);
				score = max(score, 0.0f);
				SaveScore(score, playerName);
				resetGame();
				enemy.setAlive(true);
				startTime = GetTime();
				currentScreen = GameScreen::MainMenu;
			}
			// Winning condition: Check if all treasures are collected and the enemy health is zero
			if (allTreasuresCollected(treasureflag) && !enemy.getAlive() && carryingtreasure == false) {
				float score = (treasuresCollected * 15) + (defeatedEnemies * 20) - ((float)timeSpent/10);
				score = max(score, 0.0f);
				SaveScore(score, playerName);
				resetGame();
				enemy.setAlive(true);	
				startTime = GetTime();
				currentScreen = GameScreen::MainMenu; 
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

			
			// ---------------------------------------> health Carrot for player
			if (!CheckCollisionRecs(collsionrect, carrot) and carrot1flag)
			{
				DrawTexturePro(Carrot, { 0.0f, 0.0f, (float)Carrot.width, (float)Carrot.height }, carrot, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				if (carrot1flag)
					playerHealth = min(playerHealth + maxHealth / 2, maxHealth);
				carrot1flag = false;
			}
			if (!CheckCollisionRecs(collsionrect, carrot2) and carrot2flag)
			{
				DrawTexturePro(Carrot, { 0.0f, 0.0f, (float)Carrot.width, (float)Carrot.height }, carrot2, { 0, 0 }, 0.0f, WHITE);
			}
			else
			{
				if (carrot2flag)
					playerHealth = min(playerHealth + maxHealth / 2, maxHealth);
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
					treasuresCollected++;
					treasureflag[&treasure_recs - &treasures[0]] = false;
					carryingtreasure = !treasureflag[&treasure_recs - &treasures[0]];
				}
				else if (carryingtreasure and CheckCollisionRecs(collsionrect, treasure_recs) and treasureflag[&treasure_recs - &treasures[0]])
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

			if (IsKeyDown(KEY_F) && !carryingtreasure && canAttack)
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
				else {
					if (currentIdleSprite == "west" || currentIdleSprite == "north")
						player.assignTexture("carryIdle", 1, 5.0f);
					else
						player.assignTexture("carryIdleEast", 1, 5.0f);
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
			else {
				if (currentIdleSprite == "north")
					player.assignTexture("north", 8, 10.0f);
				else if (currentIdleSprite == "south")
					player.assignTexture("south", 8, 10.0f);	
				else if (currentIdleSprite == "east")		
					player.assignTexture("east", 8, 10.0f);
				else if (currentIdleSprite == "west")
					player.assignTexture("west", 8, 10.0f);
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
		else if (currentScreen == GameScreen::HighScore)
		{
			UpdateHighScores();
			/*menuBackground.Update();
			menuBackground.Draw();*/
			
			// Draw Text and highscores
			DrawTextEx(font, "Highscores:", {200,100}, 60.0f, 8, BLACK);
			/*DrawRectangle(180, 180, 500, 5, BLACK);*/
			float yOffset = 210;
			int it = 1;
			
			for (auto& score : highScores) {

				DrawTextEx(font, TextFormat("%d.  %s: %.3f", it++, score.second.c_str(), score.first), { 200, yOffset }, 60.0f, 8, BLACK);
				yOffset += 80;
			}
			
			// Draw back button
			backButton.Draw();
			if (backButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
				currentScreen = GameScreen::MainMenu;
			}
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
