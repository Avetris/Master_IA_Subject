#ifndef __FLOCKING_MIND_H__
#define __FLOCKING_MIND_H__ 1

#include <ia\mind.h>
#include <ia\flocking\alignment.h>

class FlockingBody;
class World;

class FlockingMind: public Mind {
    public:
        FlockingMind() {};
        ~FlockingMind() {};

        void update(uint32_t dt, std::vector<const KinematicStatus*> around){
            _alignment.calculate(*body_->getKinematic(), around);

            /*   _alignment.calculate(*body_->getKinematic(), around);
               _alignment.calculate(*body_->getKinematic(), around);*/

            static_cast<FlockingBody*>(body_)->setAlignment(&_alignment);
            /* body->setCohesion(&_alignment);
             body->setSeparation(&_alignment);*/
        }
    private:
        Alignment _alignment;
};

#endif