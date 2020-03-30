#ifndef __ALIGNMENT_H__
#define __ALIGNMENT_H__ 1

#include <vector>
#include <ia\movement\steering\align.h>

class Alignment{
    public:
        Alignment() {}
        virtual ~Alignment() {}
        
        void calculate(const KinematicStatus& character, const std::vector<const KinematicStatus*> flocking)  {
            steering_.velocity_linear = { 0.0f, 0.0f };
            steering_.rotation_angular = 0.0f;
            Align align;
            for (const KinematicStatus* kinematic : flocking) {
                Steering steer;
                align.calculate(character, kinematic, &steer);
                steering_.velocity_linear.addEq(steer.velocity_linear);
                steering_.rotation_angular += steer.rotation_angular;
            }
        }

        Steering* getSteering() {
            return &steering_;
        }

    private:
        Steering steering_;

};
#endif