//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "engine/scene.h"
#include "ia/defines.h"
#include "engine/ui_manager.h"

#include <SDL/SDL.h>

class Game {
	public:
		Game() {};
		~Game() { UIManager::instance().shutdown();}

		void init();
		void start();
		void shutdown();
	private:
		void handleInput();
		void update(uint32_t dt);
		void render();

		bool quit_ = false;
		uint8_t fps_sprite_ = 0;

		int8_t slo_mo_ = 1;

};

#endif
