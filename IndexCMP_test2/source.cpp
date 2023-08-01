#include<stdio.h>
#include<malloc.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS_
#define OK 2
#define correct 3//��ȷ��aggc����
#define ERROR -1
#define ERROR_01 -3
#define ERROR_02 -4
#define ERROR_03 -5
#define OVERFLOW -2
#define big 1
#define change 1//���ڽ���0��ʼ���±��ɴ�1��ʼ���±�
#define CHANGE 2//�����ַ�ת��������
#define equal 0
#define small -1 //��������ʱstrcompare���ع�
#define TRUE 1
#define FALSE 0
#define Status int//����ֵ����״̬��״ֵ̬������ʾ��
#define ElemType char//��һ�붼���ַ�����
#define InitSize 500
#define IncreaseSize 10 
#define resultlen 2//�ɱ䳤����result�ĳ��ȣ�1��status��2����ֵ��
#define statu 0
#define index 1
#define data 2
#define RESULT_SIZE 2//result�ĳ���
#define position 0//CMP���бȽ�ʱ�������0��ʼ���бȽϣ�֮����Խ����޸ġ�
#define none 0//δ����
//ʹ��CMP�㷨���д���
typedef struct str
{
	ElemType* base;
	int CurLen;
}String;//�����ַ����ṹ��,��Ҫ�Ǹ�Station��Target�õ�,base�ĵ�һ��base[0]���������ڴ���ַ����ܳ���Curlen(��next�ȵط�����ռλ)
//void visit(String* target)
//{
//	for (int i = 0; i <= target->CurLen; i++)
//	{
//		printf("%c\, %d\n\n\n", target->base[i],i);
//	}
//}
int max(int first, int secoend)
{
	if (first < secoend) return secoend;
	else return first;
}//ȡǰһ���һ��֮�и����һ���
Status STRCOPY(char* target, String* result)
{
	if (!target) return ERROR;
	if (!result) return ERROR;
	int Total_Length = strlen(target);//����ܵĳ���
	int i = 0;
	result->base[i] = Total_Length;//��һλ����ܳ��ȣ�֮����Ч���ݴ�1��ʼ�����ں�ߵ����
	for ( i = 1; i <= Total_Length; i++)
	{
		result->base[i] = target[i-1];
	}
	result->CurLen = Total_Length;
	result->base[i] = '\0';
	return OK;
}//���÷Ǳ�׼�ַ�����ʽΪ��׼�ַ�����ʽ
int Strlen(char* input)
{
	if (!input) return ERROR;
	int output=0;
	while (input[output])
	{
		output++;
	}
	return output;
}//���庯������ַ����ĳ���
Status Strassign(String* Target, char* Data)
{
	if (Target) free(Target);
	int i = Strlen(Data);//�ж��ܵĳ��ȣ�Curlen
	if (!i)
	{
		Target->base = NULL;
		Target->CurLen = 0;
	}//�����DAta֮��û�ж������մ�
	else
	{
		Target->base = (char*)malloc((i + data) * sizeof(char));
		if (!Target->base) return OVERFLOW;
		STRCOPY(Data, Target);//��data����Target֮��
		Target->CurLen = i;//����Ϊi
	}
	return OK;
}//���ڽ��ַ������д���
int StrCompare(String* stringa, String* stringb)
{
	if (!stringa || !stringb) return ERROR;
	if (stringa->CurLen > stringb->CurLen) return big;
	if (stringa->CurLen < stringb->CurLen) return small;
	if (stringa->CurLen == stringb->CurLen)//������һ��ʱ��һֱѰ�ҵ���һ���ģ�Ȼ��Ƚ�
	{
		int flag = 1;
		int i = 1;
		while (flag && i <= max(stringa->CurLen, stringb->CurLen))
		{
			if (stringa->base[i] == stringb->base[i]) flag = flag;
			else flag = 0;
			i++;
		}
		if (i == max(stringa->CurLen, stringb->CurLen)+change) return equal;
		else
		{
			if (stringa->base[i] > stringb->base[i]) return big;
			else return small;
		}
	}
	return ERROR;
}//���бȽϣ����������ַ�����A��B����big A=B����equal A��B����small
Status StrClear(String* input)
{
	if (!input) return ERROR;
	input->CurLen = 0;
	free(input->base);
	input->base = (char*)malloc(input->CurLen * sizeof(char));
	return OK;
}//��������ַ�������
Status get_next(String* Target, int* next)
{
	if (!Target) return ERROR;
	if (!Target->base) return ERROR;
	int i = 1;//next���±꣬��1��ʼ��0λ���ڴ洢�ܴ����ȡ�
	next[i] = 0;
	next[0] = Target->CurLen;//0λ�ô���T�����ܳ���
	int k = 0;//kΪ����������������󳤶�ֵ��
	while (i < Target->CurLen)
	{
		if (k == 0 || Target->base[i] == Target->base[k])
		{
			i++;
			k++;
			next[i] = k;
		}//���Pj=Pk����ֱ��Ϊ��,��ô��ֱ�Ӽ�һ�����ˡ�
		else k = next[k];//������������������ʱ��ֱ�ӽ��д�����kȡ��next��k����
	}
	return OK;
}//����ȡ����һλ�Ĳ���
Status Index_CMP(String* Station, String* Target, int pos,int* result)
//SΪĿ�괮��TΪģʽ����posΪ��ʼ�Ƚ�λ�ã�resultΪ���ص�ֵ������
{
	if (!Station || !Target||!result) return ERROR;
	if (pos<0 || pos>Station->CurLen) return ERROR;
	if (StrCompare(Station, Target) == small)
	{
		result[statu] = FALSE;
		return FALSE;
	}//���ڱȽ�Ŀ�괮��ģʽ���ĳ��ȣ�Ŀ�괮�̾Ͳ��ñ���
	int* next = (int*)malloc((Target->CurLen+data) * sizeof(int));
	memset(next, 0, (Target->CurLen + data) * sizeof(int));
	if (!next) return OVERFLOW;//next����Ǳ��1��ʼ�������ߵ�ʹ�ã�������⣬��+data
	get_next(Target, next);
	int i = pos;
	int k = 1;
	while (i <= Station->CurLen && k <= Target->CurLen)
	{
		if (k == 0 || Station->base[i] == Target->base[k])
		{
			++k;
			++i;
		}//���Ϊ0����ô�൱������ƶ�һλ�����������ȣ�����ƶ�һλ����������������
		else
		{
			k = next[k];//���ƥ��ʧ�ܣ�ֱ�ӽ���next[k]λ�ý���ƥ�����жϡ����ʱ��i�ǲ����ġ�
		}
	}
	free(next);
	if (k > Target->CurLen)
	{
		result[statu] = OK;
		result[index] = i - Target->CurLen;//���ڱȵ������һλ��Ҫ�ص���һλ�����ص�һλ��ֵ��
		return OK;
	}
	else
	{
		result[statu] = FALSE;
		return FALSE;//��k�޷�����T������ʱ����Ϊ��û�а취���ȫ��T���ıȽϡ�ֱ��ʧЧ��
	}
}//ִ��KMP�㷨�����ַ������з����õ���λ��ַ
Status IsError01(int aggc,char** argv)
{
	if (!argv) return ERROR;
	if (aggc ==correct) return FALSE;
	else return TRUE;
}//�ж��Ƿ������в�������
Status InitString(String* input,int curlen)
{
	if (!input) return ERROR;
	input->CurLen = curlen;
	input->base = (char*)malloc((input->CurLen+data) * sizeof(char));
	return OK;
}//��ʼ���ַ����ṹ�塣
int final_Index_Package(int aggc, char** argv,int *result)
{
	if (!result) return ERROR;
	if (!argv) return ERROR;
	if (IsError01(aggc,argv))
	{
		printf("ERROR_01");
		return ERROR_01;
	}//�ж��Ƿ������в�������.
	String* Station = (String*)malloc(sizeof(String));
	if (!Station) return OVERFLOW;
	InitString(Station,strlen(argv[1]));//���ڴ��Ŀ�괮
	String* Target = (String*)malloc(sizeof(String));
	if (!Target) return OVERFLOW;
	InitString(Target,strlen(argv[2]));//���ڴ��ģʽ��
	STRCOPY(argv[1], Station);//������������в�����Ž�Ŀ�괮��
	STRCOPY(argv[2], Target);//������������в�����Ž�ģʽ����
	//visit(Station);
	//visit(Target);
	Index_CMP(Station, Target,position,result);//CMP�㷨
	free(Station);
	free(Target);
	return 0;
}//����KMPѰַ֮ǰ�ĵ������ʼ������
Status Ans(int* result)
{
	if (!result) return ERROR;
	switch (result[statu])
	{
		case FALSE:
		{
			printf("-1");
			return FALSE;//FALSE��ʾδ�ҵ�
		}
		case OK:
		{
			if (result[index]!=none)
			{
				printf("%d", result[index]);
			}
			else //��data��Ϊnoneʱ��ģʽ��û���κ����룬����ERROR_01
			{
				printf("ERROR_01");
			}
			return OK;
		}
	}
	return ERROR;
}//�Դ𰸽������

int main(int argc, char** argv)
{
	int* result = (int*)malloc((RESULT_SIZE) * sizeof(int));//result���ڴ�Ž��
	memset(result, 0, RESULT_SIZE);
	if (!result) return OVERFLOW;
	final_Index_Package(argc,argv, result);//����Ѱ���ַ�����������
	Ans(result);//����������
	free(result);
}