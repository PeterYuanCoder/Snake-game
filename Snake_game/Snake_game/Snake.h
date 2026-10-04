#pragma once

#include <windows.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>


#define POS_X 24
#define POS_Y 5
#define FOOD L'◆'
#define KEY_PRESS(VK)  ((GetAsyncKeyState(VK)&0x1) ? 1 : 0)

//结构定义
//蛇的方向
enum DIRECTION
{
	UP = 1,//上
	DOWN,//下
	LEFT,//左
	RIGHT//右
	
};

//游戏的状态
enum GAME_STATUS
{
	OK,            //正常运行
	KILL_BY_WALL,  //撞墙
	KILL_BY_SELF,  //自己撞自己
	EMD_NORMAL    //正常退出
};


//贪吃蛇的结点定义
typedef struct SnakeNode
{
	int x;
	int y;
	struct SnakeNode* next;
}SnakeNode,*pSnakeNode;

//贪吃神
typedef struct Snake
{
	pSnakeNode _pSnake;  //指向蛇头的指针
	pSnakeNode _pFood;   //指向食物的指针
	enum DIRECTION _dir;   //使用枚举来列举蛇的方向
	enum GAME_STATUS _status;  //游戏的状态
	int _food_weight;      //一个食物的分数
	int _score;             //总分数
	int _sleep_time;         //休息时间
}Snake,*pSnake;

//函数声明

//游戏的初始化
void GameStart(pSnake ps);

//欢迎界面的打印
void WelcomeToGame();

//创建地图
void CreateMap();

//初始化蛇身
void InitSnake(pSnake ps);

//创建食物
void CreateFood(pSnake ps);


//运行游戏
void GameRun(pSnake ps);