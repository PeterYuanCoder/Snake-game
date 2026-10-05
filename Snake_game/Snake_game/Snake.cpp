#define _CRT_SECURE_NO_WARNINGS

#include"Snake.h"

void SetPos(short x, short y)
{
	//获得标准输出设备的句柄
	HANDLE houtput = NULL;
	houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	//定位光标的位置
	COORD pos = { x,y };
	SetConsoleCursorPosition(houtput, pos);
}

void WelcomeToGame()
{
	SetPos(40, 14);
	wprintf(L"欢迎来到贪吃蛇小游戏\n");
	SetPos(42, 15);
	system("pause");
	system("cls");   //清空屏幕
	SetPos(25, 14);
	wprintf(L"用↑.↓.←.→来控制蛇的移动，按F3加速，按F4减速\n");
	SetPos(25, 15);
	wprintf(L"加速能够得到更高的分数\n");
	SetPos(42, 20);
	system("pause");
	system("cls");
}

void CreateMap()
{
	//上
	for (int i = 0; i < 29; i++)
	{
		wprintf(L"%lc", L'■');
	}
	//下
	SetPos(0, 26);
	for (int i = 0; i < 29; i++)
	{
		wprintf(L"%lc", L'■');
	}
	//左
	for (int i = 1; i <= 25; i++)
	{
		SetPos(0, i);
		wprintf(L"%lc", L'■');
	}
	//右
	for (int i = 1; i <= 25; i++)
	{
		SetPos(56, i);
		wprintf(L"%lc", L'■');
	}
}


void InitSnake(pSnake ps)
{
	pSnakeNode cur = NULL;
	for (int i = 0; i < 5; i++)
	{
		//开辟空间
		cur = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (cur == NULL)
		{
			perror("InitSnake()::malloc()");
			return;
		}
		//设置坐标
		cur->next = NULL;
		cur->x = POS_X + 2 * i;
		cur->y = POS_Y;

		//头插法
		if (ps->_pSnake == NULL)
		{
			ps->_pSnake = cur;
		}
		else
		{
			cur->next = ps->_pSnake;
			ps->_pSnake = cur;
		}
	}

	//遍历打印蛇身
	cur = ps->_pSnake;
	while (cur)
	{
		//定位蛇头
		SetPos(cur->x, cur->y);
		wprintf(L"%lc",BODY);
		cur = cur->next;
	}

	//初始化蛇的属性
	ps->_sleep_time = 200;//毫秒
	ps->_food_weight = 10;
	ps->_score = 0;
	ps->_status = OK;
	ps->_dir = RIGHT;
}


void CreateFood(pSnake ps)
{
	//先确定范围
	int x = 0;
	int y = 0;
again:
	do
	{
		x = rand() % 53 + 2;
		y = rand() % 25 + 1;
	} while (x % 2 != 0);

	//蛇身和食物不冲突
	pSnakeNode cur = ps->_pSnake;
	while (cur)
	{
		if (cur->x == x && cur->y == y)
		{
			goto again;
		}
		cur = cur->next;
	}

	pSnakeNode pFood = (pSnakeNode)malloc(sizeof(SnakeNode));//创建食物结点
	if (pFood == NULL)
	{
		perror("CreateFood::malloc()");
		return;
	}
	else
	{
		pFood->x = x;
		pFood->y = y;
		SetPos(pFood->x, pFood->y);
		wprintf(L"%c", FOOD);
		ps->_pFood = pFood;
	}
}


void GameStart(pSnake ps)
{
	//0. 设置窗口的大小
	system("mode con cols=100 lines=30");
	system("title 贪吃蛇");

	//1. 光标隐藏
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);   //创建句柄
	CONSOLE_CURSOR_INFO CursorInfo;    //创建光标结构体
	GetConsoleCursorInfo(houtput, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(houtput, &CursorInfo);//设置控制台光标状态

	//2. 打印欢迎界面和功能介绍
	WelcomeToGame();

	//3. 地图绘制
	CreateMap();

	//4. 初始化蛇身
	InitSnake(ps);

	//5. 创建食物
	CreateFood(ps);
}

void PrintHelpInfo()
{
	
	SetPos(64, 13);
	wprintf(L"%ls", L"不能撞墙，不能咬到自己\n");
	SetPos(64, 14);
	wprintf(L"%ls", L"用↑.↓.←.→来控制蛇的移动\n");
	SetPos(64, 15);
	wprintf(L"%ls", L"按F3加速，按F4减速\n");
	SetPos(64, 16);
	wprintf(L"%ls", L"按ESC退出游戏，按空格暂停游戏\n");
}

void pause()
{
	while (1)
	{
		Sleep(200);
		if (KEY_PRESS(VK_SPACE))
		{
			break;
		}
	}
}

int NextIsFood(pSnakeNode pNextNode, pSnake ps)
{
	if (ps->_pFood->x == pNextNode->x && ps->_pFood->y == pNextNode->y)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void EatFood(pSnakeNode pNextNode, pSnake ps)
{
	//头插法
	pNextNode->next = ps->_pSnake;
	ps->_pSnake = pNextNode;

	//打印蛇身
	pSnakeNode cur = ps->_pSnake;
	while (cur)
	{
		SetPos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	ps->_score += ps->_food_weight;
	//释放食物结点
	free(ps->_pFood);
	ps->_pFood = NULL;
	//创建新的食物
	CreateFood(ps);
}

void NoFood(pSnakeNode pNextNode, pSnake ps)
{
	//头插
	pNextNode->next = ps->_pSnake;
	ps->_pSnake = pNextNode;

	//打印蛇身
	pSnakeNode cur = ps->_pSnake;
	while (cur->next->next)
	{
		SetPos(cur->x, cur->y);
		wprintf(L"%lc", BODY);
		cur = cur->next;
	}
	//最后一个结点位置打印空格，并释放结点
	SetPos(cur->next->x, cur->next->y);
	printf("  ");
	free(cur->next);
	cur->next = NULL;

}

void KILLBYWALL(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake;
	if (cur->x == 0 || cur->x == 56 || cur->y == 0 || cur->y == 26)
	{
		ps->_status = KILL_BY_WALL;
	}
}

void KILLBYSELF(pSnake ps)
{
	pSnakeNode cur = ps->_pSnake->next;
	while (cur)
	{
		if (cur->x == ps->_pSnake->x && cur->y == ps->_pSnake->y)
		{
			ps->_status = KILL_BY_SELF;
			break;
		}
		cur = cur->next;
	}
}

void SnakeMove(pSnake ps)
{
	pSnakeNode pNextNode = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (pNextNode == NULL)
	{
		perror("SnakeMove()::malloc()");
		return;
	}
	switch (ps->_dir)
	{
	case UP:
		pNextNode->x = ps->_pSnake->x;
		pNextNode->y = ps->_pSnake->y - 1;
		break;
	case DOWN:
		pNextNode->x = ps->_pSnake->x;
		pNextNode->y = ps->_pSnake->y + 1;
		break;
	case LEFT:
		pNextNode->x = ps->_pSnake->x - 2;
		pNextNode->y = ps->_pSnake->y;
		break;
	case RIGHT:
		pNextNode->x = ps->_pSnake->x + 2;
		pNextNode->y = ps->_pSnake->y;
		break;
	}

	//如果下一个结点是食物
	if (NextIsFood(pNextNode, ps))
	{
		//吃掉食物
		EatFood(pNextNode, ps);
	}
	else
	{
		NoFood(pNextNode, ps);
	}
	
	//每走一步都需要检测是否撞墙或者是撞到自己
	//撞墙
	KILLBYWALL(ps);

	//撞到自己
	KILLBYSELF(ps);
}




void GameRun(pSnake ps)
{
	//打印帮助信息
	PrintHelpInfo();
	//按键状态
	do
	{
		SetPos(64, 10);
		printf("总得分：%d", ps->_score);
		SetPos(64, 11);
		printf("一个食物的分数：%2d", ps->_food_weight);
		if (KEY_PRESS(VK_UP) && ps->_dir != DOWN)
		{
			ps->_dir = UP;
		}
		else if (KEY_PRESS(VK_DOWN) && ps->_dir != UP)
		{
			ps->_dir = DOWN;
		}
		else if (KEY_PRESS(VK_LEFT) && ps->_dir != RIGHT)
		{
			ps->_dir = LEFT;
		}
		else if (KEY_PRESS(VK_RIGHT) && ps->_dir != LEFT)
		{
			ps->_dir = RIGHT;
		}
		else if (KEY_PRESS(VK_SPACE))
		{
			//暂停
			pause();
		}
		else if (KEY_PRESS(VK_ESCAPE))
		{
			ps->_status = EMD_NORMAL;
			break;
		}
		else if (KEY_PRESS(VK_F3))
		{
			//加速
			if (ps->_sleep_time > 80)
			{
				ps->_sleep_time -= 30;
				ps->_food_weight += 2;
			}
		}
		else if (KEY_PRESS(VK_F4))
		{
			//减速
			if (ps->_food_weight > 2)
			{
				ps->_food_weight -= 2;
				ps->_sleep_time += 30;
			}
		}
		Sleep(ps->_sleep_time);//休息时间
		SnakeMove(ps);//走一步

	} while (ps->_status == OK);
}


void GameEnd(pSnake ps)
{
	SetPos(24, 14);
	switch (ps->_status)
	{
	case KILL_BY_SELF:
		printf("你撞到自己了，游戏结束\n");
		break;
	case KILL_BY_WALL:
		printf("你撞到墙了，游戏结束\n");
		break;
	case EMD_NORMAL:
		printf("你主动结束游戏\n");
		break;
	}

	//释放蛇身链表
	pSnakeNode cur = ps->_pSnake;
	while (cur)
	{
		pSnakeNode del = cur->next;
		free(cur);
		cur = del;
	}
	ps->_pSnake = NULL;
	free(ps->_pFood);
	ps->_pFood = NULL;
}