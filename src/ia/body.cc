//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/body.h"
#include "engine/debug_draw.h"
#include "ia/agent.h"
#include "ia/defines.h"

void Body::init(uint16_t UID, const Color color, const Type type, MathLib::Vec2 position ) {
    _UID = UID;
    type_ = type;
    color_ = color;

  switch(color) {
    case Color::Green: sprite_.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: sprite_.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: sprite_.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: sprite_.loadFromFile(AGENT_RED_PATH); break;
    default: sprite_.loadFromFile(AGENT_GREEN_PATH);
  }

  kinmeticTarget_.position = position;
  setSteering(SteeringMode::Kinematic_Seek);
}

void Body::update(const uint32_t dt) {
  if (type_ == Type::Autonomous) {
    KinematicStatus* targetStatus = target_ ? target_->getKinematic() : &kinmeticTarget_;

    if (targetStatus && movement_ && 
        (targetStatus->position.x() > 0 && targetStatus->position.y() > 0)) {

        Steering steer;
        movement_.get()->calculate(state_, targetStatus, &steer);
        updateAutomatic(dt, steer);
        if(state_.needsToOrientate) setOrientation(state_.velocity);
    }
  } else {
    updateManual(dt);
  }

  sprite_.setPosition(state_.position.x(), state_.position.y());
  sprite_.setRotation(state_.orientation);
}

void Body::render() const {
  sprite_.render();

  DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
  DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
  DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
  DebugDraw::drawPositionHist(state_.position);
}

void Body::setTarget(Agent* target) {
  target_ = target;
}

void Body::setTarget(Vec2* target) {
    kinmeticTarget_.position = *target;
}

void Body::setSteering(const SteeringMode mode)
{
    if (movement_) {
        movement_.reset();
    }
    state_.needsToOrientate = true;
    switch (mode) {
    case SteeringMode::Kinematic_Seek: {
        movement_ = std::make_unique<KinematicSeek>();
        break; }
    case SteeringMode::Kinematic_Flee: {
        movement_ = std::make_unique<KinematicFlee>();
        break; }
    case SteeringMode::Kinematic_Arrive: {
        movement_ = std::make_unique<KinematicArrive>();
        break; }
    case SteeringMode::Kinematic_Wander: {
        movement_ = std::make_unique<KinematicWander>();
        break; }
    case SteeringMode::Seek: {
        movement_ = std::make_unique<Seek>();
        break; }
    case SteeringMode::Flee: {
        movement_ = std::make_unique<Flee>();
        break; }
    case SteeringMode::Arrive: {
        movement_ = std::make_unique<Arrive>();
        break; }
    case SteeringMode::Align: {
        state_.needsToOrientate = false;
        movement_ = std::make_unique<Align>();
        break; }
    case SteeringMode::Velocity_Matching: {
        movement_ = std::make_unique<VelocityMatching>();
        break; }
    case SteeringMode::Pursue: {
        movement_ = std::make_unique<Pursue>();
        break; }
    case SteeringMode::Face: {
        movement_ = std::make_unique<Face>();
        break; }
    case SteeringMode::LookGoing: {
        movement_ = std::make_unique<LookGoing>();
        break; }
    case SteeringMode::Wander: {
        movement_ = std::make_unique<Wander>();
        break; }
    }
}

void Body::updateAutomatic(const uint32_t dt, const Steering& steering) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  if (isKinematic_) {
      state_.velocity = steering.velocity_linear;
      state_.rotation = steering.rotation_angular;
  }
  else {
      state_.velocity += steering.velocity_linear;
      state_.rotation += steering.rotation_angular;
  }
  
  state_.position += steering.velocity_linear * time;
  state_.orientation += steering.rotation_angular * time;

  if (!isKinematic_) keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::updateManual(const uint32_t dt) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  MathLib::Vec2 orientation;
  orientation.fromPolar(1.0f, state_.orientation);
  state_.velocity = orientation.normalized() * state_.speed;
  state_.position += state_.velocity * time;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = state_.position;
  dd.green.v = state_.velocity;
}

void Body::setOrientation(const Vec2& velocity) {
  if (velocity.length2() > 0) {
    state_.orientation = atan2(velocity.y(), velocity.x());
  }
}

void Body::keepInBounds() {
  if (state_.position.x() > WINDOW_WIDTH) state_.position.x() = 0.0f;
  if (state_.position.x() < 0.0f) state_.position.x() = WINDOW_WIDTH;
  if (state_.position.y() > WINDOW_HEIGHT) state_.position.y() = 0.0f;
  if (state_.position.y() < 0.0f) state_.position.y() = WINDOW_HEIGHT;
}

void Body::keepInSpeed() {
  if (state_.velocity.length() > max_speed_) {
    state_.velocity = state_.velocity.normalized() * max_speed_;
  }
}