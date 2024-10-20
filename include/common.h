/*
 * Copyright (c) 2024 Shiyadh S
 * Licensed under the MIT License. See LICENSE file for details.
 */



#pragma once

#include "raylib.h"

// Global Const Variables
const int screenWidth = 1920;
const int screenHeight = 1080;
const float planeSpeed = 10.0f;
const int MAX_BULLET{ 100 };
const int MAX_ASTEROID{ 15 };
// Forward Function Declarations

void initializeWindow();
//void displayGameLogo();
void initializeGame();
void updateAndDrawGame();

// Global Variables
//int score{ 0 };

// Game state
enum  GameState
{
	LOGO,
	STORY_TEXT,
	MENU,
	PLAYING,
	GAME_OVER
};

// Plane Struct
typedef struct Plane {
	Texture2D planeSprite;
	Vector2 planePosition;
	bool planeActive = true;
}Plane;

// Bullet Struct
typedef struct Bullet {
	bool bulletActive = false;
	Vector2 bulletPosition;
	Vector2 bulletDirection;
	Texture2D bulletSprite;
	float bulletSpeed;

}Bullet;

// Asteroid Struct
typedef struct Asteroid {
	Texture2D asteroidSprite;
	Vector2 asteroidPosition;
	float asteroidSpeed;
	bool asteroidActive = true;
	float asteroidRotation;
	float asteroidRotationSpeed;
}Asteroid;

// SpaceShip Struct
typedef struct SpaceShip
{
	Texture2D spaceShipSprite;
	Vector2 spaceShipPosition;
	float spaceShipSpeed;
	bool spaceShipAcive = true;
	

}SpaceShip;