#ifndef __WORLD_PATH_H__
#define __WORLD_PATH_H__ 1

#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "ia/pathfinding/map_world.h"
#include "mathlib/vec2.h"
#include <ia\door.h>
#include <ia\pathfinding\path_manager.h>
#include <ia\pathfinding\mind.h>

using MathLib::Vec2;

class WorldPath: public MapWorld {
  public:
        WorldPath() : MapWorld() {
        _agents[0] = Agent(this, Color::Green, Type::Autonomous, BodyType::Pathfinding, MindType::Pathfinding);
        _agents[0].getKinematic()->position = Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        _agents[0].getBody()->getMovement()->setTimeToTarget(0.01f);
    };
    ~WorldPath() {};

    void update(uint32_t dt) final {
        for (uint32_t i = 0; i < nAgents; ++i) {
            _agents[i].update(dt);
        }
    }

    void render() const final {
        MapWorld::render();
        for (uint32_t i = 0; i < nAgents; ++i) {
            _agents[i].render();
        }
    }
    void openDoor(uint8_t doorNumber) {
        if (doorNumber < _numDoors && doorNumber >= 0) {
            _doors[doorNumber].changeStatus();
            PathManager::instance().applyDoor(&_doors[doorNumber]);
            if (!_doors[doorNumber].isOpen()) {
                for (uint32_t i = 0; i < nAgents; ++i) {
                    static_cast<MindPathFinding*>(_agents[i].getMind())->checkDoor(&_doors[doorNumber]);
                }
            }
        }
    }

    Agent* getIA() {
        return &_agents[0];
    }
private:
    static constexpr uint32_t nAgents = 1;
    Agent _agents[nAgents];
};

#endif