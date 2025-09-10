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

void movement(Player& object, const SDL_FRect& block) 
{
    float dt = getDeltaTime();
    const bool* keys = SDL_GetKeyboardState(NULL);
    object.set_dx(0.0f);
    if (keys[SDL_SCANCODE_A])  object.set_dx(object.get_dx() - 1.0f);
    if (keys[SDL_SCANCODE_D])  object.set_dx(object.get_dx() + 1.0f);
    
    if (object.get_dx() != 0.0f) {
        //Ignore for now
        //float len = SDL_sqrtf(dx * dx );
        //dx /= len;
    }

    
    

    //const float gravity = 30.0f;

    

    object.set_x(object.get_x() + object.get_velX() * dt);
    object.set_velX(object.get_dx() * object.get_speed());
    object.set_velY(object.get_velY() + gravity * dt);   
    object.set_y(object.get_y() + object.get_velY() * dt);



    int w = 0, h = 0;
    w = renderContext.logicalW;
    h = renderContext.logicalH;

    bool onGround = false;

    if (object.get_y() + object.get_h() >= h)
    {
        onGround = true;
        object.set_velY(0);
    }

    if (keys[SDL_SCANCODE_SPACE] && onGround) {
        object.set_velY(-jumpStrength); // jump impulse
    }

    object.set_x(clamp(object.get_x(), 0.0f, w - object.get_w()));
    object.set_y(clamp(object.get_y(), 0.0f, h - object.get_h()));

    object.collisionHandler(block);
}

void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList)
{
    if (blockList.empty())
    {
        
        int w, h;
        w = renderContext.logicalW;
        h = renderContext.logicalH;
        
        //float blockSize = 50;
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
        //SDL_RenderClear(renderer);
}