/*寻找图片中的最小路径的问题，张文硕-2206110686-自动化03*/

#include<stdio.h>		//调用内部包
#include<malloc.h>		//调用内存分配文件包
#include<string.h>
#include "map.h"		//调用map文件,其中存放了图的基本信息和宏定义信息,方便之后对图片文件进行跟换，只需更改InitMap即可。

typedef int PathMatrix[MaxNum][MaxNum+change];		//定义路径矩阵
typedef struct {
	int weight[MaxNum];								//定义最短路径长度和
	Status Visited[MaxNum];							//定义列表标记是否被访问过
}ShortPathList;

//判断入参数量是否正确
//如果参数为3，		则认为没有错误
//如果参数不为3，	则认为有错误
Status IfError_01(int argc)
{
	if (argc == 3) return FALSE;
	else 
	{
		printf("ERROR_01");
		return TRUE;
	}
}

//寻找输入的地点在地点列表中的位置
//如果找到了，则返回下标
//如果找不到，则返回ERROR_02
int FindPlace(const char* PlaceName[], char* name,Graph* map)
{
	if (!PlaceName || !name|| !map) return ERROR;					//鲁棒性检测
	int i = 0;
	while (i < map->vexnum)											//进行寻址
	{
		if (strcmp(PlaceName[i], name) == 0) return i;
		i++;
	}
	printf("ERROR_02");												//当跳出循环还没返回答案时，认为出错了
	return ERROR_02;
}

//定义函数初始化最短路径列表
//所有visit状态更新为FALSE
//所有权重进行处理，除自己到自己之外全部为无穷大，方便之后修改
Status InitList(ShortPathList* List,Graph map,int start)
{
	if (!List) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		List->Visited[i] = FALSE;
		List->weight[i] = InAccessible;
	}
	List->weight[start] = 0;
	return OK;
}
//功能：让path矩阵的每行第一列表示路径长度.
//每行第一列全部初始化为FALSE
Status InitPath(PathMatrix& path,Graph map,int start)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		for (int j = 0; j < map.vexnum + change; j++)
		{
			path[i][j] = none;						//将所有的长度初始化为-1
		}
	}
	path[start][0] = 1;
	path[start][1] = start;
	return OK;
}
//功能：更新路径，使得路径更小
//输入：使得路径更新为到起点距离更小的路径，更小路径结点横坐标为now，原先的路径横坐标结点为before
//输出：状态
Status UpdatePath(PathMatrix &path,int before,int now,Graph map)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		path[before][i] = path[now][i];				//将小的直接抄过来
	}
	return OK;
}

//功能：修改结点到起点的路径长度最小值，并调用函数UpdatePath更新每个结点的最短路径。
//结果：如果新的长度小于原有长度，则进行更新。跟新后最后一个结点路径指向被更新的结点。
//                                           总的长度进行加一运算，同时对权重也进行更新。
Status ChangeWeight(Graph map, ShortPathList* list, int curnode, PathMatrix& path)
{
	if (!list||!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] > list->weight[curnode] + map.arcs[curnode][i])
			{
				UpdatePath(path, i, curnode,map);										//当满足条件时更新路径
				path[i][PLen]++;														//总的路径长度path[i][0]加一
				path[i][path[i][PLen]] = i;												//最后一步补上当前的位置
				list->weight[i] = list->weight[curnode] + map.arcs[curnode][i];			//将权重更新为较小的值
			}
		}
	}
	return OK;
}
//功能：寻找最短的路径的结点并修改访问状态
//输入：list存放有每个节点到起点的距离；map用于存放图的信息
//输出：最小的所处的位置；
int FindShort(ShortPathList* list,Graph map)
{
	if (!list) return ERROR;
	int shortest=none;
	int flag = TRUE;
	int i = 0;
	while ( i < map.vexnum && flag)								//寻找第一个未被访问的结点
	{
		if (list->Visited[i] == FALSE)
		{
			shortest = i;
			flag =FALSE;
		}
		i++;
	}
	for (int i = 0; i < map.vexnum; i++)						//寻找所有节点中未被访问的最小长度结点
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] < list->weight[shortest])
			{
				shortest = i;
			}
		}
	}
	//if (shortest != none) list->Visited[shortest] = TRUE;		//更改访问状态
	return shortest;											//返回角标，如果角标是none，认为所有都访问完了，可以给结果了
}
//函数功能：求出指定起点到终点的最短路径，并且存放最短路径
/*输入：map		已经初始化完成的有向图
		start	起点序号
		end		终点序号
		list	存放最短路径长度的数组
		path	用于记录脚步的
  返回值：返回从start到end的最短路径权值*/
int DJT_ShortPath(Graph map, PathMatrix& path, ShortPathList* list, int start, int end)
{
	if (!path || ! list) return ERROR;
	InitList(list,map,start);
	InitPath(path, map,start);
	int cursor;
	while (FindShort(list, map) != none)
	{
		cursor = FindShort(list, map);
		list->Visited[cursor] = TRUE;
		ChangeWeight(map, list, cursor, path);
	}
	return list->weight[end];
}

Status PrintPath(PathMatrix path, int end,const char** PlaceName)
{
	if (!PlaceName) return ERROR;
	int flag = TRUE;
	for (int i = 0; i < path[end][0]; i++)
	{
		if (flag)
		{
			printf("\n%s", PlaceName[path[end][i + change]]);
			flag = FALSE;
		}
		else
		{
			printf("->%s", PlaceName[path[end][i + change]]);
		}
	}
	return OK;
}
int main(int argc, char* argv[])
{
	if(IfError_01(argc)) return ERROR_01;
	Graph* map = (Graph*)malloc(sizeof(Graph));
	if (!map) return OVERFLOW;
	InitMap(map);
	PathMatrix path = { none };														//定义记录路径的矩阵
	ShortPathList* list=(ShortPathList*) malloc(sizeof(ShortPathList));				//定义所有结点到起点的路径长度，
	if (!list) return OVERFLOW;								
	int start = FindPlace(PlaceName, argv[1], map);			//起始地点的坐标
	if (start == ERROR_02) return ERROR_02;
	int end = FindPlace(PlaceName, argv[2], map);			//终止地点的坐标
	if (end == ERROR_02) return ERROR_02;
	int shortestdistance = 0;
	shortestdistance=DJT_ShortPath(*map, path, list, start, end);					//寻找最小路径长度并将其存放进shortestdistance
	printf("%d", shortestdistance);
	PrintPath(path,end,PlaceName);
	free(map);																		//释放map指针
	free(list);																		//释放list指针
	return 0;
}

