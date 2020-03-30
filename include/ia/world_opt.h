
#ifndef __WORLD_OPT_H__
#define __WORLD_OPT_H__ 1

#include "ia/world.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"
#include <ia\flocking\flocking_manager.h>

class WorldOPT: public World {
  public:
      WorldOPT() {
          _flockingManager.init(this);
    };
    ~WorldOPT() {
        _flockingManager.shutdown();
    };

    void update(const float dt) { 
        _flockingManager.update(dt);
    }
    void render() const { _flockingManager.render(); }

    void addAgent() { _flockingManager.addAgent(); }
    void removeAgent() { _flockingManager.removeAgent(); }
    int getNumAgent() { return _flockingManager.getNumAgents(); }

    FlockingManager* flocking() { return &_flockingManager; }
  private:
    FlockingManager _flockingManager;
};

#endif