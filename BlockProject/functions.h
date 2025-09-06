#pragma once
#include "player.h"

// tell the compiler these exist in another file
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// declare functions
float clamp(float value, float min, float max);
SDL_Texture* loadSprites(SDL_Surface* spriteSheetSurface);
float getDeltaTime();
void movement(Player& object);
void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList);