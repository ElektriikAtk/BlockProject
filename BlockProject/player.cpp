#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "functions.h"
#include "player.h"



Player::Player(float x, float y, float s, SDL_Texture* t)
    : x{ x }, y{ y }, speed{ s }, texture{ t }, box{ box },
      velX { velX }, velY{ velY }
{
    SDL_GetTextureSize(t, &w, &h);
}

void Player::render(SDL_Renderer* renderer)
{
    box = { x, y, w, h };
    SDL_RenderTexture(renderer, texture, NULL, &box);
}

float Player::get_speed() { return speed; }
float Player::get_x() { return x; }
float Player::get_y() { return y; }
float Player::get_w() { return w; }
float Player::get_h() { return h; }
float Player::get_dx() { return dx; }
float Player::get_velY() { return velY; };
float Player::get_velX() { return velX; };
void Player::set_x(float x) { this->x = x; }
void Player::set_y(float y) { this->y = y; }
void Player::set_dx(float dx) { this->dx = dx; }
void Player::set_velY(float velY) { this->velY = velY; };
void Player::set_velX(float velX) { this->velX = velX; };
void Player::set_texture(SDL_Texture* texture) { this->texture = texture; }


bool Player::collision(const SDL_FRect& block) 
{
    // Added for clarity, maybe best to remove to save resources and comment instead
	// Remember y-scale increases downwards
    const float& playerTop = this->y;
    const float& playerBottom = this->y + this->h;
    const float& playerLeft = this->x;
    const float& playerRight = this->x + this->w;

    const float& blockTop = block.y;
    const float& blockBottom = block.y + block.h;
    const float& blockLeft = block.x;
    const float& blockRight = block.x + block.w;

    const bool& playerUnderBlock = playerTop > blockBottom;
    const bool& playerAboveBlock = playerBottom < blockTop;
    const bool& playerAfterBlock = int(playerLeft) > blockRight;
    const bool& playerBeforeBlock = int(playerRight) < blockLeft;

    return !(playerUnderBlock || playerAboveBlock || playerAfterBlock || playerBeforeBlock);
}

void Player::collisionHandler(const SDL_FRect& block)
{
    std::cout << "VelX: " << this->velX << " | x: " << this->x << " | ";
    if (!collision(block))
    {
        std::cout << "N" << std::endl;
        return;
    }
    else
    {
        std::cout << "Y" << std::endl;
        //collision(block);
        if (this->velY < 0 && !((int(this->x) == block.x + block.w) || (int(this->x) == block.x - this->w)))
        {
            this->velY = 0;
            //this->velY *= -1; // Bounces downwards
            this->y = block.y + block.h;

        }
        else if (this->velY > 0 && !((int(this->x) == block.x + block.w) || (int(this->x) == block.x - this->w)))
        {
            this->velY = 0;
            this->y = block.y - this->h; // Satys on top of block

        }
        else if (this->velX > 0 && !(int(this->x) == block.x + block.w))
        {
            this->x = block.x - this->w; // Stops at left of block
            this->velX = 0;
        }
        else if (this->velX < 0 && !(int(this->x) == block.x - this->w))
        {
            this->x = block.x + block.w; // Stops at right of block
            this->velX = 0;
        }
    }
}