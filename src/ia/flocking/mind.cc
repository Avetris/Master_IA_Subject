#include "ia/flocking/mind.h"
#include <cstdint>
#include "ia/flocking/body.h"
#include "ia/flocking/world.h"

MindFlocking::MindFlocking(Agent* agent, Body* body)
  : Mind(agent, body) {
}

void MindFlocking::update(uint32_t dt) {
  static_cast<BodyFlocking*>(_body)->setTarget(static_cast<WorldFlocking*>(_agent->_world)->getTarget());
}
