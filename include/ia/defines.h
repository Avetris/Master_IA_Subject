//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __IA_DEFINES_H__
#define __IA_DEFINES_H__ 1

#include "mathlib/vec2.h"

#define AGENT_BLUE_PATH "../assets/images/agent_blue.png"
#define AGENT_RED_PATH "../assets/images/agent_red.png"
#define AGENT_GREEN_PATH "../assets/images/agent_green.png"
#define AGENT_PURPLE_PATH "../assets/images/agent_purple.png"

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

enum class BodyType {
  Steering
};

enum class MindType {
  Steering
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

struct KinematicSteering {
  MathLib::Vec2 velocity{ 0.0f, 0.0f };
  float rotation{ 0.0f };
};

struct Steering {
  MathLib::Vec2 linear{ 0.0f, 0.0f };    //linear acceleration
  float angular{ 0.0f };                //angular acceleration
};

struct KinematicStatus {
  MathLib::Vec2 position{ 0.0f, 0.0f };
  float orientation {0.0f};
  MathLib::Vec2 velocity{ 0.0f, 0.0f };  //linear velocity
  float rotation{0.0f};               //angular velocity

  float speed{ 0.0f };
};

#endif