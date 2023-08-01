// datastructure_exp03.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 该文件可对通过命令行输入的字符串进行霍夫曼编码，且能够将命令行输入的霍夫曼编码翻译成字符串
#include<iostream>
#include<stdio.h>
#include<string.h>

#define maxsize 1000//输入最长为100
#define false 0
#define ok 1
#define STACK_INIT_SIZE 100
#define STACKINCREASE 10

typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTcode, * HuffmanTree;//动态分配数组存储霍夫曼树
typedef char** HuffmanCode;//动态分配数组存储霍夫曼编码表
typedef struct {
	char c;
	unsigned int time;
}timecode, * timearray;//动态分配数组存储输入中各字符及出现次数
typedef int SElemType;
typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

timearray(w);//全局变量w存放字符与其权值

//建立一个工作栈
//S：	输入，已经声明的工作栈
//返回值：	程序运行的状态表示
int InitStack(SqStack& S) {
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S.base) return OVERFLOW;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return ok;
}

//将一个元素压栈
//S：	输入，已经建立的工作栈
//e：	输入，需要压栈的元素
//返回值：	程序运行的状态表示
int Push(SqStack& S, SElemType e) {
	if (S.top - S.base >= S.stacksize)
	{
		S.base = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREASE) * sizeof(SElemType));
		if (!S.base) return false;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREASE;
	}
	*S.top = e;
	S.top++;
	return ok;
}

//将栈顶元素出栈
//S：	输入，已经建立的工作栈
//e：	返回值，存放出栈的元素
int Pop(SqStack& S, SElemType& e) {
	if (S.top == S.base) return false;
	e = *--S.top;
	return e;
}

//获得栈顶记录
//S：	输入，已经建立的工作栈
//e：	返回值，栈顶记录
int GetTop(SqStack& S, SElemType& e) {
	if (S.top == S.base) return false;
	e = *(S.top - 1);
	return e;
}

//获得输入字符串中各元素的出现次数并存放在数组中
//input：	输入，要进行统计的字符串
//w：		输入，存放各字符及其权值的数组
//返回值：	出现的字符数
int gettime(char* input, timearray& w) {
	if (!input || !w)
		return 0;//判断输入及权值数组是否存在
	int i = 0, j = 0, exist = 0, pos = 0;
	for (i = 0, pos = 1; input[i] && i < maxsize; i++) {//pos记录发现新字符要存放的位置
		exist = 0;
		for (j = 1; w[j].c; ++j) {
			if (input[i] == w[j].c) {
				w[j].time++;
				exist = 1;//若w数组中已有此字符，则exist=1表示存在
				if (j == pos)pos++;
				break;
			}
		}
		if (exist == 0 && w[pos].time == 0) {
			w[pos].c = input[i];//否则新加入w中
			w[pos].time = 1;
			pos++;
		}
	}
	for (i = 1; w[i].time != 0; ++i);
	return i - 1;//返回出现的字符数
}

//选择parent为0且weight最小的两个结点，其序号分别为s1和s2
//ht：	输入，已经分配空间的指针
//w：	输入，存放字符及其权值的数组
//num：	输入，要找寻权值最小结点的最大序号
//n：	输入，出现的字符数
//s1，s2：	输入，权值最小结点的序号
//返回值：	函数运行状态
int select(HuffmanTree& ht, timearray& w, int num, int n, int& s1, int& s2) {
	if (!ht || !w)return false;//检查数组地址是否为空
	int min1 = 0, min2 = 0, i = 0, j = 0, t = 0;
	for (i = 1, min1 = maxsize, s1 = 1; i <= num; ++i) {
		if (ht[i].parent != 0)continue;
		if (ht[i].weight < min1) {//取s1
			min1 = ht[i].weight;
			s1 = i;
		}
		if (ht[i].weight == min1 && i <= n) {//权值相等取字符ASCII码较小的作为s1
			if (w[i].c < w[s1].c)s1 = i;
		}
	}
	for (i = 1, min2 = maxsize; i <= num; ++i) {//取s2
		if (i == s1 || ht[i].parent != 0)continue;
		if (ht[i].weight < min2) {
			min2 = ht[i].weight;
			s2 = i;
		}
		if (ht[i].weight == min2 && i <= n) {//权值相等取字符ASCII码较小的作为s2
			if (w[i].c < w[s2].c)s2 = i;
		}
	}
	if (min1 == min2) {
		if (w[s1].c == '\0' && w[s2].c != '\0') {
			s2 = t;
			s2 = s1;
			s1 = t;
		}
	}
	return ok;
	//得到weight最小且字符对应ASCII码最小的结点s1,s2
}

//构造霍夫曼树ht，并求出n个字符的霍夫曼编码hc
//ht：	输入，已经分配空间的霍夫曼树指针
//hc：	输入，已经分配空间的霍夫曼编码指针
//w：	输入，存放字符及其权值的数组
//n：	输入，出现的字符数
//返回值：	函数的运行状态
int HuffmanCoding(HuffmanTree& ht, HuffmanCode& hc, timearray& w, int n) {
	if (n <= 1) {
		printf("ERROR_02");
		return false;
	}
	int max = 2 * n - 1, i, s1, s2;
	HuffmanTree(p);
	ht = (HuffmanTree)malloc((max + 1) * sizeof(HTcode));
	if (!ht) {
		printf("ERROR_02");
		return false;
	}
	for (p = ht + 1, i = 1; i <= n; ++i, ++p) {
		*p = { w[i].time,0,0,0 };
	}
	for (; i <= max; ++i, ++p) {
		*p = { 0,0,0,0 };
	}
	for (i = n + 1; i <= max; ++i) {//建霍夫曼树
		select(ht, w, i - 1, n, s1, s2);//寻找最小权值结点
		ht[s1].parent = i;
		ht[s2].parent = i;
		ht[i].lchild = s1;
		ht[i].rchild = s2;
		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}
	//求n个字符的霍夫曼编码
	hc = (HuffmanCode)malloc((n + 1) * sizeof(char*));//分配n个字符编码的头指针向量
	if (!hc) {
		printf("ERROR_02");
		return false;
	}
	char* cd = NULL;
	int start = 0, c, f;
	cd = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
	if (!cd) {
		printf("ERROR_02");
		return false;
	}
	cd[n - 1] = '\0';
	for (i = 1; i <= n; ++i) {
		start = n - 1;
		for (c = i, f = ht[i].parent; f != 0; c = f, f = ht[f].parent) {
			if (ht[f].lchild == c)cd[--start] = '0';
			else cd[--start] = '1';
		}
		hc[i] = (char*)malloc((n - start) * sizeof(char));
		if (!hc[i]) {
			printf("ERROR_02");
			return false;
		}
		strcpy_s(hc[i], strlen(cd) + 1 - start, &cd[start]);
	}
	free(cd);
	return ok;
}

//将输入的霍夫曼编码翻译成为字符串
//ht：	输入，已经分配空间的霍夫曼树指针
//w：	输入，存放字符及其权值的数组
//n：	输入，出现的字符数
//input：	输入，要进行翻译的霍夫曼编码
//返回值：	函数的运行状态
int translate(HuffmanTree& ht, timearray& w, char* input, int n) {
	if (!ht || !input || !w || !n) {
		printf("ERROR_03");
		return false;
	}
	int i = 0, j = 0;
	for (i = 0, j = 2 * n - 1; input[i]; ++i) {
		if (input[i] == '0')j = ht[j].lchild;
		else if (input[i] == '1')j = ht[j].rchild;
		if (ht[j].lchild == 0 && ht[j].rchild == 0) {
			printf("%c", w[j].c);
			j = 2 * n - 1;
		}
	}
	return ok;
}

//检查输入的霍夫曼编码是否正确（能否找到对应字符串）
//ht：	输入，已经分配空间的霍夫曼树指针
//w：	输入，存放字符及其权值的数组
//n：	输入，出现的字符数
//input：	输入，要进行翻译的霍夫曼编码
//返回值：	函数的运行状态
int input2check(HuffmanTree& ht, timearray& w, char* input, int n) {
	if (!ht || !input || !w || !n) {
		return false;
	}
	int i = 0, j = 0;
	for (i = 0, j = 2 * n - 1; input[i]; ++i) {
		if (input[i] == '0')j = ht[j].lchild;
		else if (input[i] == '1')j = ht[j].rchild;
		if (ht[j].lchild == 0 && ht[j].rchild == 0) {
			j = 2 * n - 1;
		}
	}
	if (j != 2 * n - 1) return false;
	else return ok;
}

//打印所访问结点的序号，weight,parent,lchild,rchild,度及层数
//ht：	输入，已经建立的霍夫曼树
//k：	输入，要访问的结点序号
//n：	输入，输入字符串中出现的字符数
//返回值：	函数的运行状态
int visit(HuffmanTree& ht, int k, int n) {
	if (!ht || n <= 1 || k == 0) {
		return false;
	}
	int i = 0, j = 0;
	printf("结点序号:%d	weight:%d	parent:%d	lchild:%d	rchild:%d	", k, ht[k].weight, ht[k].parent, ht[k].lchild, ht[k].rchild);
	if (ht[k].lchild != 0)
		i++;
	if (ht[k].rchild != 0)
		i++;
	printf("度:%d	", i);
	for (j = 1; k != 2 * n - 1; j++)
		k = ht[k].parent;
	printf("层数:%d\n", j);
	return ok;
}

//霍夫曼树的先序遍历
//ht：	输入，已经建立好的霍夫曼树
//n：	输入，输入字符串中出现的字符数
//返回值：	程序的运行状态
int preordertraverse(HuffmanTree& ht, int n) {
	if (!ht || n <= 1) {
		return false;
	}
	SqStack(S);
	InitStack(S);//建立工作栈
	int k = 2 * n - 1;//结点序号从根节点开始
	int topelement = 0;//栈顶元素
	while (k != 0 || GetTop(S, topelement) != 0) {
		if (k != 0) {
			visit(ht, k, n);
			Push(S, k);
			k = ht[k].lchild;
		}
		else {
			Pop(S, k);//若孩子为空，则退栈
			k = ht[k].rchild;
		}
	}
	free(S.base);
	return ok;
}

//主函数
//argc：	命令行参数个数
//argv[]：	命令行参数
int main(int argc, char* argv[]) {
	if (argc != 3) {//参数数量错误
		printf("ERROR_01");
		return -1;
	}
	if (strlen(argv[1]) < 20) {
		printf("ERROR_02");
		return false;
	}
	int n = 0, i = 0, j = 0;
	w = (timearray)malloc(128 * sizeof(timecode));
	for (i = 0; i < 128; i++) {//ASCII码表中共有128个字符
		w[i].c = '\0';
		w[i].time = 0;
	}
	n = gettime(argv[1], w);
	if (n == 1) {
		printf("ERROR_02");
		return false;
	}
	HuffmanTree(ht);
	HuffmanCode(hc);
	if (HuffmanCoding(ht, hc, w, n) == 1) {//如果能够成功编码，进行译码与输出
		preordertraverse(ht, n);
		if (input2check(ht, w, argv[2], n) == 0) {
			printf("ERROR_03");//如果码字输入不正确导致译码失败，报错
			return -1;
		}
		else {
			for (i = 0; argv[1][i]; ++i) {
				for (j = 1; j <= n; ++j) {
					if (argv[1][i] == w[j].c) {//输出命令行的霍夫曼编码
						printf("%s", hc[j]);
						break;
					}
				}
			}
			printf(" ");
			translate(ht, w, argv[2], n);
		}
		free(ht);//对molloc的空间进行free
		free(hc);
	}
	else free(ht);
	free(w);
	return 0;
}