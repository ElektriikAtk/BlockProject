#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "functions.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
RenderContext renderContext;
std::vector<SDL_FRect> blockList;

Player* player;




void init(SDL_Renderer* renderer) //Performed once at startup. Maybe to preload textures?
{
    SDL_SetWindowSurfaceVSync(window, 1); //Sets VSYNC for window to 1, maybe unnecessary?
    //SDL_SetRenderScale(renderer, SCALING_FACTOR, SCALING_FACTOR);

    //setupRenderer(renderer);

    SDL_Surface* surface = IMG_Load("assets/box_250x250.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface) {
        throw std::invalid_argument{ "Failed to load surface" };
    }
    if (!texture) {
        throw std::invalid_argument{ "Failed to load texture" };
    }
    SDL_DestroySurface(surface);
    float player_w = 0, player_h = 0;
    SDL_GetTextureSize(texture, &player_w, &player_h);
    
    player = new Player{ WINDOW_WIDTH - player_w, WINDOW_HEIGHT - player_h, playerSpeed, texture }; //Find better solution
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("BlockProject", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);

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
        SDL_RenderClear(renderer);
        SDL_SetRenderScale(renderer, SCALING_FACTOR, SCALING_FACTOR);
        setupRenderer(renderer); // So that the renderContext updates if the window is resized

        player->render(renderer);
        createBlocks(renderer, blockList);        
        player->handleCollision(blockList);
        player->movement();

        SDL_SetRenderDrawColor(renderer, 63, 27, 71, 255);
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

