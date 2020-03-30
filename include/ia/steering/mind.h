//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_STEERING_H__
#define __MIND_STEERING_H__ 1

#include "ia/mind.h"

class MindSteering: public Mind {
  public:
    MindSteering(World* world, Body* body);
    ~MindSteering() {};

    void update(uint32_t dt) final;
};

#endif