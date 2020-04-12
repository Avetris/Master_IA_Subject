#include <ia/final/world.h>
#include <ia\final\message_manager.h>
#include <ia\pathfinding\path_manager.h>
#include <engine\ui_manager.h>

WorldFinal::WorldFinal() : MapWorld(){
    _slaveManager.setWorld(this);
    _guardManager.setWorld(this);
    _soldierManager.setWorld(this);
    _spriteAlarm = UIManager::instance().addAlarm();
}

void WorldFinal::reset()
{
    _slaveManager.reset();
    _guardManager.reset();
    _soldierManager.reset();
}

void WorldFinal::addAgent(AgentType type) {
    switch (type) {
    case AgentType::Slave:
        _slaveManager.addAgent();
        break;
    case AgentType::Guard:
        _guardManager.addAgent();
        break;
    case AgentType::Soldier:
        _soldierManager.addAgent();
        break;
    }
}

void WorldFinal::removeAgent(AgentType type) {
    switch (type) {
    case AgentType::Slave:
        _slaveManager.removeAgent();
        break;
    case AgentType::Guard:
        _guardManager.removeAgent();
        break;
    case AgentType::Soldier:
        _soldierManager.removeAgent();
        break;
    }
}

void WorldFinal::update(uint32_t dt) {
    if (_timeToAlarm > 0) {
        _timeToAlarm -= dt * 0.001f;
    }
    UIManager::instance().setVisible(_spriteAlarm, _timeToAlarm > 0);
    _slaveManager.update(dt);
    _guardManager.update(dt);
    _soldierManager.update(dt);
    MessageManager::instance().changeStack();
}

void WorldFinal::render() const{
    MapWorld::render();
    _slaveManager.render();
    _guardManager.render();
    _soldierManager.render();
}

void WorldFinal::openDoor(uint8_t doorNumber) {
    if (doorNumber < _numDoors && doorNumber >= 0) {
        _doors[doorNumber].changeStatus();
    }
}

Agent* WorldFinal::isAgentNear(MathLib::Vec2 coord, AgentType type, int distance) {
    Agent* ag;
    switch (type)
    {
    case AgentType::Slave:
        ag = _slaveManager.isAgentNear(coord, distance);
        break;
    case AgentType::Guard:
        ag = _guardManager.isAgentNear(coord, distance);
        break;
    case AgentType::Soldier:
        ag = _soldierManager.isAgentNear(coord, distance);
        break;
    }
    return ag;
}

Door* WorldFinal::isDoorNear(MathLib::Vec2 coord, int distance) {
    t_coord ccoord = {(int) coord.x() / 8, (int) coord.y() / 8};
    MathLib::Vec2 d = { -1,-1 };
    Door* door = nullptr;
    std::vector<t_coord> coords = getInterestPositions(Zone::Door);
    for (t_coord c : coords) {
        if ((getDistance(c, ccoord) <= distance)) {
            d = { (float)c.x * 8, (float)c.y * 8 };
            break;
        }
    }
    if (d.x() != -1 && d.y() != -1) {
        for (size_t i = 0; !door && i < _numDoors; i++) {
            if (_doors[i].isDoor(d)) {
                door = &_doors[i];
            }
        }
    }
    return door;
}

uint16_t WorldFinal::getCount(AgentType type) {
    uint16_t res = 8;
    switch (type)
    {
    case AgentType::Slave:
        res = _slaveManager.getCount();
        break;
    case AgentType::Guard:
        res = _guardManager.getCount();
        break;
    case AgentType::Soldier:
        res = _soldierManager.getCount();
        break;
    default:
        break;
    }
    return res;
}

void WorldFinal::setAlarm()
{
    _timeToAlarm = TIME_ALARM;
    _slaveManager.takeAlarmDecission();
    _guardManager.takeAlarmDecission();
    _soldierManager.takeAlarmDecission();
}

bool WorldFinal::isAlarm() const
{
    return _timeToAlarm > 0;
}
