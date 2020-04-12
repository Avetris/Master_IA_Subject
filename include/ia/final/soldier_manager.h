#ifndef __SOLDIER_MANAGER_H__
#define __SOLDIER_MANAGER_H__ 1

#include <cstdint>
#include <ia/final/agent_manager.h>

class SoldierManager : public AgentManager{
  public:
      SoldierManager();

    SoldierManager(const SoldierManager&) = default;
    SoldierManager(SoldierManager&&) noexcept = default;
    SoldierManager& operator=(const SoldierManager&) = default;
    SoldierManager& operator=(SoldierManager&&) noexcept = default;

    void reset() final {
        _agents.clear();
        _doors.clear();
    }
    
    void addAgent();
    
    void receiveMessage() final;

    void takeDecission() final;

    void takeAlarmDecission() final {}
    
  private:
      std::vector<t_coord> _doors;
};

#endif
