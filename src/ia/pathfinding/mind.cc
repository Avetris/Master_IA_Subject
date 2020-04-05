#include "ia/pathfinding/mind.h"
#include <cstdint>
#include "ia/pathfinding/body.h"
#include "ia/world.h"
#include "ia/pathfinding/world.h"

MindPathFinding::MindPathFinding(World* world, Body* body)
  : Mind(world, body) {
}

void MindPathFinding::update(uint32_t dt) {
    if (_nextTarget) {
        _pathTarget = std::move(_nextTarget);
        _nextTarget = nullptr;
        if (_pathTarget->pathFound) {
            static_cast<BodyPathFinding*>(_body)->setTargetPath(_pathTarget.get());
        }
        else {
            static_cast<BodyPathFinding*>(_body)->setTarget(nullptr);
        }
    }
}

void MindPathFinding::setTargetPath(Path path) {
    _nextTarget = std::make_unique<Path>(path);
}

void MindPathFinding::checkDoor(Door* door)
{
    std::pair<MathLib::Vec2, MathLib::Vec2> fromTo = door->getCoord();
    if (_pathTarget) {
        for (uint16_t i = _pathTarget->index; i < _pathTarget->path.size(); i++) {
            if (fromTo.first.x() <= _pathTarget->path[i].x() && fromTo.second.x() >= _pathTarget->path[i].x() && fromTo.first.y() <= _pathTarget->path[i].y() && fromTo.second.y() >= _pathTarget->path[i].y()) {
                t_coord startPos = { _body->getKinematic()->position.x(), _body->getKinematic()->position.y() };
                MathLib::Vec2 endVec = _pathTarget->path.back();
                t_coord endPos = { (int)endVec.x(), (int)endVec.y() };
                PathManager::instance().addPath(startPos, endPos, this, AgentType::Slave, _pathTarget->draw);
                break;
            }
        }
    }
}
