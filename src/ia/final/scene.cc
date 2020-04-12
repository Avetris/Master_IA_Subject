#include "ia\final\scene.h"
#include <ia\final\world.h>

#include <cstdio>
#include <engine\ui_manager.h>
#include <string>

void SceneFinal::init() {
  printf("Final Scene Is Being Initialized\n");
  if (!_loaded) {
      _background.loadFromFile(MAP_PATH);
  }
  initText();
  _loaded = true;
}

void SceneFinal::initText() {
    uint32_t y = WINDOW_HEIGHT - 20;
    std::string str = "";
    int i = 0;
    while (i < _world.getNumDoors()) {
        str += std::to_string(i++);
    }
    str += " Doors";
    _sprites.push_back(UIManager::instance().addText(const_cast<char*>(str.c_str()), 10, y));

    while (i < _world.getNumDoors()) {
        str += std::to_string(i++);
    }
    str = "";
    for (int j = 0; j < 2; j++) {
        str += std::to_string(i++);
    }
    str += " +/- Slaves";
    char* cstr = new char[255];
    strcpy(cstr, str.c_str());
    _sprites.push_back(UIManager::instance().addText(const_cast<char*>(str.c_str()), 200, y));
    str = "";
    for (int j = 0; j < 2; j++) {
        str += std::to_string(i++);
    }
    str += " +/- Guards";
    strcpy(cstr, str.c_str());
    _sprites.push_back(UIManager::instance().addText(const_cast<char*>(str.c_str()), 400, y));
    str = "";
    for (int j = 0; j < 2; j++) {
        str += std::to_string(i++);
    }
    str += " +/- Soldiers";
    strcpy(cstr, str.c_str());
    _sprites.push_back(UIManager::instance().addText(const_cast<char*>(str.c_str()), 600, y));

    y -= 20;
    strcpy(cstr, "Slaves 0");
    _nSlaves = UIManager::instance().addText(const_cast<char*>(str.c_str()), 10, y);

    y -= 20;
    strcpy(cstr, "Guards 0");
    _nGuards = UIManager::instance().addText(const_cast<char*>(str.c_str()), 10, y);

    y -= 20;
    strcpy(cstr, "Soldiers 0");
    _nSoldier = UIManager::instance().addText(const_cast<char*>(str.c_str()), 10, y);


    UIManager::instance().setColor(_nSlaves, ACTIVE_COLOR);
    UIManager::instance().setColor(_nGuards, ACTIVE_COLOR);
    UIManager::instance().setColor(_nSoldier, ACTIVE_COLOR);

}

void SceneFinal::shutdown() {
  printf("Final Scene Has Been Shutdown\n");
  PathManager::instance().reset();
  for (uint32_t uid : _sprites) {
      UIManager::instance().removeText(uid);
  }
  UIManager::instance().removeText(_nSoldier);
  UIManager::instance().removeText(_nSlaves);
  UIManager::instance().removeText(_nGuards);
  _nSoldier = 0;
  _nSlaves = 0;
  _nGuards = 0;
  _world.reset();
}

void SceneFinal::update(const uint32_t dt) {

    char text[255];
    uint16_t n_slaves = _world.getCount(AgentType::Slave);
    uint16_t n_guards = _world.getCount(AgentType::Guard);
    uint16_t n_soldiers = _world.getCount(AgentType::Soldier);
    sprintf_s(text, "N Slaves %d", static_cast<uint16_t>(n_slaves));
    if(_nSlaves > 0)
        UIManager::instance().setText(text, _nSlaves);
    sprintf_s(text, "N Guards %d", static_cast<uint16_t>(n_guards));
    if (_nGuards > 0)
        UIManager::instance().setText(text, _nGuards);
    sprintf_s(text, "N Soldiers %d", static_cast<uint16_t>(n_soldiers));
    if (_nSoldier > 0)
        UIManager::instance().setText(text, _nSoldier);

    _world.update(dt);
}

void SceneFinal::handleMouseEvent(SDL_Event e, int x, int y) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        if (_fristClick.x == -1 || _fristClick.y == -1) {
            _fristClick = { x / 8, y / 8 };
        }
        else {
            PathManager::instance().addPath(_fristClick, { x / 8, y / 8 }, nullptr, AgentType::Slave, true, PathType::A_Pre_Manhattan);
            _fristClick = { -1, -1 };
        }

    }
}

void SceneFinal::handleKeyEvent(const SDL_Keycode key) {
    uint8_t lastKey = SDLK_0 + _world.getNumDoors();
    if (key >= SDLK_0 && key <= SDLK_9) {
        if (key < lastKey) {
            _world.openDoor(key - SDLK_0);
        }
        else if (key == lastKey) {
            _world.addAgent(AgentType::Slave);
        }
        else if (key == lastKey + 1) {
            _world.removeAgent(AgentType::Slave);
        }
        else if (key == lastKey + 2) {
            _world.addAgent(AgentType::Guard);
        }
        else if (key == lastKey + 3) {
            _world.removeAgent(AgentType::Guard);
        }
        else if (key == lastKey + 4) {
            _world.addAgent(AgentType::Soldier);
        }
        else if (key == lastKey + 5) {
            _world.removeAgent(AgentType::Soldier);
        }
    }
    else {
        if (key == SDLK_d) {
            _world.debug();
        }
    }
}

void SceneFinal::render() {
    _background.render();
    UIManager::instance().renderPath();
    _world.render();
}