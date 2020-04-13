#include "ia/flocking/body.h"
#include "engine/debug_draw.h"
#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include <ia\flocking\world.h>

BodyFlocking::BodyFlocking(Agent* agent, const Color color) {
  _color = color;
  _agent = agent;

  switch(color) {
    case Color::Green: _sprite.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: _sprite.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: _sprite.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: _sprite.loadFromFile(AGENT_RED_PATH); break;
    default: _sprite.loadFromFile(AGENT_GREEN_PATH);
  }

  setSteering(SteeringMode::Kinematic_Seek);
}

void BodyFlocking::update(const uint32_t dt) {
    std::vector<const KinematicStatus*> around = static_cast<WorldFlocking*>(_agent->_world)->flocking()->getFlock(_agent->_UID, _state.position);

    Steering steer;

    if (_seek_percent > 0.0f) {
        Steering seekSteer;
        _movement.get()->calculate(_state, _target->getKinematic(), &seekSteer);
        steer.velocity_linear = seekSteer.velocity_linear * _seek_percent;
    }

    if (_alignment_percent > 0.0f) {
        steer.velocity_linear += _alignment.calculate(_state, around).velocity_linear * _alignment_percent;
    }

    if (_cohesion_percent > 0.0f) {
        steer.velocity_linear += _cohesion.calculate(_state, around).velocity_linear * _cohesion_percent;
    }

    if (_separation_percent > 0.0f) {
        steer.velocity_linear += _separation.calculate(_state, around).velocity_linear * _separation_percent;
    }

    if (steer.velocity_linear.length2() > 0) {
        steer.velocity_linear = steer.velocity_linear.normalized() * 100.0f;
    }


    updateAutomatic(dt, steer);
    setOrientation(_state.velocity);

    _sprite.setPosition(_state.position.x(), _state.position.y());
    _sprite.setRotation(_state.orientation);
}

void BodyFlocking::render() const {
  _sprite.render();

  DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
  DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
  DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
  DebugDraw::drawPositionHist(_state.position);
}

void BodyFlocking::setTarget(Agent* target) {
    _target = target;
}

void BodyFlocking::updateAutomatic(const uint32_t dt, const Steering& steering) {
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

void BodyFlocking::updateManual(const uint32_t dt) {
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

void BodyFlocking::setOrientation(const Vec2& velocity) {
  if (velocity.length2() > 0) {
    _state.orientation = atan2(velocity.y(), velocity.x());
  }
}

void BodyFlocking::keepInBounds() {
  if (_state.position.x() > WINDOW_WIDTH) _state.position.x() = 0.0f;
  if (_state.position.x() < 0.0f) _state.position.x() = WINDOW_WIDTH;
  if (_state.position.y() > WINDOW_HEIGHT) _state.position.y() = 0.0f;
  if (_state.position.y() < 0.0f) _state.position.y() = WINDOW_HEIGHT;
}

void BodyFlocking::keepInSpeed() {
  if (_state.velocity.length() > _max_speed) {
    _state.velocity = _state.velocity.normalized() * _max_speed;
  }
}