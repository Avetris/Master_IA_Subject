
#include <ia/final/slave_manager.h>
#include <ia\final\message_manager.h>
#include <ia\final\world.h>

SlaveManager::SlaveManager() {
    _timeDecide = TIME_DECISSION_SLAVE;
}
    
void SlaveManager::addAgent() {
    std::vector<t_coord> pos = _world->getInterestPositions(Zone::RestSpawn);
    for (size_t i = 0; i < N_AGENTS_MODIFY; i++) {
        _agents.push_back(new Agent(
            _world,
            Color::Slave,
            Type::Autonomous,
            BodyType::Final,
            MindType::Slave,
            { (float)pos[0].x * 8, (float)pos[0].y * 8 }
        ));
    }
}

void SlaveManager::removeAgent() {
    std::vector<uint32_t> agentsRemoved;
    for (size_t i = 0; i < N_AGENTS_MODIFY; i++) {
        if (_agents.size() > 0) {
            agentsRemoved.push_back(_agents.back()->_UID);
            delete _agents.back();
            _agents.pop_back();
        }
    }
    auto it = _working.begin();
    while (it < _working.end()) {
        if (std::count(agentsRemoved.begin(), agentsRemoved.end(), (*it))) {
            it = _working.erase(it);
        }
        else {
            it++;
        }
    }
    it = _reachedBase.begin();
    while ( it < _reachedBase.end()) {
        if (std::count(agentsRemoved.begin(), agentsRemoved.end(), (*it))) {
            it = _reachedBase.erase(it);
        }
        else {
            it++;
        }
    }
}
    
void SlaveManager::receiveMessage() {
     std::vector<const Message*> messages = MessageManager::instance().getMessage(AgentType::SlaveManager, 0);
     for (const Message* msg : messages) {
         if (msg) {
             switch (msg->next_state)
             {
             case State::M_In_Base:
                 _reachedBase.push_back(msg->uid); 
                 for (auto it = _working.begin(); it < _working.end(); it++) {
                     if ((*it) == msg->uid) {
                         _working.erase(it);
                         break;
                     }
                 }
                 break;
             case State::M_Finding_Door: 
             {
                 MessageManager::instance().addMessage({
                        AgentType::Slave,
                        0,
                        State::M_Going_Base,
                        msg->pos
                 });
                 break;
             }
             default:
                 break;
             }
         }
     }
}

void SlaveManager::takeDecission() {
    if (_world->isAlarm()) {
        MessageManager::instance().addMessage({
            AgentType::Slave,
            0,
            State::M_Going_Base,
            {-1,-1}
        });
    }
    else {

        uint8_t size = _agents.size() - _reachedBase.size();
        uint8_t doors = size / 10;
        if (doors == 0) doors = 1;
        size = ceil(size / 2.0f);
        std::vector<t_coord> exitZone = _world->getInterestPositions(Zone::FortressExitZone);
        std::vector<t_coord> workStart = _world->getInterestPositions(Zone::WorkStart);

        uint8_t vWork = 0;
        std::vector<uint32_t> wk;
        for (Agent* ag : _agents) {
            bool reached = std::count(_reachedBase.begin(), _reachedBase.end(), ag->_UID);
            if (!reached) {
                bool isWorking = std::count(_working.begin(), _working.end(), ag->_UID);                
                if (size > 0 && !isWorking) {
                    MessageManager::instance().addMessage({
                        AgentType::Slave,
                        ag->_UID,
                        State::M_Going_Work,
                        workStart[vWork]
                        });
                    vWork++;
                    if (vWork >= workStart.size()) vWork = 0;
                    size--;
                    wk.push_back(ag->_UID);
                }
                else if (doors > 0 && rand() > (RAND_MAX / 2)) {
                    int v = rand() % exitZone.size();
                    MessageManager::instance().addMessage({
                        AgentType::Slave,
                        ag->_UID,
                        State::M_Finding_Door,
                        exitZone[v]
                        });
                    doors--;
                    if (isWorking) {
                        for (auto it = _working.begin(); it < _working.end(); it++) {
                            if (*it == ag->_UID) {
                                _working.erase(it);
                                break;
                            }
                        }
                    }
                }
            }
        }
        std::vector<t_coord> restSpawn = _world->getInterestPositions(Zone::RestSpawn);
        for (Agent* ag : _agents) {
            if (std::count(_working.begin(), _working.end(), ag->_UID)) {
                MessageManager::instance().addMessage({
                    AgentType::Slave,
                    ag->_UID,
                    State::M_Going_Rest,
                    restSpawn[0]
                    });                        
            }
        }
        _working = std::vector(wk);
    }
}

void SlaveManager::takeAlarmDecission() {
    MessageManager::instance().addMessage({
        AgentType::Slave,
        0,
        State::M_Going_Base,
        {-1,-1}
        });
}