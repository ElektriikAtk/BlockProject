#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "functions.h"
#include "player.h"

Player::Player(float s, float x, float y, SDL_Texture* t)
    : speed{ s }, x{ x }, y{ y }, texture{ t }
{
    SDL_GetTextureSize(t, &w, &h);
}

void Player::render(SDL_Renderer* renderer)
{
    box = { x, y, w, h };
    SDL_RenderTexture(renderer, texture, NULL, &box);
}


//SDL_RenderTexture(renderer, texture, NULL, &box);
//SDL_RenderPresent(renderer);

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