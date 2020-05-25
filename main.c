#include <stdio.h>
#include "screen.h"
#include "2048.h"
#include <time.h>
#include <stdlib.h>
int main()
{

    //设置随机数种子
    srandom(time(NULL));
	
	//1.创建（初始化）我们的字符窗口屏幕
	open_screen();
	
	//2.进入2048游戏主循环
	game_2048();
	
	//3.关闭字符屏幕
	close_screen();
	
	return 0;
}
