#include "ia/final/mind.h"
#include <ia/pathfinding/path_manager.h>

void MindFinal::setTargetPath(Path path) {
    _waitingPath = false;
    _nextTarget = std::make_unique<Path>(path);
}

void MindFinal::askForPath(t_coord goal, AgentType type) {
    if (!_waitingPath) {
        PathManager::instance().addPath({ -1,-1 }, goal, _agent, type, false);
        _waitingPath = true;
        _target = { -1,-1 };
    }
    else {
        _target = goal;
    }
    
}