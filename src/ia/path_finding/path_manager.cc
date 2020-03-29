//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/path_finding/path_manager.h"
#include "engine/debug_draw.h"
#include "engine/window.h"
#include "ia/defines.h"
#include "ia/scene_steering.h"

#include <cstdio>
#include <iostream>

void PathManager::init() {
	Map::loadMap(_costs);
	_loopThread = std::thread(&PathManager::calculatePath, this);
}

void PathManager::addPath(t_coord startPos, t_coord endPos, Agent* target, PathFinding::Type type) {

	_mutex.lock();
	_pathQueue.push({startPos, endPos, target, type });
	_mutex.unlock();
}

void PathManager::applyDoor(Door* door)
{
	std::pair<MathLib::Vec2, MathLib::Vec2> fromTo = door->getCoord();
	fromTo.first /= 8;
	fromTo.second /= 8;
	for (uint16_t y = fromTo.first.y(); y < fromTo.second.y(); y++) {
		for (uint16_t x = fromTo.first.x(); x < fromTo.second.x(); x++) {
			if (_costs[x][y] != 0) {
				_costs[x][y] = door->isOpen() ? 1 : -1;
			}
		}
	}
}

void PathManager::calculatePath()
{
	while (true) {
		if (!_pathQueue.empty()) {
			_pathQueue.front().init(_costs);
			_pathQueue.front().findPath();
			_mutex.lock();
			_pathQueue.pop();
			_mutex.unlock();
		}
	}
}
