//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/agent.h"
#include "ia/world.h"
#include <engine\ui_manager.h>
#include <ia\steering\mind.h>
#include <ia\pathfinding\path_manager.h>
#include <ia\pathfinding\body.h>
#include <ia\pathfinding\mind.h>
#include <ia\final\mind_slave.h>
#include <ia\final\mind_guard.h>
#include <ia\final\mind_soldier.h>
#include <ia\final\body.h>
#include <ia\flocking\body.h>
#include <ia\flocking\mind.h>

uint32_t Agent::_GUID = 1;

Agent::Agent(World* world, const Color color, const Type type, BodyType bodyType, MindType mindType, MathLib::Vec2 position) {
    _UID = _GUID++;
   _world = world;
  switch (bodyType) {
  case BodyType::Steering: _body = std::make_unique<BodySteering>(color, type); break;
    case BodyType::Pathfinding: _body = std::make_unique<BodyPathFinding>(color); break;
    case BodyType::Final: _body = std::make_unique<BodyFinal>(world, color); break;
    case BodyType::Flocking: _body = std::make_unique<BodyFlocking>(this, color); break;
  }

  switch (mindType) {
    case MindType::Steering: _mind = std::make_unique<MindSteering>(this, _body.get()); break;
    case MindType::Pathfinding: _mind = std::make_unique<MindPathFinding>(this, _body.get()); break;
    case MindType::Slave: _mind = std::make_unique<MindSlave>(this, _body.get()); break;
    case MindType::Guard: _mind = std::make_unique<MindGuard>(this, _body.get()); break;
    case MindType::Soldier: _mind = std::make_unique<MindSoldier>(this, _body.get()); break;
    case MindType::Flocking: _mind = std::make_unique<MindFlocking>(this, _body.get()); break;
  }
  if (position.x() != -1 && position.y() != -1) {
      _body->getKinematic()->position = position;
  }
}

Agent::~Agent() {
  _world = nullptr;
}

void Agent::update(const uint32_t dt) {
  _mind->update(dt);
  _body->update(dt);
}

void Agent::render() const {
  _body->render();
}
