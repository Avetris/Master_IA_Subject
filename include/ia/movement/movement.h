//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__ 1

#include "ia/defines.h"
#include "engine/math.h"

class Movement {
  public:
    Movement() {}
    virtual ~Movement() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) = 0;
    
    void setMaxAcceleration(float max_acceleration) { max_acceleration_ = max_acceleration; }
    void setMaxAngAcc(float max_ang_acc) { max_ang_acc_ = max_ang_acc; }
    void setMaxPrediction(float max_prediction) { max_prediction_ = max_prediction; }
    void setMaxRotation(float max_rotation) { max_rotation_ = max_rotation; }
    void setMaxSpeed(float max_speed) { max_speed_ = max_speed; }

    void setSlowRadius(float slow_radius) { slow_radius_ = slow_radius; }
    void setSQRadius(float slow_radius) { slow_radius_ = slow_radius; }
    void setTimeToTarget(float time_to_target) { time_to_target_ = time_to_target; }

    void setWanderOffset(float wander_offset) { wander_offset_ = wander_offset; }
    void setWanderOrientation(float wander_orientation) { wander_orientation_ = wander_orientation; }
    void setWanderRadius(float wander_radius) { wander_radius_ = wander_radius; }
    void setWanderRate(float wander_rate) { wander_rate_ = wander_rate; }

  protected:
      float max_acceleration_ = 5.0f;
      float max_ang_acc_ = 2.0f;
      float max_prediction_ = 2.0f;
      float max_rotation_ = 2.0f;
      float max_speed_ = 100.0f;

      float slow_radius_ = 0.2f;
      float sq_radius_ = 10.0f;      //squared radius
      float time_to_target_ = 1.0f;


      float wander_offset_ = 50.0f;       //forward offset of circle
      float wander_orientation_ = 0.0f;   //current orientation of target
      float wander_radius_ = 20.0f;       //radius of circle
      float wander_rate_ = 2.0f;          //max rate at which wander orientation can change
};
#endif