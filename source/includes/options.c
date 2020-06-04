// Declare variables
int optionsItem = 0;

bool backToMenu = false;
// Starting Menu
void printOptions()
{
    printf(
        CONSOLE_ESC(6;9H)"____        __  _"
        CONSOLE_ESC(7;8H)"/ __ \\____  / /_(_)___  ____  _____"
        CONSOLE_ESC(8;7H)"/ / / / __ \\/ __/ / __ \\/ __ \\/ ___/"
        CONSOLE_ESC(9;6H)"/ /_/ / /_/ / /_/ / /_/ / / / (__  ) "
        CONSOLE_ESC(10;6H)"\\____/ .___/\\__/_/\\____/_/ /_/____/"
        CONSOLE_ESC(11;10H)"/_/"
    );

    if (optionsItem < 0)
        optionsItem = 1;
    if (optionsItem > 1)
        optionsItem = 0;

    printf(
        CONSOLE_ESC(14;6H)">  Emulation:"
        CONSOLE_ESC(16;6H)">  Back"
    );

    if (emulationEnvironment) printf(CONSOLE_ESC(14;20H)"Yes");
    else printf(CONSOLE_ESC(14;20H)"No");

    if (optionsItem == 0)
        printf(CONSOLE_ESC(14;7H)">");
    if (optionsItem == 1)
        printf(CONSOLE_ESC(16;7H)">");
    // Update the console, sending a new frame to the display
}

void get_options_input()
{
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_DUP)
    {
        optionsItem -= 1;
        printf(
            CONSOLE_ESC(14;7H)" "
            CONSOLE_ESC(16;7H)" "
        );
    }

    if (kDown & KEY_DDOWN)
    {
        optionsItem += 1;
        printf(
            CONSOLE_ESC(14;7H)" "
            CONSOLE_ESC(16;7H)" "
        );
    };

    if (kDown & KEY_A)
    {
        if (optionsItem == 0) emulationEnvironment = !emulationEnvironment;
        if (optionsItem == 1) goToOptions = false;
    };

    if (kDown & KEY_PLUS) backToHbMenu = true;
}

void options_menu() {
    consoleInit(NULL);
    printf(CONSOLE_ESC(2J));
    printOptions();
    wave();
    consoleUpdate(NULL);
    get_options_input();
}
