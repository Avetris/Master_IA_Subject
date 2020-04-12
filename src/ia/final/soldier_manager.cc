#include <ia/final/soldier_manager.h>
#include <ia\final\message_manager.h>
#include <ia/final/world.h>

SoldierManager::SoldierManager() {
    _timeDecide = TIME_DECISSION_SOLDIER;
}

void SoldierManager::addAgent() {
    std::vector<t_coord> pos = _world->getInterestPositions(Zone::Base);
    int v = 0;
    for (size_t i = 0; i < N_AGENTS_MODIFY; i++) {
        _agents.push_back(new Agent(
            _world,
            Color::Soldier,
            Type::Autonomous,
            BodyType::Final,
            MindType::Soldier,
            { (float)pos[v].x * 8, (float)pos[v].y * 8 }
        ));
        v++;
        if (v == pos.size()) {
            v = 0;
        }
    }
}
    
void SoldierManager::receiveMessage() {
    std::vector<const Message*> messages = MessageManager::instance().getMessage(AgentType::SoldierManager, 0);
    for (const Message* msg : messages) {
        if (msg && msg->next_state == State::M_Finding_Door) {
            t_coord p = msg->pos;
            bool found = false;
            for (int i = 0; i < _doors.size() && !found; i++) {
                found = _doors[i].x == p.x && _doors[i].y == p.y;
            }
            if (!found) {
                _doors.push_back(p);
            }
        }
    }
}

void SoldierManager::takeDecission() {
    if (_doors.size() > 0) {
        for (Agent* ag : _agents) {
            if (rand() > (RAND_MAX / 2)) {
                int v = rand() % _doors.size();
                MessageManager::instance().addMessage({
                    AgentType::Soldier,
                    ag->_UID,
                    State::M_Finding_Door,
                    _doors[v]
                });
            }
        }
    }
}
