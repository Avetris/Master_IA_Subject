//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/scene_steering.h"

#include <cstdio>

void SceneSteering::init(World* world) {
  printf("Steering Scene Is Being Initialized\n");
  world_ = world;
}

void SceneSteering::shutdown() {
  world_ = nullptr;
  printf("Steering Scene Has Been Shutdown\n");
}

void SceneSteering::update(const uint32_t dt) {}

void SceneSteering::handleMouseEvent(const SDL_Event e) {}

void SceneSteering::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
    case SDLK_1:
      world_->ia()->setSteering(Body::SteeringMode::Kinematic_Seek);
      printf("Behavior Of Agent Changed To Kinematic_Seek\n");
    break;
    case SDLK_2:
      world_->ia()->setSteering(Body::SteeringMode::Kinematic_Flee);
      printf("Behavior Of Agent Changed To Kinematic_Flee\n");
    break;
    case SDLK_3:
      world_->ia()->setSteering(Body::SteeringMode::Kinematic_Arrive);
      printf("Behavior Of Agent Changed To Kinematic_Arrive\n");
      break;
    case SDLK_4:
      world_->ia()->setSteering(Body::SteeringMode::Kinematic_Wander);
      printf("Behavior Of Agent Changed To Kinematic_Wander\n");
      break;
    case SDLK_q:
      world_->ia()->setSteering(Body::SteeringMode::Seek);
      printf("Behavior Of Agent Changed To Seek\n");
      break;
    case SDLK_w:
      world_->ia()->setSteering(Body::SteeringMode::Flee);
      printf("Behavior Of Agent Changed To Flee\n");
      break;
    case SDLK_e:
      world_->ia()->setSteering(Body::SteeringMode::Arrive);
      printf("Behavior Of Agent Changed To Arrive\n");
      break;
    case SDLK_r:
      world_->ia()->setSteering(Body::SteeringMode::Align);
      printf("Behavior Of Agent Changed To Align\n");
      break;
    case SDLK_t:
      world_->ia()->setSteering(Body::SteeringMode::Velocity_Matching);
      printf("Behavior Of Agent Changed To Velocity_Matching\n");
      break;
    case SDLK_a:
      world_->ia()->setSteering(Body::SteeringMode::Pursue);
      printf("Behavior Of Agent Changed To Pursue\n");
      break;
    case SDLK_s:
      world_->ia()->setSteering(Body::SteeringMode::Face);
      printf("Behavior Of Agent Changed To Face\n");
      break;
    case SDLK_d:
      world_->ia()->setSteering(Body::SteeringMode::LookGoing);
      printf("Behavior Of Agent Changed To LookGoing\n");
      break;
    case SDLK_f:
      world_->ia()->setSteering(Body::SteeringMode::Wander);
      printf("Behavior Of Agent Changed To Wander\n");
      break;
    default:{}
  }
}

void SceneSteering::render() {}