#include "sort.h"

//#define ARRAYSIZE 1000000
#define ARRAYSIZE 10000000

int main(int argc, char** argv) {
  /*
  // 小数据测试
  int listsize = 8;
  int array[8] = {29,25,34,64,34,12,32,45};
  sort<int>(array, listsize);
  Print<int>(array, listsize);
  */
	
  int input = 0;	   // 排序类型: -1 --不增 ; +1 - 不降; 0 -- 随机

  Randomize();
 

  int listsize = 10000000;	
  int* array;  
  array = new int[ARRAYSIZE]; 

  /*
  // 也可以采用下列命令行的形式
  // <sortname> [+/-] <size_of_test> <threshold>
  //        -1 --不增 ; +1 - 不降; 无参数 --  0 随机
  // <size_of_test> 测试数据规模
  // <threshold> 快速排序、归并排序等某些需要阈值的排序

  int currarg;
  if ((argc < 2) || (argc > 4)) {
    cout << "Usage: <sortname> [+/-] <size> [<threshold>]\n";
    exit(-1);
  }
  currarg = 1;
  if (argv[currarg][0] == '-') {
    input = -1;
    currarg++;
  }
  else if (argv[currarg][0] == '+') {
    input = 1;
    currarg++;
  }
  listsize = atoi(argv[currarg++]);
  if (argc > currarg)
    THRESHOLD = atoi(argv[currarg]);
  if ((listsize > ARRAYSIZE) || (listsize < 0)) {
    cout << "Selected list size is too big\n";
    exit(-1);
  }
  cout << "Input: " << input << ", size: " << listsize << ", threshold: "
       << THRESHOLD << "\n";
  */

 
	int i, j;
	int k = ARRAYSIZE / listsize;

	if (input == -1)  {
		for (i=0; i<k; i++)							// 逆序
			for (j=0; j<listsize; j++)
				array[i*listsize + j] = listsize - j;
		}
	else if (input == 0)
		for (i=0; i<ARRAYSIZE; i++)
			array[i] = Random(32003);				// 随机
	else {											// 正序
		for (i=0; i<k; i++)	 
			for (j=0; j<listsize; j++)
				array[i*listsize + j] = j;
		}

	Settime();
	for (i=0; i<ARRAYSIZE; i+=listsize) {
		//sort<int>(&array[i], listsize);
//		Print(&array[i], listsize);
	}

	cout << "Sort with list size " << listsize
	   << ", array size " << ARRAYSIZE
       << ", and threshold " << THRESHOLD << ": "
       << Gettime()/k << " seconds\n";


	for (i=0; i<ARRAYSIZE; i+=listsize)
		for(int j=i+1; j<i+listsize; j++)
			if(array[j-1] > array[j])
				cout << "ERROR!!!\n";

	return 0;
}
