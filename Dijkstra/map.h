#pragma once
#ifndef _HEAD_H_
#define _HEAD_H_
#define InAccessible   100000		//无法抵达时的总长度
#define Status int
#define OK 2
#define TRUE 1
#define change 1					//用于扩大1位，进行占位和存放
#define FALSE 0
#define PLen 0
#define none -1
#define ERROR -1					//用于判定是否出现错误
#define OVERFLOW -2					//用于判断是否分配成功
#define ERROR_01 -3					//入参不正确
#define ERROR_02 -4					//无法生成虽短路径	
#define MaxNum 20					//最大支持的存放的节点数目


//图的数据结构
typedef struct GRAPH
{
	int vexs[MaxNum];				//定义结点矩阵，用于存放所有的结点
	int arcs[MaxNum][MaxNum];		//定义邻接矩阵，用于之后的计算和分析
	int vexnum;						//总的节点数目
	int arcnum;						//总的边的数目
}Graph;								//图的结构体的名称


//所有的地点名称及其所对应的在图中顶点的脚标
extern const char* PlaceName[13] = { "北门","饮水思源","传送门3","传送门1","腾飞塔","图书馆","传送门2","传送门4","教学主楼","宪梓堂","西迁馆","活动中心","南门" };
									 //0        1          2         3         4        5        6         7         8         9         10       11       12

//对图片进行初始化
extern Status InitMap(Graph* map)
{
	if (!map) return ERROR;									//鲁棒性检测
	map->vexnum = 13;										//所有结点的个数
	map->arcnum = 18;										//所有边的条数
	//初始化所有的结点和边
	int i = 0, j = 0;				
	for (i = 0; i < map->vexnum; ++i) {
		map->vexs[i] = i;
		for (j = 0; j < map->vexnum; ++j)					//对角线上初始化为0
			if (i == j) map->arcs[i][j] = 0;
			else {
				map->arcs[i][j] = InAccessible;				//全部先初始化为不连通
			}
	}
	//初始化邻接矩阵
	map->arcs[0][2] = map->arcs[2][0] = 22;
	map->arcs[0][1] = map->arcs[1][0] = 18;
	map->arcs[1][3] = map->arcs[3][1] = 27;
	map->arcs[1][4] = map->arcs[4][1] = 19;
	map->arcs[4][2] = map->arcs[2][4] = 4;
	map->arcs[4][7] = map->arcs[7][4] = 32;
	map->arcs[3][5] = map->arcs[5][3] = 4;
	map->arcs[5][6] = map->arcs[6][5] = 4;
	map->arcs[4][5] = map->arcs[5][4] = 23;
	map->arcs[5][8] = map->arcs[8][5] = 15;
	map->arcs[8][11] = map->arcs[11][8] = 21;
	map->arcs[11][12] = map->arcs[12][11] = 30;
	map->arcs[7][9] = map->arcs[9][7] = 4;
	map->arcs[8][9] = map->arcs[9][8] = 8;
	map->arcs[6][12] = map->arcs[12][6] = 21;
	map->arcs[6][10] = map->arcs[10][6] = 43;
	map->arcs[10][12] = map->arcs[12][10] = 20;
	map->arcs[9][12] = map->arcs[12][9] = 14;
	return OK;
}
extern Graph;
#endif