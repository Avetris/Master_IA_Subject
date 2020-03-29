
#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__ 1

#include <vector>
#include <ia\map.h>
#include <ia\agent.h>


class PathFinding {
  public:
    enum class Type {
      A_Manhattan,
      A_Diagonal,
      A_Euclidean,
      Dijstra,
    };

    PathFinding(t_coord startPos, t_coord _goalPos, Agent* target, Type type);
    ~PathFinding() {};

    void init(uint8_t map[MAP_L1_WIDTH][MAP_L1_HEIGHT]);
    void findPath();
    Path getPath() const { return _pathFound; }
  private:
      Type _type;
      t_coord _startPos, _goalPos;
      PathNode _nodes[MAP_L1_WIDTH][MAP_L1_HEIGHT];
      PathNode* _bestNode = nullptr;
      Agent* _target;
      
      Path _pathFound;
      float _timeLast = 0;

      void findAlgorithm();
      bool getNearNodes(t_coord parent, PathNode** nodeArray);
      uint32_t getCost(uint32_t parentCost, PathNode* node);
      void getHeuristicCost(PathNode* node);
      void printNodeMatrixG();

};

#endif