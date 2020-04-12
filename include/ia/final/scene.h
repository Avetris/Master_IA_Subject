#ifndef __SCENE_FINAL_H__
#define __SCENE_FINAL_H__ 1

#include <engine\scene.h>
#include <ia\final\world.h>
#include <ia\pathfinding\path_manager.h>

#include <unordered_map>

class SceneFinal: public Scene {
	public:
		SceneFinal() {};
		~SceneFinal() {};

		void init() final;
		void shutdown() final;

		void update(uint32_t dt) final;
		void handleMouseEvent(SDL_Event e, int x, int y) final;
		void handleKeyEvent(SDL_Keycode key) final;
		void render() final;
	private:
		void initText();
		WorldFinal _world;

		Sprite _background;
		bool _loaded = false;
		t_coord _fristClick = {-1, -1};
		uint32_t _nSoldier = 0, _nSlaves = 0, _nGuards = 0;
		std::vector<uint32_t> _sprites;
};

#endif