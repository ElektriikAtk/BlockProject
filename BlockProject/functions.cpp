#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "functions.h"

void setupRenderer(SDL_Renderer* renderer)
{
    SDL_GetRenderScale(renderer, &renderContext.scale, NULL); // scaleX = scaleY assumed

    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    renderContext.logicalW = w / renderContext.scale;
    renderContext.logicalH = h / renderContext.scale;
}

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



void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList)
{
    if (blockList.empty())
    {
        
        int h;
        h = renderContext.logicalH;
        
        for (int i = 0; i < h/blockSize; i ++)
        {
            SDL_FRect block = { i*blockSize, i * blockSize, blockSize, blockSize};
            blockList.push_back(block);

        }
    }
    for (int i = 0; i < blockList.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 25, 69, 50, 255); // static box
        SDL_RenderFillRect(renderer, &blockList[i]);
    }
}