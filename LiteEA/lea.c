/*
 ============================================================================
 Name        : lea.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 一些工程算法或面试题目
 ============================================================================
 */
#include "lea.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * 验证一个数组是否是一个二叉查找树的后序遍历结果
 * @param  squence 待验证的数组
 * @param  length  数组长度
 * @return         验证通过返回looc_true，否则返回looc_false
 */
looc_bool verifySquenceOfBST(int squence[], int length) {
	if (squence == NULL || length <= 0) {
		return looc_false;
	}
	/* 二叉查找树的根节点是后序遍历的最后一个元素 */
	int root = squence[length - 1];
	/* 左子树的节点小于根节点 */
	int i = 0;
	for (; i < length - 1; i++) {
		if (squence[i] > root) {
			break;
		}
	}
	/* 右子树的元素应该大于根节点 */
	int j = i;
	for (; j < length - 1; j++) {
		if (squence[j] < root) {
			return looc_false;
		}
	}
	/* 递归验证左子树是不是一个BST */
	looc_bool left = looc_true;
	if (i > 0) {
		left = verifySquenceOfBST(squence, i);
	}
	/* 递归验证右子树是不是一个BST */
	looc_bool right = looc_true;
	if (i < length - 1) {
		right = verifySquenceOfBST(squence + i, length - i - 1);
	}
	return (left & right);
}

static void _transNum(int origin, int num, int d, char result[]) {
	static int noEnd = 1;
	static int i = 0;
	int mod;

	if (num == origin) {
		i = 0;
		noEnd = 1;
	}

	mod = num % d;
	num = num / d;
	while (noEnd && num) {
		_transNum(origin, num, d, result);
	}
	noEnd = 0;
	if (mod >= 10) {
		mod = mod - 10 + 'A';
	} else {
		mod = mod + '0';
	}
	result[i++] = mod;
	result[i] = '\0';
}

/**
 * 数制转换(辗转相除)
 * @param num    待转换的数字
 * @param d      进制
 * @param result 存放转换后的字符串
 */
void transNum(int num, int d, char result[]) {
	_transNum(num, num, d, result);
}

/**
 * 判断CPU是否是小端模式
 * @return  如果是小端模式返回true，否则返回false
 */
looc_bool isLittleEndian(void) {
	union w {
		int a;
		char b;
	} c;
	c.a = 1;
	if (c.b == 1) {
		return looc_true;
	}
	return looc_false;
}

/**
 * 判断是否为完全平方数
 * @param  x 待判断的整数
 * @return   是完全平方数返回true，否则返回false
 */
looc_bool isPerfectSquare(int x) {
	int sum = 0;
	int i = 1;
	for (i = 1; sum < x; i += 2) {
		sum += i;
	}
	if (sum == x) {
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 计算第n个斐波那契数
 * @param  n 第几个斐波那契数
 * @return   返回计算结果
 */
int Fibonacci(int n) {
	int result;
	int last, nextToLast;
	int i;
	if (n <= 1) {
		return 1;
	}
	last = nextToLast = 1;
	for (i = 0; i < n - 1; i++) {
		result = last + nextToLast;
		nextToLast = last;
		last = result;
	}
	return result;
}

/**
 * RabinKarp字符串匹配算法
 * @param  T        待查字符串
 * @param  n        字符串T的长度
 * @param  P        模式字符串
 * @param  m        模式字符串长度
 * @param  q        将字符映射成数字用的模
 * @param  position 保存所有的有效位移
 * @return          返回有效位移的数量
 * 时间复杂度：O(m+n)
 */
int RabinKarp(char* T, int n, char* P, int m, int q, int* position) {
	int count = 0;
	int i, j;
	int h = 1;	//P元素最高次幂
	int p = 0;	//P的十进制值
	int t = 0;
	/* 求h的hash值 */
	for (i = 1; i < m; i++) {
		h = (h * 10) % q;
	}
	/* 计算模式字符串的hash值和待查字符串前m个字符的hash值 */
	for (i = 0; i < m; i++) {
		p = (10 * p + P[i]) % q;
		t = (10 * t + T[i]) % q;
	}
	/* 比较n-m+1次 */
	for (i = 0; i <= n - m; i++) {
		if (p == t) {
			/* 如果模值相等则使用朴素算法再次判断 */
			for (j = 0; j < m; j++) {
				if (P[j] != T[i + j]) {
					break;
				}
			}
			if (j >= m) {
				/* 保存有效位移 */
				position[count++] = i;
			}
		}
		if (i < n - m) {
			t = (10 * (t - T[i] * h) + T[i + m]) % q;	//递归求新的t值
			if (t < 0) {
				t = t + q;
			}
		}
	}
	return count;
}

/**
 * 最大公约数(辗转求余)
 * @param  a 整数a
 * @param  b 整数b
 * @return   返回a和b的最大公约数
 */
int gcd(int a, int b) {
	int max = a > b ? a : b;
	int min = max == a ? b : a;
	int r;
	while (min) {
		r = max % min;
		max = min;
		min = r;
	}
	return max;
}

/**
 * 判断一个数是否为素数
 * @param  number 待判定的数
 * @return        是素数就返回true，否则返回false
 */
looc_bool isPrime(int number) {
	int i;
	for (i = 2; i * i <= number; i++) {
		if (number % i == 0) {
			break;
		}
	}
	if (i * i > number) {
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 寻找指定范围内的所有素数(筛选法)
 * @param number 指定范围
 * @param result 保存指定范围内的素数
 * @return       返回指定范围内素数的个数
 */
int prime(int number, int* result) {
	int i, j;
	int count = 0;
	int* temp = (int*) looc_malloc(sizeof(int) * number, "prime_temp",
	looc_file_line);
	/* 初始化temp数组保存1~number所有整数 */
	for (i = 0; i < number; i++) {
		temp[i] = i + 1;
	}
	/* 筛选法找素数 */
	for (i = 2; i * i <= number; i++) {
		for (j = 2 * i; j <= number; j += i) {
			temp[j - 1] = 0;
		}
	}
	/* 保存素数 */
	for (i = 1; i < number; i++) {
		if (temp[i]) {
			result[count++] = i + 1;
		}
	}
	looc_free(temp);
	return count;
}

/**
 * 寻找指定范围内的完美数(一个自然数的所有因子的和是这个数的本身)
 * @param  n      指定的数据
 * @param result  保存指定范围内的完美数
 * @return        返回指定范围内完美数的个数
 */
int PerfectNumber(int n, int* result) {
	int i, j, k;
	int num;
	int totalPrime;
	int s, r, q;
	int count = 0;
	/* 保存素数表 */
	int* tPrime = (int*) looc_malloc(sizeof(int) * 100, "perfectNumber_tPrime",
	looc_file_line);
	/* 保存质因数分解结果 */
	int* tFactor = (int*) looc_malloc(sizeof(int) * 100,
			"perfectNumber_tFactor",
			looc_file_line);
	/* 先求解n以内的所有质因数 */
	totalPrime = prime(n, tPrime);
	/* 循环判断每个数k是否是完美数 */
	for (k = 1; k <= n; k++) {
		num = k;
		j = 0;
		while (1) {
			for (i = 0; i < totalPrime; i++) {
				/* 找到了质因数 */
				if (tPrime[i] && num % tPrime[i] == 0) {
					/* 保存到质因数表中 */
					tFactor[j++] = tPrime[i];
					num /= tPrime[i];
					break;
				}
			}
			if (num <= 1) {
				break;
			}
		}
		/* 判断是否是完美数 */
		r = 1;
		s = 1;
		q = 1;
		for (i = 0; i < j; i++) {
			r *= tFactor[i];
			s += r;
			if (i + 1 < j && tFactor[i] == tFactor[i + 1]) {
				continue;
			} else {
				q *= s;
				r = 1;
				s = 1;
			}
		}
		/* 完美数等于其质因数的i次幂之和的乘积的一半 */
		/* 比如28=(2^0+2^1+2^2)*(7^0+7^1)/2 */
		if ((k == 1) || (q == 2 * k)) {
			result[count++] = k;
		}
	}
	/* 释放内存 */
	looc_free(tPrime);
	looc_free(tFactor);
	return count;
}

/**
 * 将n张牌重新洗牌(实质上是简单的数组元素交换)
 * @param card 牌数组
 * @param n    牌的张数，一般扑克牌共52张，包括花色
 */
void shuffle(int* card, int n) {
	int i, j, temp;
	for (i = 0; i < n; i++) {
		card[i] = i + 1;
	}
	srand(time(NULL));	//随机数种子
	/* 进行洗牌 */
	for (i = 0; i < n; i++) {
		j = rand() % n + 1;	//1,2,3...n
		temp = card[i];
		card[i] = card[j];
		card[j] = temp;
	}
}

/**
 * 约瑟夫问题
 * @param n     总人数
 * @param m     死亡报数
 * @param order 保存自杀的顺序号
 */
void Josephus(int n, int m, int* order) {
	int* status = (int*) looc_malloc(sizeof(int) * n, "Josephus_status",
	looc_file_line);
	int i, j;
	int count;
	for (i = 0; i < n; i++) {
		status[i] = 1;	//1表示活着，0表示死亡
	}
	i = 0;
	j = 0;
	while (count < n) {
		if (status[i]) {
			j++;
			if (j == m) {
				status[i] = 0;
				j = 0;
				order[count++] = i;
			}
		}
		i++;
		if (i == n) {
			i = 0;
		}
	}
	looc_free(status);
}

static int HanoiSteps = 0;

static void _hanoiMove(int n, char a, char b, char c, int show) {
	if (n == 1) {
		HanoiSteps++;
		if (show) {
			printf("%c --> %c\r\n", a, c);
		}
	} else {
		/* 先将a中的n-1个圆盘借助c移动到b中 */
		_hanoiMove(n - 1, a, c, b, show);
		/* 将a中最后一个圆盘移动到c中 */
		HanoiSteps++;
		if (show) {
			printf("%c --> %c\r\n", a, c);
		}
		/* 再将b中n-1个圆盘借助a移动到c中 */
		_hanoiMove(n - 1, b, a, c, show);
	}
}

/**
 * 汉诺塔问题(将a中的圆盘借助b移动到c中，其中任何时刻要保证大盘在下，小盘在上)
 * @param  n    汉诺塔中需要移动的圆盘的数目
 * @param  a    圆盘a的标识
 * @param  b    圆盘b的标识
 * @param  c    圆盘c的标识
 * @param  show 是否需要打印中间过程
 * @return      总共需要的步骤
 */
int stepsOfHanoi(int n, char a, char b, char c, int show) {
	HanoiSteps = 0;
	_hanoiMove(n, a, b, c, show);
	return HanoiSteps;
}

/**
 * 迭代法求平方根
 * @param  a 根
 * @return   返回a的平方根
 * 求解平方根的迭代公式：
 * 	X(n+1) = 1/2*(X(n)+a/X(n))
 */
double SQR(double a) {
	double x = a, y = 0;	//迭代初始值
	while (fabs(y - x) > 0.000001) {
		y = x;
		x = 0.5 * (x + a / x);
	}
	return x;
}

/**
 * 一重定积分(梯形法)
 * @param  express	表达式函数指针
 * @param  down 	积分下限
 * @param  up 		积分上限
 * @return   		返回定积分值
 */
double DefiniteIntegration(double (*express)(double x), double down, double up) {
	double T1, T2;
	double S = 0;	//梯形面积和
	double x = down;
	double h = up - down;
	/* 初始化面积 */
	T1 = h / 2 * (express(down) + express(up));
	while (1) {
		S = 0;
		x = down + h / 2;
		while (x < up) {
			S += express(x);
			x += h;
		}
		T2 = T1 / 2 + h / 2 * S;
		if (fabs(T1 - T2) >= 0.000001) {
			h /= 2;
			T1 = T2;
		} else {
			break;
		}
	}
	return T2;
}

static void shift_reverse(int* array, int start, int end) {
	int temp;
	int i, j;
	/* 对称翻转 */
	for (i = start, j = end; i < j; i++, j--) {
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

/**
 * 将数组array循环左移num个单位
 * @param array 数组
 * @param len   array的长度
 * @param num   循环左移的长度
 * 原理：通过ab来求ba可以转换成将a翻转，再将b翻转，最后将数组整体翻转
 */
void shift_left(int* array, int len, int num) {
	shift_reverse(array, 0, num - 1);	//递归对每一部分进行翻转
	shift_reverse(array, num, len - 1);
	shift_reverse(array, 0, len - 1);	//对整个数组进行翻转
}

static void keepMinHeap(int* array, int n, int k) {
	int left = k * 2 + 1;
	int right = k * 2 + 2;
	int min = k;
	int temp;
	if (left < n && array[left] < array[k]) {
		min = left;
	}
	if (right < n && array[right] < array[min]) {
		min = right;
	}
	if (min != k) {
		/* 因为局部根不是最小节点，所以需要替换 */
		temp = array[min];
		array[min] = array[k];
		array[k] = temp;
		/* 依次向下递归 */
		keepMinHeap(array, n, min);
	}
}

static void buildMinHeap(int* array, int n) {
	int i = n / 2 - 1;	//最后一个非叶子结点
	/* 从最后一个非叶子结点开始向上递归，保持最小堆的性质 */
	while (i >= 0) {
		keepMinHeap(array, n, i);
		i--;
	}
}

/**
 * 寻找最大的N个数
 * @param  data   待查找数组
 * @param  m      数组data的长度
 * @param  N      寻找最大的N个数
 * @param  result 保存最大的N个数
 * @return        成功返回true，失败返回false
 * 时间复杂度：O(mlog2N)，比直接快速排序要好
 */
looc_bool maxN(int* data, int m, int N, int* result) {
	int i;
	if (data == NULL || N > m) {
		return looc_false;
	}
	/* 用data数组中的前N数据来初始化最小堆 */
	for (i = 0; i < N; i++) {
		result[i] = data[i];
	}
	buildMinHeap(result, N);
	/* 拿data中后续的数据中大的值与最小堆中的根节点替换 */
	for (i = N; i < m; i++) {
		if (data[i] > result[0]) {
			result[0] = data[i];
			keepMinHeap(result, N, 0);
		}
	}
	return looc_true;
}

/**
 * 查找数组中的最大值和最小值
 * @param  array  数组
 * @param  n      数组大小
 * @param  result 保存最大值和最小值，result[0]是最大值，result[1]是最小值
 * @return        成功返回true，失败返回false
 * 时间复杂度：O(n)，但是该方法遍历整个数组只需要1.5n次比较
 */
looc_bool getMaxMin(int* array, int n, int* result) {
	int max, min;
	int i;
	if (array == NULL || n <= 0) {
		return looc_false;
	}
	max = min = array[n - 1];
	for (i = 0; i < n / 2; i++) {
		if (array[2 * i] < array[2 * i + 1]) {
			max = array[2 * i + 1] > max ? array[2 * i + 1] : max;
			min = array[2 * i] < min ? array[2 * i] : min;
		} else if (array[2 * i] > array[2 * i + 1]) {
			max = array[2 * i] > max ? array[2 * i] : max;
			min = array[2 * i + 1] < min ? array[2 * i + 1] : min;
		}
	}
	/* 保存最大最小值 */
	result[0] = max;
	result[1] = min;
	return looc_true;
}

/**
 * 计算整数n的二进制表示中的1的个数
 * @param  n      整数n
 * @return        成功n的二进制表示中的1的个数
 */
int countBinaryOne(int n) {
	int count = 0;
	while (n) {
		count++;
		n = n & (n - 1);
	}
	return count;
}

struct _opera_s {
	char opr;
	int prio;
};

static int getOpPrio(char oper) {
	int ret = -1;
	switch (oper) {
	case '(':
		ret = 0;
		break;
	case '+':
		ret = 1;
		break;
	case '-':
		ret = 1;
		break;
	case '*':
		ret = 2;
		break;
	case '/':
		ret = 2;
		break;
	case '^':
		ret = 3;
		break;
	default:
		break;
	}
	return ret;
}

static double getOpResult(char oper, double num1, double num2) {
	double ret = 0;
	switch (oper) {
	case '+':
		ret = num2 + num1;
		break;
	case '-':
		ret = num2 - num1;
		break;
	case '*':
		ret = num2 * num1;
		break;
	case '/':
		ret = num2 / num1;
		break;
	case '^':
		ret = pow(num2, num1);
		break;
	default:
		break;
	}
	return ret;
}

/**
 * 表达式求值(加、减、乘、除、乘方)
 */
double computeExpress(const char* express) {
	double result = 0.0;
	const char* p = express;
	struct _opera_s opStack[20];
	struct _opera_s tempOP;
	double numStack[20];
	int opIndex = 0;
	int numIndex = 0;
	double internNum = 0;
	char numStr[10];
	int strIndex = 0;
	double num1, num2;
	while (*p) {
		if ((*p >= '0' && *p <= '9') || *p == '.') {
			numStr[strIndex++] = *p;
		} else if (*p != ' ') {
			/* 遇到运算符 */
			if (strIndex) {
				numStr[strIndex] = '\0';
				strIndex = 0;
				internNum = atof(numStr);
				numStack[numIndex++] = internNum;
			}

			if (opIndex) {
				tempOP = opStack[opIndex - 1];
				if (*p == '(' || getOpPrio(*p) > tempOP.prio) {
					opStack[opIndex].opr = *p;
					opStack[opIndex++].prio = getOpPrio(*p);
				} else if (*p == ')') {
					while (tempOP.opr != '(') {
						num1 = numStack[numIndex - 1];
						num2 = numStack[numIndex - 2];
						num1 = getOpResult(tempOP.opr, num1, num2);
						numStack[numIndex - 2] = num1;
						numIndex--;
						opIndex--;
						tempOP = opStack[opIndex - 1];
					}
					opIndex--;
				} else {
					while (tempOP.prio >= getOpPrio(*p)) {
						num1 = numStack[numIndex - 1];
						num2 = numStack[numIndex - 2];
						num1 = getOpResult(tempOP.opr, num1, num2);
						numStack[numIndex - 2] = num1;
						numIndex--;
						opIndex--;
						if (opIndex) {
							tempOP = opStack[opIndex - 1];
						} else {
							break;
						}
					}
					opStack[opIndex].opr = *p;
					opStack[opIndex++].prio = getOpPrio(*p);
				}
			} else {
				opStack[opIndex].opr = *p;
				opStack[opIndex++].prio = getOpPrio(*p);
			}
		}
		p++;
	}
	if (strIndex) {
		numStr[strIndex] = '\0';
		strIndex = 0;
		internNum = atof(numStr);
		numStack[numIndex++] = internNum;
	}
	while (opIndex) {
		tempOP = opStack[opIndex - 1];
		num1 = numStack[numIndex - 1];
		num2 = numStack[numIndex - 2];
		num1 = getOpResult(tempOP.opr, num1, num2);
		numStack[numIndex - 2] = num1;
		numIndex--;
		opIndex--;
	}
	result = numStack[numIndex - 1];
	return result;
}

/**
 * 寻找数组中的主元素
 * 成功返回0，错误返回-1
 * 基本思路：如果让主元素与一个非主元素“配对”，则最后多出来的元素就是主元素
 * 从前向后扫描数组元素，假定遇到的当前值选定为主元素，再次遇到它时，技术加1，遇到与它不等的值时，计数减1。当计数减为0后，将遇到的下一个值重新选定为主元素
 * 扫描完毕，当前选定的元素（计数大于0）可能是主元素，但未必一定就是，还需要对数组再进行一次扫描，记录它出现的实际个数，以判定它是否是主元素
 */
int findMajority(int A[], int n, int* major) {
	int i;
	int c = A[0];
	int count = 1;
	for (i = 1; i < n; i++) {
		if (c == A[i]) {
			count++;
		} else if (count) {
			count--;
		} else {
			c = A[i];
			count = 1;
		}
	}
	if (count > 0) {
		count = 0;
		for (i = 0; i < n; i++) {
			if (A[i] == c) {
				count++;
			}
		}
		if (count > n / 2) {
			*major = c;
			return 0;
		}
	}
	return -1;
}
