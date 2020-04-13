#ifndef __MIND_FLOCKING_H__
#define __MIND_FLOCKING_H__ 1

#include <ia\mind.h>
#include <ia\flocking\alignment.h>
#include <ia\flocking\cohesion.h>
#include <ia\flocking\separation.h>


class World;

class MindFlocking: public Mind {
    public:
        MindFlocking(Agent* agent, Body* body);
        ~MindFlocking() {};

        void update(uint32_t dt);
};

#endif