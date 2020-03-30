#ifndef __FLOCKING_MANAGER_H__
#define __FLOCKING_MANAGER_H__ 1

#include <ia\flocking\flocking_agent.h>

#include <cstdint>

class FlockingManager {
  public:
    FlockingManager() {};
    ~FlockingManager() {};


    void init(World* world);
    void update(uint32_t dt);
    void render() const;

    void addAgent();
    void removeAgent();
    int getNumAgents() { return _agents.size(); }
    void setTarget(MathLib::Vec2 pos) { _target.zeros(); _target.addEq(pos); }

    void shutdown();
  private:
      std::vector<const KinematicStatus*> getFlock(uint16_t agent_uid, MathLib::Vec2 pos) const;
      World* world_;
      std::vector<FlockingAgent> _agents;
      MathLib::Vec2 _target;
      float _sq_radius = 10.0f;

      MathLib::Vec2 getCenter() const;
};

#endif