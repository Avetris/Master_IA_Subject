//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WORLD_STEERING_H__
#define __WORLD_STEERING_H__ 1

#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "ia/world.h"
#include "mathlib/vec2.h"

using MathLib::Vec2;

class WorldSteering: public World {
  public:
    WorldSteering() {
      _agents[0] = Agent(this, Color::Red, Type::Manual, BodyType::Steering, MindType::Steering);
      _agents[1] = Agent(this, Color::Green, Type::Autonomous, BodyType::Steering, MindType::Steering);
      _agents[0].getKinematic()->position = Vec2(0.0f, 0.0f);
      _agents[1].getKinematic()->position = Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    };

    ~WorldSteering() {};

    void update(uint32_t dt) final {
      for(uint32_t i = 0; i < nAgents; ++i) {
        _agents[i].update(dt);
      }
    }
    void render() const final {
      for (uint32_t i = 0; i < nAgents; ++i) {
        _agents[i].render();
      }
    }

    Agent* getTarget() {
      return &_agents[0];
    }

    Agent* getIA() {
      return &_agents[1];
    }
  private:
    static constexpr uint32_t nAgents = 2;
    Agent _agents[nAgents];
};

#endif