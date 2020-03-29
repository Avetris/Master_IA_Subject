#ifndef __WORLD_PATH_H__
#define __WORLD_PATH_H__ 1

#include "ia/world.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"
#include <ia\door.h>
#include <ia\path_finding\path_manager.h>

using MathLib::Vec2;

class WorldPath: public World {
  public:
    WorldPath() {
        PathManager::instance().init();
        doors[0].init({152.0f, 240.0f}, {160.0f, 304.0f});
        doors[1].init({ 886.0f, 24.0f }, { 944.0f, 32.0f });
        doors[2].init({ 672.0f, 432.0f }, { 680.0f, 464.0f });
        doors[3].init({ 408.0f, 656.0f }, { 416.0f, 704.0f });
        for (int i = 0; i < 4; i++) {
            PathManager::instance().applyDoor(&doors[i]);
        }
        ia_.init(this, Body::Color::Green, Body::Type::Autonomous);
        ia_.getKinematic()->position = Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        ia_.setSteering(Body::SteeringMode::Kinematic_Arrive);
    };
    ~WorldPath() {
        ia_.shutdown();
    };

    void update(const float dt) { ia_.update(dt); }
    void render() const { 
        for (Door d : doors) {
            d.render();
        }
        ia_.render(); 
    }
    void openDoor(uint8_t doorNumber) {
        if (doorNumber < 4 && doorNumber >= 0) {
            doors[doorNumber].changeStatus();
            PathManager::instance().applyDoor(&doors[doorNumber]);
        }
    }

    Agent* ia() { return &ia_; }
  private:
    Agent ia_;
    Door doors[4];
};

#endif