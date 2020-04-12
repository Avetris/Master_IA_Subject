#ifndef __MIND_GUARD_H__
#define __MIND_GUARD_H__ 1

#include "ia/final/mind.h"

class MindGuard: public MindFinal {
    public:
        MindGuard(Agent* agent, Body* body) : MindFinal(agent, body) {}
        ~MindGuard() {};

        void update(uint32_t dt) final;

        bool receiveMessage() final;

    private:
        void checkNearSlave();
        void checkWaiting();
        void checkPatroling();
        void checkFindingDoor();
        void findPatrol();
        void findDoor();
};

#endif