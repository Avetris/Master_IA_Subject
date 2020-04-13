#ifndef __COHESION_H__
#define __COHESION_H__ 1

#include <vector>
#include <ia/movement/kinematic/kinematicseek.h>

class Cohesion {
public:
    Cohesion() {}
    virtual ~Cohesion() {}

    Steering calculate(const KinematicStatus& character, const std::vector<const KinematicStatus*> flocking) {
        Steering steering;
        if (flocking.size() > 0) {
            KinematicStatus status;
            for (const KinematicStatus* kinematic : flocking) {
                status.position.addEq(kinematic->position);
            }
            status.position.divEq(flocking.size());
            KinematicSeek seek;
            seek.calculate(character, &status, &steering);
        }
        return steering;
    }
};
#endif