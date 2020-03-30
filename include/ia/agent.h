//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __AGENT_H__
#define __AGENT_H__ 1

#include "ia/defines.h"

#include <cstdint>
#include "steering/body.h"
#include "steering/mind.h"

class World;

class Agent {
  public:
    Agent() = default;
    Agent(World* world, Color color, Type type, BodyType bodyType, MindType mindType);
    ~Agent();

    Agent(const Agent&) = default;
    Agent(Agent&&) noexcept = default;
    Agent& operator=(const Agent&) = default;
    Agent& operator=(Agent&&) noexcept = default;

    void update(uint32_t dt);
    void render() const;

    Body* getBody() {
      return _body.get();
    }

    Mind* getMind() {
      return _mind.get();
    }

    const KinematicStatus* getKinematic() const { return _body->getKinematic(); }
    KinematicStatus* getKinematic() { return _body->getKinematic(); }
  private:
    World * _world = nullptr;

    std::unique_ptr<Body> _body;
    std::unique_ptr<Mind> _mind;
};

#endif