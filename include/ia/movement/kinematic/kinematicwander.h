//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_WANDER_H__
#define __KINEMATIC_WANDER_H__ 1

#include "ia/movement/movement.h"

class KinematicWander : public Movement {
  public:
    KinematicWander() {}
    ~KinematicWander() {}
    
    void calculate(const KinematicStatus& character, const KinematicStatus*, Steering* steering) override {
        MathLib::Vec2 orientation;
        //orientation of character as vector
        orientation.fromPolar(1.0f, character.orientation);

        steering->velocity_linear = orientation * max_speed_;    //max speed
        //rotate to random (binomial distribution around 0)
        steering->rotation_angular = max_rotation_ * (randomFloat(0.0f, 1.0f) - randomFloat(0.0f, 1.0f));
    } 

};
#endif