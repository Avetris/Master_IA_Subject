#ifndef __MIND_SLAVE_H__
#define __MIND_SLAVE_H__ 1

#include "ia/final/mind.h"

class MindSlave: public MindFinal {
    public:
        MindSlave(Agent* agent, Body* body) : MindFinal(agent, body) {}
        ~MindSlave() {};

        void update(uint32_t dt) final;

        bool receiveMessage() final;

    private:
        void checkArrested();
        void checkWaiting();
        void checkFindingDoor();
        void checkGoingBase();
        void checkGoingWork();
        void checkWorking();
        void checkWorkingBack();
        void checkGoingRest();
        void findBase();
        void findDoor();
        void goWork();
        void goRest();
        void work(bool go, int workPos = -1);

        bool _escaped = false;
};

#endif