//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/mind.h"
#include "ia/body.h"
#include "ia/world.h"
#include <engine\ui_manager.h>

void Mind::init(uint16_t UID, World* world, Body* body) {
    _UID = UID;
    world_ = world;
    body_ = body;
}

void Mind::render() const{}

void Mind::update(const uint32_t dt){

    if (_pathFound.pathFound && !_pathFound.path.empty() && _pathFound.index < _pathFound.path.size()) {
        Vec2* nextCoord = &_pathFound.path[_pathFound.index];
        Vec2 pos = body_->getPosition();
        const float distance = getDistance(pos, *nextCoord);
        if (distance <= _chagenDist / 2) {
            _pathFound.index++;
            if (_pathFound.index < _pathFound.path.size()) {
                nextCoord = &_pathFound.path[_pathFound.index];
                body_->setTarget(nextCoord);
            }
        }
    }
}

void Mind::setPath(Path pathFound)
{
    pathFound.uid = _pathFound.uid;
    _pathFound = pathFound;
    char text[255];
    if (pathFound.pathFound && pathFound.timeLast > 0) {
        sprintf_s(text, "LAST TIME IN PATH FINDING: %f", pathFound.timeLast);
        Vec2* coord = &_pathFound.path[_pathFound.index];
        _pathFound.index++;
        Vec2* target = &_pathFound.path[_pathFound.index];
        body_->getKinematic()->position = *coord;
        body_->setTarget(target);
        _chagenDist = getDistance(*coord, *target);
    }
    else {
        sprintf_s(text, "PATH NOT FOUND");
    }
    if (pathFound.uid > 0) {
        UIManager::instance().setText(text, _pathFound.uid);
    }
    else {
        _pathFound.uid = UIManager::instance().addText(text, WINDOW_WIDTH / 2, 0);
    }
}

std::pair<bool, Path*> Mind::checkDoor(Door* door)
{
    std::pair<MathLib::Vec2, MathLib::Vec2> fromTo = door->getCoord();
    std::pair<bool, Path*> result;
    result.first = false;
    for (uint16_t i = _pathFound.index; !result.first && i < _pathFound.path.size();  i++) {
        if (fromTo.first.x() <= _pathFound.path[i].x() && fromTo.second.x() >= _pathFound.path[i].x() && fromTo.first.y() <= _pathFound.path[i].y() && fromTo.second.y() >= _pathFound.path[i].y()) {
            result.first = true;
            result.second = &_pathFound;
        }
    }

    return result;
}

float Mind::getDistance(MathLib::Vec2 origin, MathLib::Vec2 target)
{
    const MathLib::Vec2 direction = origin - target;
    return direction.length();    //distance to target
}
