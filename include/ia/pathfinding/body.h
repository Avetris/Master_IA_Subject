#ifndef __BODY_PATHFINDING_H__
#define __BODY_PATHFINDING_H__ 1

#include "engine/sprite.h"
#include "ia/body.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"

class Agent;

class BodyPathFinding : public Body {
public:

    BodyPathFinding(Color color);
    ~BodyPathFinding() {};

    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    void setTargetPath(Path* path);
private:
    void updateAutomatic(uint32_t dt, const Steering& steering);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();

    Sprite _sprite;
    Color _color;
    Path* _pathTarget = nullptr;
    Agent* _target = nullptr;

    const float _max_speed = 100.0f;

    struct {
        struct {
            MathLib::Vec2 pos;
            MathLib::Vec2 v;
        } green, red, blue;
    } dd;

    float _chagenDist;
};

#endif