#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define SElemType char
#define Status int
#define OK 2
#define TRUE 1
#define change 1//���ڴ�Ŵ�Ȩ�ڵ�ṹ��Ŀռ����ת��
#define both 3
#define left 1
#define right 2
#define FALSE 0
#define ERROR -1
#define OVERFLOW -2
#define ERROR_01 -3
#define ERROR_02 -4
#define ERROR_03 -5
#define InitSize 100
#define IncreaseSize 10

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+�����Ŵ�Ȩ�ڵ㲢����ڵ�Ȩֵ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
typedef struct NamePlusTimes
{
	char* node;//���ڴ�ų��ֹ����ַ�
	int* weight;//��Ԥ�淽�ַ���Ȩֵ
	int Total_len;//���ڴ���ܵ�·������
}Node_Weight;//����洢�ڵ�����Ȩ·�����ȵĽṹ��
int IfInNodeList(char elem, Node_Weight* Cur_Node_Weight)
{
	if (!Cur_Node_Weight) return ERROR;
	int i = 1;
	while (i <= Cur_Node_Weight->Total_len)
	{
		if (elem == Cur_Node_Weight->node[i - change]) return i;
		if (i == Cur_Node_Weight->Total_len) return FALSE;
		i++;
	}
	return FALSE;
}//�жϵ�ǰ���ַ��Ƿ��ڴ�Ȩ�ڵ�ṹ�⴮�����ߣ�����ڣ�����λ�ã��Ǳ�+1����������ڣ�FAlse��
Status PutInNodeList(char elem, Node_Weight* Cur_Node_Weight)
{
	if (!Cur_Node_Weight) return ERROR;
	//³���Լ��
	Cur_Node_Weight->node = (char*)realloc(Cur_Node_Weight->node, (change + Cur_Node_Weight->Total_len++) * sizeof(char));
	Cur_Node_Weight->weight = (int*)realloc(Cur_Node_Weight->weight, (change + Cur_Node_Weight->Total_len) * sizeof(int));
	//��̬����һλ���Ԫ�أ��ܳ���˳���1
	if (!Cur_Node_Weight->node) return OVERFLOW;
	if (!Cur_Node_Weight->weight) return OVERFLOW;
	//³���Լ��
	Cur_Node_Weight->node[Cur_Node_Weight->Total_len - change] = elem;
	Cur_Node_Weight->weight[Cur_Node_Weight->Total_len - change] = 0;
	Cur_Node_Weight->node[Cur_Node_Weight->Total_len] = '\0';
	return OK;
}//��Ԫ��elem����Cur_Node_Weight֮�У�ʹ��reallocʵ�ֿɱ䳤�ķ��䡣ֻʵ�ִ�Ž�ȥ����ʵ�ּ���
Status InitNode_Weight(Node_Weight* Cur_Node_Weight)
{
	if (!Cur_Node_Weight) return ERROR;
	Cur_Node_Weight->node = NULL;
	Cur_Node_Weight->weight = NULL;
	Cur_Node_Weight->Total_len = 0;
	return OK;
}//����һ��Node_Weight�ṹ�塣����node��weight��Ϊ�գ���һλ����һλ�������ڿɱ䳤�ġ�
Status AddWeight(int Index, Node_Weight* Cur_Node_Weight)
{
	if (!Cur_Node_Weight) return ERROR;
	Cur_Node_Weight->weight[Index]++;
	return OK;
}//�Զ�Ӧλ�õ�Ȩֵ�Ӽ�
Status CreateNode_Weight(char** argv, Node_Weight* Cur_Node_Weight)
{
	if (!Cur_Node_Weight) return ERROR;
	InitNode_Weight(Cur_Node_Weight);//��ʼ��
	int FlagLength = strlen(argv[1]);//��ѭ���Ĵ���
	int i = 0;
	while (i < FlagLength)
	{
		if (!IfInNodeList(argv[1][i], Cur_Node_Weight))
		{
			PutInNodeList(argv[1][i], Cur_Node_Weight);//���û�ڣ��Ͱ����ӽ�ȥ����һ���µĽڵ�
		}
		i++;
	}//������Ԫ���ӽ��ڵ���ȥ.
	i = 0;
	while (i < FlagLength)
	{
		if (IfInNodeList(argv[1][i], Cur_Node_Weight)) AddWeight(IfInNodeList(argv[1][i], Cur_Node_Weight) - change, Cur_Node_Weight);
		//����ڣ���ô��Ӧλ��+1
		i++;
	}
	return OK;
}//���ɽڵ㼰���Ȩ���ȣ�����֮��huffmantree�����ɡ�

Status IfError_01(int input)
{
	if (input == 3) return FALSE;
	else
	{
		printf("ERROR_01");
		return TRUE;
	}
}//�ж��ǲ��ǳ����˴���1

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+�����������л������������� -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
typedef struct OTHERINFO
{
	int index;//������
	int weight;//Ȩ��
	int parent;//���ڵ�
	int lchild;//����
	int rchild;//�Һ���
	int du;//��
	int plies;//����
}otherinfo;
typedef struct BETREE
{
	char name;
	int weight;
	struct BETREE* parent;
	struct BETREE* lchild;
	struct BETREE* rchild;
	Status Visited;
	otherinfo info;
}BetreeNode;//���ڹ������������Ľڵ㡣
typedef struct ROW_BETREE_LIST
{
	int Len;
	BetreeNode* nodelist;//���ڵ��б�Ϊָ���б�
	int* visited;
}RowBetreeList;//���ڴ�����ڵ��ǵĴ��������ڵ�ָchild��parentδ����ġ�
Status InitRowBetreeList(RowBetreeList* RowTreeList, Node_Weight* Cur_Node_Weight)
{
	if (!RowTreeList) return ERROR;
	int LENGTH = IncreaseSize + (Cur_Node_Weight->Total_len + 1) * Cur_Node_Weight->Total_len / 2;
	RowTreeList->Len = Cur_Node_Weight->Total_len;
	RowTreeList->nodelist = (BetreeNode*)malloc(LENGTH * sizeof(BetreeNode));
	if (!RowTreeList->nodelist) return OVERFLOW;
	RowTreeList->visited = (int*)malloc(LENGTH * sizeof(Status));
	if (!RowTreeList->visited) return OVERFLOW;
	return OK;
}//�Դ�����ڵ�Ĵ������г�ʼ�������ڴ�����ڵ㣬���ڵ�ָchild��parentδ����ġ�
Status InitRowBetreeNode(BetreeNode* target, char name, int weight, BetreeNode* parent, BetreeNode* lchild, BetreeNode* rchild)
{
	if (!target) return ERROR;
	target->name = name;
	target->weight = weight;
	target->parent = parent;
	target->rchild = rchild;
	target->lchild = lchild;
	target->Visited = FALSE;//��һ�����ɵ�ʱ�����нڵ�̶����Ϊδ���ʵġ�
	return OK;
}//�����ڵ���г�ʼ�������ڵ�ָchild��parentδ����ġ�
Status Create_RowBetreeList(RowBetreeList* RowTreeList, Node_Weight* Cur_Node_Weight)
{
	if (!RowTreeList || !Cur_Node_Weight) return ERROR;
	InitRowBetreeList(RowTreeList, Cur_Node_Weight);
	int i = 0;
	while (i < Cur_Node_Weight->Total_len)
	{
		InitRowBetreeNode(&RowTreeList->nodelist[i], Cur_Node_Weight->node[i], Cur_Node_Weight->weight[i], NULL, NULL, NULL);
		RowTreeList->visited[i] = FALSE;
		i++;
	}
	return OK;
}//�����е����ڵ������ڵ��б���ȥ��
Status Create_Added_BeTree_Node(RowBetreeList* RowTreeList, int lchild, int rchild, int index)
{
	if (!RowTreeList) return ERROR;
	int weight = (RowTreeList->nodelist[lchild]).weight + RowTreeList->nodelist[rchild].weight;
	RowTreeList->Len++;
	InitRowBetreeNode(&RowTreeList->nodelist[(RowTreeList->Len) - change], '~', weight, NULL, &RowTreeList->nodelist[lchild], &RowTreeList->nodelist[rchild]);
	RowTreeList->nodelist[lchild].parent = &RowTreeList->nodelist[(RowTreeList->Len) - change];
	RowTreeList->nodelist[rchild].parent = &RowTreeList->nodelist[(RowTreeList->Len) - change];
	RowTreeList->visited[lchild] = TRUE;
	RowTreeList->nodelist[lchild].info.index = index;
	RowTreeList->visited[rchild] = TRUE;
	RowTreeList->nodelist[rchild].info.index = index + change;
	RowTreeList->visited[RowTreeList->Len - change] = FALSE;
}//����lchild�͵�rchildλ���д��������һ��С����Ȼ������С���ӽ����ڵ��б���
int Not_Visited_amount(RowBetreeList* RowTreeList)
{
	int count = 0;
	int i = 0;
	while (i < RowTreeList->Len)
	{
		if (RowTreeList->visited[i] == FALSE) count++;
		i++;
	}
	return count;
}//��RowTreeList֮��û�б����ʵ��ܳ��Ƚ��м���������
int Find_lchild(RowBetreeList* RowTreeList)//���С
{
	int i = 0;
	int lchild;
	int flag = TRUE;
	while (i < RowTreeList->Len)
	{
		if (RowTreeList->visited[i] == FALSE)
		{
			if (flag)
			{
				lchild = i;
				flag = FALSE;
			}//�ж��ǲ��ǵ�һ��δ������
			else
			{
				if (RowTreeList->nodelist[i].weight <= RowTreeList->nodelist[lchild].weight)
				{
					if (RowTreeList->nodelist[i].weight < RowTreeList->nodelist[lchild].weight) lchild = i;//Ȩ��С��ֱ���޸�
					else
					{
						if (RowTreeList->nodelist[i].name < RowTreeList->nodelist[lchild].name) lchild = i;//Ȩ�����Ҫ�Ƚ�һ���ֵ���
					}
				}
			}
		}
		i++;
	}
	return lchild;
}//�������ӽڵ�
int Find_rchild(RowBetreeList* RowTreeList, int* lchild)
{
	int i = 0;
	int rchild;
	int flag = TRUE;
	while (i < RowTreeList->Len)
	{
		if (RowTreeList->visited[i] == FALSE && ((i) != *lchild))
		{
			if (flag && ((i) != *lchild))
			{
				rchild = i;
				flag = FALSE;
			}//�ж��ǲ��ǵ�һ��δ������
			else
			{
				if ((RowTreeList->nodelist[i].weight <= RowTreeList->nodelist[rchild].weight) && i != *lchild)//��ֹlchild���������ߵ������ڵ㱻����������
				{
					if (RowTreeList->nodelist[i].weight < RowTreeList->nodelist[rchild].weight) rchild = i;//Ȩ��Сֱ���޸�
					else
					{
						if (RowTreeList->nodelist[i].name < RowTreeList->nodelist[rchild].name) rchild = i;//ȫ�ش�Ҫ�Ƚ�һ���ֵ���
					}
				}
			}
		}
		i++;
	}
	return rchild;
}//Ѱ���Һ��ӽ����±ꡣ
Status Create_Huffman_Tree(RowBetreeList* RowTreeList, Node_Weight* Cur_Node_Weight)
{
	Create_RowBetreeList(RowTreeList, Cur_Node_Weight);
	int rchild;
	int lchild;
	int i = 1;
	while (Not_Visited_amount(RowTreeList) > change)
	{
		lchild = Find_lchild(RowTreeList);
		rchild = Find_rchild(RowTreeList, &lchild);
		Create_Added_BeTree_Node(RowTreeList, lchild, rchild, i);
		i += 2;
	}
	RowTreeList->nodelist[RowTreeList->Len - change].info.index = i;
	return OK;
}//���ɻ���������������ͷ������RowTreeList�����һλ�����Ǳ�ΪCur_Node_Weight->Total_len-1��λ��
Status If_Error_02(Node_Weight* Cur_Node_Weight)
{
	if (!(Cur_Node_Weight)) return ERROR;
	if (Cur_Node_Weight->Total_len <= 1)
	{
		printf("ERROR_02");
		return TRUE;
	}
	int flag = TRUE;
	int i = 0;
	while (i < Cur_Node_Weight->Total_len)
	{
		if (Cur_Node_Weight->weight[i] != 1)
		{
			flag = FALSE;
		}
		i++;
	}
	if (flag)
	{
		printf("ERROR_02");
		return TRUE;
	}
	return FALSE;
}//�ж���û�����룬������ǲ���ֻ��һ����

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-���б����������ɱ�������Ӧ�Ĺ���� -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

typedef struct otherINFOLIST
{
	int len;
	otherinfo* info;
	int maxsize;
}infolist;//��ŵ��б�
Status InitInfoList(infolist* infos, RowBetreeList* RowTreeList)
{
	if (!infos || !RowTreeList) return ERROR;
	infos->maxsize = RowTreeList->Len;
	infos->len = 0;
	infos->info = (otherinfo*)malloc(infos->maxsize * sizeof(otherinfo));
	return OK;
}//��ʼ���б�
Status IfNotLeaf(BetreeNode* cursor)
{
	if (!cursor) return ERROR;
	if (cursor->lchild && cursor->rchild) return both;
	if (cursor->lchild && (!cursor->rchild)) return left;
	if ((!cursor->lchild) && (cursor->rchild)) return right;
	if ((!cursor->lchild) && (!cursor->rchild)) return FALSE;
	return ERROR;
}//�ж������Ϊ��������ڵ㻹���ҽڵ�ռ�ݡ�
Status InputInfoList(infolist* infos, BetreeNode* node)
{
	if (!infos || !node) return ERROR;
	infos->len++;
	switch (IfNotLeaf(node))
	{
	case both:infos->info[infos->len - change].du = 2; break;
	case left:infos->info[infos->len - change].du = 1; break;
	case right:infos->info[infos->len - change].du = 1; break;
	default:infos->info[infos->len - change].du = 0; break;
	}//�ԶȽ��г�ʼ��

	infos->info[infos->len - change].index = node->info.index;
	if (node->lchild)
	{
		infos->info[infos->len - change].lchild = node->lchild->info.index;
	}
	else infos->info[infos->len - change].lchild = 0;
	if (node->rchild)
	{
		infos->info[infos->len - change].rchild = node->rchild->info.index;
	}
	else infos->info[infos->len - change].rchild = 0;
	infos->info[infos->len - change].plies = node->info.plies;
	if (node->parent)
	{
		infos->info[infos->len - change].parent = node->parent->info.index;
	}
	else infos->info[infos->len - change].parent = 0;
	infos->info[infos->len - change].weight = node->weight;
	return OK;
}//���ô�Ľڵ���Ϣ����б���
typedef struct CODENODE
{
	int len;
	char* CodeBase;//���ڴ�ű��봮�Ļ�ַ
	char name;
}CodeNode;//������һ���ַ�����Ĵ洢�ṹ
typedef struct CODELIST
{
	CodeNode* ListBase;//���ڴ�ű���ڵ���
	int len;
}CodeList;//��Ž���Ĺ����
Status InitCoedList(int max_size, struct CODELIST* target)
{
	if (!target) return ERROR;
	target->ListBase = (CodeNode*)malloc(max_size * sizeof(CodeNode));
	if (!target->ListBase) return OVERFLOW;
	target->len = 0;
	return OK;
}//��ʼ�������.
Status InitTempNode(CodeNode* target, int max_size)
{
	if (!target) return ERROR;
	target->CodeBase = (char*)malloc((max_size + change) * sizeof(char));
	if (!target->CodeBase) return OVERFLOW;
	return OK;
}//ֻ������з����ڲ���ַ���䳤�Ⱥ����������ģ����Ҫ���Ƶ���������ȥ�����ﲻǿ�з��䡣
Status IfRchildAccessb(BetreeNode* cursor)
{
	if (!cursor) return ERROR;
	if (cursor->rchild)
	{
		if (!cursor->rchild->Visited) return TRUE;//���Ҳ�ǿջ���δ������ʱ�����أ�TRUE���ɷ��ʵģ���
		else return FALSE;//��֮����FALSE����Ϊ���ɷ��ʡ�
	}
	else return FALSE;//��֮����FALSE����Ϊ���ɷ��ʡ�
}//�ж��Ҳ��Ƿ���Ա����ʡ�
Status cpy(char* output, char* input)
{
	if (!output || !input) return ERROR;
	int i = 0;
	int maxSIZE = strlen(input);
	while (i < maxSIZE)
	{
		output[i] = input[i];//����˳����
		i++;
	}
	return OK;
}
Status nodecopy(CodeNode* result, CodeNode* temp)
{
	if (!result || !temp) return ERROR;
	result->CodeBase = (char*)malloc((strlen(temp->CodeBase) + change) * sizeof(char));
	if (!result->CodeBase) return OVERFLOW;
	cpy(result->CodeBase, temp->CodeBase);//���ø�ֵ����
	result->CodeBase[strlen(temp->CodeBase)] = '\0';
	result->len = temp->len;
	result->name = temp->name;
	return OK;
}//������������д����Ϊ����ֱ����ȡ�
Status CreateCodeList(CodeList* MyCodeList, RowBetreeList* RowTreeList, Node_Weight* Cur_Node_Weight, infolist* infos)
{
	int count = 0;//�����ж��Ƿ����нڵ㶼�����ʹ���.
	BetreeNode* root = &(RowTreeList->nodelist[RowTreeList->Len - 1]);
	BetreeNode* cursor = root;
	CodeNode* temp = (CodeNode*)malloc(sizeof(CodeNode));
	if (!temp) return OVERFLOW;
	InitCoedList(RowTreeList->Len, MyCodeList);
	InitInfoList(infos, RowTreeList);
	InitTempNode(temp, RowTreeList->Len);
	int i = 0;//����temp�ڲ��ĳ����ж���
	int j = 0;//���ڹ������ж�
	int flag = 0;//root�����������Σ�˵��������������������
	while (flag < 2)//ֻ�ڶ˵���з��ʣ����Ҷ˵㲻���з��ʡ�
	{
		while (cursor->lchild)
		{
			cursor = cursor->lchild;
			temp->CodeBase[i] = '0';//����ת��Ϊ0
			i++;
		}//����߿��Է���ʱ���������ֱ��������ߵĽڵ�ľ�ͷ
		temp->CodeBase[i] = '\0';
		temp->name = cursor->name;
		temp->len = i;//��cursor����ɷ��ʵ������ʱ��������ж�ȡ�ͷ��ʡ�
		nodecopy(&MyCodeList->ListBase[j], temp);//������temp֮�еĶ����ӽ���ʽ�Ĵ浵���ȥ
		count++;
		cursor->Visited = TRUE;
		j++;
		while (!IfRchildAccessb(cursor) && flag < 2)
		{
			cursor->Visited = TRUE;//������������Ҳ಻�ɷ���ʱ�����������ͱ��������ˡ�
			cursor->info.plies = i + change;
			cursor = cursor->parent;//�����ɷ���ʱ�������˻�
			if (cursor == root) flag++;
			i--;
		}//�����Ƶ��Ҳ���Ա�����
		cursor = cursor->rchild;
		temp->CodeBase[i] = '1';//���Ҳ�ת��Ϊһ
		i++;
	}
	temp->CodeBase[temp->len] = '\0';
	root->info.plies = 1;//���ڵ�Ķ�Ϊ1
	free(temp);
	return OK;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+���Ҳ�����code��-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
typedef struct LIST
{
	int len;
	int max_size;
	char* base;
}List;
Status InitList(List* target)
{
	if (!target) return ERROR;
	target->len = 0;
	target->max_size = InitSize;
	target->base = (char*)malloc(InitSize * sizeof(char));
	if (!target->base) return OVERFLOW;
	return OK;
}//��ʼ���б�����֮������Ͳ�����
Status StringCopy(List* list, CodeNode* node)
{
	if (!list || !node) return ERROR;
	if (list->len + node->len > list->max_size)
	{
		list->base = (char*)realloc(list->base, (InitSize + (node->len) / IncreaseSize + change) * sizeof(char));
		if (!list->base) return OVERFLOW;
		list->max_size = list->len + IncreaseSize;
	}//�����ȹ���ʱ���������¹����ڴ棬���������ӳ�
	int i = list->len;
	int j = 0;
	while (j < node->len)
	{
		list->base[i] = node->CodeBase[j];
		j++;
		i++;
	}//����˳����
	list->base[i] = '\0';//���һλ����һ��\0
	list->len = i;
	return OK;
}//�������и��ƣ��ӵ�list���ȥ
int Search(CodeList* MyCodeList, char target)
{
	int i = 0;
	int flag = FALSE;
	while (!flag)
	{
		if (target == MyCodeList->ListBase[i].name)
		{
			flag = TRUE;
			return i;
		}
		i++;
	}
	return ERROR;
}//�ҵ���ͬ��Ԫ�أ����շ�����Ǳ�
Status CreateResult(List* list, CodeList* MyCodeList, char** argv)
{
	if (!list || !MyCodeList) return ERROR;
	InitList(list);
	int i = 0;
	int MAXLEN = strlen(argv[1]);
	while (i < MAXLEN)
	{
		StringCopy(list, &MyCodeList->ListBase[Search(MyCodeList, argv[1][i])]);
		i++;
	}
	return OK;
}//ÿһ���ַ�����д�����ʱ������ѹ��һ�������Ķ���֮�С���ֹ�γ�α�ַ��������������޷�ֱ�ӵ��á�
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+�Ա�����н������-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

Status IfError_03_1(BetreeNode* cursor, char input)
{
	if (!cursor) return ERROR;
	if (input == '0' && IfNotLeaf(cursor) == right) return TRUE;
	if (input == '1' && IfNotLeaf(cursor) == left) return TRUE;
	return FALSE;
}//�ж��ǲ���ERROR����03
Status EnList(List* list, char input)
{
	if (!list) return ERROR;
	if (list->len == list->max_size - change - change)//change��һλ������β��,����һλ�������װ��ȥ��
	{
		list->base = (char*)realloc(list->base, (list->max_size + IncreaseSize) * sizeof(char));
		list->max_size = list->max_size + IncreaseSize;
	}
	list->base[list->len] = input;
	list->len++;
	list->base[list->len] = '\0';
	return OK;
}//�Ѷ����ӽ�list���ȥ
Status DeCodeHulffman(char** argv, RowBetreeList* RowTreeList, List* decode)
{
	if (!argv || !RowTreeList) return ERROR;
	InitList(decode);
	int flag_argv = strlen(argv[2]);
	int i = 0;//��argvʹ�õ�
	BetreeNode* cursor = &RowTreeList->nodelist[RowTreeList->Len - change];//��ǰָ������λ�á�
	while (i < flag_argv)
	{
		while (IfNotLeaf(cursor) && i < flag_argv)
		{
			if (IfError_03_1(cursor, argv[2][i]))
			{
				printf("ERROR_03");
				return ERROR_03;
			}
			if (argv[2][i] == '0') cursor = cursor->lchild;
			if (argv[2][i] == '1') cursor = cursor->rchild;
			i++;
			if (i == flag_argv)
			{
				EnList(decode, cursor->name);
				goto k;//����Ѿ������ˣ��Ͳ�Ҫ�����ٻص����ڵ���
			}
		}
		EnList(decode, cursor->name);//��Ҷ�ӽڵ��ֵ�Ž�ȥ
		cursor = &RowTreeList->nodelist[RowTreeList->Len - change];
	}
k:cursor = cursor;
	if (!IfNotLeaf(cursor))//���cursor����Ҷ�ӽڵ㱨����֮������
	{
		return OK;
	}
	else
	{
		printf("ERROR_03");
		return ERROR_03;
	}
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+�����������ͬʱ�������������б������д��ſ�����ֱ�Ӳ��Դ�����ȥ��-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

Status preOrder(BetreeNode* ht, infolist* infos)
{
	if (!ht || !infos) return ERROR;
	BetreeNode* stack[InitSize];//��һ������ʵ��������ջ�Ĺ���
	int top = -1;//-1Ϊ��ʶ����û��ʵ�ʵ�ʹ�ã��±겻����Ϊ-1
	BetreeNode* cursor = ht;
	while (cursor != NULL || top != -1) {
		if (cursor != NULL) {
			stack[++top] = cursor;
			InputInfoList(infos, cursor); //��ջʱ���������
			cursor = cursor->lchild;
		}
		else {
			cursor = stack[top--];
			cursor = cursor->rchild;
		}
	}
	return OK;
}


void FREE(Node_Weight* Cur_Node_Weight, RowBetreeList* RowTreeList, CodeList* MyCodeList, List* list, infolist* infos)
{
	free(Cur_Node_Weight);
	free(RowTreeList);
	free(MyCodeList);
	free(list);
	free(infos);
}//������free���������
Status PRINT(infolist* infos)
{
	if (!infos || infos->len < 1) return ERROR;
	int i = 0;
	while (i < infos->len)
	{
		printf("������:%d      weight:%d        parent:%d       lchild:%d     rchild:%d        ��:%d    ����:%d\n",
			infos->info[i].index, infos->info[i].weight, infos->info[i].parent, infos->info[i].lchild, infos->info[i].rchild, infos->info[i].du, infos->info[i].plies);
		i++;
	}
	return OK;
}//��infos��ߵĶ�����ӡ��������һ�㱣֤�˲������α�������⣬�����������ֱ�ӵ���
int main(int aggc, char** argv)
{
	if (IfError_01(aggc)) return ERROR_01;//�ж��Ƿ�������1
	Node_Weight* Cur_Node_Weight = (Node_Weight*)malloc(sizeof(Node_Weight));//�����ŵ�ַ�ռ�
	if (!Cur_Node_Weight) return OVERFLOW;
	CreateNode_Weight(argv, Cur_Node_Weight);//���ɽڵ����Ȩ��
	if (If_Error_02(Cur_Node_Weight)) return ERROR_02;
	RowBetreeList* RowTreeList = (RowBetreeList*)malloc(sizeof(RowBetreeList));//�������ڵ㴮��������֮�����һλ�������ĸ��ڵ�
	if (!RowTreeList) return OVERFLOW;
	Create_Huffman_Tree(RowTreeList, Cur_Node_Weight);//���ɻ�������������RowTreeList�����һλ�ϱ�
	CodeList* MyCodeList = (CodeList*)malloc(sizeof(struct CODELIST));
	if (!MyCodeList) return OVERFLOW;
	infolist* infos = (infolist*)malloc(sizeof(infolist));//�������ڵ������
	if (!infos) return OVERFLOW;
	CreateCodeList(MyCodeList, RowTreeList, Cur_Node_Weight, infos);//�����������༭��Ϊһ�����Ե��õĹ����
	List* list = (List*)malloc(sizeof(List));
	if (!list) return OVERFLOW;
	CreateResult(list, MyCodeList, argv);//ÿһ���ַ�����д�����ʱ������ѹ��һ�������Ķ���֮�С���ֹ�γ�α�ַ��������������޷�ֱ�ӵ��á�
	List* decode = (List*)malloc(sizeof(List));
	if (!decode) return OVERFLOW;
	if (DeCodeHulffman(argv, RowTreeList, decode) == ERROR_03) return ERROR_03;//�������н������
	BetreeNode* root = &RowTreeList->nodelist[RowTreeList->Len - change];
	preOrder(root, infos);
	PRINT(infos);
	printf("%s", list->base);
	printf(" %s", decode->base);
	FREE(Cur_Node_Weight, RowTreeList, MyCodeList, list, infos);//�ͷ����е��ڴ���
	return 0;
}