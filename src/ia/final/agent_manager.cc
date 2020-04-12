#include <ia/final/agent_manager.h>
#include <ia/final/world.h>
#include <ia/agent.h>
#include <ia/agent.h>
#include <engine/math.h>
#include <ia\final\message_manager.h>
#include <ia\final\mind.h>

void AgentManager::removeAgent() {
    for (size_t i = 0; i < N_AGENTS_MODIFY; i++) {
        if (_agents.size() > 0) {
            delete _agents.back();
            _agents.pop_back();
        }
    }
}

void AgentManager::update(uint32_t dt) {
    receiveMessage();
    _currentTime -= dt * 0.001f;
    if (_currentTime <= 0) {
        takeDecission();
        _currentTime = _timeDecide;
    }
    for (uint32_t i = 0; i < _agents.size(); ++i) {
        _agents[i]->update(dt);
    }
}

void AgentManager::render() const{
    for (uint32_t i = 0; i < _agents.size(); ++i) {
        _agents[i]->render();
    }
}

Agent* AgentManager::isAgentNear(MathLib::Vec2 coord, int distance){
    Agent* ag = nullptr;
    t_coord cc = { round(coord.x() / 8), round(coord.y() / 8) };
    for (int i = 0; i < _agents.size() && !ag; i++) {
        State* sta = static_cast<MindFinal*>(_agents[i]->getMind())->_machineState;
        if (sta[0] != State::M_Arrested 
            && sta[1] != State::M_Arrested 
            && sta[1] != State::M_Going_Rest) {
            t_coord ac = {
                round(_agents[i]->getKinematic()->position.x() / 8),
                round(_agents[i]->getKinematic()->position.y() / 8)
            };
            if (_world->getDistance(cc, ac) <= distance) {
                ag = _agents[i];
            }
        }
    }
    return ag;
}
