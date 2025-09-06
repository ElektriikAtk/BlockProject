#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "functions.h"
#include "player.h"



Player::Player(float s, float x, float y, SDL_Texture* t)
    : speed{ s }, x{ x }, y{ y }, texture{ t }, box{ box },
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
float Player::get_velY() { return velY; };
void Player::set_x(float x) { this->x = x; }
void Player::set_y(float y) { this->y = y; }
void Player::set_velY(float velY) { this->velY = velY; };
void Player::set_velX(float velX) { this->velX = velX; };
void Player::set_texture(SDL_Texture* texture) { this->texture = texture; }
void Player::update(float dt)
{
    x += velX * dt;
    y += velY * dt;
}

bool Player::colissionCheck(std::vector<SDL_FRect> blockList)
{
    float playerLeft = this->x;
    float playerRight = this->x + this-> w;
    float playerTop = this->y;
    float playerBottom = this->y + this->h;
    std::cout << "PlayerLeft: " << playerLeft << " PlayerTop: " << playerTop << "\n";
    for (int i = 0; i < blockList.size(); i++)
    {
        float blockLeft = blockList[i].x;
        float blockRight = blockList[i].x + blockList[i].w;
        float blockTop = blockList[i].y;
        float blockBottom = blockList[i].y + blockList[i].h;
        bool above = false;
        bool verticalAlligned = false;
        //std::cout << "i: " << i;



        if (blockBottom <= playerTop)
        {
            //std::cout << " Above";
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //Red
            SDL_RenderFillRect(renderer, &blockList[i]);
            above = true;
        }
        else
        {
           // std::cout << "\n";
        }

        if ((blockLeft < playerRight && blockRight > playerLeft))
        {
            //std::cout << " and verticalAlligned" << std::endl;
            verticalAlligned = true;
        }
        else
        {
            std::cout << "\n";
        }

        if (verticalAlligned && !above)
        {
            int w, h;
            float scale;
            getScaledSize(w, h, scale);
            if (blockBottom + this->h <= h)
            {
                this->y = blockBottom;
            }
        }
    }   
    //std::cout << "Block 3 bottom: " << blockList[3].y + blockList[3].h << std::endl; // DO not use program crashes for some reason, should be checked
    system("cls");
    return true;
}