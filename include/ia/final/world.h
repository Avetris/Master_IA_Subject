#ifndef __WORLD_FINAL_H__
#define __WORLD_FINAL_H__ 1

#include "engine/defines.h"
#include "ia/defines.h"
#include "ia/pathfinding/map_world.h"
#include "mathlib/vec2.h"

#include <cstdlib>
#include <ia\final\soldier_manager.h>
#include <ia\final\slave_manager.h>
#include <ia\final\guard_manager.h>

using MathLib::Vec2;

class WorldFinal: public MapWorld {
  public:
      WorldFinal();
    ~WorldFinal() {};

    void reset();

    void addAgent(AgentType type);

    void removeAgent(AgentType type);

    void update(uint32_t dt) final;

    void render() const final;

    void openDoor(uint8_t doorNumber);

    Agent* isAgentNear(MathLib::Vec2 coord, AgentType type, int distance = DISTANCE_SHOW);

    Door* isDoorNear(MathLib::Vec2 coord, int distance = DISTANCE_SHOW);

    uint16_t getCount(AgentType type);

    void setAlarm();

    bool isAlarm() const;

private:
    SlaveManager _slaveManager;
    GuardManager _guardManager;
    SoldierManager _soldierManager;

    float _timeToAlarm = 0.0f;
    uint32_t _spriteAlarm;
};

#endif