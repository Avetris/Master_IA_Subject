//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/scene_steering.h"

#include <cstdio>

void SceneSteering::init() {
  printf("Steering Scene Is Being Initialized\n");
  world_.target()->getKinematic()->position = MathLib::Vec2(0.0f, 0.0f);
}

void SceneSteering::shutdown() {
  printf("Steering Scene Has Been Shutdown\n");
}

void SceneSteering::update(const uint32_t dt) {
    world_.update(dt / slo_mo_);
}

void SceneSteering::handleMouseEvent(const SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        world_.target()->getKinematic()->position = Vec2(x, y);
    }
}

void SceneSteering::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
      case SDLK_F3:
          slo_mo_ = clamp<int8_t>(++slo_mo_, 1, 10);
          printf("Slow Motion Set To %d\n", slo_mo_);
          break;
      case SDLK_F4:
          slo_mo_ = clamp<int8_t>(--slo_mo_, 1, 10);
          printf("Slow Motion Set To %d\n", slo_mo_);
          break;
      case SDLK_F5:
          DebugDraw::toggleEnabled();
          printf("Debug Draw Mode Changed\n");
          break;
      case SDLK_UP: {
          world_.target()->getKinematic()->speed += 20.0f;
          if (world_.target()->getKinematic()->speed > 140.0f) {
              world_.target()->getKinematic()->speed = 140.0f;
          }
          break; }
      case SDLK_DOWN: {
          world_.target()->getKinematic()->speed -= 20.0f;
          if (world_.target()->getKinematic()->speed <= 0.0f) {
              world_.target()->getKinematic()->speed = 0.0f;
          }
          break; }
      case SDLK_LEFT: {
          world_.target()->getKinematic()->orientation -= 0.2f;
          break;
      }
      case SDLK_RIGHT: {
          world_.target()->getKinematic()->orientation += 0.2f;
          break;
      }
    case SDLK_1:
      world_.ia()->setSteering(Body::SteeringMode::Kinematic_Seek);
      printf("Behavior Of Agent Changed To Kinematic_Seek\n");
    break;
    case SDLK_2:
      world_.ia()->setSteering(Body::SteeringMode::Kinematic_Flee);
      printf("Behavior Of Agent Changed To Kinematic_Flee\n");
    break;
    case SDLK_3:
      world_.ia()->setSteering(Body::SteeringMode::Kinematic_Arrive);
      printf("Behavior Of Agent Changed To Kinematic_Arrive\n");
      break;
    case SDLK_4:
      world_.ia()->setSteering(Body::SteeringMode::Kinematic_Wander);
      printf("Behavior Of Agent Changed To Kinematic_Wander\n");
      break;
    case SDLK_q:
      world_.ia()->setSteering(Body::SteeringMode::Seek);
      printf("Behavior Of Agent Changed To Seek\n");
      break;
    case SDLK_w:
      world_.ia()->setSteering(Body::SteeringMode::Flee);
      printf("Behavior Of Agent Changed To Flee\n");
      break;
    case SDLK_e:
      world_.ia()->setSteering(Body::SteeringMode::Arrive);
      printf("Behavior Of Agent Changed To Arrive\n");
      break;
    case SDLK_r:
      world_.ia()->setSteering(Body::SteeringMode::Align);
      printf("Behavior Of Agent Changed To Align\n");
      break;
    case SDLK_t:
      world_.ia()->setSteering(Body::SteeringMode::Velocity_Matching);
      printf("Behavior Of Agent Changed To Velocity_Matching\n");
      break;
    case SDLK_a:
      world_.ia()->setSteering(Body::SteeringMode::Pursue);
      printf("Behavior Of Agent Changed To Pursue\n");
      break;
    case SDLK_s:
      world_.ia()->setSteering(Body::SteeringMode::Face);
      printf("Behavior Of Agent Changed To Face\n");
      break;
    case SDLK_d:
      world_.ia()->setSteering(Body::SteeringMode::LookGoing);
      printf("Behavior Of Agent Changed To LookGoing\n");
      break;
    case SDLK_f:
      world_.ia()->setSteering(Body::SteeringMode::Wander);
      printf("Behavior Of Agent Changed To Wander\n");
      break;
    default:{}
  }
}

void SceneSteering::render() {
    world_.render();
}