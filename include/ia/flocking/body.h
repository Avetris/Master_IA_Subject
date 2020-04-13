
#ifndef __BODY_FLOCKING_H__
#define __BODY_FLOCKING_H__ 1

#include "engine/sprite.h"
#include <ia\body.h>
#include "ia/defines.h"
#include "mathlib/vec2.h"
#include <ia\flocking\alignment.h>
#include <ia\flocking\cohesion.h>
#include <ia\flocking\separation.h>

class Agent;

class BodyFlocking : public Body {
  public:
      BodyFlocking(Agent* agent, const Color color);
    ~BodyFlocking() {};

    void update(uint32_t dt);
    void render() const;

    void setTarget(Agent* target);
  private:
      void updateAutomatic(uint32_t dt, const Steering& steering);
      void updateManual(uint32_t dt);
      void setOrientation(const MathLib::Vec2& velocity);
      void keepInSpeed();
      void keepInBounds();

      Agent* _agent;

      Alignment _alignment;
      Cohesion _cohesion;
      Separation _separation;

      const float _seek_percent = 0.5f;
      const float _alignment_percent = 0.2f;
      const float _cohesion_percent = 0.1f;
      const float _separation_percent = 0.2f;

      Sprite _sprite;
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