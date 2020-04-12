#include "ia/pathfinding/path_manager.h"
#include "engine/debug_draw.h"
#include "engine/window.h"
#include "ia/defines.h"

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <ia\door.h>

void PathManager::init(MapWorld* world, Zone zones[MAP_L2_WIDTH][MAP_L2_WIDTH], PointNode* pointNodes, uint8_t numPointNodes)
{
	if (!_loopThread.joinable()) {
		Map::loadMap(_costs);
		for (size_t i = 0; i < MAP_L2_WIDTH; i++) {
			for (size_t j = 0; j < MAP_L2_HEIGHT; j++) {
				_zones[i][j] = &zones[i][j];
			}
		}
		_world = world;
		_numPointNodes = numPointNodes;
		_loopThread = std::thread(&PathManager::calculatePath, this, pointNodes);
	}
}

void PathManager::addPath(t_coord startPos, t_coord endPos, Agent* target, AgentType agentType, bool draw, PathType type) {
	_mutex.lock();
	_pathQueue.push({ _world, startPos, endPos, target, draw, type, agentType });
	_mutex.unlock();
}

void PathManager::applyDoor(Door* door)
{
	std::pair<MathLib::Vec2, MathLib::Vec2> fromTo = door->getCoord();
	fromTo.first /= 8;
	fromTo.second /= 8;
	for (uint16_t y = fromTo.first.y(); y < fromTo.second.y(); y++) {
		for (uint16_t x = fromTo.first.x(); x < fromTo.second.x(); x++) {
			if (_costs[x][y] != NodeType::Unavailable) {
				_costs[x][y] = door->isOpen() ? NodeType::DoorOpen : NodeType::DoorClosed;
			}
		}
	}
	if (_matrixMade) {

		for (int i = 0; i < _numPointNodes; i++) {
			for (int j = i; j < _numPointNodes; j++) {
				PointConnection* pc = &_pointNodesMatrix[i * _numPointNodes + j];
				if (pc && std::count(pc->zones.begin(), pc->zones.end(), Zone::Door)) {
					for (uint16_t y = fromTo.first.y(); y < fromTo.second.y(); y++) {
						for (uint16_t x = fromTo.first.x(); x < fromTo.second.x(); x++) {
							if (x == pc->path[1].x() / 8 && y == pc->path[1].y() / 8) {
								pc->distance = abs(pc->distance);
								_pointNodesMatrix[j * _numPointNodes + i].distance = abs(_pointNodesMatrix[j * _numPointNodes + i].distance);
								if (!door->isOpen()) {
									pc->distance *= -1;
									_pointNodesMatrix[j * _numPointNodes + i].distance *= -1;;
								}
								printPointNodeMatrix();
								return;
							}
						}
					}
				}
			}
		}
	}
}

void PathManager::calculatePath(PointNode* pointNodes)
{
	calculatePointNodeMatrix(pointNodes);
	printPointNodeMatrix();
	while (true) {
		if (!_pathQueue.empty()) {
			//std::cout << _pathQueue.size() << std::endl;
			if (_matrixMade || !_pathQueue.front().isPointPath()) {
				_pathQueue.front().init(_costs, _pointNodesMatrix, _numPointNodes);
				_pathQueue.front().findPath();
				_pathQueue.pop();
			}
		}
	}
}

void PathManager::calculatePointNodeMatrix(PointNode* pointNodes)
{
	_pointNodesMatrix = new PointConnection[_numPointNodes * _numPointNodes];
	for (int i = 0; i < _numPointNodes; i++) {
		for (int j = i; j < _numPointNodes; j++) {
			if (i == j) {
				_pointNodesMatrix[i * _numPointNodes + j] = {};
				_pointNodesMatrix[j * _numPointNodes + i] = {};
			}
			else {
				if (pointNodes[i].doorConnections.distance != 0 && pointNodes[j].doorConnections.distance != 0
					&& pointNodes[i].doorConnections.pos.x == pointNodes[j].doorConnections.pos.y
						&& pointNodes[i].doorConnections.pos.y == pointNodes[j].doorConnections.pos.x) {
					_pointNodesMatrix[i * _numPointNodes + j] = pointNodes[i].doorConnections;
					_pointNodesMatrix[j * _numPointNodes + i] = pointNodes[j].doorConnections;
				}
				else {
					std::vector<Zone> z1 = pointNodes[i].zones;
					std::vector<Zone> z2 = pointNodes[j].zones;
					for (size_t ii = 0; ii < z1.size(); ii++) {
						for (size_t jj = 0; jj < z2.size(); jj++) {
							if (z1[ii] == z2[jj] && z1[ii] != Zone::Door) {
								PathFinding pf = {
									_world,
									{pointNodes[i].pos.x, pointNodes[i].pos.y},
									{pointNodes[j].pos.x, pointNodes[j].pos.y},
									nullptr, 
									false, 
									PathType::Dijkstra, 
									AgentType::NoAgent 
								};

								pf.init(_costs, _pointNodesMatrix, _numPointNodes);
								pf.findPath();
								Path path = pf.getPath();
								uint16_t distance = path.path.size();
								const uint16_t xDist = abs(pointNodes[i].pos.x - pointNodes[j].pos.x);
								const uint16_t yDist = abs(pointNodes[i].pos.y - pointNodes[j].pos.y);
								const uint16_t h = xDist + yDist;

								_pointNodesMatrix[i * _numPointNodes + j] = {
									z1,
									z2,
									{i,j},
									pointNodes[i].pos,
									distance,
									h,
									path.path
								};
								std::reverse(path.path.begin(), path.path.end());

								_pointNodesMatrix[j * _numPointNodes + i] = {
									z2,
									z1,
									{j,i},
									pointNodes[j].pos,
									distance,
									h,
									path.path
								};
							}
						}
					}
				}
			}
		}
	}
	_matrixMade = true;
}

void PathManager::printPointNodeMatrix()
{
	if (_debug) {
		std::cout << std::endl << std::endl;

		std::cout << "  " << std::setfill('0');
		for (size_t i = 0; i < _numPointNodes; i++) {
			std::cout << "  " << std::setw(2) << i;
		}
		std::cout << std::endl;
		int color = 0;
		for (size_t i = 0; i < _numPointNodes; i++) {
			std::cout << std::setw(2) << i;
			for (size_t j = 0; j < _numPointNodes; j++) {
				if (_pointNodesMatrix[i * _numPointNodes + j].distance > 0) {
					if (color != 1) {
						std::cout << "\033[1;32m";
						color = 1;
					}
				}
				else if (_pointNodesMatrix[i * _numPointNodes + j].distance < 0) {
					if (color != 0) {
						std::cout << "\033[1;31m";
						color = 0;
					}
				}
				else {
					if (color != -1) {
						std::cout << "\033[0;0m";
						color = -1;
					}
				}
				if (i != j) {
					std::cout << " " << std::setw(3) << abs(_pointNodesMatrix[i * _numPointNodes + j].distance);
				}
				else {
					std::cout << "    ";
				}
			}
			std::cout << std::endl;
			std::cout << "\033[0;0m";
			color = 0;
		}
		std::cout << std::endl << std::endl;
	}
}
