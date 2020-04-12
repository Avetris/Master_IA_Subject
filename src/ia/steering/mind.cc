//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/steering/mind.h"
#include <cstdint>
#include "ia/steering/body.h"
#include "ia/world.h"
#include "ia/steering/world.h"

MindSteering::MindSteering(Agent* agent, Body* body)
  : Mind(agent, body) {
}

void MindSteering::update(uint32_t dt) {
  static_cast<BodySteering*>(_body)->setTarget(static_cast<WorldSteering*>(_agent->_world)->getTarget());
}
