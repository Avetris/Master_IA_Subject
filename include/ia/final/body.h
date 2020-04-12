
#ifndef __BODY_FINAL_H__
#define __BODY_FINAL_H__ 1

#include "engine/sprite.h"
#include "ia/body.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"
#include <ia\final\world.h>

class Agent;

class BodyFinal : public Body {
public:

    BodyFinal(World* world, Color color);
    ~BodyFinal() {};

    void update(uint32_t dt);

    void render() const;

    void setState(State state);

    void setTarget(Agent* target);
    void setTargetPath(Path* path);
private:
    void updatePath(uint32_t dt);
    void updateRandom(uint32_t dt);
    void updateWander(uint32_t dt);
    void updateAgainsTarget(uint32_t dt);
    void updateAutomatic(uint32_t dt, const Steering& steering);

    WorldFinal* _world;
    Sprite _sprite;
    Color _color;
    State _machineState;
    Path* _pathTarget = nullptr;
    Agent* _target = nullptr;    

    const float _max_speed = 100.0f;

    float _chagenDist;
};

#endif