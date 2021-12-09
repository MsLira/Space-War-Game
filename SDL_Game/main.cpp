#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
// Miriam lira  PICK-UPS (UP Date)

// boolean to maintain program loop
bool quit = false;

// DeltaTime init() - for frame rate ind. *2* Frame rate independence for speed on a faster machine *2*
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

// Set player speed   *2* *3*
float playerSpeed = 500.0f;

// Movement in the X axis - left or right   *2* *3*
float xDir;

// variables use to control interger creep  *2* *3*
float pos_X, pos_Y;

//enemy information *4*
#include "enemy.h";
#include <vector>

// enemy list variable
vector<Enemy> enemyList;

// Bullet Information
#include "bullet.h"
vector<Bullet> bulletList;

// create rectagles for the playerposition
SDL_Rect playerPos;

// declere SOUNDS
Mix_Chunk* laser;
Mix_Chunk* explosion;
Mix_Music* bgm;


void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			// play laser sound
			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = (pos_X + (playerPos.w / 2));

			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w / 2));

			bulletList[i].posRect.y = playerPos.y;

			bulletList[i].pos_X = pos_X;

			bulletList[i].pos_Y = playerPos.y;

			break;
		}

	}

}

//  Variables for Score and lives 
//  Score, lives and fonts
int playerScore, oldScore, playerLives, oldLives;

// declre font 
TTF_Font* font;

// font color
SDL_Color colorP1 = { 0, 255, 0, 255 };

// Create surfaces
SDL_Surface* scoreSurface, * livesSurface;

// Create textures
SDL_Texture* scoreTexture, * livesTexture;

SDL_Rect scorePos, livesPos;

string tempScore, tempLives;

void UpdateScore(SDL_Renderer* renderer)
{
	// Creating the text
	tempScore = "Player Score: " + std::to_string(playerScore);

	// Create render Text
	scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);

	// Creating texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	oldScore = playerScore;

}

void UpdateLives(SDL_Renderer* renderer)
{
	// Creating the Text 
	tempLives = "Player Lives: " + std::to_string(playerLives);

	// Creating Render Text 
	livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);

	// Creating Texture
	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);

	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	SDL_FreeSurface(livesSurface);

	oldLives = playerLives;
}

// _____________setup pickup_________________________
#include "pickup.h"
vector<Pickup> pickupList;


int main(int arg, char* argv[])
{

	SDL_Window* window;			// Declare a pointer

	// create a renderer variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2

	// Create an application window with the following settings :
	window = SDL_CreateWindow(
		"Space Game",					//Window title
		SDL_WINDOWPOS_UNDEFINED,	// Initial x position
		SDL_WINDOWPOS_UNDEFINED,    // initial y position
		1024,						//width, in pixels
		768,						//height, in pixels
		SDL_WINDOW_OPENGL			//flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// background image -- CREATE

	// create a SDL surface
	SDL_Surface* surface = IMG_Load("./Assets/bkgd.png");

	// create a bkgd surface 
	SDL_Texture* bkgd;

	// place surface into the texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	// Free the Surface
	SDL_FreeSurface(surface);

	// create rectagles for the background position
	SDL_Rect bkgdPos;

	// set bkgd x, y, width and height
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	// Background image -- CREATE END

		// player image -- CREATE

	// create a SDL Surface
	surface = IMG_Load("./Assets/player.png");

	// Create bkgd texture
	SDL_Texture* player;

	//place surface into the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);


	// set bkgdPost x, y, width and hight
	playerPos.x = 500;
	playerPos.y = 700;
	playerPos.w = 181;// player is the green bar get the w and h from it, look in the folder Properties
	playerPos.h = 58;

	// Player image -- CREATE END

	// SDL event to handle input
	SDL_Event event;

	// Create bullets and place in list
	for (int i = 0; i < 10; i++) // 10 bullets
	{
		Bullet tempBullet(renderer, i + 5, i + 5); // pasing to colaction X,Y

		bulletList.push_back(tempBullet);
	}

	//Clear enemy list *4*
	enemyList.clear();

	//Fill enemy list 6 enemyes
	for (int i = 0; i < 6; i++)
	{
		Enemy tempEnemy(renderer);

		enemyList.push_back(tempEnemy);
	}

	// init audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load LASER SOUND
	laser = Mix_LoadWAV("./Assets/laser.wav");

	// Load EXPLISION SOUND
	explosion = Mix_LoadWAV("./Assets/explosion.wav");

	//load and loop backgraundmusic
	bgm = Mix_LoadMUS("./Assets/backgroundloop.wav");

	// If the music is not playing, play it and loop
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm, -1);
	}

	// init player score, lives and font
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	TTF_Init();
	font = TTF_OpenFont("./Assets/Cosmic Blaster.ttf", 20);//20

	//set up rectagles
	scorePos.x = scorePos.y = 10;//10
	livesPos.x = 10; //pixels//10
	livesPos.y = 40; //pixels //40

	// Titles initiating 
	UpdateScore(renderer);
	UpdateLives(renderer);

	// set up our enum to handle all the game states *ADD LEVEL2
	enum GameState { MENU, INSTRUCTIONS, GAME, LEVEL2, WIN, LOSE };

	// Variable to track where we are in the game
	GameState gameState = MENU;

	// bool valuse to alow movement through the individual states 
	bool menu, instructions, game, level2, win, lose;

	// Clear  pickup list, create pickup, put pickin list
	//clear pickup list
	pickupList.clear();

	Pickup tempPickup(renderer);

	pickupList.push_back(tempPickup);


	//bassic program loop
	while (!quit)
	{
		switch (gameState)
		{
		case MENU:

			menu = true;

			std::cout << "The Game State is MENU" << endl;
			std::cout << "The I Key for Instructions" << endl;
			std::cout << "Press the S Key to Start The Game" << endl;
			std::cout << "Press the Q Key to Quit Game" << endl;
			std::cout << endl;

			// load instructions background
			// create a SDL surface
			surface = IMG_Load("./Assets/mainmenu.png");

			// place surface into the tecture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface 
			SDL_FreeSurface(surface);

			// background image -- CREATE END

			while (menu)
			{
				// create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// cheak for input 
				if (SDL_PollEvent(&event))
				{
					// close window by the window's X button
					if (event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch (event.type)
					{
						/*Look for a Keypress*/
					case SDL_KEYUP:

						/*Check the SDLKey values and more change the coords*/
						switch (event.key.keysym.sym)
						{
						case SDLK_i:
							menu = false;
							gameState = INSTRUCTIONS;
							break;

						case SDLK_s:
							menu = false;
							gameState = GAME;
							break;

						case SDLK_q:
							quit = true;
							menu = false;
							break;

						default:
							break;
						}
					}
				}

				// DRAW ***************************************************

				// Draw Section
				//clear the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				//draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW ***************************************************

			} // ends menu screen
			break;

		case INSTRUCTIONS:

			instructions = true;

			std::cout << "The Game State is INSTRUCTIONS" << endl;
			std::cout << "Press the S Key to Start The Game" << endl;
			std::cout << "Press the M Key to go to the Main Menu" << endl;
			std::cout << "Press the Q Key to Quite Game" << endl;
			std::cout << endl;

			// load instructions background 
			// create a SDL surface
			surface = IMG_Load("./Assets/instructions2.png");

			//place suface into the texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface
			SDL_FreeSurface(surface);

			// background image --CREATE END

			while (instructions)
			{
				// create deltatime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input 
				if (SDL_PollEvent(&event))
				{
					// close window by the window 's button
					if (event.type == SDL_QUIT)
					{
						quit = true;
						instructions = false;
						break;
					}

					switch (event.type)
					{
						// look for a keypress
					case SDL_KEYUP:
						// Check the SDLKey values and move change the coords
						switch (event.key.keysym.sym) {
						case SDLK_m:
							instructions = false;
							gameState = MENU;
							break;
						case SDLK_s:
							instructions = false;
							gameState = GAME;
							break;
						case SDLK_q:
							quit = true;
							instructions = false;
							break;

						default:
							break;
						}
					}
				}

				// DRAW   ************************************************************

				// Draw Section
				//Clear the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1 
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				// draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW  ************************************************************

			}// ENDs Instructions screen

			break;

		case GAME:
		{

			game = true;

			std::cout << "The Game State is GAME" << endl;

			//load instructions background 
			//create a SDL surface
			surface = IMG_Load("./Assets/bkgd.png");

			// place surface into the texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface
			SDL_FreeSurface(surface);

			// start the game fresh 
			enemyList.clear();

			// fill enemy list
			for (int i = 0; i < 6; i++)
			{
				Enemy tempEnemy(renderer);
				enemyList.push_back(tempEnemy);

			}
			//clear pick uplist
			pickupList.clear();

			Pickup tempPickup(renderer);

			pickupList.push_back(tempPickup);


			// reset player's score and lives
			playerScore = 0;
			playerLives = 3;


			while (game)
			{
				// create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					// close window by the window's X button
					if (event.type == SDL_QUIT) {
						quit = true;
						game = false;
						break;

					}

					switch (event.type) //*2*
					{
						/* Look for a keypress    */
					case SDL_KEYUP:

						// Check the Space bar *2*
						switch (event.key.keysym.sym)
						{
						case SDLK_SPACE:
							CreateBullet();
							pickupList[0].Reset();
							break;
						default:
							break;
						}
					}
				}


				// player movement  
				// get the keyboard state  
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				// check which arrows are pressed 
				if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					xDir = -1;
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					xDir = 1;
				}
				else
				{
					xDir = 0;
				}

				// UPDATE ********************************************* *3*

				// Get new position to move player using direction and deltaime *3*
				pos_X += (playerSpeed * xDir) * deltaTime;

				// move player 
				playerPos.x = (int)(pos_X + 0.5f);

				// keep player on screen
				//moving left 
				if (playerPos.x < 0)
				{
					playerPos.x = 0;
					pos_X = 0;

				}
				// keep player on screen *3*
				//moving right
				if (playerPos.x > 1024 - playerPos.w)
				{
					playerPos.x = 1024 - playerPos.w;
					pos_X = 1024 - playerPos.w;

				}

				//UPDATE Bullets 
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Update(deltaTime);
					}
				}

				// update pickup
				pickupList[0].Update(deltaTime);


				// UPDATE enemy List ******
				for (int i = 0; i < 6; i++)
				{
					enemyList[i].Update(deltaTime);
				}

				// Cheack for collision of bullets with enemies
				// for loop to scroll through all the player's bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					// cheack to see if this bullet is Active in the world
					if (bulletList[i].active == true)
					{
						// check all enemies aginst active bullet
						for (int j = 0; j < enemyList.size(); j++)
						{
							// see if ther is a collision between this bullet and this enemy using SDL
							if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
							{
								// Play Explosion Sound 
								Mix_PlayChannel(-1, explosion, 0);

								// rest the enemy
								enemyList[j].Reset();

								// rest the bullet
								bulletList[i].Reset();

								// give player points
								playerScore += 50;

								// cheak to see if the player can go to level2
								if (playerScore >= 500)
								{
									game = false;
									gameState = LEVEL2;

								}

							}

						}

					}

				}


				// check for collision with the player and all enemies
				// Cheack ALL enemies against Active Bullet
				for (int i = 0; i < enemyList.size(); i++)
				{
					// see if there is a collision between this bullet and this enemy using SDL
					if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
					{
						// Play explosion SOUND 
						Mix_PlayChannel(-1, explosion, 0);

						// rest the enemy
						enemyList[i].Reset();

						// give player points
						playerLives -= 1;

						// check to see if the player loses
						if (playerLives <= 0)
						{
							game = false;
							gameState = LOSE;


						}

					}

				}

				// check for collision with the player and pickups
				for (int i = 0; i < pickupList.size(); i++)
				{

					// see if there is a collision between the player and pickup using SDL
					if (SDL_HasIntersection(&playerPos, &pickupList[i].posRect))
					{

						//cheack to see if this id the lose a life pickup
						//if so deactive the pickup and remove a player'S life
						// cheacking for game over
						if (pickupList[i].pickupType == "lives")
						{
							// deactivate the pickup
							pickupList[i].DeActivate();

							// play explosion sound
							Mix_PlayChannel(-1, explosion, 0);

							// give player Points
							playerLives -= 1;

							//Check to see if the player Loses
							if (playerLives <= 0)
							{
								game = false;
								gameState = LOSE;

							}
						}
						else if (pickupList[i].pickupType == "bomb")
						{
							// check to see if this is the destroy all enemies pickup
							//if so cycle through enemies and reset them and give player points for each

							//deactivate the pickup
							pickupList[i].DeActivate();

							// play laser sound
							Mix_PlayChannel(-1, laser, 0);

							//Cycle through all enemies
							for (int i = 0; i < enemyList.size(); i++)
							{
								//reset the Enemy
								enemyList[i].Reset();

								//give player points
								playerScore += 10;

								// cheack to see if the player can go to level2
								if (playerScore >= 500)
								{
									game = false; // DEMO
									gameState = LEVEL2;
								}

							}

						}

					}



				}

				//Update score and lives on screen text
				if (playerScore != oldScore)
				{
					UpdateScore(renderer);
				}

				if (playerLives != oldLives)
				{
					UpdateLives(renderer);
				}


				// UPDATE ********************************************* 


				// DRAW ***********************************************

					// clear the old buffer
				SDL_RenderClear(renderer);

				//Prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				// draw bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Draw(renderer);
					}

				}

				//Prepare to draw player
				SDL_RenderCopy(renderer, player, NULL, &playerPos);

				// draw enemy list 
				for (int i = 0; i < 6; i++)
				{
					enemyList[i].Draw(renderer);

				}

				// Draw pickup
				pickupList[0].Draw(renderer);


				// draw score texture
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

				// draw the lives texture
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);



				// Draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW ******************************************

			}// ENDS game screen

			break;

		}

		case LEVEL2:

			level2 = true;

			std::cout << "The Game State is LEVEL 2" << endl;

			
			// start the game fresh 
			enemyList.clear();

			// fill enemy list
			for (int i = 0; i < 6; i++)
			{
				Enemy tempEnemy(renderer);
				enemyList.push_back(tempEnemy);

			}

			while (level2)
			{
				// create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					// close window by the window's X button
					if (event.type == SDL_QUIT) {
						quit = true;
						level2 = false;
						break;

					}

					switch (event.type) //*2*
					{
						/* Look for a keypress   *2* */
					case SDL_KEYUP:

						// Check the Space bar *2*
						switch (event.key.keysym.sym)
						{
						case SDLK_SPACE:
							CreateBullet();
							break;
						default:
							break;
						}
					}
				}

				// player movement  
				// get the keyboard state  
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				// check which arrows are pressed 
				if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					xDir = -1;
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					xDir = 1;
				}
				else
				{
					xDir = 0;
				}

				// UPDATE ********************************************* *3*

				// Get new position to move player using direction and deltaime *3*
				pos_X += (playerSpeed * xDir) * deltaTime;

				// move player 
				playerPos.x = (int)(pos_X + 0.5f);

				// keep player on screen
				//moving left 
				if (playerPos.x < 0)
				{
					playerPos.x = 0;
					pos_X = 0;

				}
				// keep player on screen *3*
				//moving right
				if (playerPos.x > 1024 - playerPos.w)
				{
					playerPos.x = 1024 - playerPos.w;
					pos_X = 1024 - playerPos.w;
				}

				//UPDATE Bullets 
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Update(deltaTime);
					}
				}

				// UPDATE enemy List ********************************************* 
				for (int i = 0; i < 6; i++)
				{
					enemyList[i].Update(deltaTime);
				}

				// Cheack for collision of bullets with enemies
				// for loop to scroll through all the player's bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					// cheack to see if this bullet is Active in the world
					if (bulletList[i].active == true)
					{
						// check all enemies aginst active bullet
						for (int j = 0; j < enemyList.size(); j++)
						{
							// see if ther is a collision between this bullet and this enemy using SDL
							if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
							{
								// Play Explosion Sound 
								Mix_PlayChannel(-1, explosion, 0);

								// rest the enemy
								enemyList[j].Reset();

								// rest the bullet
								bulletList[i].Reset();

								// give player points
								playerScore += 50;

								// cheak to see if the player wins
								if (playerScore >= 1000)
								{
									level2 = false;
									gameState = WIN;
								}

							}

						}

					}

				}

				// check for collision with the player and all enemies
				// Cheack ALL enemies against Active Bullet
				for (int i = 0; i < enemyList.size(); i++)
				{
					// see if there is a collision between this bullet and this enemy using SDL
					if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
					{
						// Play explosion SOUND 
						Mix_PlayChannel(-1, explosion, 0);

						// rest the enemy
						enemyList[i].Reset();

						// give player points
						playerLives -= 1;

						// check to see if the player loses
						if (playerLives <= 0)
						{
							level2 = false;
							gameState = LOSE;

						}

					}

				}

				//Update score and lives on screen text
				if (playerScore != oldScore)
				{
					UpdateScore(renderer);
				}

				if (playerLives != oldLives)
				{
					UpdateLives(renderer);
				}


				// UPDATE ********************************************* 


				// DRAW ***********************************************

					// clear the old buffer
				SDL_RenderClear(renderer);

				//Prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				// draw bullets
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Draw(renderer);
					}

				}

				//Prepare to draw player
				SDL_RenderCopy(renderer, player, NULL, &playerPos);

				// draw enemy list 
				for (int i = 0; i < 6; i++)
				{
					enemyList[i].Draw(renderer);

				}

				// draw score texture
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

				// draw the lives texture
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);



				// Draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW ******************************************

			}// 88888888888888888888888888888888888888888888888888888888888888888END game screen

			break;

		case WIN:

			win = true;

			std::cout << "The Game State is WIN" << endl;
			std::cout << "Press the S Key to Start The Game" << endl;
			std::cout << "Press the M Key to go to the Main Menu" << endl;
			std::cout << "Press the Q Key to Quite Game" << endl;
			std::cout << endl;

			// load win background 
			// create a SDL surface
			surface = IMG_Load("./Assets/win.png");

			// place surface into the texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			//free the surface
			SDL_FreeSurface(surface);

			// background image -- CREATE END

			while (win)
			{
				// create deltaTime
				// create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					// close window by the window's X button
					if (event.type == SDL_QUIT) {
						quit = true;
						win = false;
						break;

					}

					switch (event.type)
					{

						// Look for a keypress 
					case SDL_KEYUP:

						switch (event.key.keysym.sym) {
						case SDLK_m:
							win = false;
							gameState = MENU;
							break;
						case SDLK_s:
							win = false;
							gameState = GAME;
							break;
						case SDLK_q:
							quit = true;
							win = false;
							break;
						default:
							break;
						}
					}

				}

				// DRAW ***************************
				//Draw Section
				//claer the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				// draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW ********************************

			} // ends win screen
			break;

		case LOSE:

			lose = true;
			std::cout << "The Game State is LOSE" << endl;
			std::cout << "Press the S Key to Start The Game" << endl;
			std::cout << "Press the M Key to go to the Main Menu" << endl;
			std::cout << "Press the Q Key to Quite Game" << endl;
			std::cout << endl;

			// load lose background
			// craete a SDL surface
			surface = IMG_Load("./Assets/lose.png");

			//place surface into the texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			// free the surface
			SDL_FreeSurface(surface);

			//background image -- CREATE END
			while (lose)
			{
				// create deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				if (SDL_PollEvent(&event))
				{
					// close window by the window's X button
					if (event.type == SDL_QUIT) {
						quit = true;
						lose = false;
						break;

					}

					switch (event.type)
					{

						// Look for a keypress 
					case SDL_KEYUP:
						// cheack the SDLKey valus and Move change the coords
						switch (event.key.keysym.sym) {
						case SDLK_m:
							lose = false;
							gameState = MENU;
							break;
						case SDLK_s:
							lose = false;
							gameState = GAME;
							break;
						case SDLK_q:
							quit = true;
							lose = false;
							break;
						default:
							break;
						}
					}

				}
				// DRAW ***************************
				//Draw Section
				//claer the old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				// draw new info to the screen
				SDL_RenderPresent(renderer);

				// END DRAW ********************************

			}// end lose screen

			break;


		}// endas swith for gameStates

	}// ends game loop 


	// Close and destrioy the window
	SDL_DestroyWindow(window);

	//Clean Up 
	SDL_Quit();

	return 0;


}

