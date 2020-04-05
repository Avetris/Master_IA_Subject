//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WORLD_H__
#define __WORLD_H__ 1
#include "agent.h"

class World {
public:
  virtual ~World() {};
  virtual void update(uint32_t dt) = 0;
  virtual void render() const = 0;
};

#endif
