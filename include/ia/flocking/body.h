
#ifndef __FLOCKING_BODY_H__
#define __FLOCKING_BODY_H__ 1

#include <ia\body.h>
#include <ia\flocking\alignment.h>

class FlockingBody : public Body {
  public:
    FlockingBody() {};
    ~FlockingBody() {};

    void init(uint16_t UID, Color color, Type type, MathLib::Vec2 position);
    void update(uint32_t dt);

    void setAlignment(Alignment* alignment) { _alignment = alignment; };
    void setCohesion(Alignment* alignment) { _alignment = alignment; };
    void setSeparation(Alignment* alignment) { _alignment = alignment; };
  private:
      Alignment* _alignment = nullptr;

      float _seek_percent = 1.0f;
      float _alignment_percent = 0.4f;
      float _cohesion_percent = 0.05f;
      float _separation_percent = 0.05f;
};

#endif