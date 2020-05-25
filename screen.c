//-lib ncurses3
#include <curses.h>

//打开字符屏幕
void open_screen(void)
{
   initscr();//初始化字符窗口屏幕
   cbreak();
   noecho();
   keypad(stdscr,TRUE);

}

//关闭字符屏幕
void close_screen(void)
{
	endwin();
}
