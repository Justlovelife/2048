#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include <unistd.h>

//提示窗口相关参数
WINDOW *wel=NULL;
int wel_lines=2;      //提示窗口的行数
int wel_columns=40;   //提示窗口的列数
int wel_y0=0;         //提示窗口左上顶点的y坐标
int wel_x0=20;        //提示窗口右上顶点的x坐标

//游戏数字窗口
WINDOW *w_2048=NULL;
int w_2048_lines=4;
int w_2048_coluwns=24;
int w_2048_y0=3;
int w_2048_x0=20;

//得分窗口
WINDOW *w_score=NULL;
int w_score_lines=2;
int w_score_coluwns=24;
int w_score_y0=10;
int w_score_x0=20;

//结束窗口
WINDOW *w_over=NULL;
int w_over_lines=4;
int w_over_coluwns=24;
int w_over_y0=14;
int w_over_x0=20;



#define ITEM_NUM 4

int matrix_2048[ITEM_NUM][ITEM_NUM]=
{
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
};
//删除行
void Del(int num)
{
  int i; 
  for(i=0;i<ITEM_NUM;i++)
  {
	 if(matrix_2048[num][i]!=0)
	 	matrix_2048[num][i]=0;
	 	
  }
}
//获取0的个数
int get_zero_num()
{
	int i,j;
	int n=0;//计数
	for(i=0;i<ITEM_NUM;i++)
	{
		for(j=0;j<ITEM_NUM;j++)
		{
			if(matrix_2048[i][j]==0)
				n++;
		}
	}
	return n;
}

//在空白位置上随机填充一个数字
void fill_a_number(void)
{
	int zero_num=get_zero_num();
	if(zero_num==0)
	{
	  return;
	}
	int pos=random()%zero_num+1;  //范围是【1~zero_num】
	int n=0;//第几个0
	int i,j;	
	for(i=0;i<ITEM_NUM;i++)
	{
		for(j=0;j<ITEM_NUM;j++)
		{
			if(matrix_2048[i][j]==0)
			{
				n++;
				if(n==pos)
				{
					//要填充的位置就找到了
					int a[4]={2,2,4,8};
					int index=random()%4;
					matrix_2048[i][j]=a[index];
					return;

				}					
			}											
		}
	}	
}


//初始化游戏界面需要用到的窗口
void init_windows()
{
      start_color();
   	init_pair(1,COLOR_RED,COLOR_BLACK);
	//1.创建一个提示（欢迎）字符
	wel=newwin(wel_lines,wel_columns,wel_y0,wel_x0);
	wmove(wel,0,0);
	wattron(wel,COLOR_PAIR(1));
	waddstr(wel,"welcome to game2048");
	
	wmove(wel,1,3);
	waddstr(wel,"Author:liyingzhi");
	wrefresh(wel);
	
	

	//2.创建一个游戏数字字符窗口
	w_2048=newwin(w_2048_lines,w_2048_coluwns,w_2048_y0,w_2048_x0);

	//3.显示最高得分
     w_score=newwin(w_score_lines,w_score_coluwns,w_score_y0,w_score_x0);

	 //4.结束窗口
	  w_over=newwin(w_over_lines,w_over_coluwns,w_over_y0,w_over_x0);
	
}

void del_windows()
{
    delwin(wel);     //删除欢迎框
	delwin(w_2048);  //删除字符框
	delwin(w_score); //删除得分框

}


//在非0位置随机删除一个数字
void delete_number(int del)
{
	int zero_num=get_zero_num();
	int i,j;
	if(zero_num==0)
	{
	  return;
	}
    for(i=0;i<ITEM_NUM;i++)
	{
		for(j=0;j<ITEM_NUM;j++)
		{

			if (del==j)
			{
				matrix_2048[i][j]=0;

		    }
			
		}
    }
}



//根据矩阵matrix_2048[][]的内容，在游戏中显示
void show_matrix(void)
{
	int i,j;
	int max=matrix_2048[0][0];//最高得分
	
	for(i=0;i<ITEM_NUM;i++)
	{
		for(j=0;j<ITEM_NUM;j++)
		{
          
			//移动光标wmove
		    wmove(w_2048, i, j*6);

			//添加要显示的字符串           int转换成char
		   char buf[6];
           if(matrix_2048[i][j]==0)
           	{
		   		sprintf(buf,"%5s","_");
           	}
		   else
		   	{
			   	sprintf(buf,"%5d",matrix_2048[i][j]);
		   	}

		   waddstr(w_2048,buf);

		   
		   if(matrix_2048[i][j]>max)
			{
				max=matrix_2048[i][j];
			}
		   
	   }
		

	}

	//刷新窗口
	wrefresh(w_2048);

	//显示得分
	wmove(w_score,0,6);

	char buf1[24];
	sprintf(buf1,"score:%5d",max);
	waddstr(w_score,buf1);
	
	//刷新窗口 wrefresh
	wrefresh(w_score);
}

void move_right(void)
{
   int i;  
	for(i=0;i<ITEM_NUM;i++)
	{
		int x=0;//辅助存储非0值进行比较
		int m=ITEM_NUM-1;//指向这一行最后面（要被覆盖）的位置
		int k=0;//哨兵，从右往左，找下一个非0元素 a[i][k],和x进行比较
		for(k=ITEM_NUM-1;k>=0;k--)
		{
			if(matrix_2048[i][k]!=0)
			{
							 
				if(x==0)
				{
					x=matrix_2048[i][k];
					matrix_2048[i][k]=0;
					
				}
				else
				{
				   //找到两个非0值，进行比较
					if(matrix_2048[i][k]==x)
					{  
					 	//需要合并,后移
					 	x=x+matrix_2048[i][k];   //合并的值
						matrix_2048[i][k]=0;     //前移后的数字为0
						matrix_2048[i][m--]=x;   //合并的值后移

						x=0;//x已经合并后了，x要重新清0

					}
					
					else
					{
					    
						matrix_2048[i][m--]=x;
						x=matrix_2048[i][k];
						matrix_2048[i][k]=0;
						
					}
						
				}
				
			
			}
		}
         

		matrix_2048[i][m--]=x;
	 }
	

}

void move_up(void)
{   
	
	int i;  
	for(i=0;i<ITEM_NUM;i++)
	{
		int x=0;//辅助存储非0值进行比较
		int m=0;//指向这一行最前面（要被覆盖）的位置
		int k=0;//哨兵，从上往下，找下一个非0元素 a[i][k],和x进行比较		
		for(k=0;k<ITEM_NUM;k++)
		{
			if(matrix_2048[k][i]!=0)
			{
								 
				if(x==0)
				{
					x=matrix_2048[k][i];
					matrix_2048[k][i]=0;						
				}
				else
				{
				   //找到两个非0值，进行比较
					if(matrix_2048[k][i]==x)
					{  
						//需要合并,前移
						x=x+matrix_2048[k][i];	 //合并的值
						matrix_2048[k][i]=0;	 //前移后的数字为0
						matrix_2048[m++][i]=x;	 //合并的值前移
						x=0;//x已经合并前移了，x要重新清0
					}
						
					else
					{
						matrix_2048[m++][i]=x;
						x=matrix_2048[k][i];
						matrix_2048[k][i]=0;
						
					}
							
				}
					
				
			}
		}
			 
		matrix_2048[m++][i]=x;
	}
	
}

void move_dowm(void)
{
		
	int i;  
	for(i=0;i<ITEM_NUM;i++)
	{
		int x=0;//辅助存储非0值进行比较
		int m=ITEM_NUM-1;//指向这一行最前面（要被覆盖）的位置
		int k=0;//哨兵，从下往上，找下一个非0元素 a[i][k],和x进行比较		
		for(k=ITEM_NUM-1;k>=0;k--)
		{
			if(matrix_2048[k][i]!=0)
			{
								 
				if(x==0)
				{
					x=matrix_2048[k][i];
					matrix_2048[k][i]=0;						
				}
				else
				{
				   //找到两个非0值，进行比较
					if(matrix_2048[k][i]==x)
					{  
						//需要合并,前移
						x=x+matrix_2048[k][i];	 //合并的值
						matrix_2048[k][i]=0;	 //前移后的数字为0
						matrix_2048[m--][i]=x;	 //合并的值前移
						x=0;//x已经合并前移了，x要重新清0
					}
						
					else
					{
						matrix_2048[m--][i]=x;
						x=matrix_2048[k][i];
						matrix_2048[k][i]=0;
						
					}
							
				}
					
				
			}
		}
			 
		matrix_2048[m--][i]=x;
	}
	
}
void move_left(void)
{
	
	int i;
    
	for(i=0;i<ITEM_NUM;i++)
	{
		int x=0;//辅助存储非0值进行比较
		int m=0;//指向这一行最前面（要被覆盖）的位置
		int k=0;//哨兵，从左至右，找下一个非0元素 a[i][k],和x进行比较

	
		for(k=0;k<ITEM_NUM;k++)
		{
			if(matrix_2048[i][k]!=0)
			{
							 
				if(x==0)
				{
					x=matrix_2048[i][k];
					matrix_2048[i][k]=0;
					
				}
				else
				{
				   //找到两个非0值，进行比较
					if(matrix_2048[i][k]==x)
					{  
					 	//需要合并,前移
					 	x=x+matrix_2048[i][k];   //合并的值
						matrix_2048[i][k]=0;     //前移后的数字为0
						matrix_2048[i][m++]=x;   //合并的值前移

						x=0;//x已经合并前移了，x要重新清0

					}
					
					else
					{
						matrix_2048[i][m++]=x;
						x=matrix_2048[i][k];
						matrix_2048[i][k]=0;
						
					}
						
				}
				
			
			}
		}
       

		matrix_2048[i][m++]=x;
	}
	

	
}



int change_matrix(int mv)
{
   
   int matrix_2048_bak[4][4];//备份矩阵
   int i,j;
   //1.把matrix_2048备份到matrix_2048_bak
   for(i=0;i<ITEM_NUM;i++)
   {
		for(j=0;j<ITEM_NUM;j++)
		{
			matrix_2048_bak[i][j]=matrix_2048[i][j];
			
			
		}
   }

   //2.根据移动方向 变换矩阵
   if(mv==LEFT)
   {
		move_left();
   }
   else if(mv==RIGHT)
   {
    	move_right();
   }
    else if(mv==UP)
   {
    	move_up();
   }
	else if(mv==DOWN)
	{
		move_dowm();
	}
	   else if(mv==FIRST)
	{
	   int num=0;
		Del(0);
	}
	else if(mv==NEXT)
	{
	   int num=1;
		Del(1);
	}
	else if(mv==THEN)
	{
	   int num=2;
		Del(2);
	}
	else if(mv=LAST)
	{
	   int num=3;
		Del(3);
	}
	  
	
	
   //3.比较矩阵matrix_2048 和备份矩阵matrix_2048_bak
   //根据比较的结果返回1 or 0
   for(i=0;i<ITEM_NUM;i++)
   {
		for(j=0;j<ITEM_NUM;j++)
		{
			if(matrix_2048_bak[i][j]!=matrix_2048[i][j])
			{
				return 1;

			}
			
		}
   }
   return 0;
   
}





//返回1，表示游戏结束，返回0，表示游戏继续
int game_is_over(void)
{
	if(get_zero_num()>0)
    {
		return 0;
	}

	int i,j;
	for(i=0;i<ITEM_NUM;i++)
	{
		for(j=0;j<ITEM_NUM;j++)
		{
			//右边的那个元素存在且相等
			if(j<ITEM_NUM-1 && matrix_2048[i][j]==matrix_2048[i][j+1])
			{
				return 0;
			}
			//下边那个元素存在且相等
			if (i<ITEM_NUM-1 && matrix_2048[i][j]==matrix_2048[i+1][j])
			{
				return 0;		
			}

		}
	}
	return 1;//相邻且不相等
}


//整个2048游戏
void game_2048(void)
{
     init_windows();
	 fill_a_number();
     show_matrix();
	   
     
	while(1)
	{
       //1.等待用户输入
      int mv=get_user_input();
	   if(mv==QUIT)
	  {
	   		break;

	   }
	 
	 //如果矩阵有改变，填充一个数字
	  
	  if(change_matrix(mv))
	  {
	     fill_a_number();
		 show_matrix();
         
	  }
	
	  
	   

	   //4.判断游戏是否结束
	   if(game_is_over()==1)
	   {
	    
	   		   wmove(w_over,0,0);
			waddstr(w_over,"game over");
			
		
			
			wrefresh(w_over);
			
			break;
	   }
	}
	
	//game over
	sleep(15);  //linux c语言中单位是s， windows c语言中单位是ms
	del_windows();
}


