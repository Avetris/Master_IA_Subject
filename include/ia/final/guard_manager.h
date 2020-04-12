#ifndef __GUARD_MANAGER_H__
#define __GUARD_MANAGER_H__ 1

#include <cstdint>
#include <ia/final/agent_manager.h>

class GuardManager : public AgentManager{
  public:
      GuardManager() = default;

      GuardManager(const GuardManager&) = default;
      GuardManager(GuardManager&&) noexcept = default;
      GuardManager& operator=(const GuardManager&) = default;
      GuardManager& operator=(GuardManager&&) noexcept = default;
    
      void addAgent();
    
      void receiveMessage() final;

      void takeDecission() final;

      void takeAlarmDecission() final;
};

#endif
