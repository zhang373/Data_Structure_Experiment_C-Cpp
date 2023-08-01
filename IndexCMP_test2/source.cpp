#include<stdio.h>
#include<malloc.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS_
#define OK 2
#define correct 3//正确的aggc个数
#define ERROR -1
#define ERROR_01 -3
#define ERROR_02 -4
#define ERROR_03 -5
#define OVERFLOW -2
#define big 1
#define change 1//用于将从0开始的下标变成从1开始的下标
#define CHANGE 2//中文字符转换的问题
#define equal 0
#define small -1 //以上三个时strcompare的特供
#define TRUE 1
#define FALSE 0
#define Status int//用数值返回状态，状态值如上所示。
#define ElemType char//串一半都是字符串。
#define InitSize 500
#define IncreaseSize 10 
#define resultlen 2//可变长参数result的长度，1：status，2：数值。
#define statu 0
#define index 1
#define data 2
#define RESULT_SIZE 2//result的长度
#define position 0//CMP进行比较时本程序从0开始进行比较，之后可以进行修改。
#define none 0//未输入
//使用CMP算法进行处理
typedef struct str
{
	ElemType* base;
	int CurLen;
}String;//声明字符串结构体,主要是给Station和Target用的,base的第一个base[0]不可以用于存放字符串总长度Curlen(在next等地方用于占位)
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
}//取前一项，后一项之中更大的一项返回
Status STRCOPY(char* target, String* result)
{
	if (!target) return ERROR;
	if (!result) return ERROR;
	int Total_Length = strlen(target);//求解总的长度
	int i = 0;
	result->base[i] = Total_Length;//第一位存放总长度，之后有效数据从1开始，便于后边的理解
	for ( i = 1; i <= Total_Length; i++)
	{
		result->base[i] = target[i-1];
	}
	result->CurLen = Total_Length;
	result->base[i] = '\0';
	return OK;
}//重置非标准字符串格式为标准字符串格式
int Strlen(char* input)
{
	if (!input) return ERROR;
	int output=0;
	while (input[output])
	{
		output++;
	}
	return output;
}//定义函数求解字符串的长度
Status Strassign(String* Target, char* Data)
{
	if (Target) free(Target);
	int i = Strlen(Data);//判断总的长度，Curlen
	if (!i)
	{
		Target->base = NULL;
		Target->CurLen = 0;
	}//输入的DAta之中没有东西，空串
	else
	{
		Target->base = (char*)malloc((i + data) * sizeof(char));
		if (!Target->base) return OVERFLOW;
		STRCOPY(Data, Target);//将data导到Target之中
		Target->CurLen = i;//长度为i
	}
	return OK;
}//用于将字符串进行处理
int StrCompare(String* stringa, String* stringb)
{
	if (!stringa || !stringb) return ERROR;
	if (stringa->CurLen > stringb->CurLen) return big;
	if (stringa->CurLen < stringb->CurLen) return small;
	if (stringa->CurLen == stringb->CurLen)//当长度一样时，一直寻找到不一样的，然后比较
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
}//进行比较，输入两个字符串，A》B返回big A=B返回equal A《B返回small
Status StrClear(String* input)
{
	if (!input) return ERROR;
	input->CurLen = 0;
	free(input->base);
	input->base = (char*)malloc(input->CurLen * sizeof(char));
	return OK;
}//定义清空字符串操作
Status get_next(String* Target, int* next)
{
	if (!Target) return ERROR;
	if (!Target->base) return ERROR;
	int i = 1;//next的下标，从1开始。0位用于存储总串长度。
	next[i] = 0;
	next[0] = Target->CurLen;//0位置储存T串的总长度
	int k = 0;//k为可以满足条件的最大长度值。
	while (i < Target->CurLen)
	{
		if (k == 0 || Target->base[i] == Target->base[k])
		{
			i++;
			k++;
			next[i] = k;
		}//如果Pj=Pk或着直接为零,那么，直接加一就行了。
		else k = next[k];//当不满足上述条件的时候，直接进行处理，让k取到next【k】。
	}
	return OK;
}//定义取用下一位的操作
Status Index_CMP(String* Station, String* Target, int pos,int* result)
//S为目标串，T为模式串，pos为起始比较位置，result为返回的值参数。
{
	if (!Station || !Target||!result) return ERROR;
	if (pos<0 || pos>Station->CurLen) return ERROR;
	if (StrCompare(Station, Target) == small)
	{
		result[statu] = FALSE;
		return FALSE;
	}//用于比较目标串和模式串的长度，目标串短就不用比了
	int* next = (int*)malloc((Target->CurLen+data) * sizeof(int));
	memset(next, 0, (Target->CurLen + data) * sizeof(int));
	if (!next) return OVERFLOW;//next数组角标从1开始，方便后边的使用，便于理解，故+data
	get_next(Target, next);
	int i = pos;
	int k = 1;
	while (i <= Station->CurLen && k <= Target->CurLen)
	{
		if (k == 0 || Station->base[i] == Target->base[k])
		{
			++k;
			++i;
		}//如果为0，那么相当于向后移动一位，如果两个相等，向后移动一位是属于正常的现象。
		else
		{
			k = next[k];//如果匹配失败，直接进入next[k]位置进行匹配与判断。这个时候i是不动的。
		}
	}
	free(next);
	if (k > Target->CurLen)
	{
		result[statu] = OK;
		result[index] = i - Target->CurLen;//现在比到了最后一位。要回到第一位并返回第一位的值。
		return OK;
	}
	else
	{
		result[statu] = FALSE;
		return FALSE;//当k无法大于T串长度时，认为其没有办法完成全部T串的比较。直接失效。
	}
}//执行KMP算法对于字符串进行分析拿到首位地址
Status IsError01(int aggc,char** argv)
{
	if (!argv) return ERROR;
	if (aggc ==correct) return FALSE;
	else return TRUE;
}//判断是否命令行参数错误。
Status InitString(String* input,int curlen)
{
	if (!input) return ERROR;
	input->CurLen = curlen;
	input->base = (char*)malloc((input->CurLen+data) * sizeof(char));
	return OK;
}//初始化字符串结构体。
int final_Index_Package(int aggc, char** argv,int *result)
{
	if (!result) return ERROR;
	if (!argv) return ERROR;
	if (IsError01(aggc,argv))
	{
		printf("ERROR_01");
		return ERROR_01;
	}//判断是否命令行参数错误.
	String* Station = (String*)malloc(sizeof(String));
	if (!Station) return OVERFLOW;
	InitString(Station,strlen(argv[1]));//用于存放目标串
	String* Target = (String*)malloc(sizeof(String));
	if (!Target) return OVERFLOW;
	InitString(Target,strlen(argv[2]));//用于存放模式串
	STRCOPY(argv[1], Station);//将输入的命令行参数存放进目标串中
	STRCOPY(argv[2], Target);//将输入的命令行参数存放进模式串中
	//visit(Station);
	//visit(Target);
	Index_CMP(Station, Target,position,result);//CMP算法
	free(Station);
	free(Target);
	return 0;
}//进行KMP寻址之前的调用与初始化函数
Status Ans(int* result)
{
	if (!result) return ERROR;
	switch (result[statu])
	{
		case FALSE:
		{
			printf("-1");
			return FALSE;//FALSE表示未找到
		}
		case OK:
		{
			if (result[index]!=none)
			{
				printf("%d", result[index]);
			}
			else //当data项为none时，模式串没有任何输入，返回ERROR_01
			{
				printf("ERROR_01");
			}
			return OK;
		}
	}
	return ERROR;
}//对答案进行输出

int main(int argc, char** argv)
{
	int* result = (int*)malloc((RESULT_SIZE) * sizeof(int));//result用于存放结果
	memset(result, 0, RESULT_SIZE);
	if (!result) return OVERFLOW;
	final_Index_Package(argc,argv, result);//用于寻找字符串的主函数
	Ans(result);//用与输出结果
	free(result);
}