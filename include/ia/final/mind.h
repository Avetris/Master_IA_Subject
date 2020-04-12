#ifndef __MIND_FINAL_H__
#define __MIND_FINAL_H__ 1

#include "ia/mind.h"
#include <ia/pathfinding/path_finding.h>

class MindFinal: public Mind {
    public:
        State _machineState[3] = { State::Idle };
        MindFinal(Agent* agent, Body* body) : Mind(agent, body) {}
        ~MindFinal() {
            while(_waitingPath) {}
        };

        void setTargetPath(Path path) final;

        virtual bool receiveMessage() = 0;

    protected:
        t_coord _target = { -1,-1 };
        bool _waitingPath = false;
        
        std::unique_ptr<Path> _nextTarget = nullptr;

        void askForPath(t_coord goal, AgentType type);

};

#endif