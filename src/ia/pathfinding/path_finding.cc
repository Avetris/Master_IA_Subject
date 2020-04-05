#include "ia/pathfinding/path_finding.h"
#include "engine/debug_draw.h"
#include "ia/defines.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <engine\ui_manager.h>
#include <ia\pathfinding\mind.h>
#include <ia\pathfinding\binary_heap.h>
#include <chrono>

PathFinding::PathFinding(MapWorld* world, t_coord startPos, t_coord endPos, Mind* target, bool draw, PathType type, AgentType agentType) 
    : _world(world), _target(target), _type(type), _draw(draw), _agentType(agentType){
    _startPos.x = round(startPos.x / 8);
    _startPos.y = round(startPos.y / 8);
    _goalPos.x = round(endPos.x / 8);
    _goalPos.y = round(endPos.y / 8);
}

void PathFinding::init(uint8_t map[MAP_L1_WIDTH][MAP_L1_HEIGHT], PointConnection* pointNodesMatrix, uint8_t numPointNodes) {
    for (uint8_t i = 0; i < MAP_L1_WIDTH; i++) {
        for (uint8_t j = 0; j < MAP_L1_HEIGHT; j++) {
            uint16_t cost = NodeType::Available;
            if (map[i][j] == NodeType::Unavailable || map[i][j] == NodeType::DoorClosed) {
                cost = NodeType::Unavailable;
            }
            else if ((_agentType == AgentType::Guard || _agentType == AgentType::Soldier )&& map[i][j] == NodeType::DoorOpen) {
                cost = NodeType::Unavailable;
            }
            _nodes[i][j] = {
                0,
                {i, j},
                {i, j},
                cost,
                0,
                0
            };
        }
    }
    _numPointNodes = numPointNodes;
    if (_type == PathType::A_Pre_Manhattan) {
        _startZone = _world->getZoneMap(_startPos.x, _startPos.y);
        _goalZone = _world->getZoneMap(_goalPos.x, _goalPos.y);

        _pointNodesMatrix = new PathPointConnection[numPointNodes * numPointNodes];
        _pointNodes = new std::vector<uint8_t>[numPointNodes];
        int same = -1;
        for (uint8_t i = 0; i < numPointNodes; i++) {
            for (uint8_t j = 0; j < numPointNodes; j++) {
                uint16_t pos = i * numPointNodes + j;
                _pointNodesMatrix[pos] = {};
                _pointNodesMatrix[pos].pos = { i,j };
                _pointNodesMatrix[pos].startPos = {
                    (int)pointNodesMatrix[pos].world_post.x,
                    (int)pointNodesMatrix[pos].world_post.y
                };
                _pointNodesMatrix[pos].zones = pointNodesMatrix[pos].zones;
                _pointNodesMatrix[pos].nextZones = pointNodesMatrix[pos].nextZones;
                _pointNodesMatrix[pos].G = pointNodesMatrix[pos].distance;
                _pointNodesMatrix[pos].H = pointNodesMatrix[pos].H;
                _pointNodesMatrix[pos].path = pointNodesMatrix[pos].path;

                if ((_agentType == AgentType::Guard || _agentType == AgentType::Soldier)&& std::count(pointNodesMatrix[pos].zones.begin(), pointNodesMatrix[pos].zones.end(), Zone::Door)) {
                    _pointNodesMatrix[pos].G = -1 * abs(pointNodesMatrix[pos].distance);
                }
                else {
                    if (std::count(pointNodesMatrix[pos].zones.begin(), pointNodesMatrix[pos].zones.end(), _startZone)) {
                        if (std::count(pointNodesMatrix[pos].zones.begin(), pointNodesMatrix[pos].zones.end(), _goalZone)) {
                            _bestPointNode = &_pointNodesMatrix[pos];
                            return;
                        }
                        else  if (!std::count(_startPoints.begin(), _startPoints.end(), i)) {
                            _startPoints.push_back(i);
                        }
                        if (_pointNodesMatrix[pos].startPos == _startPos) {
                            same = i;
                        }
                    }
                    if (_pointNodesMatrix[pos].G > 0) {
                        _pointNodes[i].push_back(j);
                    }
                }
            }
        }
        if (same > -1) {
            _startPoints.clear();
            _startPoints.push_back(same);
        }
    }

}

void PathFinding::findPath() {
    auto firstTime = std::chrono::high_resolution_clock::now();
    auto finishTime = std::chrono::high_resolution_clock::now();
    std::vector<Vec2> path;
    if (_type == PathType::A_Pre_Manhattan) {
        t_coord st = _startPos;
        t_coord gl = _goalPos;
        bool sameZone = _startZone == _goalZone;
        if (!sameZone) {
            path = findPointPath();
        }

        if (path.size() > 0 || sameZone) {
            if (sameZone || (st.x != path[0].x() / 8 && st.y != path[0].y() / 8)) {
                if (!sameZone) {
                    _startPos = st;
                    _goalPos = { (int)path[0].x() / 8, (int)path[0].y() / 8 };
                }
                findAlgorithm();
                while (_bestNode) {
                    path.insert(path.begin(), { (float)_bestNode->position.x * 8, (float)_bestNode->position.y * 8 });
                    if (_bestNode->position == _startPos) {
                        _bestNode = nullptr;
                    }
                    else {
                        _bestNode = &_nodes[_bestNode->parent.x][_bestNode->parent.y];
                    }
                }
                _bestNode = nullptr;
            }
            if (!sameZone && gl.x != path.back().x() / 8 && gl.y != path.back().y() / 8) {
                _startPos = { (int) path.back().x() / 8, (int) path.back().y() / 8 };
                _goalPos = gl;
                findAlgorithm();
                std::vector<Vec2> path2;
                while (_bestNode) {
                    path2.insert(path2.begin(), { (float)_bestNode->position.x * 8, (float)_bestNode->position.y * 8 });
                    if (_bestNode->position == _startPos) {
                        _bestNode = nullptr;
                    }
                    else {
                        _bestNode = &_nodes[_bestNode->parent.x][_bestNode->parent.y];
                    }
                }
                path.insert(path.end(), path2.begin(), path2.end());

            }
        }
        finishTime = std::chrono::high_resolution_clock::now();
    }
    else {
        findAlgorithm();
        while (_bestNode) {
            path.insert(path.begin(), { (float)_bestNode->position.x * 8, (float)_bestNode->position.y * 8 });
            if (_bestNode->position == _startPos) {
                _bestNode = nullptr;
            }
            else {
                _bestNode = &_nodes[_bestNode->parent.x][_bestNode->parent.y];
            }
        }

        finishTime = std::chrono::high_resolution_clock::now();
    }

    
    long currentTime = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - firstTime).count();

    if (_draw) {
        UIManager::instance().setPath(path, currentTime / 1000.0f );
    }
    _pathFound = {
        path.size() >= 2,
        0,
        path,
        _draw
    };
    if(_target)
        static_cast<MindPathFinding*>(_target)->setTargetPath(_pathFound);
}

void PathFinding::findAlgorithm()
{
    BinaryHeap<PathNode*, PathNode> openList;
    //std::priority_queue<PathNode*, std::vector<PathNode*>, PathNode> openList;
    openList.push(&_nodes[_startPos.x][_startPos.y]);
    _bestNode = nullptr;
    while (openList.size() > 0) {
        PathNode* closingVertex = openList.extractMin();
        openList.deleteMin();
        /*PathNode* closingVertex = openList.top();
        openList.pop();*/
        closingVertex->isOpenClosed = 2;
        t_coord parent = closingVertex->position;
        PathNode* nodes[8] = { nullptr };
        if (getNearNodes(closingVertex->position, nodes)) {
            for (int pos = 0; pos < 8; pos++) {
                if (nodes[pos]) {
                    uint32_t cost = getCost(closingVertex->G, nodes[pos]);
                    if (nodes[pos]->F == 0 || nodes[pos]->F > cost) {
                        nodes[pos]->F = cost;
                        nodes[pos]->parent = closingVertex->position;
                        if (_goalPos == nodes[pos]->position) {
                            if (_bestNode == nullptr || nodes[pos]->F < _bestNode->F) {
                                _bestNode = nodes[pos];
                                if (_type != PathType::Dijkstra) {
                                    return;
                                }
                            }
                        }
                        else {
                            if (nodes[pos]->isOpenClosed == 0) {
                                nodes[pos]->isOpenClosed = 1;
                                openList.push(nodes[pos]);
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<Vec2> PathFinding::findPointPath()
{
    std::vector<Vec2> path;
    if (_bestPointNode != nullptr) {
        path.push_back({ (float) _bestPointNode->startPos.x * 8, (float) _bestPointNode->startPos.y * 8});
    }
    else {
        BinaryHeap<PathPointConnection*, PathPointConnection> openList;
        for (uint8_t i : _startPoints) {
            for (uint8_t j : _pointNodes[i]) {
                if (_pointNodesMatrix[i * _numPointNodes + j].G > 0 && 
                    !std::count(_startPoints.begin(), _startPoints.end(), j)) {
                    _pointNodesMatrix[i * _numPointNodes + j].first = true;
                    _pointNodesMatrix[i * _numPointNodes + j].isOpenClosed = 1;
                    _pointNodesMatrix[j * _numPointNodes + i].isOpenClosed = 1;

                    const uint16_t xDist = abs(_startPos.x- _pointNodesMatrix[i * _numPointNodes + j].startPos.x);
                    const uint16_t yDist = abs(_startPos.y - _pointNodesMatrix[i * _numPointNodes + j].startPos.y);
                    _pointNodesMatrix[i * _numPointNodes + j].F = xDist + yDist;

                    openList.push(&_pointNodesMatrix[i * _numPointNodes + j]);
                }
            }
        }
        while (openList.size() > 0 && !_bestPointNode) {
            PathPointConnection* closingVertex = openList.extractMin();
            openList.deleteMin();
            if (std::count(closingVertex->nextZones.begin(), closingVertex->nextZones.end(), _goalZone)) {
                _bestPointNode = closingVertex;
                break;
            }
            closingVertex->isOpenClosed = 2;
            _pointNodesMatrix[closingVertex->pos.y * _numPointNodes + closingVertex->pos.x].isOpenClosed = 2;
            for (uint8_t j : _pointNodes[closingVertex->pos.y]) {
                if (closingVertex->pos.y != j) {
                    PathPointConnection* ppc = &_pointNodesMatrix[closingVertex->pos.y * _numPointNodes + j];
                    uint16_t cost = closingVertex->F + ppc->G +ppc->H;
                    if (ppc->G > 0 && ppc->isOpenClosed != 1 && (ppc->F == 0 || ppc->F > cost)) {
                        ppc->F = cost;
                        ppc->parent = { closingVertex->pos.x, closingVertex->pos.y };
                        if (std::count(ppc->nextZones.begin(), ppc->nextZones.end(), _goalZone)) {
                            _bestPointNode = ppc;
                        }
                        else {
                            if (ppc->isOpenClosed == 0) {
                                ppc->isOpenClosed = 1;
                                _pointNodesMatrix[ppc->pos.y * _numPointNodes + ppc->pos.x].isOpenClosed = 1;
                                openList.push(ppc);
                            }
                        }
                    }
                }
            }
        }
        if (_bestPointNode) {
            path.insert(path.begin(), _bestPointNode->path.begin(), _bestPointNode->path.end());
            while (!_bestPointNode->first) {
                _bestPointNode = &_pointNodesMatrix[_bestPointNode->parent.x * _numPointNodes + _bestPointNode->parent.y];
                path.insert(path.begin(), _bestPointNode->path.begin(), _bestPointNode->path.end());
            }
        }
    }
    return path;
}

bool PathFinding::getNearNodes(t_coord parent, PathNode** nodeArray)
{
    bool enter = false;
    int pos = 0;
    for (uint8_t x = parent.x - 1; x < parent.x + 2; x++) {
        for (uint8_t y = parent.y - 1; y < parent.y + 2; y++) {
            t_coord node = { x,y };
            if(node != parent){
                if (node != _startPos &&
                    node.x >= 0 && node.x < MAP_L1_WIDTH &&
                    node.y >= 0 && node.y < MAP_L1_HEIGHT &&
                    !_nodes[node.x][node.y].isOpenClosed != 2 &&
                    _nodes[node.x][node.y].G != NodeType::Unavailable) {
                    bool diagonalEnter = false;
                    if (node.x != parent.x && node.y != parent.y) {
                        if (node.x < parent.x && node.y < parent.y) {
                            if (_nodes[node.x + 1][node.y].G != NodeType::Unavailable && _nodes[node.x][node.y + 1].G != NodeType::Unavailable) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x > parent.x&& node.y > parent.y) {
                            if (_nodes[node.x - 1][node.y].G != NodeType::Unavailable && _nodes[node.x][node.y - 1].G != NodeType::Unavailable) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x < parent.x && node.y > parent.y) {
                            if (_nodes[node.x + 1][node.y].G != NodeType::Unavailable && _nodes[node.x][node.y - 1].G != NodeType::Unavailable) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x > parent.x&& node.y < parent.y) {
                            if (_nodes[node.x - 1][node.y].G != NodeType::Unavailable && _nodes[node.x][node.y + 1].G != NodeType::Unavailable) {
                                diagonalEnter = true;
                            }
                        }
                        if (diagonalEnter) {
                            enter = true;
                            nodeArray[pos] = &_nodes[node.x][node.y];
                            nodeArray[pos]->G = NodeCost::Diagonal;
                        }
                    }
                    else {
                        enter = true;
                        nodeArray[pos] = &_nodes[node.x][node.y];
                        nodeArray[pos]->G = NodeCost::Normal;
                    }
                }
                pos++;
            }
        }
    }
    return enter;
}

uint32_t PathFinding::getCost(uint32_t parentCost, PathNode* node)
{
    node->G = parentCost + node->G;
    if (_type == PathType::Dijkstra) {
        return parentCost + node->G;
    }
    else {
        if (node->H == 0) {
            getHeuristicCost(node);
        }
        return parentCost + node->G + node->H;
    }
}

void PathFinding::getHeuristicCost(PathNode* node)
{
    uint16_t distance = 0;
    const uint16_t xDist = abs(node->position.x - _goalPos.x);
    const uint16_t yDist = abs(node->position.y - _goalPos.y);
    switch (_type)
    {
    case PathType::A_Manhattan:
    case PathType::A_Pre_Manhattan:
        distance = NodeCost::Normal * (xDist + yDist);
        break;
    case PathType::A_Diagonal:
        distance = (NodeCost::Normal * (xDist + yDist)) + ((NodeCost::Diagonal - (NodeCost::Normal * 2)) * std::min(xDist, yDist));
        break;
    case PathType::A_Euclidean:
        distance = NodeCost::Normal * sqrt((xDist * xDist) + (yDist * yDist));
        break;    
    default:
        break;
    }
    node->H = distance;
}

void PathFinding::printNodeMatrixG()
{
    std::ofstream myfile;
    myfile.open("../pathResult.txt");    
    if (myfile.is_open()) {
        myfile << "PRUEBA TEXTO" << std::endl;
        myfile << "   || ";
        for (int i = 0; i < 3; i++) {
            for (int x = 0; x < MAP_L1_WIDTH; x++) {
                if (i == 0) {
                    if (x < 10) {
                        myfile << "  ";
                    }
                    else if (x < 100) {
                        myfile << " ";
                    }
                    myfile << "   " << x;
                    myfile << " | ";
                }else{
                    if (x == 0) {
                        myfile << "    ";
                    }
                    myfile << " _______ ";
                }
            }
            myfile << std::endl;
        }
        for (int y = 0; y < MAP_L1_HEIGHT; y++) {
            for (int x = 0; x < MAP_L1_WIDTH; x++) {
                if (x == 0) {
                    if (y < 10) {
                        myfile << "  ";
                    }else if (y < 100) {
                        myfile << " ";
                    }
                    myfile << y << " |";
                }
                myfile << "|    ";
                if (_nodes[x][y].position == _startPos) {
                    myfile << " STR";
                } else if (_nodes[x][y].position == _goalPos) {
                        myfile << " END";
                }
                else {
                    if (_nodes[x][y].F < 10) {
                        myfile << "   ";
                    }
                    else if (_nodes[x][y].F < 100) {
                        myfile << "  ";
                    }
                    else if (_nodes[x][y].F < 1000) {
                        myfile << " ";
                    }
                    myfile << _nodes[x][y].F;
                }
                myfile << " ";
            }
            myfile << "|" << std::endl;
            myfile << "    |";
            for (int x = 0; x < MAP_L1_WIDTH; x++) {
                myfile << "| ";
                if (_nodes[x][y].position.x < 10) {
                    myfile << "  ";
                }
                else if (_nodes[x][y].position.x < 100) {
                    myfile << " ";
                }
                myfile << _nodes[x][y].position.x << ",";
                if (_nodes[x][y].position.y < 10) {
                    myfile << "  ";
                }
                else if (_nodes[x][y].position.y < 100) {
                    myfile << " ";
                }
                myfile << _nodes[x][y].position.y << " ";
            }
            myfile << "|" << std::endl;
            myfile << "    |";
            for (int x = 0; x < MAP_L1_WIDTH; x++) {
                myfile << "| ";
                if (_nodes[x][y].parent.x < 10) {
                    myfile << "  ";
                }
                else if (_nodes[x][y].parent.x < 100) {
                    myfile << " ";
                }
                myfile << _nodes[x][y].parent.x << ",";
                if (_nodes[x][y].parent.y < 10) {
                    myfile << "  ";
                }
                else if (_nodes[x][y].parent.y < 100) {
                    myfile << " ";
                }
                myfile << _nodes[x][y].parent.y << " ";
            }
            myfile << "|" << std::endl;
            myfile << "    ||";
            for (int t = 0; t < MAP_L1_WIDTH; t++) {
                myfile << " _______  ";
            }
            myfile << std::endl;
        }
        myfile.close();
    }
}
