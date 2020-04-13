#ifndef __SEPARATION_H__
#define __SEPARATION_H__ 1

#include <vector>
#include <ia/movement/kinematic/kinematicflee.h>

class Separation {
public:
    Separation() {}
    virtual ~Separation() {}

    Steering calculate(const KinematicStatus& character, const std::vector<const KinematicStatus*> flocking) {
        Steering steering;
        if (flocking.size() > 0) {
            KinematicStatus status;
            for (const KinematicStatus* kinematic : flocking) {
                status.position.addEq(kinematic->position);
            }
            status.position.divEq(flocking.size());
            KinematicFlee flee;
            flee.calculate(character, &status, &steering);
            /*std::cout << status.position.x() << " " << status.position.y();
            std::cout << " " << character.position.x() << " " << character.position.y();
            std::cout << " " << steering.velocity_linear.x() << " " << steering.velocity_linear.y() << std::endl;*/
        }
        return steering;
    }
};
#endif