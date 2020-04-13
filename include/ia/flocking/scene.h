#ifndef __SCENE_FLOCKING_H__
#define __SCENE_FLOCKING_H__ 1

#include <engine\scene.h>
#include <ia\flocking\world.h>

class SceneFlocking : public Scene {
	public:
		SceneFlocking() {};
		~SceneFlocking() {};

		void init() final;
		void shutdown() final;

		void update(uint32_t dt) final;
		void handleMouseEvent(SDL_Event e, int x, int y) final;
		void handleKeyEvent(SDL_Keycode key) final;
		void render() final;
	private:
		WorldFlocking _world;

		uint16_t _addAgent, _removeAgent, _numAgents;

		int8_t slo_mo_ = 1;
};

#endif