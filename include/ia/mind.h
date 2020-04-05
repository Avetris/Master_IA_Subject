//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_H__
#define __MIND_H__ 1

#include <cstdint>
#include <ia/defines.h>
#include <ia\door.h>

class Body;
class World;

class Mind {
public:
  Mind(World* world, Body* body) : _world(world), _body(body) {};
  virtual ~Mind() { _world = nullptr; _body = nullptr; };

  Mind(const Mind&) = default;
  Mind(Mind&&) noexcept = default;
  Mind& operator=(const Mind&) = default;
  Mind& operator=(Mind&&) noexcept = default;

  virtual void update(uint32_t dt) = 0;
protected:
  World* _world = nullptr;
  Body*_body = nullptr;
};

#endif
