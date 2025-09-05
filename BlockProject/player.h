#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


class Player
{
public:
    Player(float s, float x, float y, SDL_Texture* t);
    void render(SDL_Renderer* renderer);
    float get_speed();
    float get_x();
    float get_y();
    float get_w();
    float get_h();
    float get_velY();
    void set_x(float x);
    void set_y(float y);
    void set_velY(float velY);
    void set_velX(float velX);
    void set_texture(SDL_Texture* texture);
    void update(float dt);
private:
    float speed;
    float x, y;
    float w, h;
    SDL_Texture* texture;
    SDL_FRect box;
    float velX, velY;
};