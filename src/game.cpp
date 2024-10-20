/*
 * Copyright (c) 2024 Shiyadh S
 * Licensed under the MIT License. See LICENSE file for details.
 */




#include "common.h"
#include <iostream>
#include <filesystem>

Plane plane;						// instance of Plane Struct
SpaceShip spaceShip;				// instance of Spaceship
Bullet bullet[MAX_BULLET];			// Array of bullet struct to hold multiple bullets
Texture2D bulletTexture;			// Loaded once and reused for all bullets
GameState currentState;				// instance of GameState ENUM
Asteroid asteroid[MAX_ASTEROID];	// Array of asteroid to be spawned randomly
Texture2D asteroidTexture;			// Loaded and used for all Asteroid
Sound explosionSound;				// Sound for asteroid Explosion
Sound shootSound;					// Sound for Bullet Shots
Sound playGameSound;				// Played while Running the game for the first time
Sound gameOverSound;

// Global Variables
int score = 0;
float logoDiplayTime = 1.0f;		// Display the logo text
float logoElapsedTime = 0.0f;
bool displayCreatorLogo = true;
int spaceShipDirection = 1;			// 1 is for left direction 

float storyDisplayTime = 6.5f;
float storyElapsedTime = 0.0f;
bool displayStoryText = true;

const std::string basePath = std::filesystem::current_path().string();

void initializeWindow()
{
	InitWindow(screenWidth, screenHeight, "Asteroid Shooter v0.3");
	SetTargetFPS(60);
}


void initializeGame()
{
	// SET CURRENT GAME STATE TO MENU
	currentState = LOGO;

	// Initialize Audio

	InitAudioDevice();				
	SetMasterVolume(1.0f);
	explosionSound = LoadSound((basePath + "/resources/audio/explosion.wav").c_str());
	shootSound = LoadSound((basePath + "/resources/audio/shoot.wav").c_str());
	playGameSound = LoadSound((basePath + "/resources/audio/playGame.wav").c_str());
	gameOverSound = LoadSound((basePath + "/resources/audio/gameOver.wav").c_str());
	if (shootSound.frameCount == 0) {
		std::cerr << "Failed to load shoot sound!" << std::endl;
	}
	

	// gotta update the file path 
	plane.planeSprite = LoadTexture((basePath + "/resources/sprites/shooterPlane.png").c_str());
	plane.planePosition = { (float)screenWidth / 2,(float)screenHeight / 2 };
	plane.planeActive = true;
	// Bullet Texture loading
	bulletTexture = LoadTexture((basePath + "/resources/sprites/bullet.png").c_str());
	for (int i{ 0 }; i < MAX_BULLET; i++)
	{
		bullet[i].bulletActive = false;			// set the bullet inactive initially unless the user press Space
		bullet[i].bulletSprite = bulletTexture;
		bullet[i].bulletSpeed = 15.0f;
	}

	// Asteroid Texture Loading

	asteroidTexture = LoadTexture((basePath + "/resources/sprites/asteroid.png").c_str());
	for (int i{ 0 }; i < MAX_ASTEROID; i++)
	{
		
		asteroid[i].asteroidSprite = asteroidTexture;
		
	}

	// Space Ship Active Status initialized to true at the start of the Game
	spaceShip.spaceShipSprite = LoadTexture((basePath + "/resources/sprites/spaceShip.png").c_str());
	spaceShip.spaceShipAcive = true;
	spaceShip.spaceShipPosition = { (float)screenWidth / 2.0f,(float)screenHeight - spaceShip.spaceShipSprite.height };
	spaceShip.spaceShipSpeed = 3.0f;
	
}



// Basic Keyboard Movements
void planeMovements()
{
	if (IsKeyDown(KEY_A)) {
		plane.planePosition.x -= planeSpeed;
	}
	if (IsKeyDown(KEY_D)) {
		plane.planePosition.x += planeSpeed;
	}
	if (IsKeyDown(KEY_W)) {
		plane.planePosition.y -= planeSpeed;
	}
	if (IsKeyDown(KEY_S)) {
		plane.planePosition.y += planeSpeed;
	}
}

// Plane and Border Collision Check
void borderCollisionOfPlane()
{
	if (plane.planePosition.x  < 0){
		plane.planePosition.x = 0;
	}
	if (plane.planePosition.x + plane.planeSprite.width > screenWidth) {
		plane.planePosition.x = screenWidth - plane.planeSprite.width;
	}
	if (plane.planePosition.y < 0) {
		plane.planePosition.y = 0;
	}
	if (plane.planePosition.y + plane.planeSprite.height > screenHeight) {
		plane.planePosition.y = screenHeight - plane.planeSprite.height;
	}

}

void shootBullet()
{
	// Check if player wants to shoot
	if (IsKeyPressed(KEY_SPACE)) {
		
		for (int i{ 0 }; i < MAX_BULLET; i++)
		{

			if (!bullet[i].bulletActive)
			{
				bullet[i].bulletActive = true;
				// set the bullet spawn relative to the player
				bullet[i].bulletPosition.x = plane.planePosition.x + (plane.planeSprite.width / 2) - (bullet[i].bulletSprite.width / 2);
				bullet[i].bulletPosition.y = plane.planePosition.y;
				
				PlaySound(shootSound);
				break;
			}
			

			
		}
	}
}


// Initial Menu
void drawMenu()
{
	ClearBackground(RAYWHITE);
	BeginDrawing();
	DrawText("Asteroid Shooter", screenWidth / 2.0 - 250.0, screenHeight / 2.0 - 70, 60, MAROON);
	DrawText("Press ENTER to Start Game", screenWidth / 2.0 - 150.0, screenHeight / 2.0 , 20.0, DARKBLUE);
	EndDrawing();
}

// Game Over
void gameOver()
{
	ClearBackground(RED);
	BeginDrawing();
	DrawText("Game Over!", screenWidth / 2.0 - 150.0, screenHeight / 2.0, 50.0, BLACK);
	DrawText("Press Enter to Play Again.", screenWidth / 2.0 - 150.0, screenHeight / 2.0 + 80.0, 20.0, BLACK);
	
	EndDrawing();
}


void drawCreatorLogo()
{
	ClearBackground(BLACK);
	BeginDrawing();
	DrawText("From ZYDX", screenWidth / 2.0 - 150, screenHeight / 2.0, 60, RAYWHITE);
	EndDrawing();
}

// Draw Story Explanation
void drawStoryText()
{
	ClearBackground(BLACK);
	BeginDrawing();
	DrawText("Soldier,Our planet has been Destroyed by some Unknown Source of Power.", screenWidth / 2.0 - 600, screenHeight / 2.0, 30, RAYWHITE);
	DrawText("Protect Our King while we Venture out to find a new Home...", screenWidth / 2.0 - 450, screenHeight / 2.0 + 70, 30, RAYWHITE);
	EndDrawing();
}

// Draw GameEnding
void drawGameEnding()
{
	ClearBackground(BLACK);
	BeginDrawing();
	DrawText("Well Done Soldier,",screenWidth/2.0 - 200, screenHeight / 2.0, 40, RAYWHITE);
	DrawText("You Saved Our King from the Asteroids!", screenWidth / 2.0 - 350, screenHeight / 2.0 + 70, 30, RAYWHITE);
	DrawText("Now let's go on a journey to find New Worlds...", GetScreenWidth() /2.0 - 350, GetScreenHeight() /2.0 + 120, 30, WHITE);
	EndDrawing();
}



void generateAsteroids()
{
	for (int i{ 0 }; i < MAX_ASTEROID; i++)
	{
		asteroid[i].asteroidSprite = asteroidTexture;	// set texture
		asteroid[i].asteroidPosition.x = GetRandomValue(-5, screenWidth - asteroid->asteroidSprite.width);
		asteroid[i].asteroidPosition.y = GetRandomValue(-5, -1 );
		std::cout << "asteroid posX:" << asteroid[i].asteroidPosition.x << "asteroid posY:" << asteroid[i].asteroidPosition.y << '\n';
		asteroid[i].asteroidSpeed = GetRandomValue(1, 8);
		asteroid[i].asteroidActive = true;
	}
}


// Respawn the asteroid after it gets destroyed

void respawnAsteroids(int index)
{
	
		float posX = GetRandomValue(-5.0, screenWidth - asteroid->asteroidSprite.width);
		float posY = GetRandomValue(-5.0, -1);
		asteroid[index].asteroidPosition = { posX,posY };
		asteroid[index].asteroidSpeed = GetRandomValue(1, 8);
		asteroid[index].asteroidActive = true;
	
	
}



// Returns index of the asteroid that was hit by bullet
int checkBulletAsteroidCollision()
{
	for (int i{ 0 }; i < MAX_BULLET; i++)
	{
		if (bullet[i].bulletActive)
		{
			for (int j{ 0 }; j < MAX_ASTEROID; j++)
			{
				if(asteroid[j].asteroidActive)
				{
					Vector2 posOfBullet = { bullet[i].bulletPosition.x,bullet[i].bulletPosition.y };
					Vector2 posOfAsteroid = { asteroid[j].asteroidPosition.x,asteroid[j].asteroidPosition.y };

					Rectangle asteroidRec = { posOfAsteroid.x,posOfAsteroid.y,asteroid[j].asteroidSprite.width,asteroid[j].asteroidSprite.height };
					Rectangle bulletRec = { posOfBullet.x,posOfBullet.y,bullet[i].bulletSprite.width,bullet[i].bulletSprite.height };

					bool collision = CheckCollisionRecs(asteroidRec, bulletRec);
					if (collision)
					{
						return j;
					}
					
				}
				
			}
		}
		
		
	
	}
	return -1;
}

bool checkPlayerAsteroidCollision()
{
	for (int i{ 0 }; i < MAX_ASTEROID; i++)
	{
		Vector2 posOfPlane = { plane.planePosition.x,plane.planePosition.y };
		Vector2 posOfAsteroid = { asteroid[i].asteroidPosition.x,asteroid[i].asteroidPosition.y };

		Rectangle planeRec = { posOfPlane.x,posOfPlane.y,plane.planeSprite.width,plane.planeSprite.height };
		Rectangle asteroidRec = { posOfAsteroid.x,posOfAsteroid.y,asteroid[i].asteroidSprite.width,asteroid[i].asteroidSprite.height };

		bool collision = CheckCollisionRecs(planeRec, asteroidRec);
		if (collision)
		{
			
			return true;
		}
		
	}
	return false;
}


// check spaceship collision with asteroid
bool checkSpaceShipAsteroidCollision()
{
	for (int i{ 0 }; i < MAX_ASTEROID; i++)
	{
		Vector2 posOfSpaceShip = { spaceShip.spaceShipPosition.x,spaceShip.spaceShipPosition.y };
		Vector2 posOfAsteroid = { asteroid[i].asteroidPosition.x,asteroid[i].asteroidPosition.y };

		Rectangle spaceShipRec = { posOfSpaceShip.x,posOfSpaceShip.y,spaceShip.spaceShipSprite.width,spaceShip.spaceShipSprite.height };
		Rectangle asteroidRec = { posOfAsteroid.x,posOfAsteroid.y,asteroid[i].asteroidSprite.width,asteroid[i].asteroidSprite.height };

		bool collision = CheckCollisionRecs(spaceShipRec, asteroidRec);
		if (collision)
		{
			return true;
		}
	}
	return false;
}

void updateAndDrawGame()
{
	
	while (!WindowShouldClose() )
	{
		switch (currentState)
		{
		case LOGO:
		{
			if (displayCreatorLogo)
			{
				drawCreatorLogo();
				logoElapsedTime += GetFrameTime();
				
				if (logoElapsedTime >= logoDiplayTime)
				{
					displayCreatorLogo = false;
					currentState = STORY_TEXT;
					
				}
			}
			
			
			break;
		}
		case STORY_TEXT:
		{
			if (displayStoryText)
			{
				drawStoryText();
				storyElapsedTime += GetFrameTime();
				if (storyElapsedTime >= storyDisplayTime)
				{
					displayStoryText = false;
					currentState = MENU;
				}
			}
			break;
		}
		case MENU:
		{
			drawMenu();
			if (IsKeyPressed(KEY_ENTER)) {
				PlaySound(playGameSound);
				currentState = PLAYING;
				generateAsteroids();
			}
			break;
		}
		case PLAYING:
		{
			
			// UPDATES
			//drawScoreValue(score);
			planeMovements();
			borderCollisionOfPlane();

			// Shoot bullet if player press Space
			shootBullet();



			// Shoot the bullet upwards
			for (int i{ 0 }; i < MAX_BULLET; i++)
			{

				if (bullet[i].bulletActive)
				{
					bullet[i].bulletPosition.y -= bullet[i].bulletSpeed;
				}
				/*
				if (bullet[i].bulletPosition.y < 0) {
					bullet[i].bulletActive = false;
				}
				*/
			}

			// Move the Asteroids from right to left
			for (int i{ 0 }; i < MAX_ASTEROID; i++)
			{
				// create movement from left to right

				/*
				asteroid[i].asteroidPosition.x += asteroid[i].asteroidSpeed;
				// reposition the asteroid to the left if it goes out of border creating a constant motion
				if (asteroid[i].asteroidPosition.x > screenWidth) {
					asteroid[i].asteroidPosition.x = 0;
				}
				*/

				// create movement top to bottom
				asteroid[i].asteroidPosition.y += asteroid[i].asteroidSpeed;
				
				// reposition the asteroid to the top if it goes out of screen
				if (asteroid[i].asteroidPosition.y > screenHeight) {
					asteroid[i].asteroidPosition.y = 0;
				}
			}

			// Deactivate bullet and asteroid based on the collided asteroid index
			int asteroidIndex = checkBulletAsteroidCollision();
			if (asteroidIndex != -1)
			{

				//std::cout << "Collision detected! " << asteroidIndex << '\n';
				PlaySound(explosionSound);
				asteroid[asteroidIndex].asteroidActive = false;
				
				score += 10;
				//std::cout << "score:" << score << '\n';
				respawnAsteroids(asteroidIndex);							// respawn the asteroid after they get destroyed
				for (int i{ 0 }; i < MAX_BULLET; i++)
				{
					if (bullet[i].bulletActive)
					{
						bullet[i].bulletActive = false;
						break;
					}
					

				}
			}


			// Move space ship  left and Right 
			if (spaceShip.spaceShipAcive)
			{
				spaceShip.spaceShipPosition.x -= spaceShip.spaceShipSpeed * spaceShipDirection;
				if (spaceShip.spaceShipPosition.x <= 0)
				{
					spaceShipDirection = -1;
				}
				if (spaceShip.spaceShipPosition.x >= screenWidth - spaceShip.spaceShipSprite.width)
				{
					spaceShipDirection = 1;
				}
			}

			// Destroy player if it hits asteroid

			if (checkPlayerAsteroidCollision())
			{

				// sound does not work check it
				PlaySound(gameOverSound);
				plane.planeActive = false;
				currentState = GAME_OVER;
			}

			// Check asteroid and Space ship Collision
			if (checkSpaceShipAsteroidCollision())
			{
				PlaySound(gameOverSound);
				spaceShip.spaceShipAcive = false;
				currentState = GAME_OVER;
			}
			
			// End the game if score reach 150
			if (score >= 150)
			{
				currentState = GAME_ENDING;

			}
			ClearBackground(BLACK);
			// DRAW
			BeginDrawing();



			// Draw SpaceShip
			if (spaceShip.spaceShipAcive)
			{
				DrawTexture(spaceShip.spaceShipSprite, spaceShip.spaceShipPosition.x, spaceShip.spaceShipPosition.y, WHITE);
			}

			// USED WHITE TO RETAIN ORIGINAL SPRITE COLOR
			if (plane.planeActive)
			{
				//std::cout << "plane active?:" << plane.planeActive << '\n';
				DrawTexture(plane.planeSprite, plane.planePosition.x, plane.planePosition.y, WHITE);

			}

			// Draw bullets based on its Active Status
			for (int i{ 0 }; i < MAX_BULLET; i++)
			{
				if (bullet[i].bulletActive)
				{
					// Does not draw if bullet destroyed an Asteroid
					DrawTexture(bullet[i].bulletSprite, bullet[i].bulletPosition.x, bullet[i].bulletPosition.y, WHITE);
				}
				
			}
			//draw score
			DrawText(TextFormat("Score:%d", score), 1800, 40, 20, WHITE);

			// Draw asteroids

			for (int i{ 0 }; i < MAX_ASTEROID; i++)
			{
				if (asteroid[i].asteroidActive == true)
				{

					DrawTexture(asteroid[i].asteroidSprite, asteroid[i].asteroidPosition.x, asteroid[i].asteroidPosition.y, WHITE);
				}

			}
			EndDrawing();
			break;
		}
		case GAME_ENDING:
		{

			drawGameEnding();
			break;
		}

		case GAME_OVER:
			gameOver();
			score = 0;
			
			
			if (IsKeyPressed(KEY_ENTER))
			{
				// Reset game state here
				plane.planeActive = true;
				plane.planePosition = { (float)screenWidth / 2, (float)screenHeight / 2 };

				

				// Reset bullets
				for (int i = 0; i < MAX_BULLET; i++)
				{
					bullet[i].bulletActive = false;
				}

				// Reset asteroids
				for (int i = 0; i < MAX_ASTEROID; i++)
				{
					asteroid[i].asteroidActive = true; // or set to false and respawn
					respawnAsteroids(i); // Respawn asteroids
				}

				// Reset SpaceShip Position and other variables
				spaceShip.spaceShipAcive = true;
				spaceShip.spaceShipPosition = { (float)screenWidth / 2.0f,(float)screenHeight - spaceShip.spaceShipSprite.height };
				
				currentState = PLAYING;


				break;
			}
			
			break;
		}
		
	}
	UnloadTexture(spaceShip.spaceShipSprite);
	UnloadTexture(plane.planeSprite);
	UnloadTexture(asteroidTexture);
	UnloadTexture(bulletTexture);
	CloseAudioDevice();
	CloseWindow();
}
