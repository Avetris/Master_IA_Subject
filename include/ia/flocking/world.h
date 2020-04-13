
#ifndef __WORLD_FLOCKING_H__
#define __WORLD_FLOCKING_H__ 1

#include "ia/world.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"
#include <ia\flocking\flocking_manager.h>

class WorldFlocking: public World {
  public:
      WorldFlocking() {
          _flockingManager.init(this);
          _target = {
            this,
            Color::Red,
            Type::Manual,
            BodyType::Steering,
            MindType::Steering,
            {0.0f,0.0f}
          };
    };
    ~WorldFlocking() {
        _flockingManager.shutdown();
    };

    void update(uint32_t dt) {
        _target.update(dt);
        _flockingManager.update(dt);
    }
    void render() const {
        _target.render(); 
        _flockingManager.render(); 
    }

    void addAgent() { _flockingManager.addAgent(); }
    void removeAgent() { _flockingManager.removeAgent(); }
    uint8_t getNumAgent() { return _flockingManager.getNumAgents(); }

    void setTarget(MathLib::Vec2 target) {
        _target.getKinematic()->position = target;
    }

    Agent* getTarget() {
        return &_target;
    }
    FlockingManager* flocking() { return &_flockingManager; }
  private:
    FlockingManager _flockingManager;
    Agent _target;
};

#endif