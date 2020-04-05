//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __IA_DEFINES_H__
#define __IA_DEFINES_H__ 1

#include "mathlib/vec2.h"
#include <vector>

#define AGENT_BLUE_PATH "../assets/images/agent_blue.png"
#define AGENT_RED_PATH "../assets/images/agent_red.png"
#define AGENT_GREEN_PATH "../assets/images/agent_green.png"
#define AGENT_PURPLE_PATH "../assets/images/agent_purple.png"

#define MAP_PATH "../assets/images/mapa.bmp"
#define ZONES_PATH "../assets/images/zonas_min.bmp"

enum class Zone {
    Wall = 0,
    Door = 255000000,
    RestZone = 255000,
    OutdoorZone1 = 255,
    OutdoorZone2 = 240,
    OutdoorZone3 = 230,
    OutdoorZone4 = 220,
    OutdoorZone5 = 210,
    OutdoorZone6 = 200,
    OutdoorZone7 = 190,
    OutdoorZone8 = 180,
    OutdoorZone9 = 170,
    OutdoorZone10 = 160,
    OutdoorZone11 = 150,
    OutdoorZone12 = 140,
    FortressZone1 = 255240,
    FortressZone2 = 255230,
    FortressZone3 = 255220,
    FortressZone4 = 255210,
    FortressZone5 = 255200,
    FortressZone6 = 255190,
    FortressZone7 = 255180,
    FortressExitZone1 = 240255,
    FortressExitZone2 = 230255,
    FortressExitZone3 = 220255,
    FortressExitZone4 = 210255,
    FortressWorkZone1 = 240240,
    FortressWorkZone2 = 230230,
    FortressWorkZone3 = 220220,
    FortressWorkZone4 = 210210,
    FortressWorkZone5 = 200200,
    FortressWorkZone6 = 190190,
    FortressWorkZone7 = 180180,
    FortressWorkZone8 = 170170,
    FortressWorkZone9 = 160160,
    FortressWorkZone10 = 150150,
    FortressWorkZone11= 140140,
    FortressWorkZone12 = 130130,
    RestSpawn = 255125125,
    WorkStart = 255000255,
    WorkGoal = 125125125,
    Base = 255255000,
    PathPoint = 255255255
};


enum class Color {
  Green,
  Blue,
  Purple,
  Red,
};

enum class Type {
  Autonomous,
  Manual,
};

enum class BodyType {
  Steering,
  Pathfinding,
  Flocking
};

enum class MindType {
  Steering,
  Pathfinding,
  Flocking
};

enum class SteeringMode {
  Kinematic_Seek,         //1       Kinematics
  Kinematic_Flee,         //2
  Kinematic_Arrive,       //3
  Kinematic_Wander,       //4
  Seek,                   //q       Steering Basics
  Flee,                   //w
  Arrive,                 //e
  Align,                  //r
  Velocity_Matching,      //t
  Pursue,                 //a       Steering Delegated
  Face,                   //s
  LookGoing,              //d
  Wander,                 //f
};

enum class PathStatus{
    Found,
    Not_Found
};

enum class AgentType {
    NoAgent,
    Slave,
    Guard,
    Soldier
};

enum NodeType {
    Available = 1,
    Unavailable = 0,
    DoorOpen = 2,
    DoorClosed = 3
};

enum NodeCost {
    Normal = 10,
    Diagonal = 14
};

enum class PathType {
    A_Pre_Manhattan,
    A_Manhattan,
    A_Diagonal,
    A_Euclidean,
    Dijkstra
};

struct Steering {
  MathLib::Vec2 velocity_linear{ 0.0f, 0.0f };    //velocity / linear acceleration
  float rotation_angular{ 0.0f };                //rotation / angular acceleration
};

struct KinematicStatus {
  MathLib::Vec2 position{ 0.0f, 0.0f };
  float orientation {0.0f};
  bool needsToOrientate = true;
  MathLib::Vec2 velocity{ 0.0f, 0.0f };  //linear velocity
  float rotation{0.0f};               //angular velocity

  float speed{ 0.0f };
};

struct t_coord{
    int x;
    int y;

    bool operator==(t_coord& other)
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const t_coord& other)
    {
        return x != other.x || y != other.y;
    }
};


struct PathPointConnection {
    uint8_t isOpenClosed = 0; // 0 - non; 1 - Open; 2 - Closed
    bool first = false;
    t_coord pos;
    t_coord startPos;
    t_coord parent;
    std::vector<Zone> zones;
    std::vector<Zone> nextZones;
    int G = 0;
    int F = 0;
    int H = 0;
    std::vector<MathLib::Vec2> path;


    bool operator()(PathPointConnection*& lhs, PathPointConnection*& rhs)
    {
        return lhs->F > rhs->F;
    }
};


struct PointConnection {
    std::vector<Zone> zones;
    std::vector<Zone> nextZones;
    t_coord pos;
    t_coord world_post;
    int distance = 0;
    int H = 0;
    std::vector<MathLib::Vec2> path;
};

struct PointNode {
    t_coord pos;
    uint8_t uidTexture;
    PointConnection doorConnections;
    std::vector<Zone> zones;
};

struct Path {
    unsigned int pathFound = false;
    int index = 0;
    std::vector<MathLib::Vec2> path;
    bool draw = false;
};

struct PathNode{
    uint8_t isOpenClosed; // 0 - non; 1 - Open; 2 - Closed
    t_coord parent;
    t_coord position;
    uint16_t G = 0;
    uint32_t F = 0;
    uint16_t H = 0;

    bool operator()(PathNode*& lhs, PathNode*& rhs)
    {
        return lhs->F > rhs->F;
    }
};

#endif
