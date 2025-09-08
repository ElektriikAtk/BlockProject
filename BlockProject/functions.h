#pragma once
#include "player.h"
#include "renderContext.h"

// Actual dimensions for an unscaled window. If scaled, refer to struct renderContext
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800
#define SCALING_FACTOR 2

// tell the compiler these exist in another file
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern RenderContext renderContext;

// declare functions
void setupRenderer(SDL_Renderer* renderer); // To match potential upscaled coordinates with logical coordinates
float clamp(float value, float min, float max);
SDL_Texture* loadSprites(SDL_Surface* spriteSheetSurface);
float getDeltaTime();
void getScaledSize(int& w, int& h, float& scale);
void movement(Player& object);
void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList);