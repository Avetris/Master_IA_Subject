#ifndef __FLOCKING_MANAGER_H__
#define __FLOCKING_MANAGER_H__ 1

#include <ia\agent.h>
#include <engine/defines.h>

#include <cstdint>

class FlockingManager {
  public:
    FlockingManager() {};
    ~FlockingManager() {
        for (auto p : _agents)
        {
            delete p;
        }
        _agents.clear();        
    };


    void init(World* world);
    void update(uint32_t dt);
    void render() const;

    void addAgent();
    void removeAgent();
    int getNumAgents() { return _agents.size(); }
    std::vector<const KinematicStatus*> getFlock(uint16_t agent_uid, MathLib::Vec2 pos) const;

    void shutdown();
  private:
      World* _world;
      std::vector<Agent*> _agents;
      const float _sq_radius = WINDOW_WIDTH / 2;

      MathLib::Vec2 getCenter() const;
};

#endif