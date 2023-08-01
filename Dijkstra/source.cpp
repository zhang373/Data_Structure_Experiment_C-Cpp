/*Ѱ��ͼƬ�е���С·�������⣬����˶-2206110686-�Զ���03*/

#include<stdio.h>		//�����ڲ���
#include<malloc.h>		//�����ڴ�����ļ���
#include<string.h>
#include "map.h"		//����map�ļ�,���д����ͼ�Ļ�����Ϣ�ͺ궨����Ϣ,����֮���ͼƬ�ļ����и�����ֻ�����InitMap���ɡ�

typedef int PathMatrix[MaxNum][MaxNum+change];		//����·������
typedef struct {
	int weight[MaxNum];								//�������·�����Ⱥ�
	Status Visited[MaxNum];							//�����б����Ƿ񱻷��ʹ�
}ShortPathList;

//�ж���������Ƿ���ȷ
//�������Ϊ3��		����Ϊû�д���
//���������Ϊ3��	����Ϊ�д���
Status IfError_01(int argc)
{
	if (argc == 3) return FALSE;
	else 
	{
		printf("ERROR_01");
		return TRUE;
	}
}

//Ѱ������ĵص��ڵص��б��е�λ��
//����ҵ��ˣ��򷵻��±�
//����Ҳ������򷵻�ERROR_02
int FindPlace(const char* PlaceName[], char* name,Graph* map)
{
	if (!PlaceName || !name|| !map) return ERROR;					//³���Լ��
	int i = 0;
	while (i < map->vexnum)											//����Ѱַ
	{
		if (strcmp(PlaceName[i], name) == 0) return i;
		i++;
	}
	printf("ERROR_02");												//������ѭ����û���ش�ʱ����Ϊ������
	return ERROR_02;
}

//���庯����ʼ�����·���б�
//����visit״̬����ΪFALSE
//����Ȩ�ؽ��д������Լ����Լ�֮��ȫ��Ϊ����󣬷���֮���޸�
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
//���ܣ���path�����ÿ�е�һ�б�ʾ·������.
//ÿ�е�һ��ȫ����ʼ��ΪFALSE
Status InitPath(PathMatrix& path,Graph map,int start)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		for (int j = 0; j < map.vexnum + change; j++)
		{
			path[i][j] = none;						//�����еĳ��ȳ�ʼ��Ϊ-1
		}
	}
	path[start][0] = 1;
	path[start][1] = start;
	return OK;
}
//���ܣ�����·����ʹ��·����С
//���룺ʹ��·������Ϊ���������С��·������С·����������Ϊnow��ԭ�ȵ�·����������Ϊbefore
//�����״̬
Status UpdatePath(PathMatrix &path,int before,int now,Graph map)
{
	if (!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		path[before][i] = path[now][i];				//��С��ֱ�ӳ�����
	}
	return OK;
}

//���ܣ��޸Ľ�㵽����·��������Сֵ�������ú���UpdatePath����ÿ���������·����
//���������µĳ���С��ԭ�г��ȣ�����и��¡����º����һ�����·��ָ�򱻸��µĽ�㡣
//                                           �ܵĳ��Ƚ��м�һ���㣬ͬʱ��Ȩ��Ҳ���и��¡�
Status ChangeWeight(Graph map, ShortPathList* list, int curnode, PathMatrix& path)
{
	if (!list||!path) return ERROR;
	for (int i = 0; i < map.vexnum; i++)
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] > list->weight[curnode] + map.arcs[curnode][i])
			{
				UpdatePath(path, i, curnode,map);										//����������ʱ����·��
				path[i][PLen]++;														//�ܵ�·������path[i][0]��һ
				path[i][path[i][PLen]] = i;												//���һ�����ϵ�ǰ��λ��
				list->weight[i] = list->weight[curnode] + map.arcs[curnode][i];			//��Ȩ�ظ���Ϊ��С��ֵ
			}
		}
	}
	return OK;
}
//���ܣ�Ѱ����̵�·���Ľ�㲢�޸ķ���״̬
//���룺list�����ÿ���ڵ㵽���ľ��룻map���ڴ��ͼ����Ϣ
//�������С��������λ�ã�
int FindShort(ShortPathList* list,Graph map)
{
	if (!list) return ERROR;
	int shortest=none;
	int flag = TRUE;
	int i = 0;
	while ( i < map.vexnum && flag)								//Ѱ�ҵ�һ��δ�����ʵĽ��
	{
		if (list->Visited[i] == FALSE)
		{
			shortest = i;
			flag =FALSE;
		}
		i++;
	}
	for (int i = 0; i < map.vexnum; i++)						//Ѱ�����нڵ���δ�����ʵ���С���Ƚ��
	{
		if (list->Visited[i] == FALSE)
		{
			if (list->weight[i] < list->weight[shortest])
			{
				shortest = i;
			}
		}
	}
	//if (shortest != none) list->Visited[shortest] = TRUE;		//���ķ���״̬
	return shortest;											//���ؽǱ꣬����Ǳ���none����Ϊ���ж��������ˣ����Ը������
}
//�������ܣ����ָ����㵽�յ�����·�������Ҵ�����·��
/*���룺map		�Ѿ���ʼ����ɵ�����ͼ
		start	������
		end		�յ����
		list	������·�����ȵ�����
		path	���ڼ�¼�Ų���
  ����ֵ�����ش�start��end�����·��Ȩֵ*/
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
	PathMatrix path = { none };														//�����¼·���ľ���
	ShortPathList* list=(ShortPathList*) malloc(sizeof(ShortPathList));				//�������н�㵽����·�����ȣ�
	if (!list) return OVERFLOW;								
	int start = FindPlace(PlaceName, argv[1], map);			//��ʼ�ص������
	if (start == ERROR_02) return ERROR_02;
	int end = FindPlace(PlaceName, argv[2], map);			//��ֹ�ص������
	if (end == ERROR_02) return ERROR_02;
	int shortestdistance = 0;
	shortestdistance=DJT_ShortPath(*map, path, list, start, end);					//Ѱ����С·�����Ȳ������Ž�shortestdistance
	printf("%d", shortestdistance);
	PrintPath(path,end,PlaceName);
	free(map);																		//�ͷ�mapָ��
	free(list);																		//�ͷ�listָ��
	return 0;
}

