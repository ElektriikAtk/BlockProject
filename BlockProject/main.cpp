#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include "functions.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
std::vector<SDL_FRect> blockList;


Player* player;
void init(SDL_Renderer* renderer) //Performed once at startup. Maybe to preload textures?
{
    SDL_SetWindowSurfaceVSync(window, 1); //Sets VSYNC for window to 1, maybe unnecessary?

    SDL_Surface* surface = IMG_Load("assets/gd2.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface) {
        throw std::invalid_argument{ "Failed to load surface" };
    }
    if (!texture) {
        throw std::invalid_argument{ "Failed to load texture" };
    }
    SDL_DestroySurface(surface);

    player = new Player{ 500, 50, 50, texture };
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

void createBlocks(SDL_Renderer* renderer)
{
    if (blockList.size() == 0)
    {
        for (int i = 0; i < 120; i += 12)
        {
            SDL_FRect block = { i, i ,10, 10 };
            blockList.push_back(block);

        }
    }
    for (int i = 0; i < blockList.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // static box
        SDL_RenderFillRect(renderer, &blockList[i]);
        //SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);
    }
}

    void run()
    {
        createBlocks(renderer);
        //SDL_RenderClear(renderer);
        movement(*player);
        player->render(renderer);
        SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);
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

