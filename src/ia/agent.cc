//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/agent.h"
#include "ia/world.h"
#include <engine\ui_manager.h>
#include <ia\pathfinding\path_manager.h>
#include <ia\pathfinding\body.h>
#include <ia\pathfinding\mind.h>

Agent::Agent(World* world, const Color color, const Type type, BodyType bodyType, MindType mindType)
: _world(world) {
  switch (bodyType) {
    case BodyType::Steering: _body = std::make_unique<BodySteering>(color, type);
    case BodyType::Pathfinding: _body = std::make_unique<BodyPathFinding>(color);
  }

  switch (mindType) {
    case MindType::Steering: _mind = std::make_unique<MindSteering>(world, _body.get());
    case MindType::Pathfinding: _mind = std::make_unique<MindPathFinding>(world, _body.get());
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
