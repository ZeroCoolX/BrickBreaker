#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

bool quit = false;
SDL_Event event;

// real position on screen
SDL_Rect ballRect;
int ballX = 10;
int ballY = 10;
int ballVelocityX = 3;
int ballVelocityY = 3;

const int WIDTH = 800;
const int HEIGHT = 600;
const int MIN_WIDTH = 0;
const int MIN_HEIGHT = 0;

int batX = WIDTH / 2;
int batY = HEIGHT - 30;//size of sprite
int batSpeed = 5;

int brickWidth = 80;
int brickHeight = 35;
SDL_Surface *brick;
SDL_Texture *brickTexture;
SDL_Rect brickRect[3][7];

void InitializeBricks(){
    int yPos = 50;
    int xPos = 50;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 7; ++j){
            brickRect[i][j] = {xPos, yPos, brickWidth, brickHeight};
            xPos += 100;
        }
        xPos = 50;
        yPos += 50;
    }
}

void EventHandler(){
    SDL_PollEvent(&event);
    // Check for quit events
    quit = (event.type == SDL_QUIT);
    if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_LEFT && batX > MIN_WIDTH){
            // move bat five pixel to the left
            batX -= batSpeed;
        }else if(event.key.keysym.sym == SDLK_RIGHT && batX < WIDTH-60){
            // move bat five pixel to the right
            batX += batSpeed;
        }
    }
}
void MoveBall(){
    ballX += ballVelocityX;
    ballY += ballVelocityY;
}

void CheckForBallCollision(){
    // hit the X boundaries
    if(ballX <= MIN_WIDTH || ballX >= WIDTH-30){//30 is the width of the ball - extract this better later
        ballVelocityX *= -1;
    }
    // hit the Y boundaries
    if(ballY <= MIN_HEIGHT || ballY >= HEIGHT-30){//30 is the width of the ball - extract this better later
        ballVelocityY *= -1;
    }

    // bat collision
    int ballScaling = 20;
    if(ballY + ballScaling >= batY && ballY+ballScaling <= batY + 30 && ballX + ballScaling >= batX && ballX + ballScaling <= batX + 60){
        ballVelocityY *= -1;
    }
}

void BallBrickCollision(){
    bool a;

}

int main(int argc, char ** argv){
    // Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error initializing SDL" << endl;
		return 1;
	}

	// Setup window
	SDL_Window* window = SDL_CreateWindow("Classic Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN); // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr) {
		SDL_Quit();
		cout << "Window error" << endl;
		return 1;
	}
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_Quit();
		cout << "Renderer error" << endl;
		return 1;
	}

    // Setup background texture
    SDL_Surface *background = SDL_LoadBMP("bmps\\bk.bmp");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_Rect backgroundRect = {0, 0, WIDTH, HEIGHT};

    // Setup brick textures
    brick = SDL_LoadBMP("bmps\\brick.bmp");
    brickTexture = SDL_CreateTextureFromSurface(renderer, brick);
    InitializeBricks();

    // Setup bat texture
    SDL_Surface *bat = SDL_LoadBMP("bmps\\bat.bmp");
    SDL_Texture *batTexture = SDL_CreateTextureFromSurface(renderer, bat);

    // Setup the ball texture
    SDL_Surface *ball = SDL_LoadBMP("bmps\\ball.bmp");
    SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(renderer, ball);

    float now = 0;
    float lastTime = 0;
    float deltaTime = 0;

    while(!quit){
        EventHandler();
        SDL_RenderClear(renderer);

        now = SDL_GetTicks();
        deltaTime = now - lastTime;

        ballRect = {ballX, ballY, 20, 20};
        SDL_Rect batRect = {batX, batY, 60, 30};

        CheckForBallCollision();
        MoveBall();

        // copy all textures
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        int yPos = 50;
        int xPos = 50;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 7; ++j){
            SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[i][j]);
                xPos +=50;
            }
            yPos += 50;
        }
        SDL_RenderCopy(renderer, ballTexture, NULL, &ballRect);
        SDL_RenderCopy(renderer, batTexture, NULL, &batRect);

        // Render everything in the renderer to the screen
        SDL_RenderPresent(renderer);

        lastTime = now;
    }
//    	SDL_DestroyWindow(window);
//
//    	SDL_DestroyRenderer(renderer);
//
//	    SDL_DestroyTexture(texture);
//
//    	SDL_FreeSurface(surface);

    SDL_Quit();

    return 0;
}
