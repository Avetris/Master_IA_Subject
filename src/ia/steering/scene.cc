//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/steering/scene.h"

#include <cstdio>

void SceneSteering::init() {
  printf("Steering Scene Is Being Initialized\n");
}

void SceneSteering::shutdown() {
  printf("Steering Scene Has Been Shutdown\n");
}

void SceneSteering::update(const uint32_t dt) {
  _world.update(dt);
}

void SceneSteering::handleMouseEvent(const SDL_Event e, int x, int y) {
  if (e.type == SDL_MOUSEBUTTONUP) {
    _world.getTarget()->getKinematic()->position = Vec2(x, y);
  }
}

void SceneSteering::handleKeyEvent(const SDL_Keycode key) {
  switch (key) {
    case SDLK_1:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Kinematic_Seek);
      printf("Behavior Of Agent Changed To Kinematic_Seek\n");
    break;
    case SDLK_2:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Kinematic_Flee);
      printf("Behavior Of Agent Changed To Kinematic_Flee\n");
    break;
    case SDLK_3:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Kinematic_Arrive);
      printf("Behavior Of Agent Changed To Kinematic_Arrive\n");
      break;
    case SDLK_4:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Kinematic_Wander);
      printf("Behavior Of Agent Changed To Kinematic_Wander\n");
      break;
    case SDLK_q:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Seek);
      printf("Behavior Of Agent Changed To Seek\n");
      break;
    case SDLK_w:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Flee);
      printf("Behavior Of Agent Changed To Flee\n");
      break;
    case SDLK_e:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Arrive);
      printf("Behavior Of Agent Changed To Arrive\n");
      break;
    case SDLK_r:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Align);
      printf("Behavior Of Agent Changed To Align\n");
      break;
    case SDLK_t:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Velocity_Matching);
      printf("Behavior Of Agent Changed To Velocity_Matching\n");
      break;
    case SDLK_a:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Pursue);
      printf("Behavior Of Agent Changed To Pursue\n");
      break;
    case SDLK_s:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Face);
      printf("Behavior Of Agent Changed To Face\n");
      break;
    case SDLK_d:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::LookGoing);
      printf("Behavior Of Agent Changed To LookGoing\n");
      break;
    case SDLK_f:
      static_cast<BodySteering*>(_world.getIA()->getBody())->setSteering(SteeringMode::Wander);
      printf("Behavior Of Agent Changed To Wander\n");
      break;

    case SDLK_UP: {
      _world.getTarget()->getKinematic()->speed += 20.0f;
      if (_world.getTarget()->getKinematic()->speed > 140.0f) {
        _world.getTarget()->getKinematic()->speed = 140.0f;
      }
      break; }
    case SDLK_DOWN: {
      _world.getTarget()->getKinematic()->speed -= 20.0f;
      if (_world.getTarget()->getKinematic()->speed <= 0.0f) {
        _world.getTarget()->getKinematic()->speed = 0.0f;
      }
      break; }
    case SDLK_LEFT: {
      _world.getTarget()->getKinematic()->orientation -= 0.2f;
      break;
    }
    case SDLK_RIGHT: {
      _world.getTarget()->getKinematic()->orientation += 0.2f;
      break;
    }

    default:{}
  }
}

void SceneSteering::render() {
  _world.render();
}