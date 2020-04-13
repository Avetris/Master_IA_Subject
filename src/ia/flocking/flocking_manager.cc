
#include "ia/flocking/flocking_manager.h"
#include <ia\flocking\mind.h>

void FlockingManager::init(World* world) {
    _world = world;
    addAgent();
}

void FlockingManager::render() const
{
    for (auto it = _agents.begin(); it < _agents.end(); it++) {
        (*it)->render();
    }
}

void FlockingManager::update(const uint32_t dt){
    for (auto it = _agents.begin(); it < _agents.end(); it++) {        
        (*it)->update(dt);
    }
}

MathLib::Vec2 FlockingManager::getCenter() const
{
    MathLib::Vec2 sum = { 0.0f ,0.0f };
    for (auto it = _agents.begin(); it < _agents.end(); it++) {
        sum.addEq((*it)->getKinematic()->position);
    }
    if (_agents.size() > 0) {
        sum.divEq(_agents.size());
    }
    else {
        sum = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    }
    return sum;
}

void FlockingManager::addAgent()
{
    _agents.push_back(new Agent(
        _world, 
        Color::Green, 
        Type::Autonomous, 
        BodyType::Flocking, 
        MindType::Flocking, 
        { static_cast<float>(rand() % WINDOW_WIDTH), static_cast<float>(rand() % WINDOW_HEIGHT) }
    ));
}

void FlockingManager::removeAgent()
{
    if (_agents.size() > 1) {
        _agents.pop_back();
    }
}

std::vector<const KinematicStatus*> FlockingManager::getFlock(uint16_t agent_uid, MathLib::Vec2 pos) const
{
    std::vector<const KinematicStatus*> result;
    for (auto it = _agents.begin(); it < _agents.end(); it++) {
        if ((*it)->_UID != agent_uid) {
            const KinematicStatus* status = (*it)->getKinematic();
            MathLib::Vec2 aux = status->position.sub(pos);
            if (aux.length2() <= _sq_radius) {
                result.push_back(status);
            }
        }
    }
    return result;
}

void FlockingManager::shutdown()
{
    _agents.clear();
}
