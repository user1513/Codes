#include "sortfunc.h"

//#include<stdio.h>

//int arr[] = { 10,6,5,2,3,8,7,4,9,1 };
//int n = sizeof(arr) / sizeof(int);

//ð�������㷨
void bubbleSort(float *arr, int n) {
    int i=0,j=0;
	float temp=0;
	for ( i = 0; i<n - 1; i++)
        for ( j = 0; j < n - i - 1; j++)
        {
            //���ǰ������Ⱥ���󣬽��н���
            if (arr[j] > arr[j + 1]) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
}


//������ð�������㷨
void bubbleSort_super(float *arr, int n) {
    //�����������ұ߽�
    int left = 0, right = n - 1;
	int i=0,j=0;
	float temp=0;
    //�����ұ߽�δ�غ�ʱ����������
    while (left<right) {
        //�����ұ���ѡ���������ŵ������ұ�
        for ( i =left; i < right; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i]; arr[i] = arr[i + 1]; arr[i + 1] = temp;
            }
        }
        right--;
        //���ҵ������ѡ����С�����ŵ��������
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
