//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_ARRIVE_H__
#define __KINEMATIC_ARRIVE_H__ 1

#include "ia/movement/movement.h"

class KinematicArrive : public Movement {
  public:
    KinematicArrive() {}
    ~KinematicArrive() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //direction to the target
      steering->velocity_linear = (target->position - character.position);
      if (steering->velocity_linear.length2() < sq_radius_) {  //inside the radius
        steering->velocity_linear.x() = 0.0f;        //no velocity
        steering->velocity_linear.y() = 0.0f;
      } else {
        steering->velocity_linear /= time_to_target_;     //velocity adjusted to time
        if (steering->velocity_linear.length() > max_speed_) {     //max out
          //normalized direction to max speed
          steering->velocity_linear = steering->velocity_linear.normalized() * max_speed_;
        }
      }
      steering->rotation_angular = 0.0f;   //no rotation
    }
};
#endif