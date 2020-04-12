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
class Agent;

class Mind {
public:
    std::unique_ptr<Path> _pathTarget = nullptr;
  Mind(Agent* agent, Body* body) : _agent(agent), _body(body) {};
  virtual ~Mind() { _agent = nullptr; _body = nullptr; };

  Mind(const Mind&) = default;
  Mind(Mind&&) noexcept = default;
  Mind& operator=(const Mind&) = default;
  Mind& operator=(Mind&&) noexcept = default;

  virtual void update(uint32_t dt) = 0;
  virtual void setTargetPath(Path path) {}
protected:
  Agent* _agent = nullptr;
  Body*_body = nullptr;
};

#endif
