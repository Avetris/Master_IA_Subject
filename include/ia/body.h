//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include "ia/defines.h"
#include "ia/movement/movement.h"

class Body {
public:
  Body() = default;
  virtual ~Body() = default;

  Body(const Body&) = default;
  Body(Body&&) noexcept = default;
  Body& operator=(const Body&) = default;
  Body& operator=(Body&&) noexcept = default;

  virtual void update(uint32_t dt) = 0;
  virtual void render() const = 0;

  const KinematicStatus* getKinematic() const { return &_state; }
  KinematicStatus* getKinematic() { return &_state; }

  Movement* getMovement() const { return _movement.get(); }
protected:
  KinematicStatus _state;

  std::unique_ptr<Movement> _movement;
  bool _isKinematic = false;
};

#endif
