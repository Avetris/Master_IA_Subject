//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WORLD_STEERING_H__
#define __WORLD_STEERING_H__ 1

#include "ia/world.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"

using MathLib::Vec2;

class WorldSteering: public World {
  public:
    WorldSteering() {
      target_.init(this, Body::Color::Red, Body::Type::Manual);
      ia_.init(this, Body::Color::Green, Body::Type::Autonomous);
      ia_.getKinematic()->position = Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
      ia_.setTarget(&target_);
    };
    ~WorldSteering() {
      target_.shutdown();
      ia_.shutdown();
    };

    void update(const float dt) { 
        target_.update(dt); 
        ia_.update(dt); 
    }
    void render() const { target_.render(); ia_.render(); }

    Agent* target() { return &target_; }
    Agent* ia() { return &ia_; }
  private:
    Agent target_, ia_;
};

#endif