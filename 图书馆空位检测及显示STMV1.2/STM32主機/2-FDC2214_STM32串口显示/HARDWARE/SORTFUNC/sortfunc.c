#include "sortfunc.h"

//#include<stdio.h>

//int arr[] = { 10,6,5,2,3,8,7,4,9,1 };
//int n = sizeof(arr) / sizeof(int);

//冒泡排序算法
void bubbleSort(float *arr, int n) {
    int i=0,j=0;
	float temp=0;
	for ( i = 0; i<n - 1; i++)
        for ( j = 0; j < n - i - 1; j++)
        {
            //如果前面的数比后面大，进行交换
            if (arr[j] > arr[j + 1]) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
}


//升级版冒泡排序算法
void bubbleSort_super(float *arr, int n) {
    //设置数组左右边界
    int left = 0, right = n - 1;
	int i=0,j=0;
	float temp=0;
    //当左右边界未重合时，进行排序
    while (left<right) {
        //从左到右遍历选出最大的数放到数组右边
        for ( i =left; i < right; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i]; arr[i] = arr[i + 1]; arr[i + 1] = temp;
            }
        }
        right--;
        //从右到左遍历选出最小的数放到数组左边
        for ( j = right;j> left-1; j--)
        {
            if (arr[j + 1] < arr[j])
            {
                 temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
		
        left++;
    }

}

float EliminateFunc(float *arr,int digit, int n)
{
	int i=0;
	float sumval=0,averageval=0;
	for(i=0;i<n;i++)
	{
		sumval +=arr[digit-1+i];
	}
	averageval =sumval/n;
	return averageval;	
}


void MyStrncpy(char * _Dest, const char * _source, char length)
{
	int i=0;
	for (i = 0; i < length; i++, _Dest++, _source++)
	{
		*_Dest = *_source;
	}
}



