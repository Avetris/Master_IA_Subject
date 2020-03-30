//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__ 1

#include "engine/defines.h"
#include "engine/scene.h"

#include <cstdint>
#include <SDL/SDL.h>

class SceneManager {
public:
  ~SceneManager() {}
  SceneManager(SceneManager const&) = delete;
  void operator=(SceneManager const&) = delete;

  static SceneManager& instance() {
    static SceneManager instance;
    return  instance;
  }

  void createScenes();
  void shutdownScenes();
  void nextScene(const int8_t sign);

  void update(uint32_t dt);
  void handleMouseEvent(SDL_Event e, int x, int y);
  void handleKeyEvent(SDL_Keycode key);
  void render();

private:
  SceneManager() {}

  int8_t _curr_scene = 0;
  Scene* _scenes[SCENE_NUMBER];
};
#endif