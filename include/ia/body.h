//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __BODY_H__
#define __BODY_H__ 1

#include "ia/defines.h"

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

  const KinematicStatus* getKinematic() const { return &state_; }
  KinematicStatus* getKinematic() { return &state_; }
protected:
  KinematicStatus state_;
};

#endif