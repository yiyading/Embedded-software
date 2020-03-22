#include"./barrel.h"
#include<stdio.h>

/*
len: length of input array
ascending: 1 ascending sort ;0 descending sort
*/
void barrel(int* input_array, int len, int* output_array, int ascending)
{
	int barrel_array[barrel_num]={0};
	int i, k=0;
	for(i = 0; i < len; i++)
	{
		barrel_array[ (int)input_array[i] ]++;//fill water into barrels
	}
	if(ascending == 1)//ascending sort
	{
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
	else if(ascending == 0)//descending sort 
	{
		for(i = (barrel_num-1); i >= 0; i--)
                {
                        while( barrel_array[i] > 0)
                        {
                                barrel_array[i]--;
                                output_array[k] = i;
                                k++;
                        }
                }
	}

}
