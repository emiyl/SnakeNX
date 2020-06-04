// Declare variables
int snake[100][100]; // Track snake segment x,y in arena
int snakeHeadPointer; // Record current head of snake in snake array
int snakeTailPointer; // Record current tail of snake in snake array
//int highScoreValue[10]; // Array of high score table scores
int gameSpeed; // Speed of game, how fast the worm moves
int foodTimer; // Bonus score from picking up food
int score; // Tracks current score
int length; // Current snake length
int snakeDirection; // Right = 0, Down = 1, Left = 2, Up = 3

//char highScoreName[10][14]; // 11 arrays of 14 chars, last array is blank
char arena[60][45]; // Record what data is in arena x,y
char snakeBody[0]; // What the snake body character should be
char snakeFood[0]; // What the snake character food should be

bool snakeGrow; // Whether the snake should grow or not
bool forever; // Whether or not to keep looping
bool backToHbMenu; // Whether to go back to HbMenu or not

// Set default high scores
/*void initialise_high_scores() {
    int i;
    for (i=0;i<10;i++) {
        strcpy(highScoreName[i], "---");
        highScoreValue[i] = 0;
    };
}*/

// Clear screen of 81x45
void clear_screen() {
    consoleInit(NULL);
    printf(CONSOLE_ESC(2J));
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (emulationEnvironment) msec *= 20;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void initialise_game() {
    // Clear screen
    clear_screen();

    // Set variables
    snakeHeadPointer = 4; // Record current head of snake in snake array
    snakeTailPointer = 1; // Record current tail of snake in snake array
    gameSpeed = 75; // Speed of game, how fast the worm moves
    foodTimer = 100; // Bonus score from picking up food
    score = 0; // Tracks current score
    length = 4; // Current snake length
    snakeDirection = 0; // Right = 0, Down = 1, Left = 2, Up = 3

    snakeBody[0] = '#'; // What the snake body character should be
    snakeFood[0] = '+'; // What the snake character food should be

    snakeGrow = false; // Whether the snake should grow or not
    forever = true; // Whether or not to keep looping
    backToHbMenu = false; // Whether to go back to HbMenu or not

    // Create the arena
    for (int ix=0;ix<60;ix++)
    {
      for (int iy=0;iy<45;iy++)
        arena[ix][iy] = ' ';
    }

    // Creates snake in memory
    for (int i=1;i<5;i++)
    {
      snake[i][1] = 5 + i;
      snake[i][2] = 6;
      arena[5 + i][6] = snakeBody[0];
    }

    // Randomizer seed
    srand(time(NULL));
}

void drop_food()
{
    // Creates random coordinate for food to spawn into
    int x, y;
    do {
        x = (rand() % 57) + 2;
        y = (rand() % 41) + 2;
    } while ((arena[x][y] == snakeFood[0]) | (arena[x][y] == snakeBody[0]));
    // This repeats if it spawns into where the food or snake is currently

    // Puts new food in position in memory and on screen, then resets the timer
    arena[x][y] = snakeFood[0];
    printf(CONSOLE_ESC(%i;%iH)"+", y, x);
    foodTimer = 200;
}

void display_arena() {
    // Clear screen
    consoleInit(NULL);

    // Display stats at side of screen
    printf(
      CONSOLE_ESC(3;64H) "Score: %i"
      CONSOLE_ESC(5;64H) "Bonus: %i"
      CONSOLE_ESC(7;64H) "Length: %i"
      , score, foodTimer, length
    );

    // Write blank spaces to all arena spaces
    for (int ix=0;ix<61;ix++)
    {
      for (int iy=0;iy<46;iy++)
        printf(CONSOLE_ESC(%i;%iH) " ", iy, ix);
    }
}

/*void display_walls() {
  int ix, iy;
  for (ix=0;ix<60;ix++) {
      printf(CONSOLE_ESC(%i;%iH) "#", 0, ix);
      arena[ix][0] = '#';
      printf(CONSOLE_ESC(%i;%iH) "#", 45, ix);
      arena[ix][44] = '#';
  }
  for (iy=0;iy<45;iy++) {
      printf(CONSOLE_ESC(%i;%iH) "#", iy, 0);
      arena[0][iy] = '#';
      printf(CONSOLE_ESC(%i;%iH) "#", iy, 60);
      arena[59][iy] = '#';
  }
}*/

void update_stats(int scoreType) {
    if (scoreType == 0)
    {
        printf(
            CONSOLE_ESC(3;71H) "             "
            CONSOLE_ESC(3;64H) "Score: %i"
            , score
        );
    }
    if (scoreType == 1)
    {
        printf(
            CONSOLE_ESC(5;71H) "             "
            CONSOLE_ESC(5;64H) "Bonus: %i"
            , foodTimer
        );
    }
    if (scoreType == 2)
    {
        printf(
            CONSOLE_ESC(7;72H) "             "
            CONSOLE_ESC(7;64H) "Length: %i"
            , length
        );
    }
}

void get_input() {
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & (KEY_DLEFT|KEY_LSTICK_LEFT) && (snakeDirection != 0))
        snakeDirection = 2;
    if (kDown & (KEY_DRIGHT|KEY_LSTICK_RIGHT) && (snakeDirection != 2))
        snakeDirection = 0;
    if (kDown & (KEY_DUP|KEY_LSTICK_UP) && (snakeDirection != 1))
        snakeDirection = 3;
    if (kDown & (KEY_DDOWN|KEY_LSTICK_DOWN) && (snakeDirection != 3))
        snakeDirection = 1;

    if (kDown & KEY_B)
        snakeAlive = false;
    if (kDown & KEY_PLUS)
        backToHbMenu = true;
}

void collision_detection_and_score() {
    if (arena[snake[snakeHeadPointer][1]][snake[snakeHeadPointer][2]] == '+')
    {
        snakeGrow = true;
        score += (10 + foodTimer);
        if (gameSpeed < 5) gameSpeed -= 5;
        update_stats(0);
        drop_food();
    }

    if (arena[snake[snakeHeadPointer][1]][snake[snakeHeadPointer][2]] == (snakeBody[0]))
        snakeAlive = false;
}

void move_snake() {
    int snakeHeadX, snakeHeadY;

    // Add one to headpointer
    snakeHeadX = snake[snakeHeadPointer][1];
    snakeHeadY = snake[snakeHeadPointer][2];
    snakeHeadPointer++;
    if (snakeHeadPointer == 100) snakeHeadPointer = 0;

    // Moves head in direction of current direction
    if (snakeDirection == 0) snakeHeadX++;
    if (snakeDirection == 1) snakeHeadY++;
    if (snakeDirection == 2) snakeHeadX--;
    if (snakeDirection == 3) snakeHeadY--;

    // Wrap the snake around the screen if needed
    if (snakeHeadX > 60) snakeHeadX = 0;
    if (snakeHeadX < 0) snakeHeadX = 60;
    if (snakeHeadY > 45) snakeHeadY = 0;
    if (snakeHeadY < 0) snakeHeadY = 45;

    // Set the snake's new position
    snake[snakeHeadPointer][1] = snakeHeadX;
    snake[snakeHeadPointer][2] = snakeHeadY;
    collision_detection_and_score();
    arena[snakeHeadX][snakeHeadY] = snakeBody[0];

    // Draw the new snake head
    printf(CONSOLE_ESC(%i;%iH) "%c", snakeHeadY, snakeHeadX, snakeBody[0]);

    // Delete old segment from tail
    if (snakeGrow) {
        length++;
        snakeGrow = false;
        update_stats(2);
    } else {
        arena[snake[snakeTailPointer][1]][snake[snakeTailPointer][2]] = ' ';
        printf(CONSOLE_ESC(%i;%iH) " ", snake[snakeTailPointer][2], snake[snakeTailPointer][1]);
        snakeTailPointer++;
        if (snakeTailPointer > 99) snakeTailPointer = 0;
    }
}

void update_food_timer() {
    if (foodTimer > 0) foodTimer--;
    update_stats(1);
}

void game_over_message() {
    clear_screen();
    printf(CONSOLE_ESC(%i;%iH) "G A M E   O V E R", 21, 32);
    printf(CONSOLE_ESC(%i;%iH) "Press any button to continue", 23, 27);
    consoleUpdate(NULL);
    u64 kDown;
    do {
        hidScanInput();
        kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    } while (!kDown);
    msleep(1000);
    clear_screen();
    consoleUpdate(NULL);
}

/* SwkbdTextCheckResult Keyboard_ValidateText(char *string, size_t size) {
	if (strcmp(string, "") == 0) {
		strncpy(string, "The name cannot be empty.", size);
		return SwkbdTextCheckResult_Bad;
	}

	return SwkbdTextCheckResult_OK;
}

const char *Keyboard_GetText(const char *guide_text, const char *initial_text) {
	Result ret = 0;
	SwkbdConfig swkbd;
	static char input_string[256];

	if (R_FAILED(ret = swkbdCreate(&swkbd, 0))) {
		swkbdClose(&swkbd);
		return "";
	}

	swkbdConfigMakePresetDefault(&swkbd);

	if (strlen(guide_text) != 0)
		swkbdConfigSetGuideText(&swkbd, guide_text);

	if (strlen(initial_text) != 0)
		swkbdConfigSetInitialText(&swkbd, initial_text);

	swkbdConfigSetTextCheckCallback(&swkbd, Keyboard_ValidateText);

	if (R_FAILED(ret = swkbdShow(&swkbd, input_string, sizeof(input_string)))) {
		swkbdClose(&swkbd);
		return "";
	}

	swkbdClose(&swkbd);
	return input_string;
} */

/*void high_scores() {
  clear_screen();
  int i, j;
  bool isHighScore;
  char *name[14];

  i = 1;
  do {
    if (score > highScoreValue[i]) {
      isHighScore = true;
      break;
    } else {
      i++;
    }
  } while (i < 10);

  if (isHighScore) {
    printf(CONSOLE_ESC(%i;%iH) "N E W   H I G H   S C O R E", 6, 6);
    printf(CONSOLE_ESC(%i;%iH) "Press any button to enter your name:", 8, 6);
    consoleUpdate(NULL);
    for ( ; ; ) {
      hidScanInput();
      u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
      if (kDown) break;
    }
    //name = Keyboard_GetText("Name", 0);
    name[14] = "NEW";

    for (j=9;j<(i+2);j--) {
      strcpy(highScoreName[j], highScoreName[j - 1]);
      highScoreValue[j] = highScoreValue[j - 1];
    }

    strcpy(highScoreName[i], name[14]);
    highScoreValue[i] = score;
  }

  clear_screen();
  printf(CONSOLE_ESC(%i;%iH) "T O D A Y ' S   H I G H   S C O R E S", 6, 6);
  for (i=0;i<10;i++) {
    printf(CONSOLE_ESC(%i;%iH) "%s", (9 + (i * 2)), 6, highScoreName[i]);
    printf(CONSOLE_ESC(%i;%iH) "%i", (9 + (i * 2)), 21, highScoreValue[i]);
  }
  printf(CONSOLE_ESC(%i;%iH) "Press any button to play again", 30, 6);
  consoleUpdate(NULL);
  for ( ; ; ) {
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (kDown) break;
  }
}*/

void snake_game() {
    initialise_game();
    display_arena();
    //display_walls();
    drop_food();
    do {
      get_input();
      if (backToHbMenu) break;
      move_snake();
      update_food_timer();
      consoleUpdate(NULL);
      msleep(gameSpeed);
    } while (snakeAlive);
    game_over_message();
    //high_scores();
}
