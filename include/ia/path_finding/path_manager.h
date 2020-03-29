#ifndef __PATH_MANAGER_H__
#define __PATH_MANAGER_H__ 1

#include <ia\defines.h>
#include <ia\path_finding\path_finding.h>
#include <ia\map.h>

#include <SDL/SDL_ttf.h>
#include <thread>
#include <queue>
#include <mutex>
#include <ia\door.h>

class PathManager {
  public:
      static PathManager& instance() {
          static PathManager instance;
          return  instance;
      }
      void init();

      ~PathManager() { _loopThread.detach(); };

    void addPath(t_coord startPos, t_coord endPos, Agent* target, PathFinding::Type type);
    void applyDoor(Door* door);
  private:
    PathManager() {};
    void calculatePath();

    std::queue<PathFinding> _pathQueue;
    uint8_t _costs[MAP_L1_WIDTH][MAP_L1_HEIGHT];
    World* _world;

    std::thread _loopThread;
    std::mutex _mutex;

};

#endif