#include "ia/final/mind.h"
#include <ia/pathfinding/path_manager.h>

void MindFinal::setTargetPath(Path path) {
    _waitingPath = false;
    _nextTarget = std::make_unique<Path>(path);
}

void MindFinal::askForPath(t_coord goal, AgentType type) {
    if (!_waitingPath) {
        _waitingPath = true;
        _pathTarget = nullptr;
        PathManager::instance().addPath({ -1,-1 }, goal, _agent, type, false);
        _target = { -1,-1 };
        _machineState[0] = State::M_Waiting;
        if (_machineState[2] != State::Idle) {
            _machineState[1] = _machineState[2];
            _machineState[2] = State::Idle;
        }
    }
    else {
        _target = goal;
    }
    
}