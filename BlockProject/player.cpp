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

void Player::movement()
{
    float dt = getDeltaTime();
    const bool* keys = SDL_GetKeyboardState(NULL);
    dx = 0.0f;
    if (keys[SDL_SCANCODE_A])  dx = dx - 1.0f;
    if (keys[SDL_SCANCODE_D])  dx = dx + 1.0f;

    if (dx != 0.0f) {
        //Ignore for now
        //float len = SDL_sqrtf(dx * dx );
        //dx /= len;
    }





    int w = 0, h = 0;
    w = renderContext.logicalW;
    h = renderContext.logicalH;

    bool onGround = false;


    if (y + this->h >= h || velY == 0)
    {
            onGround = true;
        if (y + this->h >= h)
        {
            velY = 0;
        }
        else
        {
            velY = velY + gravity * dt;
        }
    }
    else
    {
        velY = velY + gravity * dt;
    }


    if (keys[SDL_SCANCODE_SPACE] && onGround) {
        velY = -jumpStrength; // jump impulse
    }

    x = x + velX * dt;
    velX = dx * speed;
    //if (velY != 0 || y + this->h >= h) {
    //    velY = velY + gravity * dt;
    //}
    y = y + velY * dt;

    x = clamp(x, 0.0f, w - this->w);
    y = clamp(y, 0.0f, h - this->h);
}

bool Player::collision(const SDL_FRect& block) 
{
    // Added for clarity, maybe best to remove to save resources and comment instead
	// Remember y-scale increases downwards
    const float& playerTop = y;
    const float& playerBottom = y + h;
    const float& playerLeft = x;
    const float& playerRight = x + w;

    const float& blockTop = block.y;
    const float& blockBottom = block.y + block.h;
    const float& blockLeft = block.x;
    const float& blockRight = block.x + block.w;

    const bool& playerUnderBlock = int(playerTop) > blockBottom; // Maybe int conversion not needed  here? Haven't tested
    const bool& playerAboveBlock = int(playerBottom) < blockTop; // Haven't tested here either
    const bool& playerAfterBlock = int(playerLeft) > blockRight;
    const bool& playerBeforeBlock = int(playerRight) < blockLeft;

    return !(playerUnderBlock || playerAboveBlock || playerAfterBlock || playerBeforeBlock);
}

void Player::handleCollision(const std::vector<SDL_FRect>& blockList)
{
    std::cout << "x: " << int(x) << " y: " << int(y) << " velY: " << std::fixed << std::setprecision(1) << velY << " velX: " << std::fixed << std::setprecision(1) << velX;
    for (int i = 0; i < blockList.size(); i++)
    {
        if (collision(blockList[i]))
        {
            if (velY < 0 && !((int(x) == blockList[i].x + blockList[i].w) || (int(x) == blockList[i].x - w)))
            {
                std::cout << " Bottom";
                velY = 0;
                y = blockList[i].y + blockList[i].h; // Stops at bottom of blockList[i]

            }
            else if (velY > 0 && !((int(x) == blockList[i].x + blockList[i].w) || (int(x) == blockList[i].x - w) || (int(y) == blockList[i].y + blockList[i].h)))
            {
                std::cout << " Top";
                velY = 0;
                y = blockList[i].y - h; // Stays on top of blockList[i]

            }
            else if (velX > 0 && !(int(x) == blockList[i].x + blockList[i].w))
            {
                std::cout << " Left";
                x = blockList[i].x - w; // Stops at left of blockList[i]
                velX = 0;
            }
            else if (velX < 0 /*&& !(x == blockList[i].x - w)*/)
            {
                std::cout << " Right";
                x = blockList[i].x + blockList[i].w; // Stops at right of blockList[i]
                //velX = 0;
            }
        }
    }
    std::cout << std::endl;
}