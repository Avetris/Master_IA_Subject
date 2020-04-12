#ifndef __AGENT_MANAGER_H__
#define __AGENT_MANAGER_H__ 1

#include "ia/defines.h"

#include <cstdint>
#include <ia/agent.h>

class WorldFinal;

class AgentManager {
  public:
      AgentManager() = default;
      ~AgentManager() {
          for (auto p : _agents)
          {
              delete p;
          }
          _agents.clear();
      }

      AgentManager(const AgentManager&) = default;
    AgentManager(AgentManager&&) noexcept = default;
    AgentManager& operator=(const AgentManager&) = default;
    AgentManager& operator=(AgentManager&&) noexcept = default;

    virtual void reset() {
        _agents.clear();
    }

    void setWorld(WorldFinal* world) { _world = world; }

    virtual void addAgent() = 0;

    virtual void removeAgent();

    void update(uint32_t dt);

    void render() const;

    Agent* isAgentNear(MathLib::Vec2 coord, int distance);

    uint16_t getCount() { return _agents.size(); }

    virtual void receiveMessage() = 0;

    virtual void takeDecission() = 0;

    virtual void takeAlarmDecission() = 0;
    
  protected:
      uint8_t _timeDecide = 8;
      float _currentTime = _timeDecide;
      WorldFinal* _world = nullptr;

      std::vector<Agent*> _agents;
};

#endif
