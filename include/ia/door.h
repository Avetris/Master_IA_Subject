
#ifndef __DOOR_H__
#define __DOOR_H__ 1

#include <cstdint>
#include <ia/defines.h>
#include <engine\window.h>

class Door {
    public:
        Door(){
        };
        ~Door() {};
        void init(MathLib::Vec2 from, MathLib::Vec2 to);
        void render() const;
        bool isOpen() { return _isOpen; }
        void changeStatus();
        std::pair<MathLib::Vec2, MathLib::Vec2> getCoord() { return { _from, _to }; }
        bool isDoor(MathLib::Vec2 coord);
    private:
        bool _isOpen = false;
        SDL_Rect rect;
        MathLib::Vec2 _from;
        MathLib::Vec2 _to;
};

#endif