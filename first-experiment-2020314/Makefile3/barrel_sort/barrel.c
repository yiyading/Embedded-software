#include"./barrel.h"
#include<stdio.h>

void barrel(int* input_array, int len, int* output_array)
{
	int barrel_array[barrel_num]={0};
	int i, k=0;
	for(i = 0; i < len; i++)
	{
		barrel_array[ (int)input_array[i] ]++;
	}
	for(i = 0; i < barrel_num; i++)
	{
		while( barrel_array[i] > 0)
		{
			barrel_array[i]--;
			output_array[k] = i;
			k++;
		}
	}
}
