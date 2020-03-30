//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __VELOCITY_MATCHING_H__
#define __VELOCITY_MATCHING_H__ 1

#include "ia/movement/movement.h"

class VelocityMatching : public Movement{
public:
  VelocityMatching() {}
  virtual ~VelocityMatching() {}

  void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
    //linear acceleration adjusted to time
    steering->velocity_linear = (target->velocity - character.velocity) / time_to_target_;
    if (steering->velocity_linear.length() > max_acceleration_) {   //max out
      //normalized to max acceleration
      steering->velocity_linear = steering->velocity_linear.normalized() * max_acceleration_;
    }

    steering->rotation_angular = 0.0f;     //no angular
  }
};
#endif