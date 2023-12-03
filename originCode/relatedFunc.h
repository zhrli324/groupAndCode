#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int numOfOne(int num) {
	// 计算十进制整数的二进制表示中1的个数，即权
	
	int numBits = sizeof(int) * 8;
	int cnt = 0;
	
	// 从最高位开始检查每一位
	for (int i = numBits - 1; i >= 0; i--) {
		// 右移 i 位并与 1 进行位运算，获取当前位的值
		int bit = (num >> i) & 1;
		if (bit == 1) cnt++;
	}
	
	return cnt;
}

void quickSort(int *a, int low, int high) {
	// 快速升序排序算法，排序依据是整数的二进制表示的权
	
	int i = low;	//第一位
	int j = high;	//最后一位
	int key = a[i]; //将第一个数作为基准值-- 先找到一个基准值
	
	while (i < j) {					
		while(i < j && numOfOne(a[j]) >= numOfOne(key))
		{
			j--;
		}
		a[i] = a[j];	
		
		while(i < j && numOfOne(a[i]) <= numOfOne(key))
		{
			i++;
		}
		a[j] = a[i];
	}
	a[i] = key;
	if (i-1 > low) {
		quickSort(a, low, i-1);
	}
	
	if (i+1 < high) {
		quickSort(a, i+1, high);
	}
} 

char* DtoB(int num, int n) {
	// 将十进制整型转化为二进制字符串
	// 输入十进制整型num和字符串的保留位数n
	
	char* binaryString = (char*)malloc((n + 1) * sizeof(char)); // +1 用于存放字符串结束符 '\0'
	
	// 初始化字符串为全0
	for (int i = 0; i < n; i++) {
		binaryString[i] = '0';
	}
	binaryString[n] = '\0'; // 字符串结束符
	
	// 从右往左设置二进制位
	for (int i = n - 1; i >= 0; i--) {
		if (num > 0) {
			binaryString[i] = (num % 2) + '0';
			num /= 2;
		}
	}
	
	return binaryString;
}

int BtoD(char* B) {
	// 将二进制字符串转化为十进制整型
	
	int result = 0;
	for (int i = 0; B[i] == '0' || B[i] == '1'; i++) {
		result = result * 2 + (int)B[i] - 48;
	}
	
	return result;
}
