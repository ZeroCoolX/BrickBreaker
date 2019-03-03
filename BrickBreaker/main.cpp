#include <SDL2/SDL.h>

using namespace std;

bool quit = false;
SDL_Event event;

int ballX = 10;
int ballY = 10;
int ballVelocityX = 1;
int ballVelocityY = 1;


const int WIDTH = 800;
const int HEIGHT = 600;
const int MIN_WIDTH = 0;
const int MIN_HEIGHT = 0;

int batX = WIDTH / 2;
int batY = HEIGHT - 30;//size of sprite

void EventHandler(){
    SDL_PollEvent(&event);
    // Check for quit events
    quit = (event.type == SDL_QUIT);
}

void MoveBall(){
    ballX += ballVelocityX;
    ballY += ballVelocityY;
}

void CheckForBallCollision(){
    if(ballX <= MIN_WIDTH || ballX >= WIDTH-30){//30 is the width of the ball - extract this better later
        ballVelocityX *= -1;
    }
    if(ballY <= MIN_HEIGHT || ballY >= HEIGHT-30){//30 is the width of the ball - extract this better later
        ballVelocityY *= -1;
    }
}

int main(int argc, char ** argv){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Classic Brick Breaker",
            SDL_WINDOWPOS_UNDEFINED, // title x pos - however this is not really working
            SDL_WINDOWPOS_UNDEFINED, // title y pos - however this is not really working
            WIDTH, // window width
            HEIGHT, // window height
            0);  // SDL Window flags
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);


    // Setup background texture
    SDL_Surface *background = SDL_LoadBMP("bmps\\bk.bmp");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_Rect backgroundRect = {0, 0, WIDTH, HEIGHT};

    // Setup bat texture
    SDL_Surface *bat = SDL_LoadBMP("bmps\\bat.bmp");
    SDL_Texture *batTexture = SDL_CreateTextureFromSurface(renderer, bat);
    SDL_Rect batRect = {batX, batY, 60, 30};

    // Setup the ball texture
    SDL_Surface *ball = SDL_LoadBMP("bmps\\ball.bmp");
    SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(renderer, ball);

    float now = 0;

    while(!quit){
        EventHandler();

        now = SDL_GetTicks();

        CheckForBallCollision();
        MoveBall();
        SDL_Rect ballRect = {ballX, ballY, 20, 20};

        // copy all textures
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, batTexture, NULL, &batRect);
        SDL_RenderCopy(renderer, ballTexture, NULL, &ballRect);

        // Render everything in the renderer to the screen
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    SDL_Quit();

    return 0;
}
