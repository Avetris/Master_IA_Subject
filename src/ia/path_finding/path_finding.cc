#include "ia/path_finding/path_finding.h"
#include "engine/debug_draw.h"
#include "ia/defines.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <engine\ui_manager.h>

PathFinding::PathFinding(t_coord startPos, t_coord endPos, Agent* target, bool draw, Type type) : _target(target), _type(type), _draw(draw) {
    _startPos.x = round(startPos.x / 8);
    _startPos.y = round(startPos.y / 8);
    _goalPos.x = round(endPos.x / 8);
    _goalPos.y = round(endPos.y / 8);
}

void PathFinding::init(uint8_t map[MAP_L1_WIDTH][MAP_L1_HEIGHT]) {
    for (uint8_t i = 0; i < MAP_L1_WIDTH; i++) {
        for (uint8_t j = 0; j < MAP_L1_HEIGHT; j++) {
            _nodes[i][j] = {
                0,
                {i, j},
                {i, j},
                (uint16_t) (map[i][j] == 1 ? 1 : 0),
                0,
                0
            };
        }
    }
}

void PathFinding::findPath() {
    Uint32 currentTime = SDL_GetTicks();
    findAlgorithm();

    currentTime = SDL_GetTicks() - currentTime;
    //printNodeMatrixG();
    std::vector<Vec2> path;
    while (_bestNode) {
        path.insert(path.begin(), { (float) _bestNode->position.x * 8, (float) _bestNode->position.y * 8 });
        if (_bestNode->position == _startPos) {
            _bestNode = nullptr;
        }
        else {
            _bestNode = &_nodes[_bestNode->parent.x][_bestNode->parent.y];
        }
    }
    if (_draw) {
        UIManager::instance().setPath(path);
    }
    _pathFound = {
        0,
        !path.empty(),
        0,
        path,
        currentTime / 1000.0f,
        _draw
    };
    if(_target)
        _target->setPath(_pathFound);
}

void PathFinding::findAlgorithm()
{
    std::priority_queue<PathNode*, std::vector<PathNode*>, PathNode> openList;
    openList.push(&_nodes[_startPos.x][_startPos.y]);
    _bestNode = nullptr;
    while (!openList.empty()) {
        PathNode* closingVertex = openList.top();
        openList.pop();
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
                                if (_type != Type::Dijstra) {
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
                    _nodes[node.x][node.y].G != 0) {
                    bool diagonalEnter = false;
                    if (node.x != parent.x && node.y != parent.y) {
                        if (node.x < parent.x && node.y < parent.y) {
                            if (_nodes[node.x + 1][node.y].G != 0 && _nodes[node.x][node.y + 1].G != 0) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x > parent.x&& node.y > parent.y) {
                            if (_nodes[node.x - 1][node.y].G != 0 && _nodes[node.x][node.y - 1].G != 0) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x < parent.x && node.y > parent.y) {
                            if (_nodes[node.x + 1][node.y].G != 0 && _nodes[node.x][node.y - 1].G != 0) {
                                diagonalEnter = true;
                            }
                        }
                        else if (node.x > parent.x&& node.y < parent.y) {
                            if (_nodes[node.x - 1][node.y].G != 0 && _nodes[node.x][node.y + 1].G != 0) {
                                diagonalEnter = true;
                            }
                        }
                        if (diagonalEnter) {
                            enter = true;
                            nodeArray[pos] = &_nodes[node.x][node.y];
                            nodeArray[pos]->G = NODE_COST::Diagonal;
                        }
                    }
                    else {
                        enter = true;
                        nodeArray[pos] = &_nodes[node.x][node.y];
                        nodeArray[pos]->G = NODE_COST::Normal;
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
    if (_type == Type::Dijstra) {
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
    case Type::A_Manhattan:
        distance = NODE_COST::Normal * (xDist + yDist);
        break;
    case Type::A_Diagonal:
        distance = (NODE_COST::Normal * (xDist + yDist)) + ((NODE_COST::Diagonal - (NODE_COST::Normal * 2)) * std::min(xDist, yDist));
        break;
    case Type::A_Euclidean:
        distance = NODE_COST::Normal * sqrt((xDist * xDist) + (yDist * yDist));
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


