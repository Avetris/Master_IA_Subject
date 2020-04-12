#ifndef __SLAVE_MANAGER_H__
#define __SLAVE_MANAGER_H__ 1

#include <cstdint>
#include <ia/final/agent_manager.h>

class SlaveManager : public AgentManager{
  public:
      SlaveManager();

      SlaveManager(const SlaveManager&) = default;
      SlaveManager(SlaveManager&&) noexcept = default;
      SlaveManager& operator=(const SlaveManager&) = default;
      SlaveManager& operator=(SlaveManager&&) noexcept = default;
    
      void reset() final {
          _agents.clear();
          _working.clear();
          _reachedBase.clear();
      }

      void addAgent();

      void removeAgent() final;
    
      void receiveMessage() final;

      void takeDecission() final;

      void takeAlarmDecission() final;
    
  private:
      std::vector<uint32_t> _working;
      std::vector<uint32_t> _reachedBase;
};

#endif