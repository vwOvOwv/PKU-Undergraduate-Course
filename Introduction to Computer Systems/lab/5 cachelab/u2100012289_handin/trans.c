/* 刘沛雨 2100012289 */
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    int n, m, i, j, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

    /*32*32*/
    /*  利用分块技术，将矩阵分为8*8的子矩阵，对对角块进行特殊处理
    *   首先将A的对角块复制到B中（循环展开），再在B中进行转置
    *   可以有效减少抖动等不命中情况
    */
    if (N == 32)
    {
        for (m = 0; m < 32; m += 8)
        {
            for (n = 0; n < 32; n += 8)
            {
                if (n != m) //非对角块直接转置即可
                {
                    for (i = m; i < m + 8; i++)
                    {
                        for (j = n; j < n + 8; j++)
                        {
                            B[j][i] = A[i][j];
                        }
                    }
                }
                else    //对角块
                {
                    for (i = m; i < m + 8; i++) //将该对角块从A复制到B
                    {
                        tmp1 = A[i][n];
                        tmp2 = A[i][n + 1];
                        tmp3 = A[i][n + 2];
                        tmp4 = A[i][n + 3];
                        tmp5 = A[i][n + 4];
                        tmp6 = A[i][n + 5];
                        tmp7 = A[i][n + 6];
                        tmp8 = A[i][n + 7];
                        B[i][n] = tmp1;
                        B[i][n + 1] = tmp2;
                        B[i][n + 2] = tmp3;
                        B[i][n + 3] = tmp4;
                        B[i][n + 4] = tmp5;
                        B[i][n + 5] = tmp6;
                        B[i][n + 6] = tmp7;
                        B[i][n + 7] = tmp8;
                    }
                    for (i = m; i < m + 8; i++) //在B中进行转置
                    {
                        for (j = n + i - m + 1; j < n + 8; j++)
                        {
                            tmp8 = B[i][j];
                            B[i][j] = B[j][i];
                            B[j][i] = tmp8;
                        }
                    }
                }
            }
        }
    }
    /*64*64*/
    /*  同样使用8*8分块，对角块与非对角块采用不同方式处理
    *   非对角块：
    *   将一个8*8的块分为4个4*4的块（左上角，右上角，左下角，右下角），逐个进行处理
    *   对角块：
    *   利用该块右侧的一个8*8块作为“缓冲区”，逐个小块进行处理
    *   不能处理整个矩阵最右下角的8*8块，该块用32*32中的对角块转置方式
    *   
    *   具体处理方式见函数内注释
    */
    else if (N == 64)
    {
        for (m = 0; m < 64; m += 8)
        {
            for (n = 0; n < 64; n += 8)
            {
                if (m != n)
                {
                    // 将A左上角转置后复制到B右下角，右上角正常转置到B左下角
                    for (i = m; i < m + 4; i++)
                    {
                        tmp1 = A[i][n];
                        tmp2 = A[i][n + 1];
                        tmp3 = A[i][n + 2];
                        tmp4 = A[i][n + 3];
                        tmp5 = A[i][n + 4];
                        tmp6 = A[i][n + 5];
                        tmp7 = A[i][n + 6];
                        tmp8 = A[i][n + 7];
                        B[n + 4][i + 4] = tmp1;
                        B[n + 5][i + 4] = tmp2;
                        B[n + 6][i + 4] = tmp3;
                        B[n + 7][i + 4] = tmp4;
                        B[n + 4][i] = tmp5;
                        B[n + 5][i] = tmp6;
                        B[n + 6][i] = tmp7;
                        B[n + 7][i] = tmp8;
                    }
                    // 逐行平移B右下角到B左上角，同时逐行转置A右下角到B右下角
                    for (i = n + 4; i < n + 8; i++)
                    {
                        tmp1 = B[i][m + 4];
                        tmp2 = B[i][m + 5];
                        tmp3 = B[i][m + 6];
                        tmp4 = B[i][m + 7];
                        //注意先转置再平移，可以减少内存的驱逐和替换
                        B[i][m + 4] = A[m + 4][i];
                        B[i][m + 5] = A[m + 5][i];
                        B[i][m + 6] = A[m + 6][i];
                        B[i][m + 7] = A[m + 7][i];
                        //
                        B[i - 4][m] = tmp1;
                        B[i - 4][m + 1] = tmp2;
                        B[i - 4][m + 2] = tmp3;
                        B[i - 4][m + 3] = tmp4;
                    }
                    // 转置A左下角到B右上角
                    for (i = n; i < n + 4; i++)
                    {
                        tmp1 = A[m + 4][i];
                        tmp2 = A[m + 5][i];
                        tmp3 = A[m + 6][i];
                        tmp4 = A[m + 7][i];
                        B[i][m + 4] = tmp1;
                        B[i][m + 5] = tmp2;
                        B[i][m + 6] = tmp3;
                        B[i][m + 7] = tmp4;
                    }
                }
                else
                {
                    if (n == 56 && m == 56) //最右下角的块需要特殊处理
                    {
                        for (i = m; i < m + 8; i++)
                        {
                            tmp1 = A[i][n];
                            tmp2 = A[i][n + 1];
                            tmp3 = A[i][n + 2];
                            tmp4 = A[i][n + 3];
                            tmp5 = A[i][n + 4];
                            tmp6 = A[i][n + 5];
                            tmp7 = A[i][n + 6];
                            tmp8 = A[i][n + 7];
                            B[i][n] = tmp1;
                            B[i][n + 1] = tmp2;
                            B[i][n + 2] = tmp3;
                            B[i][n + 3] = tmp4;
                            B[i][n + 4] = tmp5;
                            B[i][n + 5] = tmp6;
                            B[i][n + 6] = tmp7;
                            B[i][n + 7] = tmp8;
                        }
                        for (i = m; i < m + 8; i++)
                        {
                            for (j = n + i - m + 1; j < n + 8; j++)
                            {
                                tmp8 = B[i][j];
                                B[i][j] = B[j][i];
                                B[j][i] = tmp8;
                            }
                        }
                    }
                    else    //处理其他非对角块
                    {
                        //将A的左上角转置后移动到B右侧块的左上角
                        //将A的右上角转置后移动到B右侧块的右上角
                        for (i = m; i < m + 4; i++) 
                        {
                            tmp1 = A[i][n];
                            tmp2 = A[i][n + 1];
                            tmp3 = A[i][n + 2];
                            tmp4 = A[i][n + 3];
                            tmp5 = A[i][n + 4];
                            tmp6 = A[i][n + 5];
                            tmp7 = A[i][n + 6];
                            tmp8 = A[i][n + 7];
                            B[n][i + 8] = tmp1;
                            B[n + 1][i + 8] = tmp2;
                            B[n + 2][i + 8] = tmp3;
                            B[n + 3][i + 8] = tmp4;
                            B[n][i + 12] = tmp5;
                            B[n + 1][i + 12] = tmp6;
                            B[n + 2][i + 12] = tmp7;
                            B[n + 3][i + 12] = tmp8;
                        }
                        //将B右侧块的左上角移动到B的左上角
                        //将B右侧块的右上角移动到B的左下角
                        for (i = m; i < m + 4; i++)
                        {
                            tmp1 = B[i][n + 8];
                            tmp2 = B[i][n + 9];
                            tmp3 = B[i][n + 10];
                            tmp4 = B[i][n + 11];
                            tmp5 = B[i][n + 12];
                            tmp6 = B[i][n + 13];
                            tmp7 = B[i][n + 14];
                            tmp8 = B[i][n + 15];
                            B[i][n] = tmp1;
                            B[i][n + 1] = tmp2;
                            B[i][n + 2] = tmp3;
                            B[i][n + 3] = tmp4;
                            B[i + 4][n] = tmp5;
                            B[i + 4][n + 1] = tmp6;
                            B[i + 4][n + 2] = tmp7;
                            B[i + 4][n + 3] = tmp8;
                        }
                        //类似处理A的下半部分
                        for (i = m + 4; i < m + 8; i++)
                        {
                            tmp1 = A[i][n];
                            tmp2 = A[i][n + 1];
                            tmp3 = A[i][n + 2];
                            tmp4 = A[i][n + 3];
                            tmp5 = A[i][n + 4];
                            tmp6 = A[i][n + 5];
                            tmp7 = A[i][n + 6];
                            tmp8 = A[i][n + 7];
                            B[n][i + 4] = tmp1;
                            B[n + 1][i + 4] = tmp2;
                            B[n + 2][i + 4] = tmp3;
                            B[n + 3][i + 4] = tmp4;
                            B[n][i + 8] = tmp5;
                            B[n + 1][i + 8] = tmp6;
                            B[n + 2][i + 8] = tmp7;
                            B[n + 3][i + 8] = tmp8;
                        }
                        for (i = m; i < m + 4; i++)
                        {
                            tmp1 = B[i][n + 8];
                            tmp2 = B[i][n + 9];
                            tmp3 = B[i][n + 10];
                            tmp4 = B[i][n + 11];
                            tmp5 = B[i][n + 12];
                            tmp6 = B[i][n + 13];
                            tmp7 = B[i][n + 14];
                            tmp8 = B[i][n + 15];
                            B[i][n + 4] = tmp1;
                            B[i][n + 5] = tmp2;
                            B[i][n + 6] = tmp3;
                            B[i][n + 7] = tmp4;
                            B[i + 4][n + 4] = tmp5;
                            B[i + 4][n + 5] = tmp6;
                            B[i + 4][n + 6] = tmp7;
                            B[i + 4][n + 7] = tmp8;
                        }
                    }
                }
            }
        }
    }
    /*68*68*/
    /*  枚举不同分块大小即可，尝试发现24*4分块可以满足要求
    */
    else if (N == 68)
    {
        tmp1=24;
        tmp2=4;
        for (n = 0; n < 68; n += tmp1)
        {
            if (n + tmp1 < 68)    //避免数组越界访问
                tmp3 = n + tmp1;
            else
                tmp3 = 68;
            for (m = 0; m < 60; m += tmp2)
            {
                if (m + tmp2 < 60)    //避免数组越界访问
                    tmp4 = m + tmp2;
                else
                    tmp4 = 60;
                for (i = n; i < tmp3; i++)
                {
                    for (j = m; j < tmp4; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

