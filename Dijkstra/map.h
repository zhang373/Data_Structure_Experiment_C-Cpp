#pragma once
#ifndef _HEAD_H_
#define _HEAD_H_
#define InAccessible   100000		//�޷��ִ�ʱ���ܳ���
#define Status int
#define OK 2
#define TRUE 1
#define change 1					//��������1λ������ռλ�ʹ��
#define FALSE 0
#define PLen 0
#define none -1
#define ERROR -1					//�����ж��Ƿ���ִ���
#define OVERFLOW -2					//�����ж��Ƿ����ɹ�
#define ERROR_01 -3					//��β���ȷ
#define ERROR_02 -4					//�޷��������·��	
#define MaxNum 20					//���֧�ֵĴ�ŵĽڵ���Ŀ


//ͼ�����ݽṹ
typedef struct GRAPH
{
	int vexs[MaxNum];				//������������ڴ�����еĽ��
	int arcs[MaxNum][MaxNum];		//�����ڽӾ�������֮��ļ���ͷ���
	int vexnum;						//�ܵĽڵ���Ŀ
	int arcnum;						//�ܵıߵ���Ŀ
}Graph;								//ͼ�Ľṹ�������


//���еĵص����Ƽ�������Ӧ����ͼ�ж���Ľű�
extern const char* PlaceName[13] = { "����","��ˮ˼Դ","������3","������1","�ڷ���","ͼ���","������2","������4","��ѧ��¥","������","��Ǩ��","�����","����" };
									 //0        1          2         3         4        5        6         7         8         9         10       11       12

//��ͼƬ���г�ʼ��
extern Status InitMap(Graph* map)
{
	if (!map) return ERROR;									//³���Լ��
	map->vexnum = 13;										//���н��ĸ���
	map->arcnum = 18;										//���бߵ�����
	//��ʼ�����еĽ��ͱ�
	int i = 0, j = 0;				
	for (i = 0; i < map->vexnum; ++i) {
		map->vexs[i] = i;
		for (j = 0; j < map->vexnum; ++j)					//�Խ����ϳ�ʼ��Ϊ0
			if (i == j) map->arcs[i][j] = 0;
			else {
				map->arcs[i][j] = InAccessible;				//ȫ���ȳ�ʼ��Ϊ����ͨ
			}
	}
	//��ʼ���ڽӾ���
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