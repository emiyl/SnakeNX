int menuItem = 0;

// Starting Menu
void printMenu()
{
    // SnakeNX text on main scren
    printf(
        CONSOLE_ESC(6;9H)"_____             __        _   ___  __"
        CONSOLE_ESC(7;8H)"/ ___/____  ____ _/ /_____  / | / / |/ /"
        CONSOLE_ESC(8;8H)"\\__ \\/ __ \\/ __ `/ //_/ _ \\/  |/ /|   /"
        CONSOLE_ESC(9;7H)"___/ / / / / /_/ / ,< /  __/ /|  //   |"
        CONSOLE_ESC(10;6H)"/____/_/ /_/\\__,_/_/|_|\\___/_/ |_//_/|_|"
    );

    if (menuItem < 0)
        menuItem = 2;
    if (menuItem > 2)
        menuItem = 0;

    printf(
        CONSOLE_ESC(14;6H)">  Play"
        CONSOLE_ESC(16;6H)">  Options"
        CONSOLE_ESC(18;6H)">  Exit"
    );

    if (menuItem == 0)
      printf(CONSOLE_ESC(14;7H)">");
    else if (menuItem == 1)
      printf(CONSOLE_ESC(16;7H)">");
    else if (menuItem == 2)
      printf(CONSOLE_ESC(18;7H)">");
}

void get_menu_input()
{
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_DUP)
    {
        menuItem -= 1;
        printf(
            CONSOLE_ESC(14;7H)" "
            CONSOLE_ESC(16;7H)" "
            CONSOLE_ESC(18;7H)" "
        );
    }

    if (kDown & KEY_DDOWN)
    {
        menuItem += 1;
        printf(
            CONSOLE_ESC(14;7H)" "
            CONSOLE_ESC(16;7H)" "
            CONSOLE_ESC(18;7H)" "
        );
    };

    if (kDown & KEY_A)
    {
        if (menuItem == 0) snakeAlive = true;
        if (menuItem == 1) goToOptions = true;
        if (menuItem == 2) backToHbMenu = true;
    };

    if (kDown & KEY_PLUS) backToHbMenu = true;
}

// This looks painful but it just makes the wave on the home screen
void wave() {
  int i, j, k, l, m;
  l =  35;
  for (m=1;m<82;m+=20) {
    for (k=0;k<3;k++) {
      for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
          printf(CONSOLE_ESC(%i;%iH)"#", l + j + k, m + i + (k*4));
        }
      }
    }
  }
  for (m=11;m<72;m+=20) {
    for (k=0;k<3;k++) {
      for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
          printf(CONSOLE_ESC(%i;%iH)"#", l + 2 + j - k, m + i + (k*4));
        }
      }
    }
  }
}

void menu() {
    consoleInit(NULL);
    printf(CONSOLE_ESC(2J));
    printMenu();
    wave();
    consoleUpdate(NULL);
    get_menu_input();
}
