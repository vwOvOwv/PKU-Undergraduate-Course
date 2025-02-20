/*  刘沛雨 2100012289 */
/*  该程序模拟了访问内存时cache的行为并对访问内存造成的miss, hit, eviction进行计数
*   采用LRU替换算法
*/
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define INT_MAX 2147483647

typedef struct  //用结构体表示每个高速缓存行
{
    int valid;  //有效位
    long tag;   //标记位
    int last_access_time;   //上次访问时间（用于执行LRU算法）
} cache_line;

int s, E, b, S, B;
long t_mask, s_mask;    //分别是计算标记位和组数所需的掩码
char *file_path = NULL; //用于读取.trace文件
//计数
int hit_count = 0;  
int miss_count = 0;
int eviction_count = 0;

char operation; //访问内存的操作：L/S/M
long address;   //访问内存的地址
int size;   //访问内存块的大小
int cur_cycle = 0;  //循环计数，用于记录内存块的访问时间

/*cache_run*/
/*模拟一次操作中cache的运行*/
void cache_run(cache_line **M)
{
    long addr = address;
    //根据掩码计算标记位和组数
    long tag_bit = addr & t_mask;
    long s_bit = addr & s_mask;
    long ss = ((unsigned)s_bit) >> b;
    //进行模拟
    int hit = 0;
    int ee;
    for (ee = 0; ee < E; ee++)  //遍历查找对应组的所有缓存行，直到遇到第一个无效行
    {
        cache_line tmp = M[ss][ee];
        if (tmp.valid == 1 && tmp.tag == tag_bit)   //找到对应行，命中
        {
            hit = 1;
            break;
        }
        if (tmp.valid == 0) //查找结束
            break;
    }
    if (!hit)   //未命中的情况
    {
        miss_count++;
        if (ee < E) //该缓存组没有被完全填充，即冷不命中
        {
            M[ss][ee].valid = 1;
            M[ss][ee].tag = tag_bit;
            M[ss][ee].last_access_time = cur_cycle;
        }
        else    //该缓存组已经被完全填充，需要进行驱逐/替换
        {
            eviction_count++;
            //根据最近访问时间查找应当被替换的缓存行
            int ee_evicted = 0;
            int min_last_access_time = INT_MAX;
            for (int i = 0; i < E; i++)
            {
                cache_line tmp = M[ss][i];
                if (tmp.last_access_time < min_last_access_time)
                {
                    min_last_access_time = tmp.last_access_time;
                    ee_evicted = i;
                }
            }
            //将被驱逐的缓存行替换为被访问缓存行
            M[ss][ee_evicted].tag = tag_bit;
            M[ss][ee_evicted].last_access_time = cur_cycle;
        }
    }
    else    //命中的情况
    {
        hit_count++;
        M[ss][ee].last_access_time = cur_cycle;
    }
    return;
}

/*main*/
/*  读取命令行参数，获取cache参数
*   根据cache参数分配内存以表示缓存
*   读取.trace文件并进行模拟
*/
int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 's':
            // 处理选项 's'
            if (optarg != NULL)
                s = atoi(optarg);
            break;
        case 'E':
            // 处理选项 'E'
            if (optarg != NULL)
                E = atoi(optarg);
            break;
        case 'b':
            // 处理选项 'b'
            if (optarg != NULL)
                b = atoi(optarg);
            break;
        case 't':
            // 处理选项 't'
            if (optarg != NULL)
                file_path = optarg;
            break;
        case '?':
            // 处理未知选项
            return EXIT_FAILURE;
        default:
            // 处理其他情况
            return EXIT_FAILURE;
        }
    }
    // 计算辅助量
    S = 1 << s;
    B = 1 << b;
    t_mask = ((long)1 << 63) >> (63 - s - b);
    s_mask = (~t_mask) & (((long)1 << 63) >> (63 - b));
    // 分配内存空间
    cache_line **M = (cache_line **)calloc(S, sizeof(cache_line *));
    for (int i = 0; i < S; i++)
        M[i] = (cache_line *)calloc(E, sizeof(cache_line));
    // 读trace文件内容，进行模拟
    if (file_path)
    {
        FILE *file = fopen(file_path, "r");
        if (file == NULL)   //文件打开错误，程序结束
        {
            for (int i = 0; i < S; i++)
                free(M[i]);
            free(M);
            return EXIT_FAILURE;
        }
        while (fscanf(file, " %c %lx,%d", &operation, &address, &size) != EOF)
        {
            switch (operation)
            {
            case 'L':   //读操作
                cache_run(M);
                break;
            case 'S':   //写操作
                cache_run(M);
                break;
            case 'M':   //修改内存数据可以看作一次读和一次写
                cache_run(M);
                cache_run(M);
                break;
            default:
                break;
            }
            cur_cycle++;    //循环计数
        }
    }
    // 输出结果
    printSummary(hit_count, miss_count, eviction_count);
    // 释放内存
    for (int i = 0; i < S; i++)
        free(M[i]);
    free(M);

    return EXIT_SUCCESS;
}