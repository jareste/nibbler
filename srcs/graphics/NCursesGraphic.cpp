#include "../../incs/NCursesGraphic.hpp"

NCursesGraphic::NCursesGraphic() : width(0), height(0), gameWindow(nullptr), isInitialized(false) {}

NCursesGraphic::~NCursesGraphic() {
	if (!isInitialized) {
		return;
	}
	
	std::cout << BBLU << "[NCurses] Cleaning up..." << RESET << std::endl;
	
	// Delete subwindow first
	if (gameWindow) {
		werase(gameWindow);
		wnoutrefresh(gameWindow);
		doupdate();
		delwin(gameWindow);
		gameWindow = nullptr;
	}
	
	// Clear window without destroying/resetting it
	if (stdscr) {
		wclear(stdscr);
		wrefresh(stdscr);
	}
	
	// Reset terminal state
	fflush(stdout);
	
	isInitialized = false;
}

bool NCursesGraphic::loadAsciiArtFile(const std::string& filepath, AsciiArtFile& art) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "Failed to load ASCII art file: " << filepath << std::endl;
		return false;
	}
	
	std::string line;
	// Logo Width
	std::getline(file, line);
	art.width = std::stoi(line);
	// Logo height
	std::getline(file, line);
	art.height = std::stoi(line);
	
	// Logo contents
	art.lines.clear();
	while (std::getline(file, line)) {
		art.lines.push_back(line);
	}
	
	return true;
}

void NCursesGraphic::init(int w, int h) {
	setlocale(LC_ALL, "");
	width = w;
	height = h;
	
	bool wasEnded = (isendwin() == TRUE);
	
	if (wasEnded || stdscr == nullptr) {
		initscr();
	} else {
		refresh();
	}
	
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);

if (has_colors()) {
	start_color();
	use_default_colors();
	
	if (can_change_color()) {
		// RGB to ncurses scale (0-1000): value = (RGB_value * 1000) / 255
		// Also: I'm reusing some colors (CYAN, GREEN, MAGENTA, YELLOW) to cover for the possibility of not being able to generate new ones
		init_color(COLOR_RED, 705, 204, 227);		// Red
		init_color(COLOR_BLUE, 275, 510, 706);		// Blue
		init_color(COLOR_CYAN, 996, 290, 318);		// Light Red (food)
		init_color(COLOR_GREEN, 300, 300, 300);		// Gray for ground
		init_color(COLOR_MAGENTA, 565, 933, 565);	// Light Green (144, 238, 144) - AI snake
		init_color(COLOR_YELLOW, 1000, 843, 0);		// Golden Yellow (255, 215, 0) - Snake B
	}
	
	init_pair(1, COLOR_BLUE, COLOR_BLACK);    // Snake A (blue on black)
	init_pair(2, COLOR_CYAN, COLOR_BLACK);    // Food (light red/cyan on black)
	init_pair(3, COLOR_BLACK, COLOR_BLACK);   // Background
	init_pair(4, COLOR_WHITE, COLOR_BLACK);   // UI text
	init_pair(5, COLOR_GREEN, COLOR_BLACK);   // Ground (gray)
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // Snake AI (light green)
	init_pair(7, COLOR_YELLOW, COLOR_BLACK);  // Snake B (golden yellow)
	
	bkgd(COLOR_PAIR(0));
	clear();
	refresh();
}	
	// +8 for quad borders (4 left, 4 right, 4 top, 4 bottom)
	// Width doubled for square-ish cells
	gameWindow = newwin(
		height + 8,
		(width * 2) + 8,
		1,
		1
	);
	
	if (gameWindow) {
		keypad(gameWindow, TRUE);
		nodelay(gameWindow, TRUE);
	}
	
	generateGroundPattern();
	
	// Load ASCII art files once during initialization
	loadAsciiArtFile("logos/ncurses_title_small_A.txt", titleSmallA);
	loadAsciiArtFile("logos/ncurses_title_small_B.txt", titleSmallB);
	loadAsciiArtFile("logos/ncurses_title_small_C.txt", titleSmallC);
	loadAsciiArtFile("logos/ncurses_title_small_D.txt", titleSmallD);
	loadAsciiArtFile("logos/ncurses_title_big_A.txt", titleBigA);
	loadAsciiArtFile("logos/ncurses_title_big_B.txt", titleBigB);
	loadAsciiArtFile("logos/ncurses_title_big_C.txt", titleBigC);
	loadAsciiArtFile("logos/ncurses_title_big_D.txt", titleBigD);
	loadAsciiArtFile("logos/ncurses_gameover_small.txt", gameoverSmall);
	loadAsciiArtFile("logos/ncurses_gameover_big.txt", gameoverBig);
	
	isInitialized = true;
}

void NCursesGraphic::render(const GameState& state, float deltaTime) {
	(void)deltaTime;

	werase(gameWindow);
	
	/* int win_height, win_width;
	getmaxyx(gameWindow, win_height, win_width);
	(void)win_height; */
	drawGround();

	drawBorder();
	
	drawSnake(state);
	
	drawFood(state);
	
	// Double buffer: stage stdscr first, then gameWindow
	wnoutrefresh(stdscr);
	wnoutrefresh(gameWindow);
	doupdate();
}

void NCursesGraphic::drawTitle(int win_height, int win_width)
{
	if (win_width / 2 < 42)
	{
		// Small version
		int anchorX = (win_width - titleSmallD.width) / 2;
		int anchorY = (win_height - titleSmallD.height) / 2 - 1;
		
		// Draw "bbler"
		for (size_t i = 0; i < titleSmallD.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleSmallD.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4));
		}
		
		// Draw "i base"
		anchorX = (win_width - titleSmallC.width) / 2;
		anchorY = (win_height - titleSmallC.height) / 2 - 1;
		for (size_t i = 0; i < titleSmallC.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(1));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleSmallC.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(1));
		}
		
		// Draw "i dot"
		anchorX = (win_width - titleSmallB.width) / 2;
		anchorY = (win_height - titleSmallB.height) / 2 - 1;
		for (size_t i = 0; i < titleSmallB.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(2));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleSmallB.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(2));
		}
		
		// Draw "n"
		anchorX = (win_width - titleSmallA.width) / 2;
		anchorY = (win_height - titleSmallA.height) / 2 - 1;
		for (size_t i = 0; i < titleSmallA.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleSmallA.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4));
		}
	}
	else {
		// Big version
		int anchorX = (win_width - titleBigD.width) / 2;
		int anchorY = (win_height - titleBigD.height) / 2;
		
		// Draw "bbler"
		for (size_t i = 0; i < titleBigD.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4) | A_BOLD);
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleBigD.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4) | A_BOLD);
		}
		
		// Draw "i base"
		anchorX = (win_width - titleBigC.width) / 2;
		anchorY = (win_height - titleBigC.height) / 2;
		for (size_t i = 0; i < titleBigC.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(1));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleBigC.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(1));
		}
		
		// Draw "i dot"
		anchorX = (win_width - titleBigB.width) / 2;
		anchorY = (win_height - titleBigB.height) / 2;
		for (size_t i = 0; i < titleBigB.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(2));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleBigB.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(2));
		}
		
		// Draw "n"
		anchorX = (win_width - titleBigA.width) / 2;
		anchorY = (win_height - titleBigA.height) / 2;
		for (size_t i = 0; i < titleBigA.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4));
			mvwaddstr(gameWindow, anchorY + i, anchorX, titleBigA.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4));
		}
	}
}

void NCursesGraphic::drawMode(const GameState &state, int win_height, int win_width, int anchorY) {
	(void)win_height;

	std::string modeText;
	std::string base;
	std::string combined;

	anchorY += 1;
	switch (state.config.mode) {
		case GameMode::SINGLE:
			modeText = "SINGLE";
			base = " - MULTI - VsAI";
			combined = modeText + base;

			wattron(gameWindow, COLOR_PAIR(5));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2 + 6, base.c_str());
			wattroff(gameWindow, COLOR_PAIR(5));
			
			wattron(gameWindow, COLOR_PAIR(1));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2, modeText.c_str());
			wattroff(gameWindow, COLOR_PAIR(1));

			break;
		
		case GameMode::MULTI:
			modeText = "MULTI";
			base = "SINGLE -       - VsAI";
			combined = base;

			wattron(gameWindow, COLOR_PAIR(5));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2, base.c_str());
			wattroff(gameWindow, COLOR_PAIR(5));

			wattron(gameWindow, COLOR_PAIR(7));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2 + 9, modeText.c_str());
			wattroff(gameWindow, COLOR_PAIR(7));
			break;

		case GameMode::AI:
			modeText = "VsAI";
			base = "SINGLE - MULTI -     ";
			combined = base;

			wattron(gameWindow, COLOR_PAIR(5));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2, base.c_str());
			wattroff(gameWindow, COLOR_PAIR(5));

			wattron(gameWindow, COLOR_PAIR(6));
			mvwaddstr(gameWindow, anchorY, (win_width - combined.size()) / 2 + 17, modeText.c_str());
			wattroff(gameWindow, COLOR_PAIR(6));
			break;
	}
}

void NCursesGraphic::drawInstructions(const GameState &state, int win_height, int win_width) {
	int anchorY = ((win_width / 2) < 42) ? win_height / 2 : (win_height / 2) + 5;

	drawMode(state, win_height, win_width, anchorY);

	wattron(gameWindow, COLOR_PAIR(4));
	const char *instructions = "[ENTER]·······START";
	int instrY = anchorY + 3;
	mvwaddstr(gameWindow, instrY, 
			(win_width / 2) - 10, instructions);
	wattroff(gameWindow, COLOR_PAIR(4));
	wattron(gameWindow, COLOR_PAIR(5));
	const char *instructionsDots = "·······";
	mvwaddstr(gameWindow, instrY, 
			(win_width / 2) - 3, instructionsDots);
	wattroff(gameWindow, COLOR_PAIR(5));

	wattron(gameWindow, COLOR_PAIR(4));
	const char *mode = "[SPACE]········MODE";
	int modeY = instrY + 1;
	mvwaddstr(gameWindow, modeY, 
			(win_width / 2) - 10, mode);
	wattroff(gameWindow, COLOR_PAIR(4));
	wattron(gameWindow, COLOR_PAIR(5));
	const char *modeDots = "········";
	mvwaddstr(gameWindow, modeY, 
			(win_width / 2) - 3, modeDots);
	wattroff(gameWindow, COLOR_PAIR(5));

	wattron(gameWindow, COLOR_PAIR(4));	
	const char *controls = "[↑↓←→]·········MOVE";
	int controlsY = modeY + 1;
	mvwaddstr(gameWindow, controlsY,
			(win_width / 2) - 10, controls);
	wattroff(gameWindow, COLOR_PAIR(4));
	wattron(gameWindow, COLOR_PAIR(5));
	const char *controlsDots = "·········";
	mvwaddstr(gameWindow, controlsY, 
			(win_width / 2) - 4, controlsDots);
	wattroff(gameWindow, COLOR_PAIR(5));

	wattron(gameWindow, COLOR_PAIR(4));
	const char *libs = "[1/2/3]······TRAVEL";
	int libsY = controlsY + 1;
	mvwaddstr(gameWindow, libsY,
			(win_width / 2) - 10, libs);
	wattroff(gameWindow, COLOR_PAIR(4));
	wattron(gameWindow, COLOR_PAIR(5));
	const char *travelDots = "······";
	mvwaddstr(gameWindow, libsY, 
			(win_width / 2) - 3, travelDots);
	wattroff(gameWindow, COLOR_PAIR(5));

	wattron(gameWindow, COLOR_PAIR(4));
	const char *quit = "[Q/ESC]········QUIT";
	int quitY = libsY + 1;
	mvwaddstr(gameWindow, quitY,
			(win_width / 2) - 10, quit);
	wattroff(gameWindow, COLOR_PAIR(5));	
	wattron(gameWindow, COLOR_PAIR(5));
	const char *quitDots = "········";
	mvwaddstr(gameWindow, quitY, 
			(win_width / 2) - 3, quitDots);
	wattroff(gameWindow, COLOR_PAIR(5));
	wattron(gameWindow, COLOR_PAIR(4));
}

void NCursesGraphic::renderMenu(const GameState &state, float deltaTime) {
	(void)state;
	(void)deltaTime;
	
	werase(gameWindow);

	int win_height, win_width;
	getmaxyx(gameWindow, win_height, win_width);

	wattron(gameWindow, COLOR_PAIR(4));
	for (int x = 0; x < win_width - 1; x++)
	{
		for (int y = 0; y < win_height - 1; y++)
		{
			mvwaddch(gameWindow, y, x, ' ');
		}
	}
	wattroff(gameWindow, COLOR_PAIR(4));

	drawBorder();

	drawTitle(win_height, win_width);
	drawInstructions(state, win_height, win_width);	
	
	wnoutrefresh(stdscr);
	wnoutrefresh(gameWindow);
	doupdate();
}

void NCursesGraphic::drawGameOverTitle(int win_height, int win_width)
{
	if (win_width / 2 < 42)
	{
		// Small version
		int anchorX = (win_width - gameoverSmall.width) / 2;
		int anchorY = ((win_height - gameoverSmall.height) / 2 - 2);
		for (size_t i = 0; i < gameoverSmall.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4));
			mvwaddstr(gameWindow, anchorY + i, anchorX, gameoverSmall.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4));
		}
	} else {
		// Big version
		int anchorX = (win_width - gameoverBig.width) / 2;
		int anchorY = ((win_height - gameoverBig.height) / 2 - 2);
		
		anchorX = (win_width - gameoverBig.width) / 2;
		anchorY += 2.0f;
		for (size_t i = 0; i < gameoverBig.lines.size(); ++i) {
			wattron(gameWindow, COLOR_PAIR(4));
			mvwaddstr(gameWindow, anchorY + i, anchorX, gameoverBig.lines[i].c_str());
			wattroff(gameWindow, COLOR_PAIR(4));
		}
	}
}

void NCursesGraphic::drawGameOverScreen(const GameState &state, int win_height, int win_width)
{
	int anchorY = ((win_width / 2) < 42) ? win_height / 2 : (win_height / 2) + 5;

	if (state.config.mode != GameMode::SINGLE && state.snake_B) {
		enum Result { A_win, B_win, Draw };
		Result res = Draw;
		
		if (state.snake_A.isDead() && !state.snake_B->isDead())
			res = B_win;
		else if (!state.snake_A.isDead() && state.snake_B->isDead())
			res = A_win;
		else if (state.snake_A.isDead() && state.snake_B->isDead()) {
			if (state.score > state.scoreB)
				res = A_win;
			else if (state.scoreB > state.score)
				res = B_win;
		} else
			res = A_win;
		
		if (res == Draw) {
			if (state.score > state.scoreB)
				res = A_win;
			else if (state.scoreB > state.score)
				res = B_win;
		}
		
		if (state.score == 0 && state.score == state.scoreB)
			res = Draw;
		
		std::string winnerText;
		int winnerColor;
		
		switch (res) {
			case A_win:
				winnerText = "PLAYER 1 WINS";
				winnerColor = 1; // Blue
				break;
			case B_win:
				winnerText = "PLAYER 2 WINS";
				winnerColor = 7; // Yellow
				break;
			case Draw:
				winnerText = "MATCH ENDED IN DRAW";
				winnerColor = 4; // White
				break;
		}
		
		int winnerY = ((win_width / 2) < 42) ? (win_height / 2) - 6 : (win_height / 2) - 8;
		wattron(gameWindow, COLOR_PAIR(winnerColor));
		mvwaddstr(gameWindow, winnerY - 1, (win_width / 2) - (winnerText.length() / 2) - 1, winnerText.c_str());
		wattroff(gameWindow, COLOR_PAIR(winnerColor));
	}

	drawGameOverTitle(win_height, win_width);

	int scoreY = ((win_width / 2) < 42) ? anchorY + 2.5 : anchorY + 4.5;
	
	if (state.config.mode != GameMode::SINGLE && state.snake_B) {
		// p1
		std::string scoreStringA = std::to_string(state.score);
		std::string appleWordA = (state.score == 1) ? "APPLE" : "APPLES";
		
		wattron(gameWindow, COLOR_PAIR(1)); // Blue
		mvwaddstr(gameWindow, scoreY, (win_width / 2) - 11, "PLAYER 1 ");
		wattroff(gameWindow, COLOR_PAIR(1));
		
		wattron(gameWindow, COLOR_PAIR(4)); // White
		mvwaddstr(gameWindow, scoreY, (win_width / 2) - 2, "ATE ");
		wattroff(gameWindow, COLOR_PAIR(4));
		
		wattron(gameWindow, COLOR_PAIR(2)); // Light red/Food color
		mvwaddstr(gameWindow, scoreY, (win_width / 2) + 2, scoreStringA.c_str());
		wattroff(gameWindow, COLOR_PAIR(2));
		
		wattron(gameWindow, COLOR_PAIR(4)); // White
		std::string appleTextA = " " + appleWordA;
		mvwaddstr(gameWindow, scoreY, (win_width / 2) + scoreStringA.length() + 2, appleTextA.c_str());
		wattroff(gameWindow, COLOR_PAIR(4));
		
		// p2
		std::string scoreStringB = std::to_string(state.scoreB);
		std::string appleWordB = (state.scoreB == 1) ? "APPLE" : "APPLES";
		
		int scoreYB = scoreY + 1;
		
		wattron(gameWindow, COLOR_PAIR(7)); // Yellow
		mvwaddstr(gameWindow, scoreYB, (win_width / 2) - 11, "PLAYER 2 ");
		wattroff(gameWindow, COLOR_PAIR(7));
		
		wattron(gameWindow, COLOR_PAIR(4)); // White
		mvwaddstr(gameWindow, scoreYB, (win_width / 2) - 2, "ATE ");
		wattroff(gameWindow, COLOR_PAIR(4));
		
		wattron(gameWindow, COLOR_PAIR(2)); // Light red/Food color
		mvwaddstr(gameWindow, scoreYB, (win_width / 2) + 2, scoreStringB.c_str());
		wattroff(gameWindow, COLOR_PAIR(2));
		
		wattron(gameWindow, COLOR_PAIR(4)); // White
		std::string appleTextB = " " + appleWordB;
		mvwaddstr(gameWindow, scoreYB, (win_width / 2) + scoreStringB.length() + 2, appleTextB.c_str());
		wattroff(gameWindow, COLOR_PAIR(4));
		
		scoreY = scoreYB; // Adjust for controls positioning
	} else {
		// SINGLE PLAYER MODE 
		wattron(gameWindow, COLOR_PAIR(1));
		mvwaddstr(gameWindow, scoreY, (win_width / 2) - 9, "YOU ");
		wattroff(gameWindow, COLOR_PAIR(1));

		wattron(gameWindow, COLOR_PAIR(5));
		std::string scoreString = std::to_string(state.score);
		std::string scoreMessage;
		if (state.score == 1) {
			scoreMessage = "ATE   APPLE";
		}
		else if (state.score == 0) {
			scoreMessage = "ATE   APPLES";
		}
		else if (state.score > 1 && state.score < 9) {
			scoreMessage = "ATE   APPLES";
		} else {
			scoreMessage = "ATE    APPLES";
		}

		mvwaddstr(gameWindow, scoreY, 
				(win_width / 2) - 5, scoreMessage.c_str());

		wattroff(gameWindow, COLOR_PAIR(5));

		wattron(gameWindow, COLOR_PAIR(2));
		mvwaddstr(gameWindow, scoreY, 
				(win_width / 2) - 1, scoreString.c_str());
		wattroff(gameWindow, COLOR_PAIR(2));
	}

	// Retry instruction
	wattron(gameWindow, COLOR_PAIR(4));
	const char *replay = "[ENTER]·······RETRY";
	int controlsY = scoreY + 2;
	mvwaddstr(gameWindow, controlsY,
			(win_width / 2) - 10, replay);
	wattroff(gameWindow, COLOR_PAIR(4));
	
	wattron(gameWindow, COLOR_PAIR(5));
	const char *retryDots = "·······";
	mvwaddstr(gameWindow, controlsY, 
			(win_width / 2) - 3, retryDots);
	wattroff(gameWindow, COLOR_PAIR(5));
	
	// Quit instruction
	wattron(gameWindow, COLOR_PAIR(4));
	const char *libs = "[Q/ESC]········QUIT";
	int libsY = controlsY + 1;
	mvwaddstr(gameWindow, libsY,
			(win_width / 2) - 10, libs);
	wattroff(gameWindow, COLOR_PAIR(4));
	
	wattron(gameWindow, COLOR_PAIR(5));
	const char *quitDots = "········";
	mvwaddstr(gameWindow, libsY, 
			(win_width / 2) - 3, quitDots);
	wattroff(gameWindow, COLOR_PAIR(5));
}

void NCursesGraphic::renderGameOver(const GameState &state, float deltaTime)
{
	(void)deltaTime;
	werase(gameWindow);

	int win_height, win_width;
	getmaxyx(gameWindow, win_height, win_width);

	wattron(gameWindow, COLOR_PAIR(4));
	for (int x = 0; x < win_width - 1; x++)
	{
		for (int y = 0; y < win_height - 1; y++)
		{
			mvwaddch(gameWindow, y, x, ' ');
		}
	}
	wattroff(gameWindow, COLOR_PAIR(4));

	drawBorder();

	drawGameOverScreen(state, win_height, win_width);

	wnoutrefresh(stdscr);
	wnoutrefresh(gameWindow);
	doupdate();
}

Input NCursesGraphic::pollInput() {
	int ch = getch();
	switch (ch) {
		case KEY_UP:    return Input::Up_A;
		case KEY_DOWN:  return Input::Down_A;
		case KEY_LEFT:  return Input::Left_A;
		case KEY_RIGHT: return Input::Right_A;
		case 'w':       return Input::Up_B;
		case 's':       return Input::Down_B;
		case 'a':       return Input::Left_B;
		case 'd':       return Input::Right_B;
		case 'q':       return Input::Quit;
		case 'Q':       return Input::Quit;
		case 27 :		return Input::Quit; // ESC key
		case '1':       return Input::SwitchLib1;
		case '2':       return Input::SwitchLib2;
		case '3':       return Input::SwitchLib3;
		case ' ':		return Input::Pause;
		case '\n':      return Input::Enter; // Enter key (also covers value 10)
		case KEY_ENTER: return Input::Enter; // Alternative Enter key
		default:        return Input::None;
	}
}

void NCursesGraphic::drawGround() {
	wattron(gameWindow, COLOR_PAIR(5) | A_DIM);
	for (int y = 4; y <= height + 3; ++y) {
		for (int x = 0; x <= width; ++x) {
			int screenX = (x * 2) + 4;
			char groundChar = groundPattern[y - 4][x];
			if (groundChar != ' ') {
				mvwaddch(gameWindow, y, screenX, groundChar);
				mvwaddch(gameWindow, y, screenX + 1, ' ');
			} else {
				mvwaddstr(gameWindow, y, screenX, "  ");
			}
		}
	}
	wattroff(gameWindow, COLOR_PAIR(5) | A_DIM);
}

void NCursesGraphic::drawBorder() {
	int win_height, win_width;
	getmaxyx(gameWindow, win_height, win_width);

	wattron(gameWindow, COLOR_PAIR(4));

	// 4 layer border
	const char *layers[] = {"░", "▒", "▓", "█"};
	
	for (int layer = 0; layer < 4; layer++) {
		int offset = 3 - layer;
		const char *pattern = layers[layer];
		
		for (int x = offset; x < win_width - offset; x++) {
			mvwaddstr(gameWindow, offset, x, pattern);
		}
		
		for (int x = offset; x < win_width - offset; x++) {
			mvwaddstr(gameWindow, win_height - 1 - offset, x, pattern);
		}
		
		for (int y = offset + 1; y < win_height - 1 - offset; y++) {
			mvwaddstr(gameWindow, y, offset, pattern);
		}
		
		for (int y = offset + 1; y < win_height - 1 - offset; y++) {
			mvwaddstr(gameWindow, y, win_width - 1 - offset, pattern);
		}
	}

	wattroff(gameWindow, COLOR_PAIR(4));
}

void NCursesGraphic::drawSnake(const GameState &state) {
	wattron(gameWindow, COLOR_PAIR(1));
	for (int i = 0; i < state.snake_A.getLength(); ++i) {
		int y = state.snake_A.getSegments()[i].y + 4;
		int x = (state.snake_A.getSegments()[i].x * 2) + 4;
		
		if (i == 0) {
			mvwaddstr(gameWindow, y, x, "⬢ ");
		} else {
			mvwaddstr(gameWindow, y, x,
				(i == state.snake_A.getLength() - 1) ? "○ " :
				(i % 2 == 0) ? "✛ " : "✲ "
			);
		}
	}
	wattroff(gameWindow, COLOR_PAIR(1));

	if (state.config.mode != GameMode::SINGLE) {
		wattron(gameWindow, COLOR_PAIR(7));
		for (int i = 0; i < state.snake_B->getLength(); ++i) {
			int y = state.snake_B->getSegments()[i].y + 4;
			int x = (state.snake_B->getSegments()[i].x * 2) + 4;
			
			if (i == 0) {
				mvwaddstr(gameWindow, y, x, "⬢ ");
			} else {
				mvwaddstr(gameWindow, y, x,
					(i == state.snake_B->getLength() - 1) ? "○ " :
					(i % 2 == 0) ? "✛ " : "✲ "
				);
			}
		}
		wattroff(gameWindow, COLOR_PAIR(7));
	}
}

void NCursesGraphic::drawFood(const GameState &state) {
	wattron(gameWindow, COLOR_PAIR(2));
	int foodX = (state.food.getPosition().x * 2) + 4;
	mvwaddstr(gameWindow, state.food.getPosition().y + 4, foodX, state.food.getFoodChar());
	wattroff(gameWindow, COLOR_PAIR(2));
}

void NCursesGraphic::generateGroundPattern() {
	groundPattern.resize(height);
	for (int y = 0; y < height; ++y) {
		groundPattern[y].resize(width + 1);
	}
	
	const char groundChars[] = {
		'.', '.', '.', '.', '.',
		',', ',',
		'`', '`',
		';',
		':', ':',
		'-',
		' ', ' ', ' '
	};
	const int numChars = sizeof(groundChars) / sizeof(groundChars[0]);
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> charDist(0, numChars - 1);
	std::uniform_int_distribution<> densityDist(0, 100);
	
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x <= width; ++x) {
			if (densityDist(gen) < 75) {
				groundPattern[y][x] = groundChars[charDist(gen)];
			} else {
				groundPattern[y][x] = ' ';
			}
		}
	}
}

extern "C" IGraphic* createGraphic() {
	return new NCursesGraphic();
}

extern "C" void destroyGraphic(IGraphic* g) {
	delete g;
}