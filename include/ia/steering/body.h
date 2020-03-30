//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_STEERING_H__
#define __BODY_STEERING_H__ 1

#include "engine/sprite.h"
#include "ia/body.h"
#include "ia/defines.h"
#include "ia/movement/kinematic/kinematicarrive.h"
#include "ia/movement/kinematic/kinematicflee.h"
#include "ia/movement/kinematic/kinematicseek.h"
#include "ia/movement/kinematic/kinematicwander.h"
#include "ia/movement/steering/align.h"
#include "ia/movement/steering/arrive.h"
#include "ia/movement/steering/delegated/face.h"
#include "ia/movement/steering/delegated/lookgoing.h"
#include "ia/movement/steering/delegated/pursue.h"
#include "ia/movement/steering/delegated/wander.h"
#include "ia/movement/steering/flee.h"
#include "ia/movement/steering/seek.h"
#include "ia/movement/steering/velocity_matching.h"
#include "mathlib/vec2.h"

class Agent;

class BodySteering: public Body {
  public:

    BodySteering(Color color, Type type);
    ~BodySteering() {};

    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    void setSteering(const SteeringMode mode) { steering_mode_ = mode; };
    
  private:
    void updateKinematic(uint32_t dt, const KinematicSteering& steering);
    void updateSteering(uint32_t dt, const Steering& steering);
    void updateManual(uint32_t);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();

    Sprite sprite_;
    Type type_;
    Color color_;
    SteeringMode steering_mode_;
    Agent* target_;

    const float max_speed_ = 100.0f;

    struct {
      struct {
        MathLib::Vec2 pos;
        MathLib::Vec2 v;
      } green, red, blue;
    } dd;

    KinematicSeek k_seek_;
    KinematicFlee k_flee_;
    KinematicArrive k_arrive_;
    KinematicWander k_wander_;

    Seek seek_;
    Flee flee_;
    Arrive arrive_;
    Align align_;
    VelocityMatching vel_matching_;

    Pursue pursue_;
    Face face_;
    LookGoing look_going_;
    Wander wander_;
};

#endif