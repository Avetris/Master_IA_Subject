
#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__ 1

#include <vector>
#include <ia\pathfinding\map.h>
#include <ia\pathfinding\map_world.h>
#include <ia\agent.h>

class PathFinding {
  public:
    PathFinding(MapWorld* world, t_coord startPos, t_coord _goalPos, Mind* target, bool draw, PathType type, AgentType agentType);
    ~PathFinding() {
        if (_pointNodesMatrix != nullptr) {
            delete[] _pointNodesMatrix;
            delete[] _pointNodes;
        }
    };

    void init(uint8_t map[MAP_L1_WIDTH][MAP_L1_HEIGHT], PointConnection* pointNodesMatrix, uint8_t numPointNodes);
    void findPath();
    bool isPointPath() { return _type == PathType::A_Pre_Manhattan; }
    Path getPath() const { return _pathFound; }
  private:
      bool _draw = false;
      PathType _type;
      AgentType _agentType;
      t_coord _startPos, _goalPos;
      Zone _startZone, _goalZone;
      PathNode _nodes[MAP_L1_WIDTH][MAP_L1_HEIGHT];
      PathPointConnection* _pointNodesMatrix = nullptr;
      std::vector<uint8_t>* _pointNodes = nullptr;
      std::vector<uint16_t> _startPoints;
      uint8_t _numPointNodes;
      PathNode* _bestNode = nullptr;
      PathPointConnection* _bestPointNode = nullptr;
      Mind* _target = nullptr;
      MapWorld* _world = nullptr;
      
      
      Path _pathFound;
      float _timeLast = 0;

      void findAlgorithm();
      std::vector<Vec2> findPointPath();
      bool getNearNodes(t_coord parent, PathNode** nodeArray);
      uint32_t getCost(uint32_t parentCost, PathNode* node);
      void getHeuristicCost(PathNode* node);
      void printNodeMatrixG();

};

#endif