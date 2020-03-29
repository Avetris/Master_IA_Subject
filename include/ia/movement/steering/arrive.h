//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __ARRIVE_H__
#define __ARRIVE_H__ 1

#include "ia/movement/movement.h"

class Arrive : public Movement {
  public:
    Arrive(){}
    virtual ~Arrive() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //direction to the target
      const MathLib::Vec2 direction = target->position - character.position;
      const float distance = direction.length();    //distance to target

      float target_speed = max_speed_;      //max speed
      if (distance < slow_radius_) {        //inside the slow zone
        //speed slowing down
        target_speed = (max_speed_ * distance) / slow_radius_;
      }

      //velocity towards the target
      const MathLib::Vec2 target_velocity = direction.normalized() * target_speed;
      //linear acceleration adjusted to time
      steering->velocity = (target_velocity - character.velocity) / time_to_target_;
      if (steering->velocity.length() > max_acceleration_) {   //max out
        //normalized to max acceleration
        steering->velocity = steering->velocity.normalized() * max_acceleration_;
      }

      steering->rotation = 0.0f;     //no angular
    }
};
#endif