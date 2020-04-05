#include "engine/game.h"
#include "engine/window.h"
#include "engine/defines.h"
#include <thread>

#include <ctime>
#include <cstdlib>

int main(int, char* []) {
    srand(time(NULL));

    Window::instance().init(GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);

    {
        Game game;

        game.init();
        game.start();
        game.shutdown();
    }

    Window::instance().shutdown();

    return 0;
}
