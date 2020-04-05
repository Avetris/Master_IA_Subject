//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SCENE_SEEK_H__
#define __SCENE_SEEK_H__ 1

#include "engine/scene.h"
#include "ia/steering/world.h"

class SceneSteering: public Scene {
  public:
    SceneSteering() {};
    ~SceneSteering() {};

    void init() final;
    void shutdown() final;

    void update(uint32_t dt) final;
    void handleMouseEvent(SDL_Event e, int x, int y) final;
    void handleKeyEvent(SDL_Keycode key) final;
    void render() final;
  private:
    WorldSteering _world;
};

#endif