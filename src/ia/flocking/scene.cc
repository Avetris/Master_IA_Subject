#include <ia\flocking\scene.h>
#include <ia\flocking\world.h>

#include <cstdio>
#include <engine\ui_manager.h>

void SceneFlocking::init() {
  printf("Flocking Scene Is Being Initialized\n");
  const uint32_t y = WINDOW_HEIGHT - 30;
  _addAgent = UIManager::instance().addText("1 Add Agent", 10, y);
  _removeAgent = UIManager::instance().addText("2 Remove Agent", 200, y);
  char text[255];
  uint8_t num = _world.getNumAgent();
  sprintf_s(text, "Nº Agents: %i", static_cast<uint8_t>(num));
  _numAgents = UIManager::instance().addText(text, 500, y);

  if (num <= 1) {
      UIManager::instance().setColor(_removeAgent, SDL_Color SHADOW_COLOR);
  }
  else {
      UIManager::instance().setColor(_removeAgent, SDL_Color FOREGROUND_COLOR);
  }
}

void SceneFlocking::shutdown() {
  printf("Flocking Scene Has Been Shutdown\n");
}

void SceneFlocking::update(const uint32_t dt) {
    _world.update(dt);
}

void SceneFlocking::handleMouseEvent(SDL_Event e, int x, int y) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        SDL_GetMouseState(&x, &y);
        _world.setTarget({ (float)x, (float)y });
    }
}

void SceneFlocking::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
    case SDLK_1:
        _world.addAgent();
      break;
    case SDLK_2:
        _world.removeAgent();
        break;
    default:{}
  }
  char text[255];
  uint8_t num = _world.getNumAgent();
  sprintf_s(text, "Nº Agents: %i", static_cast<uint8_t>(num));
  UIManager::instance().setText(text, _numAgents);
  if (num <= 1) {
      UIManager::instance().setColor(_removeAgent, SDL_Color SHADOW_COLOR);
  }
  else {
      UIManager::instance().setColor(_removeAgent, SDL_Color FOREGROUND_COLOR);
  }
}

void SceneFlocking::render() {
    _world.render();
}