
#include <ia/final/guard_manager.h>
#include <ia\final\message_manager.h>
#include <ia/final/world.h>

void GuardManager::addAgent() {
    std::vector<t_coord> zones = _world->getInterestPositions(Zone::FortressExitZone);
    std::vector<t_coord> z = _world->getInterestPositions(Zone::FortressZone);
    zones.insert(zones.end(), z.begin(), z.end());
    z = _world->getInterestPositions(Zone::RestZone);
    zones.insert(zones.end(), z.begin(), z.end());
    for (size_t i = 0; i < N_AGENTS_MODIFY; i++) {
        int v = rand() % zones.size();
        _agents.push_back(new Agent(
            _world,
            Color::Guard,
            Type::Autonomous,
            BodyType::Final,
            MindType::Guard,
            { (float)zones[v].x * 8, (float)zones[v].y * 8 }
        ));
    }
}
    
void GuardManager::receiveMessage() {
}

void GuardManager::takeDecission() {
}

void GuardManager::takeAlarmDecission() {
    std::vector<t_coord> coords = _world->getInterestPositions(Zone::DoorInPoint);
    size_t pos = 0;
    for (Agent* ag : _agents) {
        MessageManager::instance().addMessage({
            AgentType::Guard,
            ag->_UID,
            State::M_Finding_Door,
            coords[pos]
            });
        pos++;
        if (pos >= coords.size()) pos = 0;
    }
}