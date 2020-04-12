#include "ia/final/mind_slave.h"
#include <cstdint>
#include "ia/final/body.h"
#include "ia/world.h"
#include "ia/pathfinding/world.h"
#include <ia\final\message_manager.h>
#include <ia\final\world.h>

void MindSlave::update(uint32_t dt) {
    receiveMessage();
    checkArrested();
    switch (_machineState[0])
    {
    case State::Idle:
        _machineState[0] = State::M_Resting;
        break;
    case State::M_Waiting: checkWaiting(); break;
    case State::M_Finding_Door: checkFindingDoor(); break;
    case State::M_Going_Base: checkGoingBase(); break;
    case State::M_Going_Work: checkGoingWork(); break;
    case State::M_Working: checkWorking(); break;
    case State::M_Working_Back: checkWorkingBack(); break;
    case State::M_Arrested:
    case State::M_Going_Rest: checkGoingRest(); break;
    case State::M_Resting: _nextTarget = nullptr; _pathTarget = nullptr; break;
    }

    State st = State::Idle;
    switch (_machineState[0]) {
    case State::M_Waiting:
        st = State::B_Waiting;
        break;
    case State::M_Finding_Door:
    case State::M_Going_Base:
    case State::M_Going_Work:
    case State::M_Going_Rest:
    case State::M_Working_Back:
    case State::M_Arrested:
        st = State::B_Pathing;
        break;
    case State::M_Working: st = State::B_Pathing_Slow; break;
    case State::M_Resting: st = State::B_Wander; break;
    case State::M_In_Base: st = State::B_Stopped; break;
    }
    static_cast<BodyFinal*>(_body)->setState(st);
}

void MindSlave::checkArrested()
{
    if (static_cast<WorldFinal*>(_agent->_world)->isAlarm()
        && !_escaped
        && _machineState[0] != State::M_Arrested && _machineState[1] != State::M_Arrested) {
        Agent* ag = static_cast<WorldFinal*>(_agent->_world)->isAgentNear(_body->getKinematic()->position, AgentType::Guard, 2);
        if (ag) {
            _machineState[2] = State::M_Arrested;
            goRest();
        }
    }
}

void MindSlave::checkWaiting()
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
            case State::M_Going_Base: 
                if (static_cast<WorldFinal*>(_agent->_world)->isAlarm()) findDoor();
                else goRest();
            case State::M_Finding_Door: findDoor(); break;
            case State::M_Going_Work: goWork(); break;
            case State::M_Arrested: 
                goRest();
                _machineState[0] = State::M_Waiting;
                _machineState[1] = State::M_Arrested;
                break;
            case State::M_Going_Rest: goRest(); break;
            case State::M_Working: work(true); break;
            case State::M_Working_Back: work(false); break;
            }
        }
    }
}

void MindSlave::checkFindingDoor()
{
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_body->getKinematic()->position);
    if (d && d->isOpen()) {
        MessageManager::instance().addMessage({
            AgentType::SlaveManager,
            0,
            State::M_Finding_Door,
            position
            });
        findBase();
    }
    else if (!_pathTarget || _pathTarget->index >= _pathTarget->path.size()) {
        findDoor();
        _machineState[0] = State::M_Waiting;
        _machineState[1] = State::M_Finding_Door;
    }
}

void MindSlave::checkGoingBase()
{
    t_coord position = {
            round(_body->getKinematic()->position.x() / 8),
            round(_body->getKinematic()->position.y() / 8)
    };
    bool found = false;
    if (!_escaped) {
        std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::OutdoorZone);

        for (t_coord coord : pos) {
            if (coord == position) {
                MessageManager::instance().addMessage({
                    AgentType::SlaveManager,
                    _agent->_UID,
                    State::M_In_Base
                    });
                _escaped = true;
                found = true;
                break;
            }
        }

        if (!_escaped) {
            if (_pathTarget && _pathTarget->index + 1 < _pathTarget->path.size()) {
                Door* d = static_cast<WorldFinal*>(_agent->_world)->isDoorNear(_pathTarget->path[_pathTarget->index + 1], 2);
                if (d && !d->isOpen()) {
                    findBase();
                    found = true;
                }
            }
            else {
                goRest();
                found = true;
            }
        }
    }
    else {
        std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::Base);
        for (t_coord coord : pos) {
            if (coord == position) {
                _machineState[0] = State::M_In_Base;
                _machineState[1] = State::M_In_Base;
                _machineState[2] = State::M_In_Base;
                found = true;
                break;
            }
        }
    }
    if (!found && (!_pathTarget || _pathTarget->index >= _pathTarget->path.size())) {
        findBase();
    }
}

void MindSlave::checkGoingWork()    
{
    if (!_pathTarget->pathFound || _pathTarget->index >= _pathTarget->path.size() - 1) {
        std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkStart);
        t_coord position = {
            round(_body->getKinematic()->position.x() / 8),
            round(_body->getKinematic()->position.y() / 8)
        };
        for (int i = 0; i < pos.size(); i++) {
            if (pos[i] == position) {
                work(true, i);
                break;
            }
        }
    }
}

void MindSlave::checkWorking()
{
    std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkGoal);
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    bool found = false;
    int h = 0;
    for (int i = 0; i < pos.size(); i++) {
        if (pos[i] == position) {
            work(false, i);
            found = true;
            break;
        }
        else if (pos[i].y == position.y) {
            h = i;
        }
    }
    if (!found && (!_pathTarget || _pathTarget->index >= _pathTarget->path.size())) {
        work(true, h);
    }
}

void MindSlave::checkWorkingBack()
{
    std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkStart);
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    bool found = false;
    int h = 0;
    for (int i = 0; i < pos.size(); i++) {
        if (pos[i] == position) {
            work(true, i);
            found = true;
            break;
        }
        else if (pos[i].y == position.y) {
            h = i;
        }
    }
    if (!found && (!_pathTarget || _pathTarget->index >= _pathTarget->path.size())) {
        work(false, h);
    }
}

void MindSlave::checkGoingRest()
{
    std::vector<t_coord> pos = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::RestSpawn);
    t_coord position = {
        round(_body->getKinematic()->position.x() / 8),
        round(_body->getKinematic()->position.y() / 8)
    };
    bool found = false;
    for (t_coord coord : pos) {
        if (coord == position) {
            _machineState[0] = State::M_Resting;
            _machineState[1] = State::Idle;
            found = true;
            break;
        }
    }
    if (!found && (!_pathTarget || _pathTarget->index >= _pathTarget->path.size())) {
        goRest();
    }
}

void MindSlave::findBase()
{
    _machineState[2] = State::M_Going_Base;
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::Base);
    int v = (rand() % zones.size());
    t_coord position = {
            round(_body->getKinematic()->position.x() / 8),
            round(_body->getKinematic()->position.y() / 8)
    };
    askForPath(zones[v], AgentType::Slave);
}

void MindSlave::findDoor()
{
    _machineState[2] = State::M_Finding_Door;
    t_coord position = {
           round(_body->getKinematic()->position.x() / 8),
           round(_body->getKinematic()->position.y() / 8)
    };
    t_coord goal = { -1,-1 };
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::FortressExitZone);
    int v = 0;
    do {
        v = (rand() % zones.size());
    } while (zones[v].x == position.x && zones[v].y == position.y);        
    askForPath(zones[v], AgentType::Slave);
}

void MindSlave::goWork()
{
    _machineState[2] = State::M_Going_Work;
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkStart);
    int v = (rand() % zones.size());
    askForPath(zones[v], AgentType::Slave);
}

void MindSlave::goRest()
{
    _machineState[2] = _machineState[2] == State::M_Arrested ? State::M_Arrested : State::M_Going_Rest;
    std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::RestSpawn);
    askForPath(zones[0], AgentType::Slave);
}

void MindSlave::work(bool go, int workPos)
{
    t_coord goal = { -1,-1 };
    if (go) {
        _machineState[2] = State::M_Working;
        std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkGoal);
        int v = workPos == -1 ? (rand() % zones.size()) : workPos;
        goal = zones[v];
    }
    else {
        _machineState[2] = State::M_Working_Back;
        WorldFinal* w = static_cast<WorldFinal*>(_agent->_world);
        std::vector<t_coord> zones = static_cast<WorldFinal*>(_agent->_world)->getInterestPositions(Zone::WorkStart);
        int v = workPos == -1 ? (rand() % zones.size()) : workPos;
        goal = zones[v];
    }
    askForPath(goal, AgentType::Slave);
}

bool MindSlave::receiveMessage()
{
    std::vector<const Message*> messages = MessageManager::instance().getMessage(AgentType::Slave, _agent->_UID);
    for (const Message* msg : messages) {
        if (msg) {
            if (!_escaped && _machineState[0] != State::M_Going_Base && _machineState[0] != State::M_Arrested && _machineState[1] != State::M_Arrested) {
                _target = msg->pos;
                _pathTarget = nullptr;
                _nextTarget = nullptr;
                _machineState[2] = msg->next_state;                
                if (msg->next_state == State::M_Going_Base) findBase();
                else askForPath(_target, AgentType::Slave);
            }
        }
    }
    return messages.size() > 0;
}
