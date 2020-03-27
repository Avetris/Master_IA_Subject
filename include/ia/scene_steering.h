//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SCENE_SEEK_H__
#define __SCENE_SEEK_H__ 1

#include "engine/scene.h"
#include "ia/agent.h""

class SceneSteering: public Scene {
  public:
    SceneSteering() {};
    ~SceneSteering() {};

    void init() final;
    void shutdown() final;

    void update(uint32_t dt) final;
    void handleMouseEvent(SDL_Event e) final;
    void handleKeyEvent(SDL_Keycode key) final;
    void render() final;

  private:
      World world_;

      int8_t slo_mo_ = 1;
};

#endif