#ifndef __SCENE_PATH_H__
#define __SCENE_PATH_H__ 1

#include <engine\scene.h>
#include <ia\pathfinding\world.h>
#include <ia\pathfinding\path_manager.h>

#include <unordered_map>

class ScenePath: public Scene {
	public:
		ScenePath() {};
		~ScenePath() {};

		void init() final;
		void shutdown() final;

		void update(uint32_t dt) final;
		void handleMouseEvent(SDL_Event e, int x, int y) final;
		void handleKeyEvent(SDL_Keycode key) final;
		void render() final;
	private:
		void changeType(PathType type);
		WorldPath _world;

		Sprite _background;
		t_coord _fristClick = {-1, -1};

		PathType _type = PathType::Dijkstra;
		std::unordered_map<PathType, uint32_t> _typeSpriteMap;
};

#endif