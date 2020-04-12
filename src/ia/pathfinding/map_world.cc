#include <ia/pathfinding/map_world.h>
#include <ia\pathfinding\path_manager.h>
#include <SDL/SDL_image.h>
#include <algorithm> 
#include <engine\ui_manager.h>
#include <string>

MapWorld::MapWorld() {
    readMap();
    PathManager::instance().init(this, _map, _pointNodes, _numPointNodes);
    for (int i = 0; i < 4; i++) {
        PathManager::instance().applyDoor(&_doors[i]);
    }
}

void MapWorld::render() const {
    for (size_t i = 0; i < _numDoors; i++) {
        _doors[i].render();
    }
    SDL_Renderer* renderer = Window::instance().getRenderer();
    if (_debug > 1) {
        for (size_t i = 0; i < MAP_L2_WIDTH; i++) {
            for (size_t j = 0; j < MAP_L2_HEIGHT; j++) {
                int b = (int)_map[i][j] % 1000;
                int g = (int)_map[i][j] / 1000;
                int r = g / 1000;
                g = g % 1000;
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_Rect rect;
                rect.x = i * 8;
                rect.y = j * 8;
                rect.w = 8;
                rect.h = 8;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        UIManager::instance().renderPath();
    }
    if (_debug == 2) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t i = 0; i < _numPointNodes; i++) {
            SDL_Rect rect;
            rect.x = _pointNodes[i].pos.x * 8;
            rect.y = _pointNodes[i].pos.y * 8;
            rect.w = 8;
            rect.h = 8;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void MapWorld::debug()
{
  _debug++; 
  if (_debug > 3) _debug = 0;
  if (_debug == 1 || _debug == 2) {
      for (size_t i = 0; i < _numPointNodes; i++) {
          UIManager::instance().setVisible(_pointNodes[i].uidTexture, true);
      }
  }
  else {
      for (size_t i = 0; i < _numPointNodes; i++) {
          UIManager::instance().setVisible(_pointNodes[i].uidTexture, false);
      }
  }

  PathManager::instance().debug(_debug != 0);
}

std::vector<t_coord> MapWorld::getInterestPositions(const Zone zone) const
{
    if (_interestNodes.size() > 0) {
        auto it = _interestNodes.find(zone);
        if (it != _interestNodes.end()) {
            return it->second;
        }
    }
    return std::vector<t_coord>();
}

void MapWorld::readMap()
{
    SDL_Surface* map_image;
    if ((map_image = SDL_LoadBMP(ZONES_PATH)) == false)
        return;
    SDL_LockSurface(map_image);
    {
        Uint32* pixels = (Uint32*) map_image->pixels;

        SDL_UnlockSurface(map_image);

        std::vector<Door> doors;
        std::vector<PointNode> nodes;
        std::vector<t_coord> d_pos;

        for (int i = 0; i < MAP_L2_HEIGHT; ++i) {
            for (int j = 0; j < MAP_L2_WIDTH; ++j) {
                SDL_Color rgb;
                SDL_GetRGB(*pixels, map_image->format, &rgb.r, &rgb.g, &rgb.b);
                int32_t sum = (rgb.r * 1000000) + (rgb.g * 1000) + rgb.b;
                _map[j][i] = static_cast<Zone>(sum);
                switch (_map[j][i])
                {
                case Zone::Door:
                {
                    if (d_pos.empty()) {
                        d_pos.push_back({ j, i });
                    }
                    else {
                        t_coord n = { abs(d_pos.back().x) - j, abs(d_pos.back().y - i) };
                        if (n.x > 1 || n.y > 1) {
                            Door d;
                            MathLib::Vec2 c1 = { (float)d_pos[0].x * 8, (float)d_pos[0].y * 8 };
                            MathLib::Vec2 c2 = { (float)d_pos.back().x * 8 + 8, (float)d_pos.back().y * 8 + 8 };
                            d.init(c1, c2);
                            doors.push_back(d);
                            d_pos.clear();
                            d_pos.push_back({ j, i });
                        }
                        else {
                            d_pos.push_back({ j, i });
                        }
                    }
                    std::vector<t_coord> vec;
                    auto it = _interestNodes.find(Zone::Door);
                    if (it != _interestNodes.end()) {
                        vec = it->second;
                    }
                    vec.push_back({ j,i });
                    _interestNodes[Zone::Door] = vec;
                    break;
                }
                case Zone::PathPoint:
                case Zone::Base:
                case Zone::WorkStart:
                case Zone::WorkGoal:
                case Zone::RestSpawn:
                    nodes.push_back({ {j,i} });
                    if (_map[j][i] != Zone::PathPoint) {
                        std::vector<t_coord> vec;
                        auto it = _interestNodes.find(_map[j][i]);
                        if (it != _interestNodes.end()) {
                            vec = it->second;
                        }
                        vec.push_back({ j,i });
                        _interestNodes[_map[j][i]] = vec;
                    }
                    break;
                case Zone::RestZone:
                {
                    std::vector<t_coord> vec;
                    auto it = _interestNodes.find(Zone::RestZone);
                    if (it != _interestNodes.end()) {
                        vec = it->second;
                    }
                    vec.push_back({ j,i });
                    _interestNodes[Zone::RestZone] = vec;
                    break;
                }
                case Zone::OutdoorZone5:
                case Zone::OutdoorZone7:
                case Zone::OutdoorZone9:
                case Zone::OutdoorZone11:
                {
                    std::vector<t_coord> vec;
                    auto it = _interestNodes.find(Zone::OutdoorZone);
                    if (it != _interestNodes.end()) {
                        vec = it->second;
                    }
                    vec.push_back({ j,i });
                    _interestNodes[Zone::OutdoorZone] = vec;
                    break;
                }
                case Zone::FortressExitZone1:
                case Zone::FortressExitZone2:
                case Zone::FortressExitZone3:
                case Zone::FortressExitZone4:
                {
                    std::vector<t_coord> vec;
                    auto it = _interestNodes.find(Zone::FortressExitZone);
                    if (it != _interestNodes.end()) {
                        vec = it->second;
                    }
                    vec.push_back({ j,i });
                    _interestNodes[Zone::FortressExitZone] = vec;
                    break;
                }
                case Zone::FortressZone1:
                case Zone::FortressZone2:
                case Zone::FortressZone3:
                case Zone::FortressZone4:
                case Zone::FortressZone5:
                case Zone::FortressZone6:
                case Zone::FortressZone7:
                case Zone::FortressWorkZone1:
                case Zone::FortressWorkZone2:
                case Zone::FortressWorkZone3:
                case Zone::FortressWorkZone4:
                case Zone::FortressWorkZone5:
                case Zone::FortressWorkZone6:
                case Zone::FortressWorkZone7:
                case Zone::FortressWorkZone8:
                case Zone::FortressWorkZone9:
                case Zone::FortressWorkZone10:
                case Zone::FortressWorkZone11:
                case Zone::FortressWorkZone12:
                {
                    std::vector<t_coord> vec;
                    auto it = _interestNodes.find(Zone::FortressZone);
                    if (it != _interestNodes.end()) {
                        vec = it->second;
                    }
                    vec.push_back({ j,i });
                    _interestNodes[Zone::FortressZone] = vec;
                    break;
                }
                default:
                    break;
                } 
               /*
               TEST Pixel for each color
               if (sum == (int) Zone::Wall) {
                    _map[j][i] = Zone::Wall;
                }
                else if (sum == (int) Zone::Door) {
                    _map[j][i] = Zone::Door;
                }
                else if (sum == (int)Zone::RestZone) {
                    _map[j][i] = Zone::RestZone;
                }
                else if (sum == (int)Zone::OutdoorZone1) {
                    _map[j][i] = Zone::OutdoorZone1;
                }
                else if (sum == (int)Zone::OutdoorZone2) {
                    _map[j][i] = Zone::OutdoorZone2;
                }
                else if (sum == (int)Zone::OutdoorZone3) {
                    _map[j][i] = Zone::OutdoorZone3;
                }
                else if (sum == (int)Zone::OutdoorZone4) {
                    _map[j][i] = Zone::OutdoorZone4;
                }
                else if (sum == (int)Zone::OutdoorZone5) {
                    _map[j][i] = Zone::OutdoorZone5;
                }
                else if (sum == (int)Zone::OutdoorZone6) {
                    _map[j][i] = Zone::OutdoorZone6;
                }
                else if (sum == (int)Zone::OutdoorZone7) {
                    _map[j][i] = Zone::OutdoorZone7;
                }
                else if (sum == (int)Zone::OutdoorZone8) {
                    _map[j][i] = Zone::OutdoorZone8;
                }
                else if (sum == (int)Zone::OutdoorZone9) {
                    _map[j][i] = Zone::OutdoorZone9;
                }
                else if (sum == (int)Zone::OutdoorZone10) {
                    _map[j][i] = Zone::OutdoorZone10;
                }
                else if (sum == (int)Zone::OutdoorZone11) {
                    _map[j][i] = Zone::OutdoorZone11;
                }
                else if (sum == (int)Zone::FortressZone1) {
                    _map[j][i] = Zone::FortressZone1;
                }
                else if (sum == (int)Zone::FortressZone2) {
                    _map[j][i] = Zone::FortressZone2;
                }
                else if (sum == (int)Zone::FortressZone3) {
                    _map[j][i] = Zone::FortressZone3;
                }
                else if (sum == (int)Zone::FortressZone4) {
                    _map[j][i] = Zone::FortressZone4;
                }
                else if (sum == (int)Zone::FortressZone5) {
                    _map[j][i] = Zone::FortressZone5;
                }
                else if (sum == (int)Zone::FortressZone6) {
                    _map[j][i] = Zone::FortressZone6;
                }
                else if (sum == (int)Zone::FortressZone7) {
                    _map[j][i] = Zone::FortressZone7;
                }
                else if (sum == (int)Zone::FortressExitZone1) {
                    _map[j][i] = Zone::FortressExitZone1;
                }
                else if (sum == (int)Zone::FortressExitZone2) {
                    _map[j][i] = Zone::FortressExitZone2;
                }
                else if (sum == (int)Zone::FortressExitZone3) {
                    _map[j][i] = Zone::FortressExitZone3;
                }
                else if (sum == (int)Zone::FortressExitZone4) {
                    _map[j][i] = Zone::FortressExitZone4;
                }
                else if (sum == (int)Zone::FortressWorkZone1) {
                    _map[j][i] = Zone::FortressWorkZone1;
                }
                else if (sum == (int)Zone::FortressWorkZone2) {
                    _map[j][i] = Zone::FortressWorkZone2;
                }
                else if (sum == (int)Zone::FortressWorkZone3) {
                    _map[j][i] = Zone::FortressWorkZone3;
                }
                else if (sum == (int)Zone::FortressWorkZone4) {
                    _map[j][i] = Zone::FortressWorkZone4;
                }
                else if (sum == (int)Zone::WorkStart) {
                    _map[j][i] = Zone::WorkStart;
                }
                else if (sum == (int)Zone::WorkGoal) {
                    _map[j][i] = Zone::WorkGoal;
                }
                else if (sum == (int)Zone::RestSpawn) {
                    _map[j][i] = Zone::RestSpawn;
                }
                else if (sum == (int)Zone::Base) {
                    _map[j][i] = Zone::Base;
                }
                else if (sum == (int)Zone::PathPoint) {
                    _map[j][i] = Zone::PathPoint;                    
                }
                else{
                    std::cout << "No existe: " << j << " " << i << std::endl;
                }
                */
                pixels++;
            }
        }
        if (!d_pos.empty()) {
            Door d;
            MathLib::Vec2 c1 = { (float)d_pos[0].x * 8, (float)d_pos[0].y * 8};
            MathLib::Vec2 c2 = { (float)d_pos.back().x * 8 + 8, (float)d_pos.back().y * 8 + 8 };
            d.init(c1, c2);
            doors.push_back(d);
            d_pos.clear();
        }
        initializeDoors(doors, &nodes);
        initializeZones(nodes);      
    }
}

void MapWorld::initializeDoors(std::vector<Door> doors, std::vector<PointNode>* nodes)
{
    _numDoors = doors.size();
    _doors = new Door[_numDoors];// &doors[0];
    for (size_t i = 0; i < _numDoors; i++) {
        _doors[i] = doors[i];
        std::pair<MathLib::Vec2, MathLib::Vec2> coord = _doors[i].getCoord();
        t_coord f = { round(coord.first.x() / 8), round(coord.first.y() / 8) };
        t_coord l = { round(coord.second.x() / 8), round(coord.second.y() / 8) };
        t_coord middle = {
            f.x == l.x ? f.x : f.x + round((l.x - f.x) / 2),
            f.y == l.y ? f.y : f.y + round((l.y - f.y) / 2)
        };
        int distance = -2;
        PointNode pn1;
        PointNode pn2;
        uint8_t size = nodes->size();
        if (middle.x == f.x) {
            Zone from = _map[middle.x - 1][middle.y];
            Zone to = _map[middle.x + 1][middle.y];
            _map[middle.x - 1][middle.y] = Zone::PathPoint;
            _map[middle.x + 1][middle.y] = Zone::PathPoint;
            
            std::vector<MathLib::Vec2> path;
            path.push_back({(float) (middle.x -1) * 8,(float) middle.y * 8 });
            path.push_back({ (float) middle.x * 8,(float) middle.y * 8 });
            path.push_back({ (float) (middle.x + 1) * 8,(float) middle.y * 8 });
            
            pn1.pos = {middle.x - 1, middle.y};
            pn1.doorConnections = { {from, Zone::Door}, {Zone::Door, to}, {size, size + 1}, {middle.x - 1, middle.y}, distance, 2,path };
            std::reverse(path.begin(), path.end());
            pn2.pos = { middle.x + 1, middle.y };
            pn2.doorConnections = { {to, Zone::Door}, {from, Zone::Door} , {size + 1, size}, {middle.x + 1, middle.y}, distance, 2,path };


            std::vector<t_coord> vecIn;
            std::vector<t_coord> vecOut;
            auto it = _interestNodes.find(Zone::DoorInPoint);
            if (it != _interestNodes.end()) {
                vecIn = it->second;
            }
            it = _interestNodes.find(Zone::DoorOutPoint);
            if (it != _interestNodes.end()) {
                vecOut = it->second;
            }
            vecOut.push_back({ middle.x - 1,middle.y });
            vecIn.push_back({ middle.x + 1,middle.y });
            _interestNodes[Zone::DoorInPoint] = vecIn;
            _interestNodes[Zone::DoorOutPoint] = vecOut;
        }
        else {
            Zone from = _map[middle.x][middle.y - 1] = Zone::PathPoint;
            Zone to = _map[middle.x][middle.y + 1] = Zone::PathPoint;
            _map[middle.x][middle.y - 1] = Zone::PathPoint;
            _map[middle.x][middle.y + 1] = Zone::PathPoint;

            std::vector<MathLib::Vec2> path;
            path.push_back({ (float) middle.x * 8,(float) (middle.y - 1) * 8 });
            path.push_back({ (float) middle.x * 8,(float) middle.y * 8 });
            path.push_back({ (float) middle.x * 8,(float) (middle.y + 1) * 8 });

            pn1.pos = { middle.x, middle.y - 1};
            pn1.doorConnections = { {from, Zone::Door}, {to, Zone::Door}, {size, size + 1}, {middle.x, middle.y - 1}, distance, 2, path };
            std::reverse(path.begin(), path.end());
            pn2.pos = { middle.x, middle.y + 1 };
            pn2.doorConnections = { {to, Zone::Door}, {from, Zone::Door} , {size + 1, size}, {middle.x, middle.y + 1}, distance, 2,path };


            std::vector<t_coord> vecIn;
            std::vector<t_coord> vecOut;
            auto it = _interestNodes.find(Zone::DoorInPoint);
            if (it != _interestNodes.end()) {
                vecIn = it->second;
            }
            it = _interestNodes.find(Zone::DoorOutPoint);
            if (it != _interestNodes.end()) {
                vecOut = it->second;
            }
            vecOut.push_back({ middle.x,middle.y - 1 });
            vecIn.push_back({ middle.x,middle.y + 1 });
            _interestNodes[Zone::DoorInPoint] = vecIn;
            _interestNodes[Zone::DoorOutPoint] = vecOut;
        }
        nodes->push_back(pn1);
        nodes->push_back(pn2);
    }
}

void MapWorld::initializeZones(std::vector<PointNode> nodes)
{
    std::vector<Zone> zones;
    _numPointNodes = nodes.size();
    _pointNodes = new PointNode[_numPointNodes];
    for (int i = 0; i < _numPointNodes;  i++) {
        _pointNodes[i] = nodes[i];
        std::string str = std::to_string(i);
        char* cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        _pointNodes[i].uidTexture = UIManager::instance().addText(cstr, _pointNodes[i].pos.x * 8, _pointNodes[i].pos.y * 8, true);
        zones.push_back(_map[_pointNodes[i].pos.x + 1][_pointNodes[i].pos.y]);
        zones.push_back(_map[_pointNodes[i].pos.x - 1][_pointNodes[i].pos.y]);
        zones.push_back(_map[_pointNodes[i].pos.x][_pointNodes[i].pos.y + 1]);
        zones.push_back(_map[_pointNodes[i].pos.x][_pointNodes[i].pos.y - 1]);

        std::sort(zones.begin(), zones.end());
        Zone z;
        auto it = zones.begin();
        while (it < zones.end()) {
            if ((it == zones.begin() || z != *it) && *it != Zone::Wall) {
                z = *it;
                ++it;
            }
            else {
                _map[_pointNodes[i].pos.x][_pointNodes[i].pos.y] = *it;
                it = zones.erase(it);
            }
        }
        
        _pointNodes[i].zones = zones;
        zones.clear();
    }
}

long MapWorld::getDistance(const t_coord c1, const t_coord c2) const {
    int x = abs(c1.x - c2.x);
    int y = abs(c1.y - c2.y);
    return (x * x) + (y * y);
}
