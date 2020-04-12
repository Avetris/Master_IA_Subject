//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include "ia/defines.h"
#include "ia/movement/movement.h"
#include <ia\movement\kinematic\kinematicseek.h>
#include <ia\movement\kinematic\kinematicflee.h>
#include <ia\movement\kinematic\kinematicarrive.h>
#include <ia\movement\kinematic\kinematicwander.h>
#include <ia\movement\steering\delegated\pursue.h>
#include <ia\movement\steering\seek.h>
#include <ia\movement\steering\align.h>
#include <ia\movement\steering\flee.h>
#include <ia\movement\steering\arrive.h>
#include <ia\movement\steering\velocity_matching.h>
#include <ia\movement\steering\delegated\face.h>
#include <ia\movement\steering\delegated\lookgoing.h>
#include <ia\movement\steering\delegated\wander.h>

class Body {
public:
  Body() = default;
  virtual ~Body() = default;

  Body(const Body&) = default;
  Body(Body&&) noexcept = default;
  Body& operator=(const Body&) = default;
  Body& operator=(Body&&) noexcept = default;

  virtual void update(uint32_t dt) = 0;
  virtual void render() const = 0;

  const KinematicStatus* getKinematic() const { return &_state; }
  KinematicStatus* getKinematic() { return &_state; }

  Movement* getMovement() const { return _movement.get(); }

  void setSteering(const SteeringMode mode)
  {
      if (_movement) {
          _movement.reset();
      }
      _state.needsToOrientate = true;
      switch (mode) {
      case SteeringMode::Kinematic_Seek: {
          _movement = std::make_unique<KinematicSeek>();
          break; }
      case SteeringMode::Kinematic_Flee: {
          _movement = std::make_unique<KinematicFlee>();
          break; }
      case SteeringMode::Kinematic_Arrive: {
          _movement = std::make_unique<KinematicArrive>();
          break; }
      case SteeringMode::Kinematic_Wander: {
          _state.needsToOrientate = false;
          _movement = std::make_unique<KinematicWander>();
          break; }
      case SteeringMode::Seek: {
          _movement = std::make_unique<Seek>();
          break; }
      case SteeringMode::Flee: {
          _movement = std::make_unique<Flee>();
          break; }
      case SteeringMode::Arrive: {
          _movement = std::make_unique<Arrive>();
          break; }
      case SteeringMode::Align: {
          _state.needsToOrientate = false;
          _movement = std::make_unique<Align>();
          break; }
      case SteeringMode::Velocity_Matching: {
          _movement = std::make_unique<VelocityMatching>();
          break; }
      case SteeringMode::Pursue: {
          _movement = std::make_unique<Pursue>();
          break; }
      case SteeringMode::Face: {
          _state.needsToOrientate = false;
          _movement = std::make_unique<Face>();
          break; }
      case SteeringMode::LookGoing: {
          _state.needsToOrientate = false;
          _movement = std::make_unique<LookGoing>();
          break; }
      case SteeringMode::Wander: {
          _state.needsToOrientate = false;
          _movement = std::make_unique<Wander>();
          break; }
      }
  }
protected:
  KinematicStatus _state;

  std::unique_ptr<Movement> _movement;
  bool _isKinematic = false;
};

#endif
