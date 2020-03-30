//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "engine/sprite.h"

#include <SDL/SDL_ttf.h>

class Game {
  public:
    Game() {};
    ~Game() {};

    void init();
    void start();
    void shutdown();
  private:
    void handleInput();
    void update(uint32_t dt);
    void render();

    bool quit_ = false;
    Sprite fps_sprite_;
    TTF_Font* font_ = nullptr;

    int8_t slo_mo_ = 1;
};

#endif