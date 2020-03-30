//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/agent.h"
#include "ia/world.h"
#include <engine\ui_manager.h>
#include <ia\path_finding\path_manager.h>

uint16_t Agent::_GUID = 0;

void Agent::init(World* world, const Body::Color color, const Body::Type type, MathLib::Vec2 position) {
  world_ = world;
  body_.init(_UID, color, type, position);
  mind_.init(_UID, world, &body_);
}

void Agent::shutdown() {
  world_ = nullptr;
}

void Agent::setPath(Path pathFound)
{
    mind_.setPath(pathFound);
}

void Agent::checkDoor(Door* door)
{
    std::pair<bool, Path*> result = mind_.checkDoor(door);
    if (result.first) {
        t_coord startPos = { body_.getPosition().x(), body_.getPosition().y() };
        MathLib::Vec2 endVec = result.second->path.back();
        t_coord endPos = { (int)endVec.x(), (int)endVec.y() };
        PathManager::instance().addPath(startPos, endPos, this, result.second->draw);
    }
}

void Agent::update(const uint32_t dt) {
  mind_.update(dt);
  body_.update(dt);
}

void Agent::render() const {
  mind_.render();
  body_.render();
}

