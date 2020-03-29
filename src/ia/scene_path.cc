#include "ia/scene_path.h"
#include <ia\world_path.h>

#include <cstdio>
#include <engine\ui_manager.h>

void ScenePath::init() {
  printf("Path Scene Is Being Initialized\n");
  _background.loadFromFile(MAP_PATH);
  const uint32_t y = WINDOW_HEIGHT - 30;
  UIManager::instance().addText("0123 Doors", 10, y);
  _typeSpriteMap.insert({ PathFinding::Type::Dijstra, UIManager::instance().addText("4 Disjtra", 200, y) });
  _typeSpriteMap.insert({ PathFinding::Type::A_Manhattan, UIManager::instance().addText("5 A* Manhattan", 350, y) });
  _typeSpriteMap.insert({ PathFinding::Type::A_Diagonal, UIManager::instance().addText("6 A* Diagonal", 600, y) });
  _typeSpriteMap.insert({ PathFinding::Type::A_Euclidean, UIManager::instance().addText("7 A* Euclidean", 824, y) });
   UIManager::instance().setColor(_typeSpriteMap[PathFinding::Type::Dijstra], SDL_Color ACTIVE_COLOR);
}

void ScenePath::shutdown() {
  printf("Path Scene Has Been Shutdown\n");
}

void ScenePath::update(const uint32_t dt) {
    world_.update(dt);
}

void ScenePath::handleMouseEvent(const SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (_fristClick.x == -1 || _fristClick.y == -1) {
            _fristClick = { x,y };
        }
        else {
            PathManager::instance().addPath(_fristClick, { x,y }, world_.ia(), true, _type);
            _fristClick = { -1, -1 };
        }
    }
}

void ScenePath::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
    case SDLK_0:
        world_.openDoor(0);
      break;
    case SDLK_1:
        world_.openDoor(1);
        break;
    case SDLK_2:
        world_.openDoor(2);
        break;
    case SDLK_3:
        world_.openDoor(3);
        break;
    case SDLK_4:
        changeType(PathFinding::Type::Dijstra);
        break;
    case SDLK_5:
        changeType(PathFinding::Type::A_Manhattan);
    break;
    case SDLK_6:
        changeType(PathFinding::Type::A_Diagonal);
    break;
    case SDLK_7:
        changeType(PathFinding::Type::A_Euclidean);
      break;
    default:{}
  }
}

void ScenePath::render() {
    _background.render();
    world_.render();
}

void ScenePath::changeType(PathFinding::Type type)
{
    UIManager::instance().setColor(_typeSpriteMap[_type], SDL_Color FOREGROUND_COLOR);
    _type = type;
    UIManager::instance().setColor(_typeSpriteMap[_type], SDL_Color ACTIVE_COLOR);
}
