#ifndef __MIND_PATHFINDING_H__
#define __MIND_PATHFINDING_H__ 1

#include "ia/mind.h"

class MindPathFinding: public Mind {
    public:
         MindPathFinding(World* world, Body* body);
        ~MindPathFinding() {};

        void update(uint32_t dt) final;

        void setTargetPath(Path path);

        void checkDoor(Door* door);

    private:
        std::unique_ptr<Path> _pathTarget;
        std::unique_ptr<Path> _nextTarget = nullptr;

};

#endif