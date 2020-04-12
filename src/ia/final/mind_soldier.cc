#include "ia/final/mind_soldier.h"
#include <cstdint>
#include "ia/final/body.h"
#include "ia/world.h"
#include "ia/pathfinding/world.h"
#include <ia\final\message_manager.h>
#include <ia\final\world.h>

void MindSoldier::update(uint32_t dt) {
    receiveMessage();
    switch (_machineState[0])
    {
        case State::Idle:
            findDoor();
            break;
        case State::M_Waiting: checkWaiting(); break;
        case State::M_Finding_Door: checkFindingDoor(); break;
        case State::M_Action_Door: checkActionDoor(); break;
        case State::M_Going_Base: checkGoingBase(); break;
    }

    State st = State::Idle;
    switch (_machineState[0]) {
    case State::M_Waiting:
        st = State::B_Waiting;
        break;
    case State::M_Action_Door:
    case State::M_Finding_Door:
    case State::M_Going_Base:
        st = State::B_Pathing;
        break;
    }
    static_cast<BodyFinal*>(_body)->setState(st);
}

void MindSoldier::checkWaiting()
{
    if (_nextTarget) {
        _pathTarget = std::move(_nextTarget);
        _nextTarget = nullptr;
        if (_target.x != -1 && _target.y != -1) {
            askForPath(_target, AgentType::Soldier);
        }
    }
    if (_pathTarget || !_waitingPath) {
        if (_pathTarget && _pathTarget->pathFound) {
            static_cast<BodyFinal*>(_body)->setTargetPath(_pathTarget.get());
            _machineState[0] = _machineState[1];
        }
        else {
            if (_machineState[1] == State::M_Going_Base || _machineState[2] == State::M_Going_Base) {
                findBase();
            }
            else {
                findDoor();
            }
        }
    }
}

void MindSoldier::checkFindingDoor()
{
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_body->getKinematic()->position);
    if (d && !d->isOpen()) {
        std::vector<t_coord> doorPos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::DoorOutPoint);
        int distance = -1;
        t_coord goal = { -1,-1 };
        for (t_coord dp : doorPos) {
            int dis = static_cast<WorldFinal*>(_agent->_world)->getDistance(position, dp);
            if (distance == -1 || dis < distance) {
                distance = dis;
                goal = dp;
            }
        }
        _machineState[2] = State::M_Action_Door;
        askForPath(goal, AgentType::Soldier);
    }
    else if (_pathTarget->index >= _pathTarget->path.size()) {
        findDoor();
    }
}

void MindSoldier::checkActionDoor()
{    
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_body->getKinematic()->position, DISTANCE_ACTION);
    if (d && !d->isOpen()) {
        MessageManager::instance().addMessage({
            AgentType::SoldierManager,
            0,
            State::M_Finding_Door,
            position
            });
        d->changeStatus();
        findBase();
    }
    else if (!_pathTarget || _pathTarget->index >= _pathTarget->path.size()) {
        findDoor();
    }
}

void MindSoldier::checkGoingBase()
{
    std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::Base);
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    bool found = false;
    for (t_coord coord : pos) {
        if (coord == position) {
            findDoor();
            found = true;
            break;
        }
    }
    if (!found && (!_pathTarget || _pathTarget->index >= _pathTarget->path.size())) {
        findBase();
    }
}

void MindSoldier::findBase()
{
    _machineState[2] = State::M_Going_Base;
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::Base);
    int v = (rand() % zones.size());
    t_coord position = {
           round(_body->getKinematic()->position.x() / 8),
           round(_body->getKinematic()->position.y() / 8)
    };
    askForPath(zones[v], AgentType::Soldier);
}

void MindSoldier::findDoor()
{
    _machineState[2] = State::M_Finding_Door;
    t_coord position = {
           round(_body->getKinematic()->position.x() / 8),
           round(_body->getKinematic()->position.y() / 8)
    };
    WorldFinal* w = static_cast<WorldFinal*>(_agent->_world);
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::OutdoorExitZone);
    int v = 0;
    do {
        v = (rand() % zones.size());
    } while (zones[v].x == position.x && zones[v].y == position.y);
    askForPath(zones[v], AgentType::Soldier);
}

bool MindSoldier::receiveMessage()
{
    std::vector<const Message*> messages = MessageManager::instance().getMessage(AgentType::Soldier, _agent->_UID);
    for (const Message* msg : messages) {
        if (msg) {
            if (msg->pos.x != -1 && msg->pos.y != -1) {
                _machineState[2] = msg->next_state;
                askForPath(msg->pos, AgentType::Soldier);
            }
        }
    }
    return messages.size() > 0;
}
