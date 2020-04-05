#ifndef __MAP_WORLD_H__
#define __MAP_WORLD_H__ 1

#include "engine/defines.h"
#include "ia/defines.h"
#include "ia/world.h"
#include "mathlib/vec2.h"
#include <ia\door.h>
#include <unordered_map>

using MathLib::Vec2;

#define MAP_L2_WIDTH 128                //Size of map in tiles
#define MAP_L2_HEIGHT 128

class MapWorld: public World {
  public:
    MapWorld();
    ~MapWorld() {};
    
    virtual void render() const override;

    virtual void openDoor(uint8_t doorNumber) = 0;

    uint8_t getNumDoors() { return _numDoors; }

    void debug();

    std::vector<t_coord> getInterestPositions(const Zone zone) const;
    Zone getZoneMap(int x, int y) { return _map[x][y]; }
    Zone getZoneMap8(int x, int y) { return _map[x / 8][y / 8]; }


protected:
    Zone _map[MAP_L2_WIDTH][MAP_L2_HEIGHT];
    std::unordered_map<Zone, std::vector<t_coord>> _interestNodes;
    PointNode* _pointNodes;
    Door* _doors;
    uint8_t _numDoors = 0;
    uint8_t _numPointNodes = 0;
    short _debug = 0;
private:
    void readMap();
    void initializeDoors(std::vector<Door> doors, std::vector<PointNode>* nodes);
    void initializeZones(std::vector<PointNode> nodes);
};

#endif