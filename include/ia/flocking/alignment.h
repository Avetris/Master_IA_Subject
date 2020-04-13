#ifndef __ALIGNMENT_H__
#define __ALIGNMENT_H__ 1

#include <vector>

class Alignment{
    public:
        Alignment() {}
        virtual ~Alignment() {}
        
        Steering calculate(const KinematicStatus& character, const std::vector<const KinematicStatus*> flocking)  {
            Steering _steering;
            if (flocking.size() > 0) {
                for (const KinematicStatus* kinematic : flocking) {
                    _steering.velocity_linear.addEq(kinematic->velocity);
                }
                _steering.velocity_linear.divEq(flocking.size());
                if(_steering.velocity_linear.x() > 0 || _steering.velocity_linear.y() > 0)
                    _steering.velocity_linear = _steering.velocity_linear.normalized();
            }
            return _steering;
        }
};
#endif