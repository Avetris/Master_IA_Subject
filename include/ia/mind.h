//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_H__
#define __MIND_H__ 1

#include <cstdint>
#include <ia/defines.h>
#include <ia\door.h>

class Body;
class World;

class Mind {
    public:
        Mind(){};
        ~Mind() {};
        uint16_t _UID;

        void init(uint16_t UID, World* world, Body* body);
        void render() const;
        void update(uint32_t dt);

        void setPath(Path pathFound);
        std::pair<bool, Path*> checkDoor(Door* door);
    private:
        float getDistance(MathLib::Vec2 origin, MathLib::Vec2 target);
    protected:
        World* world_ = nullptr;
        Body* body_ = nullptr;

        Path _pathFound;
        float _chagenDist = 0.0f;
};

#endif