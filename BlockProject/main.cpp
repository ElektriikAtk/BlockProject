#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "functions.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALING_FACTOR 2


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
std::vector<SDL_FRect> blockList;


Player* player;

void init(SDL_Renderer* renderer) //Performed once at startup. Maybe to preload textures?
{
    SDL_SetWindowSurfaceVSync(window, 1); //Sets VSYNC for window to 1, maybe unnecessary?

    


    SDL_Surface* surface = IMG_Load("assets/box.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface) {
        throw std::invalid_argument{ "Failed to load surface" };
    }
    if (!texture) {
        throw std::invalid_argument{ "Failed to load texture" };
    }
    SDL_DestroySurface(surface);
    float player_h;
    SDL_GetTextureSize(texture, NULL, &player_h);
    float playerSpeed = 100;
    player = new Player{ playerSpeed, 0, WINDOW_HEIGHT - player_h, texture }; //Find better solution
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);


}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("BlockProject", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    try
    {
        init(renderer);
    }
    catch (const std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}


/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    const bool* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE] ||    //event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

    void run()
    {
        SDL_SetRenderScale(renderer, SCALING_FACTOR, SCALING_FACTOR); //Is it better RENDER_SCALE or getRenderScale()?
        //SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH/SCALING_FACTOR, WINDOW_HEIGHT / SCALING_FACTOR, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
        
        SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);
        SDL_RenderClear(renderer);
        movement(*player);
        player->colissionCheck(blockList);
        player->render(renderer);
        createBlocks(renderer, blockList);
        
        
        SDL_RenderPresent(renderer);
    }


    SDL_AppResult SDL_AppIterate(void* appstate)
    {
        try
        {
            run();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return SDL_APP_FAILURE;
        }    
        return SDL_APP_CONTINUE;
    }


/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}

