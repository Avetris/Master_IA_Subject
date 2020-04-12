//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_STEERING_H__
#define __BODY_STEERING_H__ 1

#include "engine/sprite.h"
#include "ia/body.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"

class Agent;

class BodySteering: public Body {
  public:

    BodySteering(Color color, Type type);
    ~BodySteering() {};

    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
    
  private:
    void updateAutomatic(uint32_t dt, const Steering& steering);
    void updateManual(uint32_t dt);
    void setOrientation(const MathLib::Vec2& velocity);
    void keepInSpeed();
    void keepInBounds();

    Sprite _sprite;
    Type _type;
    Color _color;
    Agent* _target;

    const float _max_speed = 100.0f;

    struct {
      struct {
        MathLib::Vec2 pos;
        MathLib::Vec2 v;
      } green, red, blue;
    } dd;
};

#endif