
#include "ia/final/body.h"
#include "engine/debug_draw.h"
#include "engine/defines.h"
#include "ia/agent.h"
#include "ia/defines.h"

BodyFinal::BodyFinal(World* world, const Color color) {
    _color = color;
    _world = static_cast<WorldFinal*>(world);

    switch (color) {
    case Color::Green: _sprite.loadFromFile(AGENT_GREEN_PATH); break;
    case Color::Blue: _sprite.loadFromFile(AGENT_BLUE_PATH); break;
    case Color::Purple: _sprite.loadFromFile(AGENT_PURPLE_PATH); break;
    case Color::Red: _sprite.loadFromFile(AGENT_RED_PATH); break;
    case Color::Slave: _sprite.loadFromFile(AGENT_SLAVE_PATH); break;
    case Color::Guard: _sprite.loadFromFile(AGENT_GUARD_PATH); break;
    case Color::Soldier: _sprite.loadFromFile(AGENT_SOLDIER_PATH); break;
    default: _sprite.loadFromFile(AGENT_GREEN_PATH);
    }
    _movement = std::make_unique<Wander>();

    _state.speed = 100.0f;
}

void BodyFinal::update(const uint32_t dt) {
    switch (_machineState)
    {
    case State::B_Wander:
        updateWander(dt);
        break;
    case State::B_Waiting:
        break;
    case State::B_Pathing:
    case State::B_Pathing_Slow:
        updatePath(dt);
        break;
    case State::B_Chasing:
        updateAgainsTarget(dt);
        break;
    default:
        break;
    }
}

void BodyFinal::updatePath(const uint32_t dt) {
    KinematicStatus* targetState = nullptr;
    if (_pathTarget && _pathTarget->pathFound && !_pathTarget->path.empty() && _pathTarget->index < _pathTarget->path.size()) {
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
    else {
        bool t = true;

    }

    if (targetState && targetState->position.x() >= 0 && targetState->position.y() >= 0) {
        Steering steer;
        //_movement.get()->calculate(_state, targetState, &steer);
        steer.velocity_linear = (targetState->position - _state.position).normalized() * _state.speed;
        updateAutomatic(dt, steer);
    }
    else {
        bool t = true;
    }

    _sprite.setPosition(_state.position.x(), _state.position.y());
}

void BodyFinal::updateWander(uint32_t dt)
{
    Steering steer;
    std::vector<MathLib::Vec2> possibilities;

    t_coord pos = { (int)_state.position.x() / 8, (int)_state.position.y() / 8 };
    t_coord vel = { (int)_state.velocity.x() / _state.speed, (int)_state.velocity.y() / _state.speed };

    if ((vel.x == 0 && vel.y == 0 )
        || _world->getZoneMap(pos.x + vel.x, pos.y + vel.y) != Zone::RestZone
        || rand() % 100 > 90) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i != 0 || j != 0) && _world->getZoneMap(pos.x + i, pos.y + j) == Zone::RestZone) {
                    possibilities.push_back({ i * _state.speed, j * _state.speed });
                }
            }
        }
        if (possibilities.size() > 0) {
            int v = (rand() % possibilities.size());
            steer.velocity_linear = possibilities[v];
        }
    }
    else {
        steer.velocity_linear = _state.velocity;
    }
    updateAutomatic(dt, steer);
    _sprite.setPosition(_state.position.x(), _state.position.y());
}

void BodyFinal::updateAgainsTarget(uint32_t dt)
{
    Vec2 targetStatus = _target ? _target->getKinematic()->position : Vec2(-1, -1);
    if (targetStatus.x() >= 0 && targetStatus.y() >= 0 && 
        (targetStatus.x() != _state.position.x() || targetStatus.y() != _state.position.y())) {
        Steering steer;
        _movement.get()->calculate(_state, _target->getKinematic(), &steer);
        updateAutomatic(dt, steer);
    }
    _sprite.setPosition(_state.position.x(), _state.position.y());
}

void BodyFinal::render() const {
    _sprite.render();
}

void BodyFinal::setState(State state) {
    _machineState = state;
    _state.speed = 70.0f;
    switch (_machineState)
    {
    case State::B_Wander:
        setSteering(SteeringMode::Kinematic_Wander);
        break;
    case State::B_Pathing:
        setSteering(SteeringMode::Kinematic_Arrive);
        break;
    case State::B_Pathing_Slow:
        setSteering(SteeringMode::Kinematic_Arrive);
        _state.speed = 50.0f;
        break;
    case State::B_Chasing:
        setSteering(SteeringMode::Kinematic_Seek);
        break;
    default:
        break;
    }
    if (static_cast<WorldFinal*>(_world)->isAlarm()) {
        _state.speed = 100.0f;
    }
}

void BodyFinal::setTarget(Agent* target) {
    _target = target;
    _pathTarget = {};
}

void BodyFinal::setTargetPath(Path* path) {
    _target = nullptr;
    _pathTarget = path;
    if (_pathTarget) {
        _pathTarget->index = 0;
        Vec2* coord = &_pathTarget->path[_pathTarget->index];

        t_coord c1 = { round(coord->x() / 8), round(coord->y() / 8) };
        t_coord c2 = { round(_state.position.x() / 8), round(_state.position.y() / 8) };
        uint16_t distance = _world->getDistance(c1, c2);
        if (distance > 10) {
            for (int i = 0; i < _pathTarget->path.size(); i++) {
                c1 = { 
                    (int) round(_pathTarget->path[_pathTarget->index].x() / 8), 
                    (int) round(_pathTarget->path[_pathTarget->index].y() / 8)
                };
                uint16_t d = _world->getDistance(c1, c2);
                if (d < distance) {
                    _pathTarget->index = i;
                }
            }
            coord = &_pathTarget->path[_pathTarget->index];
        }
        

        _pathTarget->index++;
        Vec2* target = &_pathTarget->path[_pathTarget->index];
        _state.position = *coord;
        _chagenDist = (*coord - *target).length() / 2;
    }
}

void BodyFinal::updateAutomatic(const uint32_t dt, const Steering& steering) {
    const float time = dt * 0.001f;             //dt comes in miliseconds
       
    _state.velocity = steering.velocity_linear;
    _state.rotation = steering.rotation_angular;
   
    _state.position += _state.velocity * time;
    _state.orientation += _state.rotation * time;
}