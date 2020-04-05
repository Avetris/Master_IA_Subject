#ifndef __SCENE_OPT_H__
#define __SCENE_OPT_H__ 1

#include <engine\scene.h>
#include <ia\world_path.h>

#include <unordered_map>
#include <ia\world_opt.h>

class SceneOPT : public Scene {
	public:
		SceneOPT() {};
		~SceneOPT() {};

		void init() final;
		void shutdown() final;

		void update(uint32_t dt) final;
		void handleMouseEvent(SDL_Event e) final;
		void handleKeyEvent(SDL_Keycode key) final;
		void render() final;
	private:
		WorldOPT world_;

		uint16_t _addAgent, _removeAgent, _numAgents;

		int8_t slo_mo_ = 1;
};

#endif