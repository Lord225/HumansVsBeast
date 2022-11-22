#include "utils.h"
#include <ncurses.h>

void init_screen(void) {

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    start_color();

    init_pair(PLAYER_COLOR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(MONEY_COLOR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CAMP_COLOR, COLOR_WHITE, COLOR_GREEN);
    init_pair(BEAST_COLOR, COLOR_WHITE, COLOR_RED);

    keypad(stdscr, TRUE);

}
void done_screen(void) {
    endwin();
}
