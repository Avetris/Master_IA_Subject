//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SEEK_H__
#define __SEEK_H__ 1

#include "ia/movement/movement.h"

class Seek : public Movement {
  public:
    Seek() {}
    virtual ~Seek() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //acceleration towards the target
      steering->velocity = (target->position - character.position).normalized() * max_acceleration_;
      steering->rotation = 0.0f;   //no angular
    }
};
#endif