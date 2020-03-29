//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "engine/game.h"
#include "engine/window.h""
#include "ia/defines.h"

#include <ctime>
#include <ia\scene_steering.h>

int main(int, char* []) {
  srand(time(NULL));

  Window::instance().init(GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);

  {
    Game game;

    game.init();
    game.attachScene<SceneSteering>();
    game.start();
    game.shutdown();
  }

  Window::instance().shutdown();

  return 0;
}
