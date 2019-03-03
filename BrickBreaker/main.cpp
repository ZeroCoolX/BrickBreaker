#include <SDL2/SDL.h>

bool quit = false;
SDL_Event event;

void EventHandler(){
    SDL_PollEvent(&event);
    // Check for quit events
    quit = (event.type == SDL_QUIT);
}

using namespace std;

int main(int argc, char ** argv){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Classic Brick Breaker",
            SDL_WINDOWPOS_UNDEFINED, // title x pos - however this is not really working
            SDL_WINDOWPOS_UNDEFINED, // title y pos - however this is not really working
            800, // window height
            600, // window width
            0);  // SDL Window flags
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);


    // Setup background texture
    SDL_Surface *background = SDL_LoadBMP("bmps\\bk.bmp");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_Rect backgroundRect = {0, 0, 800, 600};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

    // Setup the ball texture
    SDL_Rect ballRect = {20, 30, 20, 20};
    SDL_Surface *ball = SDL_LoadBMP("bmps\\ball.bmp");
    SDL_Texture *ballTexture = SDL_CreateTextureFromSurface(renderer, ball);
    SDL_RenderCopy(renderer, ballTexture, NULL, &ballRect);

    // Render everything in the renderer to the screen
    SDL_RenderPresent(renderer);

    while(!quit){
        EventHandler();
    }

    SDL_Quit();

    return 0;
}
