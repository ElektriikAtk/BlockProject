#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>




class Player
{
public:
    Player(float x, float y, float s, SDL_Texture* t);
    void render(SDL_Renderer* renderer);
    float get_speed();
    float get_x();
    float get_y();
    float get_w();
    float get_h();
    float get_dx();
    float get_velY();
    float get_velX();
    void set_x(float x);
    void set_y(float y);
    void set_dx(float dx);
    void set_velY(float velY);
    void set_velX(float velX);
    void set_texture(SDL_Texture* texture);
    void movement();
    bool collision(const SDL_FRect& block);
    void handleCollision(const std::vector<SDL_FRect>& blockList);
private:
    float speed;
    float x, y;
    float w, h;
    float dx; // Maybe dy needed also
    SDL_Texture* texture;
    SDL_FRect box;
    float velX, velY;
};