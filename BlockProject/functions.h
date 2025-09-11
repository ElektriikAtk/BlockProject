#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "player.h"
#include "renderContext.h"

// Actual dimensions for an unscaled window. If scaled, refer to struct renderContext
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 950
#define SCALING_FACTOR 1

// Used in movement()
constexpr float playerSpeed = 1000.0f*SCALING_FACTOR;
constexpr float gravity = playerSpeed/SCALING_FACTOR;
constexpr float jumpStrength = gravity * 1;
constexpr float blockSize = 250; // Add functionality so that it is modified depending on player size or scale

// tell the compiler these exist in another file
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern RenderContext renderContext;


// declare functions
void setupRenderer(SDL_Renderer* renderer); // To match potential upscaled coordinates with logical coordinates
float clamp(float value, float min, float max);
SDL_Texture* loadSprites(SDL_Surface* spriteSheetSurface);
float getDeltaTime();
void movement(Player& object, const SDL_FRect& block);
void createBlocks(SDL_Renderer* renderer, std::vector<SDL_FRect>& blockList);