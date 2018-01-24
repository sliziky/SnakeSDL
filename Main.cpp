#include <iostream>
#include <stdlib.h>
#include <vector>
#include <SDL.h>
#include <time.h>
#include <random>
using namespace std;

bool checkBorders(int snakeX, int snakeY, int width, int height, int size);

int main(int argc, char* argv[]) {
	/* Window attributes */
	const int windowHeight = 480;
	const int windowWidth = 640;

	int lengthOfSnake = 1;
	int fCounter = 0;
	
	time_t now; /* RANDOM */
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(0, 23); // define the range
	
	/* Keys */
	bool keyRight = true;
	bool keyLeft = false;
	bool keyUp = false;
	bool keyDown = false;
	bool putting = true;
	
	/* Direction which is the snake going */
	bool goingRight = true; /* Initial movement of snake is to the right */
	bool goingLeft = false;
	bool goingUp = false;
	bool goingDown = false;
	
	int size = 20; /* Size of 1 block of the snake*/
	bool playing = true;

	SDL_Event e;
	SDL_Window *window;
	SDL_Renderer* renderer;
	const Uint8 *keystate;
	
	
	time(&now); /* random stuff */
	srand((unsigned int)now);
	
	SDL_Rect fruit;
	fruit.w = size; /* Creating fruit and setting first random position */
	fruit.h = size;
	fruit.x = rand() % (windowWidth/size) * size;
	fruit.y = rand() % (windowHeight/size) * size;

	
	SDL_Rect snake[256]; /* Each element represents one block of our snake */
	snake[0].x = distr(eng)*size; /* Creating the 'head' of the snake */
	snake[0].y = rand() % (windowHeight / size) * size;
	snake[0].h = size;
	snake[0].w = size;

	
	SDL_Init(SDL_INIT_VIDEO);              /* initializing SDL */

										  
	window = SDL_CreateWindow( /* Creating WINDOW */
		"SnakeGame",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,           
		windowWidth,                               
		windowHeight,                             
		SDL_WINDOW_OPENGL                  
	);

	renderer = SDL_CreateRenderer(window, -1, 0);

	while (playing) { /* Main LOOP */

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
			{
				playing = false;
			}
			if (e.type == SDL_KEYDOWN) { /* if the KEY was pressed */

				/* Not allowed to go the opposite direction - except when the length of snake is 1 */
				if (e.key.keysym.sym == SDLK_LEFT && goingRight && lengthOfSnake != 1) { playing = false; }
				if (e.key.keysym.sym == SDLK_RIGHT && goingLeft && lengthOfSnake != 1 ) { playing = false; }
				if (e.key.keysym.sym == SDLK_UP && goingDown && lengthOfSnake != 1 ) { playing = false; }
				if (e.key.keysym.sym == SDLK_DOWN && goingUp && lengthOfSnake != 1) { playing = false; }
				
				switch (e.key.keysym.sym) {

					case SDLK_RIGHT:
						keyRight = true;
						keyLeft = false;
						keyUp = false;
						keyDown = false;
						break;

					case SDLK_LEFT:
						keyLeft = true;
						keyUp = false;
						keyRight = false;
						keyDown = false;
						break;

					case SDLK_UP:
						keyUp = true;
						keyLeft = false;
						keyRight = false;
						keyDown = false;
						break;

					case SDLK_DOWN:
						keyDown = true;
						keyLeft = false;
						keyUp = false;
						keyRight = false;
						break;
				}
			}




		}
		
		if (snake[0].x == fruit.x && snake[0].y == fruit.y) { /* If we eat the FRUIT */
	
			snake[lengthOfSnake].w = size;
			snake[lengthOfSnake].h = size;

			/* We append another block of the snake */
			if (keyUp) {
				snake[lengthOfSnake].x = snake[lengthOfSnake - 1].x;
				snake[lengthOfSnake].y = snake[lengthOfSnake - 1].y + size;
			}
			if (keyDown) { 
				snake[lengthOfSnake].x = snake[lengthOfSnake - 1].x; 
				snake[lengthOfSnake].y = snake[lengthOfSnake - 1].y - size; 
			}
			if (keyRight) { 
				snake[lengthOfSnake].x = snake[lengthOfSnake - 1].x - size; 
				snake[lengthOfSnake].y = snake[lengthOfSnake - 1].y; 
			}
			if (keyLeft) { 
				snake[lengthOfSnake].x = snake[lengthOfSnake - 1].x + size; 
				snake[lengthOfSnake].y = snake[lengthOfSnake - 1].y; 
			}
			
			do { /* Do NOT put fruit into the snake */
				fruit.x = rand() % (windowWidth / size) * size;
				fruit.y = rand() % (windowHeight / size) * size;
				fCounter = 0;
				putting = true;
				for (int i = 0; i < lengthOfSnake; i++) { if (snake[i].x != fruit.x && snake[i].y != fruit.y) { fCounter++; } }
				if (fCounter == lengthOfSnake) { putting = false; }
			} while (putting);
			lengthOfSnake += 1;
			
		}

		
		/* Logic in the movement of the snake is that we replace the coordinates of [i]. block with
		   the coordinates of [i-1]. block going from the end ... [2] <- [1] ; [1] <- [0] etc.. except for the [0]. element 
		   which we change according to the key */
		if (keyRight) {
			for (int i = 1; i < lengthOfSnake; i++)
			{
				snake[lengthOfSnake - i].x = snake[lengthOfSnake - i - 1].x;
				snake[lengthOfSnake - i].y = snake[lengthOfSnake - i - 1].y;
			}
			snake[0].x += size;
			goingDown = false;
			goingLeft = false;
			goingRight = true;
			goingUp = false;
		
		}
		else if (keyLeft) {
			for (int i = 1; i < lengthOfSnake; i++)			{
				snake[lengthOfSnake - i].x = snake[lengthOfSnake - i - 1].x;
				snake[lengthOfSnake - i].y = snake[lengthOfSnake - i - 1].y;
			}
			snake[0].x -= size;
			goingDown = false;
			goingLeft = true;
			goingRight = false;
			goingUp = false;

		}
		else if (keyUp) {
		
			for (int i = 1; i < lengthOfSnake; i++) 
			{
				snake[lengthOfSnake - i].x = snake[lengthOfSnake - i - 1].x;
				snake[lengthOfSnake - i].y = snake[lengthOfSnake - i - 1].y;
			}
			snake[0].y -= size;
			goingDown = false;
			goingLeft = false;
			goingRight = false;
			goingUp = true;
			
		}
		else if (keyDown) {
			
			for (int i = 1; i < lengthOfSnake; i++) 
			{
				snake[lengthOfSnake - i].x = snake[lengthOfSnake - i - 1].x;
				snake[lengthOfSnake - i].y = snake[lengthOfSnake - i - 1].y;	
			}
			snake[0].y += size;
			goingDown = true;
			goingLeft = false;
			goingRight = false;
			goingUp = false;
		
		}
		/* Check for biting */
		for (int i = 0; i < lengthOfSnake; i++) { 
			for (int j = i+1; j < lengthOfSnake; j++) {
				if (snake[i].x == snake[j].x && snake[i].y == snake[j].y) { playing = false; }
			}
		}
		/* Check collision with borders of gameplan*/
		if (checkBorders(snake[0].x, snake[0].y, windowWidth, windowHeight, size) == false) { playing = false; };
	

		/* Rendering gameplan including snake and fruit */
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);

		/* Fruit rendering */
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &fruit);

		/* Snake rendering */
		SDL_SetRenderDrawColor(renderer, 0, 211, 0, 255);
		for (int i = 0; i < lengthOfSnake; i++) { SDL_RenderFillRect(renderer, &snake[i]); }
		
	
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 25);
	
	}
	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	cin.get();
	return 0;
}


bool checkBorders(int snakeX, int snakeY, int width, int height, int size) {

	if (snakeX < 0 || snakeY < 0 || snakeX + size > width || snakeY + size > height ) { return false; }
}

