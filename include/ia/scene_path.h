#ifndef __SCENE_PATH_H__
#define __SCENE_PATH_H__ 1

#include <engine\scene.h>
#include <ia\world_path.h>
#include <ia\path_finding\path_manager.h>

#include <unordered_map>

class ScenePath: public Scene {
	public:
		ScenePath() {};
		~ScenePath() {};

		void init() final;
		void shutdown() final;

		void update(uint32_t dt) final;
		void handleMouseEvent(SDL_Event e) final;
		void handleKeyEvent(SDL_Keycode key) final;
		void render() final;
	private:
		void changeType(PathFinding::Type type);
		WorldPath world_;

		int8_t slo_mo_ = 1;
		Sprite _background;
		t_coord _fristClick = {-1, -1};

		PathFinding::Type _type = PathFinding::Type::Dijstra;
		std::unordered_map<PathFinding::Type, uint32_t> _typeSpriteMap;
};

#endif