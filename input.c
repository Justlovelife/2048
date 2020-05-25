#include <curses.h>
#include "input.h"

//声明一个外部变量，表示这个变量是其他文件中定义的
extern WINDOW *w_2048;

int get_user_input(void)
{
    keypad(w_2048,TRUE);
	int ch=wgetch(w_2048);
	keypad(w_2048,FALSE);
	
    
	if(ch=='q'||ch=='Q')
	{
		return QUIT;
	}
	else if(ch=='a'||ch==KEY_LEFT)
	{
		return LEFT;
	}
	else if(ch=='d'||ch==KEY_RIGHT)
	{
		return RIGHT;
	}
		
	else if(ch=='s'||ch==KEY_DOWN)
	{
		return DOWN;
	
	}
	else if(ch=='w'||ch==KEY_UP)
	{
		return UP;
	}
	else if(ch=='1')

	{
		return FIRST;
	}
	else if(ch=='2')

	{
		return NEXT;
	}
	else if(ch=='3')

	{
		return THEN;
	}
	else if(ch=='4')

	{
		return LAST;
	}
	else
	{
		return UNKNOW;
	}
	

}