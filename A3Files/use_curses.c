/* name:               Chaitali Patel
 * ONE Card number:    1404270
 * Unix id:            cpatel1
 * lecture section:    B1
 * instructor's name:  Jacqueline Smith
 * lab section:        H01
 * TA's name:          Nicolas Barriga, Shida He
 *
 * sets up curses and also ends curses
 */


#include <curses.h>
#include <stdio.h>
#include <string.h> /* For memset() */
#include <math.h>
#include <stdlib.h> /* For exit() */
#include "use_curses.h"




void start_curses() {

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  refresh();
}

void end_curses() {

  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  endwin();

}

