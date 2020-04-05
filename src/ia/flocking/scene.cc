#include "ia/scene_opt.h"
#include <ia\world_path.h>

#include <cstdio>
#include <engine\ui_manager.h>

void SceneOPT::init() {
  printf("Path Scene Is Being Initialized\n");
  const uint32_t y = WINDOW_HEIGHT - 30;
  _addAgent = UIManager::instance().addText("1 Add Agent", 10, y);
  _removeAgent = UIManager::instance().addText("2 Remove Agent", 200, y);
  std::string text = "Nº Agents: ";
  text += world_.getNumAgent();
  _numAgents = UIManager::instance().addText(const_cast<char*>(text.c_str()), 500, y);
}

void SceneOPT::shutdown() {
  printf("OPT Scene Has Been Shutdown\n");
}

void SceneOPT::update(const uint32_t dt) {
    world_.update(dt);
}

void SceneOPT::handleMouseEvent(const SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        world_.flocking()->setTarget({ (float)x, (float)y });
    }
}

void SceneOPT::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
    case SDLK_1:
        world_.addAgent();
      break;
    case SDLK_2:
        world_.removeAgent();
        break;
    default:{}
  }
  std::string text = "Nº Agents: ";
  int num = world_.getNumAgent();
  text += num;
  UIManager::instance().setText(const_cast<char*>(text.c_str()), _numAgents);
  if (num <= 1) {
      UIManager::instance().setColor(_addAgent, SDL_Color SHADOW_COLOR);
  }
  else {
      UIManager::instance().setColor(_addAgent, SDL_Color FOREGROUND_COLOR);
  }
}

void SceneOPT::render() {
    world_.render();
}