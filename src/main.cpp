/*
 * Copyright (c) 2024 Shiyadh S
 * Licensed under the MIT License. See LICENSE file for details.
 */



#include "common.h"

int main()
{
	initializeWindow();
	//displayGameLogo();
    initializeGame();
	

	updateAndDrawGame();

	return 0;
}


//------TO DO---------

// The plane sprite file path is still in downloads folder
// Try to create an asset folder to store sprites
// Create sprite for bullets
// Wall Collision                -> Done
// Game window State			 -> Done
// Create Asteroids				 -> Done
// Audio                         -> Done> Done