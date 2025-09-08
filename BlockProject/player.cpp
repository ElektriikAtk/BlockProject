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

bool Player::colissionCheck(std::vector<SDL_FRect> blockList) // Should be separated so that block are generated in another function
{
    float playerLeft = this->x;
    float playerRight = this->x + this-> w;
    float playerTop = this->y;
    float playerBottom = this->y + this->h;
    int w, h;
    w = renderContext.logicalW;
    h = renderContext.logicalH;
    std::vector<float> currentlyAlignedBottom; //Bottom part of the currently vertically aligned boxes

    std::cout << "PlayerVelY: " << get_velY() << " | PlayerLeft: " << playerLeft << " PlayerTop: " << playerTop << " W, H: " << w << " " << h << "\n";
    for (int i = 0; i < blockList.size(); i++)
    {
        float blockLeft = blockList[i].x;
        float blockRight = blockList[i].x + blockList[i].w;
        float blockTop = blockList[i].y;
        float blockBottom = blockList[i].y + blockList[i].h;
        bool above = true; // Represents if the block is above the player
        bool verticalAlligned = false;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


        std::cout << "i: " << i;
        
        if ((blockLeft < playerRight && blockRight > playerLeft))
        {
            std::cout << " verticalAligned" << std::endl;
            verticalAlligned = true;
            if (currentlyAlignedBottom.size() < 2)
            {
                currentlyAlignedBottom.push_back(blockBottom);

            }
        }
        else
        {
            std::cout << " notAlligned" << std::endl;
            if (!currentlyAlignedBottom.empty())
            {
                //currentlyAlignedBottom.pop_back();
            }
        }


        int max_value = 0;
        if (currentlyAlignedBottom.size() == 2)
        {
            if (currentlyAlignedBottom[0] > currentlyAlignedBottom[1])
            {
                max_value = currentlyAlignedBottom[0];
            }
            else
            {
                max_value = currentlyAlignedBottom[1];
            }
        }

        if (max_value == int(playerTop))
        {
            std::cout << "True" << std::endl;
            if (this->velY < 0 && verticalAlligned)
            {
                h = renderContext.logicalH;
                //if (blockBottom + this->h <= h)
                //{
                this->y = max_value;
                }

                this->velY = 0;
                //std::cout << "Reset" << std::endl;
            //}
            }
        else
        {
            std::cout << "" << std::endl;
        }
    }   
    for (int i = 0; i < currentlyAlignedBottom.size(); i++)
    {
        std::cout << "currentlyAlignedBottom" << i << ": " << currentlyAlignedBottom[i] << std::endl;
    }
    //std::cout << "Block 3 bottom: " << blockList[3].y + blockList[3].h << std::endl; // DO not use program crashes for some reason, should be checked
    system("cls");
    return true;
}