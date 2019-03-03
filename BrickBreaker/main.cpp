#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

bool quit = false;
// SDL
SDL_Event event;
SDL_Window* _window;
SDL_Renderer *_renderer;
SDL_Surface *background;
SDL_Texture *backgroundTexture;
SDL_Rect backgroundRect;
SDL_Surface *bat;
SDL_Texture *batTexture;
SDL_Surface *ball;
SDL_Texture *ballTexture;

// ball
SDL_Rect ballRect;
int ballX = 10;
int ballY = 10;
int ballVelocityX = 3;
int ballVelocityY = 3;

// screen
const int WIDTH = 800;
const int HEIGHT = 600;
const int MIN_WIDTH = 0;
const int MIN_HEIGHT = 0;

// bat
int batX = WIDTH / 2;
int batY = HEIGHT - 30;//size of sprite
int batSpeed = 5;

// bricks
int brickWidth = 80;
int brickHeight = 35;
SDL_Surface *brick;
SDL_Texture *brickTexture;
const int BRICK_ROWS = 3;
const int BRICK_COLUMNS = 7;
SDL_Rect brickRect[BRICK_ROWS][BRICK_COLUMNS];

// game win conditions
int deleteBrickCount = 0;
int numberOfBricks = (BRICK_ROWS * BRICK_COLUMNS);

void InitializeBricks(){
    int yPos = 50;
    int xPos = 50;
    for(int i = 0; i < BRICK_ROWS; ++i){
        for(int j = 0; j < BRICK_COLUMNS; ++j){
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
    if(ballX <= MIN_WIDTH || ballX >= WIDTH-20){//30 is the width of the ball - extract this better later
        ballVelocityX *= -1;
    }
    // hit the Y boundaries
    if(ballY <= MIN_HEIGHT || ballY >= HEIGHT-20){//30 is the width of the ball - extract this better later
        ballVelocityY *= -1;
    }

    // bat collision
    int ballScaling = 20;
    if(ballY + ballScaling >= batY && ballY+ballScaling <= batY + 30 && ballX + ballScaling >= batX && ballX + ballScaling <= batX + 60){
        ballVelocityY *= -1;
    }
}

bool BallBrickCollisionDetected(SDL_Rect brick, SDL_Rect ball){
    // check if the rectangles are overlapping
    int brickXMin = brick.x;
    int brickXMax = brick.x + brick.w;
    if(ball.x >= brickXMin-20 && ball.x <= brickXMax){
        int brickYMin = brick.y;
        int brickYMax = brick.y + brick.h;
        if(ball.y <= brickYMax && ball.y >= brickYMin - 20){
            return true;
        }
    }
    return false;
}

void BallBrickCollision(){
    bool hit;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 7; ++j){
            hit = BallBrickCollisionDetected(brickRect[i][j], ballRect);
            if(hit){
                brickRect[i][j].x = 3000; // fishy...
                ballVelocityY *= -1;
                deleteBrickCount += 5;
            }
        }
    }
}

void Cleanup(){
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(batTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(backgroundTexture);

    SDL_FreeSurface(bat);
    SDL_FreeSurface(brick);
    SDL_FreeSurface(ball);
    SDL_FreeSurface(background);

    SDL_DestroyRenderer(_renderer);

    SDL_DestroyWindow(_window);
}

void WinTheGame(){
    SDL_RenderClear(_renderer);

    SDL_Surface *winScreen = SDL_LoadBMP("bmps\\win.bmp");
    SDL_Texture *winScreenTexture = SDL_CreateTextureFromSurface(_renderer, winScreen);
    SDL_Rect winRect = {250,200,350,350};
    SDL_RenderCopy(_renderer, winScreenTexture, NULL, &winRect);
    SDL_RenderPresent(_renderer);
    SDL_Delay(5000);
}

int main(int argc, char ** argv){
    // Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error initializing SDL" << endl;
		return 1;
	}

	// Setup window
	_window = SDL_CreateWindow("Classic Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN); // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (_window == nullptr) {
		SDL_Quit();
		cout << "Window error" << endl;
		return 1;
	}
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr) {
		SDL_Quit();
		cout << "Renderer error" << endl;
		return 1;
	}

    // Setup background texture
    background = SDL_LoadBMP("bmps\\bk.bmp");
    backgroundTexture = SDL_CreateTextureFromSurface(_renderer, background);
    backgroundRect = {0, 0, WIDTH, HEIGHT};

    // Setup brick textures
    brick = SDL_LoadBMP("bmps\\brick.bmp");
    brickTexture = SDL_CreateTextureFromSurface(_renderer, brick);
    InitializeBricks();

    // Setup bat texture
    bat = SDL_LoadBMP("bmps\\bat.bmp");
    batTexture = SDL_CreateTextureFromSurface(_renderer, bat);

    // Setup the ball texture
    ball = SDL_LoadBMP("bmps\\ball.bmp");
    ballTexture = SDL_CreateTextureFromSurface(_renderer, ball);

    float now = 0;
    float lastTime = 0;
    float deltaTime = 0;

    while(!quit){
        EventHandler();
        SDL_RenderClear(_renderer);

        now = SDL_GetTicks();
        deltaTime = now - lastTime;

        ballRect = {ballX, ballY, 20, 20};
        SDL_Rect batRect = {batX, batY, 60, 30};

        CheckForBallCollision();
        BallBrickCollision();
        MoveBall();

        if(deleteBrickCount >= numberOfBricks){
            WinTheGame();
            break;
        }

        // copy all textures
        SDL_RenderCopy(_renderer, backgroundTexture, NULL, &backgroundRect);
        int yPos = 50;
        int xPos = 50;
        for(int i = 0; i < BRICK_ROWS; ++i){
            for(int j = 0; j < BRICK_COLUMNS; ++j){
            SDL_RenderCopy(_renderer, brickTexture, NULL, &brickRect[i][j]);
                xPos +=50;
            }
            yPos += 50;
        }
        SDL_RenderCopy(_renderer, ballTexture, NULL, &ballRect);
        SDL_RenderCopy(_renderer, batTexture, NULL, &batRect);

        // Render everything in the _renderer to the screen
        SDL_RenderPresent(_renderer);

        lastTime = now;
    }

    Cleanup();
    SDL_Quit();

    return 0;
}
