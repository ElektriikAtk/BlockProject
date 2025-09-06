#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "functions.h"


float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

//Spritesheet
SDL_Texture* loadSprites(SDL_Surface* spriteSheetSurface)
{
    try
    {
        if (!spriteSheetSurface)
        {
            SDL_Log("Failed to load sprite sheet: %s", SDL_GetError());
            throw std::invalid_argument{"Failed to load spritesheet: " + *SDL_GetError()}; // Dereference SDL_GetError since it's a pointer
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    //SDL_SetSurfaceColorKey(spriteSheetSurface, true, SDL_MapRGB(SDL_GetPixelFormatDetails(spriteSheetSurface->format), NULL, 157, 217, 234)); // Background color

    SDL_Texture* spriteSheet = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
    SDL_DestroySurface(spriteSheetSurface);
    return spriteSheet;
}

// Returns delta time in seconds
float getDeltaTime()
{
    static Uint64 prevCounter = 0;

    Uint64 currentCounter = SDL_GetPerformanceCounter();
    if (prevCounter == 0) {
        prevCounter = currentCounter; // first frame, no delta yet
        return 0.0f;
    }

    Uint64 freq = SDL_GetPerformanceFrequency();
    float delta = (float)(currentCounter - prevCounter) / (float)freq;

    prevCounter = currentCounter;
    return delta;
}

void getScaledSize(int& w, int& h, float& scale)
{
    SDL_GetRenderScale(renderer, &scale, NULL); // Fix so that it is scaleless
    SDL_GetRenderOutputSize(renderer, &w, &h);
    w /= scale;
    h /= scale;
}

void movement(Player& object)
{
    float dt = getDeltaTime();
    const bool* keys = SDL_GetKeyboardState(NULL);
    float dx = 0.0f;
    if (keys[SDL_SCANCODE_A])  dx -= 1.0f;
    if (keys[SDL_SCANCODE_D])  dx += 1.0f;
    
    if (dx != 0.0f) {
        float len = SDL_sqrtf(dx * dx );
        dx /= len;
    }

    
    object.set_velX(dx * object.get_speed());

    const float gravity = 2500.0f;
    object.set_velY(object.get_velY() + gravity * dt);

    object.set_x(object.get_x() + dx * object.get_speed() * dt);
    object.set_y(object.get_y() + object.get_velY() * dt);



    int w = 0, h = 0;
    float scale;
    getScaledSize(w, h, scale);

    bool onGround = (object.get_y() + object.get_h() >= h);

    if (keys[SDL_SCANCODE_SPACE] && onGround) {
        object.set_velY(gravity / (-3.0f)); // jump impulse
    }

    object.set_x(clamp(object.get_x(), 0.0f, w - object.get_w()));
    object.set_y(clamp(object.get_y(), 0.0f, h - object.get_h()));
}

void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList)
{
    if (blockList.empty())
    {
        float scale;
        int w, h;
        getScaledSize(w, h, scale);
        //SDL_GetWindowSizeInPixels(window, &w, &h);
        float blockSize = 4;
        for (int i = 0; i < h; i += 10)
        {
            SDL_FRect block = { i, i, 10, 10    };
            blockList.push_back(block);

        }
    }
    for (int i = 0; i < blockList.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 25, 69, 50, 255); // static box
        SDL_RenderFillRect(renderer, &blockList[i]);
    }
}