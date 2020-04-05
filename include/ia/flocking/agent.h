
#ifndef __FLOCKING_AGENT_H__
#define __FLOCKING_AGENT_H__ 1

#include <ia\flocking\flocking_body.h>
#include <ia\flocking\flocking_mind.h>

#include <cstdint>

class FlockingAgent{
  public:
    FlockingAgent(){ _UID = _GUID++; };
    ~FlockingAgent() {};
    uint16_t _UID;

    FlockingAgent(const FlockingAgent&) {};

    void init(World* world, Body::Color color, Body::Type type, MathLib::Vec2 position) {
        world_ = world;
        body_.init(_UID, color, type, position);
        mind_.init(_UID, world, &body_);
    }

    void updateMind(const uint32_t dt, std::vector<const KinematicStatus*> around) { mind_.update(dt, around); };
    void updateBody(const uint32_t dt) { body_.update(dt); };
    void render() const { mind_.render(); body_.render(); }
    void shutdown() { world_ = nullptr;}

    void setSteering(Body::SteeringMode steering) { body_.setSteering(steering); }
    const KinematicStatus* getKinematic() const { return body_.getKinematic(); }
    KinematicStatus* getKinematic() { return body_.getKinematic(); }
    Body* getBody() { return &body_; }
    void setPath(Path pathFound){ mind_.setPath(pathFound); }
    void setTarget(Agent* target) { body_.setTarget(target); }
    void setTarget(MathLib::Vec2* target) { body_.setTarget(target); }

    private:
        static uint16_t _GUID;
        World* world_ = nullptr;

        FlockingBody body_;
        FlockingMind mind_;
};

inline uint16_t FlockingAgent::_GUID = 0;
#endif