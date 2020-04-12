//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "engine/scene_manager.h"
#include "engine/math.h"
#include "ia/steering/scene.h"
#include "ia/pathfinding/scene.h"
#include "ia/final/scene.h"

void SceneManager::createScenes() {
  _scenes[0] = new SceneSteering();
  _scenes[1] = new ScenePath();
  _scenes[2] = new SceneFinal();
  //_scenes[3] = new SceneSteering();
  nextScene(2);
}

void SceneManager::shutdownScenes() {
  _scenes[_curr_scene]->shutdown();
  for (uint8_t i = 0; i < SCENE_NUMBER; ++i) {
    delete _scenes[i];
  }
}

void SceneManager::nextScene(const int8_t sign) {
  _scenes[_curr_scene]->shutdown();
  _curr_scene += sign;
  _curr_scene = clamp<int8_t>(_curr_scene, 0, SCENE_NUMBER - 1);
  _scenes[_curr_scene]->init();
}

void SceneManager::update(uint32_t dt) {
  _scenes[_curr_scene]->update(dt);
}

void SceneManager::handleMouseEvent(SDL_Event e, int x, int y) {
  _scenes[_curr_scene]->handleMouseEvent(e, x, y);
}

void SceneManager::handleKeyEvent(SDL_Keycode key) {
  _scenes[_curr_scene]->handleKeyEvent(key);
}

void SceneManager::render() {
  _scenes[_curr_scene]->render();
}
