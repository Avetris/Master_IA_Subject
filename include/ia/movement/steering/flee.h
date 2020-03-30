//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __FLEE_H__
#define __FLEE_H__ 1

#include "ia/movement/movement.h"

class Flee : public Movement {
  public:
    Flee() {}
    virtual ~Flee() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //acceleration opposite to the target
      steering->velocity_linear = (character.position - target->position).normalized() * max_acceleration_;
      steering->rotation_angular = 0.0f;   //no angular
    }
};
#endif