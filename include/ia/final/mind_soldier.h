#ifndef __MIND_SOLDIER_H__
#define __MIND_SOLDIER_H__ 1

#include "ia/final/mind.h"

class MindSoldier: public MindFinal {
    public:
        MindSoldier(Agent* agent, Body* body) : MindFinal(agent, body) {}
        ~MindSoldier() {};

        void update(uint32_t dt) final;

        bool receiveMessage() final;

    private:
        void checkWaiting();
        void checkFindingDoor();
        void checkActionDoor();
        void checkGoingBase();
        void findBase();
        void findDoor();
};

#endif