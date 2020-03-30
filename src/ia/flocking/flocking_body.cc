#include "ia/flocking/flocking_body.h"

void FlockingBody::init(uint16_t UID, const Color color, const Type type, MathLib::Vec2 position) {
  Body::init(UID, color, type, position);
  setSteering(SteeringMode::Seek);
  movement_->setMaxAcceleration(50.0f);
}

void FlockingBody::update(const uint32_t dt) {
    //KinematicStatus* targetStatus = target_ ? target_->getKinematic() : &kinmeticTarget_;
    KinematicStatus* targetStatus = &kinmeticTarget_;

    Steering steer;
    if (targetStatus && movement_ &&
        (targetStatus->position.x() > 0 && targetStatus->position.y() > 0)) {

        movement_.get()->calculate(state_, targetStatus, &steer);
    }

    steer.velocity_linear = (steer.velocity_linear * _seek_percent);
    steer.velocity_linear += (_alignment->getSteering()->velocity_linear * _alignment_percent);
    steer.velocity_linear += (_alignment->getSteering()->velocity_linear * _cohesion_percent);
    steer.velocity_linear += (_alignment->getSteering()->velocity_linear * _separation_percent);


    steer.rotation_angular = (steer.rotation_angular * _seek_percent);
    steer.rotation_angular += (_alignment->getSteering()->rotation_angular * _alignment_percent);
    steer.rotation_angular += (_alignment->getSteering()->rotation_angular * _cohesion_percent);
    steer.rotation_angular += (_alignment->getSteering()->rotation_angular * _separation_percent);

    updateAutomatic(dt, steer);
    if (state_.needsToOrientate) setOrientation(state_.velocity);

    sprite_.setPosition(state_.position.x(), state_.position.y());
    sprite_.setRotation(state_.orientation);
}