
#include "ia/pathfinding/body.h"
#include "engine/debug_draw.h"
#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"
#include "ia/movement/kinematic/kinematicarrive.h"

BodyPathFinding::BodyPathFinding(const Color color) {
    _color = color;

    switch (color) {
    case Color::Green: _sprite.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: _sprite.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: _sprite.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: _sprite.loadFromFile(AGENT_RED_PATH); break;
    default: _sprite.loadFromFile(AGENT_GREEN_PATH);
    }
    _movement = std::make_unique<KinematicArrive>();
}

void BodyPathFinding::update(const uint32_t dt) {

    KinematicStatus* targetState = nullptr;
    if (_target) {
        targetState = _target->getKinematic();
    }
    else  if (_pathTarget && _pathTarget->pathFound && !_pathTarget->path.empty() && _pathTarget->index < _pathTarget->path.size()) {
        Vec2* nextCoord = &_pathTarget->path[_pathTarget->index];
        Vec2 pos = _state.position;
        const float distance = (pos - *nextCoord).length();
        if (distance <= _chagenDist) {
            _pathTarget->index++;
            if (_pathTarget->index < _pathTarget->path.size()) {
                nextCoord = &_pathTarget->path[_pathTarget->index];
            }
        }
        targetState = new KinematicStatus();
        targetState->position = *nextCoord;
    }

    if (targetState && targetState->position.x() > 0 && targetState->position.y() > 0) {
        Steering steer;
        _movement.get()->calculate(_state, targetState, &steer);
        updateAutomatic(dt, steer);
        if (_state.needsToOrientate) setOrientation(_state.velocity);
    }

    _sprite.setPosition(_state.position.x(), _state.position.y());
    _sprite.setRotation(_state.orientation);
}

void BodyPathFinding::render() const {
    _sprite.render();

    DebugDraw::drawVector(dd.red.pos, dd.red.v, 0xFF, 0x00, 0x00, 0xFF);
    DebugDraw::drawVector(dd.green.pos, dd.green.v, 0x00, 0x50, 0x00, 0xFF);
    DebugDraw::drawVector(dd.blue.pos, dd.blue.v, 0x00, 0x00, 0xFF, 0xFF);
    DebugDraw::drawPositionHist(_state.position);
}

void BodyPathFinding::setTarget(Agent* target) {
    _target = target;
    _pathTarget = {};
}

void BodyPathFinding::setTargetPath(Path* path) {
    _target = nullptr;
    _pathTarget = path;
    Vec2* coord = &_pathTarget->path[_pathTarget->index];
    _pathTarget->index++;
    Vec2* target = &_pathTarget->path[_pathTarget->index];
    _state.position = *coord;
    _chagenDist = (*coord - *target).length() / 2;
}

void BodyPathFinding::updateAutomatic(const uint32_t dt, const Steering& steering) {
    const float time = dt * 0.001f;             //dt comes in miliseconds

    if (_isKinematic) {
        _state.velocity = steering.velocity_linear;
        _state.rotation = steering.rotation_angular;
    }
    else {
        _state.velocity += steering.velocity_linear;
        _state.rotation += steering.rotation_angular;
    }

    _state.position += steering.velocity_linear * time;
    _state.orientation += steering.rotation_angular * time;

    if (!_isKinematic) keepInSpeed();
    keepInBounds();

    dd.green.pos = _state.position;
    dd.green.v = _state.velocity;
}

void BodyPathFinding::setOrientation(const Vec2& velocity) {
    if (velocity.length2() > 0) {
        _state.orientation = atan2(velocity.y(), velocity.x());
    }
}

void BodyPathFinding::keepInBounds() {
    if (_state.position.x() > WINDOW_WIDTH) _state.position.x() = 0.0f;
    if (_state.position.x() < 0.0f) _state.position.x() = WINDOW_WIDTH;
    if (_state.position.y() > WINDOW_HEIGHT) _state.position.y() = 0.0f;
    if (_state.position.y() < 0.0f) _state.position.y() = WINDOW_HEIGHT;
}

void BodyPathFinding::keepInSpeed() {
    if (_state.velocity.length() > _max_speed) {
        _state.velocity = _state.velocity.normalized() * _max_speed;
    }
}