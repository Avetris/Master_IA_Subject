#include "ia\pathfinding\scene.h"
#include <ia\pathfinding\world.h>

#include <cstdio>
#include <engine\ui_manager.h>
#include <string>

void ScenePath::init() {
  printf("Path Scene Is Being Initialized\n");
  _background.loadFromFile(MAP_PATH);
  const uint32_t y = WINDOW_HEIGHT - 20;
  std::string str = "";
  int i = 0;
  while (i < _world.getNumDoors()) {
      str += std::to_string(i++);
  }
  str += " Doors";
  UIManager::instance().addText(const_cast<char*>(str.c_str()), 10, y);
  str = std::to_string(i++) + " Dijkstra";
  char* cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  _typeSpriteMap.insert({ PathType::Dijkstra, UIManager::instance().addText(cstr, 160, y) });
  str = std::to_string(i++) + " A* Manhattan";
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  _typeSpriteMap.insert({ PathType::A_Manhattan, UIManager::instance().addText(cstr, 300, y) });
  str = std::to_string(i++) + " A* Diagonal";
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  _typeSpriteMap.insert({ PathType::A_Diagonal, UIManager::instance().addText(cstr, 500, y) });
  str = std::to_string(i++) + " A* Euclidean";
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  _typeSpriteMap.insert({ PathType::A_Euclidean, UIManager::instance().addText(cstr, 675, y) });
  str = std::to_string(i++) + " A* Pre Man";
  cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  _typeSpriteMap.insert({ PathType::A_Pre_Manhattan, UIManager::instance().addText(cstr, 875, y) });
   UIManager::instance().setColor(_typeSpriteMap[PathType::Dijkstra], SDL_Color ACTIVE_COLOR);
}

void ScenePath::shutdown() {
  printf("Path Scene Has Been Shutdown\n");
}

void ScenePath::update(const uint32_t dt) {
    _world.update(dt);
}

void ScenePath::handleMouseEvent(SDL_Event e, int x, int y) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        if (_fristClick.x == -1 || _fristClick.y == -1) {
            _fristClick = { x,y };
        }
        else {
            PathManager::instance().addPath(_fristClick, { x,y }, _world.getIA()->getMind(), AgentType::Slave, true, _type);
            _fristClick = { -1, -1 };
        }
    }
}

void ScenePath::handleKeyEvent(const SDL_Keycode key) {
    uint8_t lastKey = SDLK_0 + _world.getNumDoors();
    if (key >= SDLK_0 && key <= SDLK_9) {
        if (key < lastKey) {
            _world.openDoor(key - SDLK_0);
        }
        else if( key == lastKey){
            changeType(PathType::Dijkstra);
        }
        else if (key == lastKey + 1) {
            changeType(PathType::A_Manhattan);
        }
        else if (key == lastKey + 2) {
            changeType(PathType::A_Diagonal);
        }
        else if (key == lastKey + 3) {
            changeType(PathType::A_Euclidean);
        }
        else if (key == lastKey + 4) {
            changeType(PathType::A_Pre_Manhattan);
        }
    }
    else {
        if (key == SDLK_d) {
            _world.debug();
        }
    }
}

void ScenePath::render() {
    _background.render();
    UIManager::instance().renderPath();
    _world.render();
}

void ScenePath::changeType(PathType type)
{
    UIManager::instance().setColor(_typeSpriteMap[_type], SDL_Color FOREGROUND_COLOR);
    _type = type;
    UIManager::instance().setColor(_typeSpriteMap[_type], SDL_Color ACTIVE_COLOR);
}
