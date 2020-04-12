#include "ia/final/mind_guard.h"
#include <cstdint>
#include "ia/final/body.h"
#include "ia/world.h"
#include "ia/pathfinding/world.h"
#include <ia\final\message_manager.h>
#include <ia\final\world.h>

void MindGuard::update(uint32_t dt) {
    receiveMessage();
    checkNearSlave();
    switch (_machineState[0])
    {
    case State::Idle:
        findPatrol();
        break;
    case State::M_Waiting: checkWaiting(); break;
    case State::M_Patroling: checkPatroling(); break;
    case State::M_Finding_Door: checkFindingDoor(); break;
    }

    State st = State::Idle;
    switch (_machineState[0]) {
    case State::M_Waiting:
        st = State::B_Waiting;
        break;
    case State::M_Action_Door:
    case State::M_Finding_Door:
    case State::M_Patroling:
        st = State::B_Pathing;
        break;
    case State::M_Chasing: st = State::B_Chasing; break;
    }
    static_cast<BodyFinal*>(_body)->setState(st);
}

void MindGuard::checkNearSlave()
{
    if (static_cast<WorldFinal*>(_agent->_world)->isAlarm()) {
        Agent* ag = static_cast<WorldFinal*>(_agent->_world)->isAgentNear(_body->getKinematic()->position, AgentType::Slave, DISTANCE_CHASE);
        if (ag) {
            _machineState[0] = State::M_Chasing;
            _machineState[1] = State::M_Chasing;
            static_cast<BodyFinal*>(_body)->setTarget(ag);
        }
        else if (_machineState[0] == State::M_Chasing) {
            findDoor();
        }
    }
    else if (_machineState[0] == State::M_Chasing) {
        findPatrol();
    }
}

void MindGuard::checkWaiting()
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
            switch (_machineState[1])
            {
            case State::M_Patroling: findPatrol(); break;
            case State::M_Finding_Door: findDoor(); break;
            }
        }
    }
}

void MindGuard::checkPatroling()
{
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_body->getKinematic()->position);
    if (d && d->isOpen()) {
        std::vector<t_coord> doorPos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::DoorInPoint);
        int distance = -1;
        t_coord goal = { -1,-1 };
        for (t_coord dp : doorPos) {
            int dis = static_cast<WorldFinal*>(_agent->_world)->getDistance(position, dp);
            if (distance == -1 || dis < distance) {
                distance = dis;
                goal = dp;
            }
        }
        askForPath(goal, AgentType::Guard);

        _machineState[0] = State::M_Waiting;
        _machineState[1] = State::M_Finding_Door;
    }
    else if (_pathTarget->index >= _pathTarget->path.size()) {
        findPatrol();
    }
}

void MindGuard::checkFindingDoor()
{
    if (!_pathTarget || _pathTarget->index >= _pathTarget->path.size()) {
        t_coord position = {
            round(_body->getKinematic()->position.x() / 8),
            round(_body->getKinematic()->position.y() / 8)
        };
        Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_body->getKinematic()->position, DISTANCE_ACTION);
        if (d && d->isOpen()) {
            d->changeStatus();
            static_cast<WorldFinal*>(_agent->_world)->setAlarm();
            findDoor();
        }
        else if (_pathTarget->index >= _pathTarget->path.size()) {
            findDoor();
        }
    }
}

void MindGuard::findPatrol()
{
    _machineState[0] = State::M_Waiting;
    _machineState[1] = State::M_Patroling;
    t_coord position = {
           round(_body->getKinematic()->position.x() / 8),
           round(_body->getKinematic()->position.y() / 8)
    };
   
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::FortressExitZone);
    std::vector<t_coord> z = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::FortressZone);
    zones.insert(zones.end(), z.begin(), z.end());
    z = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::RestZone);
    zones.insert(zones.end(), z.begin(), z.end());
    int v = 0;
    do {
        v = (rand() % zones.size());
    } while (zones[v].x == position.x && zones[v].y == position.y);
    askForPath(zones[v], AgentType::Guard);
}

void MindGuard::findDoor()
{
    _machineState[0] = State::M_Waiting;
    _machineState[1] = State::M_Finding_Door;
    t_coord position = {
           round(_body->getKinematic()->position.x() / 8),
           round(_body->getKinematic()->position.y() / 8)
    };
    
    
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::DoorInPoint);
    int v = 0;
    do {
        v = (rand() % zones.size());
    } while (zones[v].x == position.x && zones[v].y == position.y);
    askForPath(zones[v], AgentType::Guard);
}

bool MindGuard::receiveMessage()
{
    std::vector<const Message*> messages = MessageManager::instance().getMessage(AgentType::Guard, _agent->_UID);
    for (const Message* msg : messages) {
        if (msg) {
            if (_machineState[0] != State::M_Finding_Door) {
                if (msg->pos.x != -1 && msg->pos.y != -1) {
                    askForPath(msg->pos, AgentType::Guard);
                }
                _machineState[0] = State::M_Waiting;
                _machineState[1] = msg->next_state;
            }
        }
    }
    return messages.size() > 0;
}
