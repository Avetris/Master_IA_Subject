//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include "engine/sprite.h"
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

class Body {
  public:
    enum class Color {
      Green,
      Blue,
      Purple,
      Red,
    };

    enum class Type {
      Autonomous,
      Manual,
    };

    enum class SteeringMode {
      Kinematic_Seek,         //1       Kinematics
      Kinematic_Flee,         //2
      Kinematic_Arrive,       //3
      Kinematic_Wander,       //4
      Seek,                   //q       Steering Basics
      Flee,                   //w
      Arrive,                 //e
      Align,                  //r
      Velocity_Matching,      //t
      Pursue,                 //a       Steering Delegated
      Face,                   //s
      LookGoing,              //d
      Wander,                 //f
    };

    Body() {};
    ~Body() {};

    void init(Color color, Type type);
    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    void setTarget(Vec2* target);
    void setSteering(const SteeringMode mode);
    const KinematicStatus* getKinematic() const { return &state_; }
    KinematicStatus* getKinematic() { return &state_; }
    MathLib::Vec2 getPosition() const { return state_.position; }
    Movement* getMovement() const { return movement_.get(); }
  private:
    void updateAutomatic(uint32_t dt, const Steering& steering);
    void updateManual(uint32_t);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();

    bool isKinematic_ = false;

    Sprite sprite_;
    Type type_;
    Color color_;
    Agent* target_ = nullptr;
    KinematicStatus kinmeticTarget_;

    const float max_speed_ = 100.0f;

    struct {
      struct {
        MathLib::Vec2 pos;
        MathLib::Vec2 v;
      } green, red, blue;
    } dd;

    KinematicStatus state_;

    std::unique_ptr<Movement> movement_;
};

#endif