#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "relatedFunc.h"

int main() {
	
	printf("*** 离散数学II课程实验之群与编码 ***\n");
	printf("----------------------------------------------\n");
	printf("----------------------------------------------\n");
	
	/* 实验一 */
	
	printf("(1).请输入m与n, 其中1<m<n<7; 并输入一致性校验矩阵的上半部分：\n");
	int m, n; 
	scanf("%d %d", &m, &n);
	int r = n - m; // 矩阵H是n*r矩阵
	
	
	// 创建并读取校验矩阵H
	int **H = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++) {
		H[i] = (int *)malloc(r * sizeof(int));
	}
	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < r; j++) {
			scanf("%d", &H[i][j]);
		}
	}
	
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			H[m + i][j] = 0;
		}
		H[m + i][i] = 1;
	}
	
	
	// 生成源码数组originCode
	int rowCount = 1 << m;
	int colCount = m;
	int **originCode = (int **)malloc(rowCount * sizeof(int *));
	for (int i = 0; i < rowCount; i++) {
		originCode[i] = (int *)malloc(colCount * sizeof(int));
		for (int j = 0; j < colCount; j++) {
			originCode[i][j] = (i >> (m - j - 1)) & 1;
		}
	}
	
	// rowCount和colCount分别是陪集表的高和宽
	colCount = 1 << m;
	rowCount = 1 << r;
	
	// 生成编码数组N
	int **N = (int **)malloc(colCount * sizeof(int *));
	for (int i = 0; i < colCount; i++) {
		N[i] = (int *)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++) {
			N[i][j] = 0;
		}
	}
	
	for (int i = 0; i < colCount; i++) {
		for (int j = 0; j < m; j++) {
			N[i][j] = originCode[i][j];
		}
		for (int j = 0; j < m; j++) {
			for (int k = 0; k < r; k++) {
				N[i][m + k] += originCode[i][j] * H[j][k];
				N[i][m+k] %= 2;
			}
		}
	}
	
	
	// 输出结果
	printf("\n所有的码字如下：\n");
	for (int i = 0; i < colCount; i++) {
		printf("e(");
		for (int j = 0; j < m; j++) {
			printf("%d", originCode[i][j]);
		}
		printf(")=");
		for (int j = 0; j < n; j++) {
			printf("%d", N[i][j]);
		}
		printf("\n");
	}

	
	// ------------------------------------------------
	// ------------------------------------------------
	
	
	/* 实验二 */
	
	printf("----------------------------------------------\n");
	printf("----------------------------------------------\n");
	printf("(2).输出左陪集表\n");
	getchar();

	// 生成所有2^n个整数，并按权升序排列，存入Dall中
	int* Dall = (int*)malloc((int)pow(2, n) * sizeof(int));
	
	for (int i = 0; i < (int)pow(2, n); i++) {
		Dall[i] = i;
	}
	quickSort(Dall, 0, (int)pow(2, n));
	
	// 定义陪集表数组cosetTable
	// cosetTable的元素(即每一种排列)以十进制整型表示，方便异或运算
	// 将第一行的所有情况的十进制整型存入陪集表中
	int **cosetTable = (int **)malloc(rowCount * sizeof(int *));
	for (int i = 0; i < rowCount; i++) {
		cosetTable[i] = (int *)malloc(colCount * sizeof(int));
	}
	for (int i = 0; i < colCount; i++) {
		int dNum = 0;
		for (int j = 0; j < n; j++) {
			dNum = dNum * 2 + N[i][j];
		}
		cosetTable[0][i] = dNum;
	}
	
	// 确定陪集头，检测Dall下一个元素是否与前面元素重复，并将十进制陪集表生成出来
	int count = 1; // 记录Dall中的第count个数
	for (int row = 1; row < rowCount; row++) { // row记录第几行
		ReLoop:
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < colCount; j++) {
				if (Dall[count] == cosetTable[i][j]) {
					count++;
					goto ReLoop;
				}
			}
		}
		
		cosetTable[row][0] = Dall[count];
		for (int i = 1; i < colCount; i++) {
			cosetTable[row][i] = cosetTable[row][0] ^ cosetTable[0][i];
		}
	}
	
	// 打印出陪集表元素的二进制表达
	printf("陪集表如下：\n");
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount-1; j++) {
			printf("%s ", DtoB(cosetTable[i][j], n));
		}
		printf("%s\n", DtoB(cosetTable[i][colCount-1], n));
	}
	
	
	// ------------------------------------------------
	// ------------------------------------------------
	
	
	/* 实验三 */
	
	printf("----------------------------------------------\n");
	printf("----------------------------------------------\n");
	printf("(3).用极大似然译码函数进行译码\n");
	
	// 通过陪集表元素，对code_b进行译码
	printf("请输入译码个数N，并输入N行比特串：\n");
	int numOfCode;
	scanf("%d", &numOfCode);
	while (numOfCode--) {
		char code_b[10];
		scanf("%s", code_b);
		int code_d = BtoD(code_b);
		int oriLoc;
		for (int i = 0; i < rowCount; i++) {
			for (int j = 0; j < colCount; j++) {
				if (code_d == cosetTable[i][j]) {
					oriLoc = j;
					break;
				}
			}
		}
		
		// 输出译码结果
		printf("译码结果是：");
		printf("d(");
		printf("%s", code_b);
		printf(")=");
		for (int i = 0; i < m; i++) {
			printf("%d", originCode[oriLoc][i]);
		}
		printf("\n");
	}
	
	
	// ------------------------------------------------
	// ------------------------------------------------
	
	
	/* 实验四 */
	
	printf("----------------------------------------------\n");
	printf("----------------------------------------------\n");
	printf("(4).用特征值进行译码\n");
	
	// 生成特征值数组syndrome
	int* syndrome = (int*)malloc(rowCount * sizeof(int));
	for (int i = 0; i < rowCount; i++) syndrome[i] = 0;
	
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < r; j++) {
			int synd = 0;
			for (int k = 0; k < n; k++) {
				synd += ((int)(DtoB(cosetTable[i][0], n)[k]) - 48) * H[k][j];
				synd %= 2;
			}
			syndrome[i] = syndrome[i] * 2 + synd;
		}
	}
	
	// 通过特征值数组来译码
	printf("请输入译码个数N，并输入N行比特串：\n");
	int numOfXt;
	scanf("%d", &numOfXt);
	
	while(numOfXt--) {
		char Xt[10];
		scanf("%s", Xt);
		
		int syndOfXt = 0;
		for (int i = 0; i < r; i++) {
			int temp = 0;
			for (int j = 0; j < n; j++) {
				temp += ((int)(Xt[j] - 48)) * H[j][i];
				temp %= 2;
			}
			syndOfXt =  syndOfXt* 2 + temp;
		}
		
		int index = 0;
		for (int i = 0; i < rowCount; i++) {
			if (syndrome[i] == syndOfXt) {
				index = i;
				break;
			}
		}
		
		int oriD = BtoD(Xt);
		int resD = oriD ^ cosetTable[index][0];
		int resCode;
		for (int i = 0; i < colCount; i++) {
			if (resD == cosetTable[0][i]) {
				resCode = i;
			}
		}
		
		// 打印结果
		printf("译码结果为：");
		printf("f(");
		printf("%s", Xt);
		printf(")=");
		printf("%s, ε=", DtoB(syndOfXt, m));
		printf("%s, d(%s)=", DtoB(cosetTable[index][0], n), Xt);
		printf("%s\n", DtoB(resCode, m));
	}
	
	printf("----------------------------------------------\n");
	printf("-------------------The End--------------------\n");
	
	// ------------------------------------------------
	// ------------------------------------------------
	
	
	// 释放动态分配的内存
	for (int i = 0; i < n; i++) {
		free(H[i]);
	}
	free(H);
	
	for (int i = 0; i < rowCount; i++) {
		free(originCode[i]);
	}
	free(originCode);
	
	for (int i = 0; i < colCount; i++) {
		free(N[i]);
	}
	free(N);
	
	free(Dall);
	
	for (int i = 0; i < rowCount; i++) {
		free(cosetTable[i]);
	}
	free(cosetTable);
	
	free(syndrome);
	
	getchar();
	return 0; 
}

