//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "engine/scene.h"
#include "ia/defines.h"

#include <SDL/SDL.h>

class Game {
  public:
    Game() {};
    ~Game();

    virtual void init();
    void start();
    void shutdown();
protected:
    virtual void handleInput();
    void update(uint32_t dt);
    virtual void render();

    virtual void createScenes();
    void nextScene(int8_t sign);

    bool quit_ = false;
    uint8_t fps_sprite_ = 0;

    int8_t curr_scene_ = 0;
    Scene* scenes_[SCENE_NUMBER];

};

#endif