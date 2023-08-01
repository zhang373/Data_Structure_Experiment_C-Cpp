// datastructure_exp03.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
// ���ļ��ɶ�ͨ��������������ַ������л��������룬���ܹ�������������Ļ��������뷭����ַ���
#include<iostream>
#include<stdio.h>
#include<string.h>

#define maxsize 1000//�����Ϊ100
#define false 0
#define ok 1
#define STACK_INIT_SIZE 100
#define STACKINCREASE 10

typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTcode, * HuffmanTree;//��̬��������洢��������
typedef char** HuffmanCode;//��̬��������洢�����������
typedef struct {
	char c;
	unsigned int time;
}timecode, * timearray;//��̬��������洢�����и��ַ������ִ���
typedef int SElemType;
typedef struct {
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;

timearray(w);//ȫ�ֱ���w����ַ�����Ȩֵ

//����һ������ջ
//S��	���룬�Ѿ������Ĺ���ջ
//����ֵ��	�������е�״̬��ʾ
int InitStack(SqStack& S) {
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S.base) return OVERFLOW;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return ok;
}

//��һ��Ԫ��ѹջ
//S��	���룬�Ѿ������Ĺ���ջ
//e��	���룬��Ҫѹջ��Ԫ��
//����ֵ��	�������е�״̬��ʾ
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

//��ջ��Ԫ�س�ջ
//S��	���룬�Ѿ������Ĺ���ջ
//e��	����ֵ����ų�ջ��Ԫ��
int Pop(SqStack& S, SElemType& e) {
	if (S.top == S.base) return false;
	e = *--S.top;
	return e;
}

//���ջ����¼
//S��	���룬�Ѿ������Ĺ���ջ
//e��	����ֵ��ջ����¼
int GetTop(SqStack& S, SElemType& e) {
	if (S.top == S.base) return false;
	e = *(S.top - 1);
	return e;
}

//��������ַ����и�Ԫ�صĳ��ִ����������������
//input��	���룬Ҫ����ͳ�Ƶ��ַ���
//w��		���룬��Ÿ��ַ�����Ȩֵ������
//����ֵ��	���ֵ��ַ���
int gettime(char* input, timearray& w) {
	if (!input || !w)
		return 0;//�ж����뼰Ȩֵ�����Ƿ����
	int i = 0, j = 0, exist = 0, pos = 0;
	for (i = 0, pos = 1; input[i] && i < maxsize; i++) {//pos��¼�������ַ�Ҫ��ŵ�λ��
		exist = 0;
		for (j = 1; w[j].c; ++j) {
			if (input[i] == w[j].c) {
				w[j].time++;
				exist = 1;//��w���������д��ַ�����exist=1��ʾ����
				if (j == pos)pos++;
				break;
			}
		}
		if (exist == 0 && w[pos].time == 0) {
			w[pos].c = input[i];//�����¼���w��
			w[pos].time = 1;
			pos++;
		}
	}
	for (i = 1; w[i].time != 0; ++i);
	return i - 1;//���س��ֵ��ַ���
}

//ѡ��parentΪ0��weight��С��������㣬����ŷֱ�Ϊs1��s2
//ht��	���룬�Ѿ�����ռ��ָ��
//w��	���룬����ַ�����Ȩֵ������
//num��	���룬Ҫ��ѰȨֵ��С����������
//n��	���룬���ֵ��ַ���
//s1��s2��	���룬Ȩֵ��С�������
//����ֵ��	��������״̬
int select(HuffmanTree& ht, timearray& w, int num, int n, int& s1, int& s2) {
	if (!ht || !w)return false;//��������ַ�Ƿ�Ϊ��
	int min1 = 0, min2 = 0, i = 0, j = 0, t = 0;
	for (i = 1, min1 = maxsize, s1 = 1; i <= num; ++i) {
		if (ht[i].parent != 0)continue;
		if (ht[i].weight < min1) {//ȡs1
			min1 = ht[i].weight;
			s1 = i;
		}
		if (ht[i].weight == min1 && i <= n) {//Ȩֵ���ȡ�ַ�ASCII���С����Ϊs1
			if (w[i].c < w[s1].c)s1 = i;
		}
	}
	for (i = 1, min2 = maxsize; i <= num; ++i) {//ȡs2
		if (i == s1 || ht[i].parent != 0)continue;
		if (ht[i].weight < min2) {
			min2 = ht[i].weight;
			s2 = i;
		}
		if (ht[i].weight == min2 && i <= n) {//Ȩֵ���ȡ�ַ�ASCII���С����Ϊs2
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
	//�õ�weight��С���ַ���ӦASCII����С�Ľ��s1,s2
}

//�����������ht�������n���ַ��Ļ���������hc
//ht��	���룬�Ѿ�����ռ�Ļ�������ָ��
//hc��	���룬�Ѿ�����ռ�Ļ���������ָ��
//w��	���룬����ַ�����Ȩֵ������
//n��	���룬���ֵ��ַ���
//����ֵ��	����������״̬
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
	for (i = n + 1; i <= max; ++i) {//����������
		select(ht, w, i - 1, n, s1, s2);//Ѱ����СȨֵ���
		ht[s1].parent = i;
		ht[s2].parent = i;
		ht[i].lchild = s1;
		ht[i].rchild = s2;
		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}
	//��n���ַ��Ļ���������
	hc = (HuffmanCode)malloc((n + 1) * sizeof(char*));//����n���ַ������ͷָ������
	if (!hc) {
		printf("ERROR_02");
		return false;
	}
	char* cd = NULL;
	int start = 0, c, f;
	cd = (char*)malloc(n * sizeof(char));//���������Ĺ����ռ�
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

//������Ļ��������뷭���Ϊ�ַ���
//ht��	���룬�Ѿ�����ռ�Ļ�������ָ��
//w��	���룬����ַ�����Ȩֵ������
//n��	���룬���ֵ��ַ���
//input��	���룬Ҫ���з���Ļ���������
//����ֵ��	����������״̬
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

//�������Ļ����������Ƿ���ȷ���ܷ��ҵ���Ӧ�ַ�����
//ht��	���룬�Ѿ�����ռ�Ļ�������ָ��
//w��	���룬����ַ�����Ȩֵ������
//n��	���룬���ֵ��ַ���
//input��	���룬Ҫ���з���Ļ���������
//����ֵ��	����������״̬
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

//��ӡ�����ʽ�����ţ�weight,parent,lchild,rchild,�ȼ�����
//ht��	���룬�Ѿ������Ļ�������
//k��	���룬Ҫ���ʵĽ�����
//n��	���룬�����ַ����г��ֵ��ַ���
//����ֵ��	����������״̬
int visit(HuffmanTree& ht, int k, int n) {
	if (!ht || n <= 1 || k == 0) {
		return false;
	}
	int i = 0, j = 0;
	printf("������:%d	weight:%d	parent:%d	lchild:%d	rchild:%d	", k, ht[k].weight, ht[k].parent, ht[k].lchild, ht[k].rchild);
	if (ht[k].lchild != 0)
		i++;
	if (ht[k].rchild != 0)
		i++;
	printf("��:%d	", i);
	for (j = 1; k != 2 * n - 1; j++)
		k = ht[k].parent;
	printf("����:%d\n", j);
	return ok;
}

//�����������������
//ht��	���룬�Ѿ������õĻ�������
//n��	���룬�����ַ����г��ֵ��ַ���
//����ֵ��	���������״̬
int preordertraverse(HuffmanTree& ht, int n) {
	if (!ht || n <= 1) {
		return false;
	}
	SqStack(S);
	InitStack(S);//��������ջ
	int k = 2 * n - 1;//�����ŴӸ��ڵ㿪ʼ
	int topelement = 0;//ջ��Ԫ��
	while (k != 0 || GetTop(S, topelement) != 0) {
		if (k != 0) {
			visit(ht, k, n);
			Push(S, k);
			k = ht[k].lchild;
		}
		else {
			Pop(S, k);//������Ϊ�գ�����ջ
			k = ht[k].rchild;
		}
	}
	free(S.base);
	return ok;
}

//������
//argc��	�����в�������
//argv[]��	�����в���
int main(int argc, char* argv[]) {
	if (argc != 3) {//������������
		printf("ERROR_01");
		return -1;
	}
	if (strlen(argv[1]) < 20) {
		printf("ERROR_02");
		return false;
	}
	int n = 0, i = 0, j = 0;
	w = (timearray)malloc(128 * sizeof(timecode));
	for (i = 0; i < 128; i++) {//ASCII����й���128���ַ�
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
	if (HuffmanCoding(ht, hc, w, n) == 1) {//����ܹ��ɹ����룬�������������
		preordertraverse(ht, n);
		if (input2check(ht, w, argv[2], n) == 0) {
			printf("ERROR_03");//����������벻��ȷ��������ʧ�ܣ�����
			return -1;
		}
		else {
			for (i = 0; argv[1][i]; ++i) {
				for (j = 1; j <= n; ++j) {
					if (argv[1][i] == w[j].c) {//��������еĻ���������
						printf("%s", hc[j]);
						break;
					}
				}
			}
			printf(" ");
			translate(ht, w, argv[2], n);
		}
		free(ht);//��molloc�Ŀռ����free
		free(hc);
	}
	else free(ht);
	free(w);
	return 0;
}