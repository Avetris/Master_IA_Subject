//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_FLEE_H__
#define __KINEMATIC_FLEE_H__ 1

#include "ia/movement/movement.h"

class KinematicFlee : public Movement {
  public:
    KinematicFlee() {}
    ~KinematicFlee() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //going full speed opposite to the target
      steering->velocity_linear = (character.position - target->position).normalized() * max_speed_;
      steering->rotation_angular = 0.0f;     //no rotation
    }
};
#endif