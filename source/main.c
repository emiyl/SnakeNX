// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

bool backToHbMenu         = false,
     emulationEnvironment = false,
     goToOptions          = false,
     snakeAlive           = false;

// Include menu and snakes game
#include "includes/snake.c"
#include "includes/menu.c"
#include "includes/options.c"

// Declare variables
char keysNames[32][32] = {
    "KEY_A", "KEY_B", "KEY_X", "KEY_Y",
    "KEY_LSTICK", "KEY_RSTICK", "KEY_L", "KEY_R",
    "KEY_ZL", "KEY_ZR", "KEY_PLUS", "KEY_MINUS",
    "KEY_DLEFT", "KEY_DUP", "KEY_DRIGHT", "KEY_DDOWN",
    "KEY_LSTICK_LEFT", "KEY_LSTICK_UP", "KEY_LSTICK_RIGHT", "KEY_LSTICK_DOWN",
    "KEY_RSTICK_LEFT", "KEY_RSTICK_UP", "KEY_RSTICK_RIGHT", "KEY_RSTICK_DOWN",
    "KEY_SL_LEFT", "KEY_SR_LEFT", "KEY_SL_RIGHT", "KEY_SR_RIGHT",
    "KEY_TOUCH", "", "", ""
};

int main(int argc, char* argv[])
{
    while (appletMainLoop() & !backToHbMenu) {
          menu();
          while (goToOptions) options_menu();
          while (snakeAlive) snake_game();
    }
    consoleExit(NULL);
    return 0;
}
