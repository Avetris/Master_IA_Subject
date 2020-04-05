#ifndef __PATH_MANAGER_H__
#define __PATH_MANAGER_H__ 1

#include <ia\defines.h>
#include <ia\pathfinding\map.h>
#include <ia\pathfinding\map_world.h>

#include <SDL/SDL_ttf.h>
#include <thread>
#include <queue>
#include <mutex>
#include <ia\door.h>
#include <ia\pathfinding\path_finding.h>

class PathManager {
  public:
      static PathManager& instance() {
          static PathManager instance;
          return  instance;
      }
      void init(MapWorld* world, Zone zones[MAP_L2_WIDTH][MAP_L2_WIDTH], PointNode* pointNodes, uint8_t numPointNodes);

      ~PathManager() { _loopThread.detach(); };

    void addPath(t_coord startPos, t_coord endPos, Mind* target, AgentType agentType, bool draw = false, PathType type = PathType::A_Pre_Manhattan);
    void applyDoor(Door* door);
    void debug(bool d) { _debug = d; }
  private:
    std::queue<PathFinding> _pathQueue;
    uint8_t _costs[MAP_L1_WIDTH][MAP_L1_HEIGHT];
    Zone* _zones[MAP_L2_WIDTH][MAP_L2_WIDTH];
    PointConnection* _pointNodesMatrix;
    uint8_t _numPointNodes;
    MapWorld* _world;
    bool _matrixMade = false;

    PathManager() {};
    void calculatePath(PointNode* pointNodes);
    void calculatePointNodeMatrix(PointNode* pointNodes);
    void printPointNodeMatrix();

    bool _debug = false;
    std::thread _loopThread;
    std::mutex _mutex;

};

#endif