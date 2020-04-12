//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/steering/body.h"
#include "engine/debug_draw.h"
#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"

BodySteering::BodySteering(const Color color, const Type type) {
  _type = type;
  _color = color;

  switch(color) {
    case Color::Green: _sprite.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: _sprite.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: _sprite.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: _sprite.loadFromFile(AGENT_RED_PATH); break;
    default: _sprite.loadFromFile(AGENT_GREEN_PATH);
  }

  setSteering(SteeringMode::Kinematic_Seek);
}

void BodySteering::update(const uint32_t dt) {
    if (_type == Type::Autonomous) {

        if (_target && _movement) {

            Vec2 targetStatus = _target->getKinematic()->position;
            if (targetStatus.x() > 0 && targetStatus.y() > 0) {
                Steering steer;
                _movement.get()->calculate(_state, _target->getKinematic(), &steer);
                updateAutomatic(dt, steer);
                if (_state.needsToOrientate) setOrientation(_state.velocity);
            }
        }
    }
    else {
        updateManual(dt);
    }

    _sprite.setPosition(_state.position.x(), _state.position.y());
    _sprite.setRotation(_state.orientation);
}

void BodySteering::render() const {
  _sprite.render();

  DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
  DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
  DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
  DebugDraw::drawPositionHist(_state.position);
}

void BodySteering::setTarget(Agent* target) {
  _target = target;
}

void BodySteering::updateAutomatic(const uint32_t dt, const Steering& steering) {
    const float time = dt * 0.001f;             //dt comes in miliseconds

    if (_isKinematic) {
        _state.velocity = steering.velocity_linear;
        _state.rotation = steering.rotation_angular;
    }
    else {
        _state.velocity += steering.velocity_linear;
        _state.rotation += steering.rotation_angular;
    }

    _state.position += _state.velocity * time;
    _state.orientation += _state.rotation * time;

    if (!_isKinematic) keepInSpeed();
    keepInBounds();

    dd.green.pos = _state.position;
    dd.green.v = _state.velocity;
}

void BodySteering::updateManual(const uint32_t dt) {
  const float time = dt * 0.001f;             //dt comes in miliseconds

  MathLib::Vec2 orientation;
  orientation.fromPolar(1.0f, _state.orientation);
  _state.velocity = orientation.normalized() * _state.speed;
  _state.position += _state.velocity * time;

  keepInSpeed();
  keepInBounds();

  dd.green.pos = _state.position;
  dd.green.v = _state.velocity;
}

void BodySteering::setOrientation(const Vec2& velocity) {
  if (velocity.length2() > 0) {
    _state.orientation = atan2(velocity.y(), velocity.x());
  }
}

void BodySteering::keepInBounds() {
  if (_state.position.x() > WINDOW_WIDTH) _state.position.x() = 0.0f;
  if (_state.position.x() < 0.0f) _state.position.x() = WINDOW_WIDTH;
  if (_state.position.y() > WINDOW_HEIGHT) _state.position.y() = 0.0f;
  if (_state.position.y() < 0.0f) _state.position.y() = WINDOW_HEIGHT;
}

void BodySteering::keepInSpeed() {
  if (_state.velocity.length() > _max_speed) {
    _state.velocity = _state.velocity.normalized() * _max_speed;
  }
}