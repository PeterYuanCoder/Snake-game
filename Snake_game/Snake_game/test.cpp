#define _CRT_SECURE_NO_WARNINGS


#include <locale.h>
#include "Snake.h"

//完成游戏的测试逻辑
void test()
{
	int ch = 0;
	int c = 0;
	do
	{
		system("cls");
		//创建贪吃蛇
		Snake snake = { 0 };
		//初始化游戏
		//1. 打印欢迎界面
		//2. 功能介绍
		//3. 绘制地图
		//4. 创建蛇
		//5. 创建食物
		GameStart(&snake);

		//运行游戏
		GameRun(&snake);
		//结束游戏--善后工作
		GameEnd(&snake);

		//丢弃游戏期间残留在控制台输入队列里的按键记录
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

		SetPos(20, 15);
		printf("再来一局吗?(Y/N):");
		ch = getchar();
		//清行尾 + 防 EOF 死循环
		while ((c = getchar()) != '\n' && c != EOF);
	} while (ch == 'Y' || ch == 'y');
	SetPos(0, 27);
}

int main()
{
	//设置适配本地环境，这样我们就可以打印宽字符。
	setlocale(LC_ALL, "");
	srand((unsigned int)time(NULL));
	test();
	return 0;
}