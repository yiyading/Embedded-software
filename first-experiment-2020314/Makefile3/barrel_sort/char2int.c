#include"./char2int.h"
#include<string.h>
#include<stdio.h>

int char2int(char* input, int* output)
{
	int i, k = 0;
	output[k] = 0;
	int len = strlen(input); 
	for(i=0; i < len; i++)
	{
		if(i == 0 && ( input[i] > '9' || input[i] < '0'))
			continue;
		else if (input[i] <='9' && input[i] >= '0')
		{
			output[k] = output[k] *10 + (int)(input[i] - '0');
		}
		else if ( ( input[i] > '9' || input[i] < '0') && ( input[i-1] <= '9' && input[i-1] >= '0') )
		{
			k++;
			output[k] = 0;
		}
	}
	if(input[len-1] <'9' && input[len-1] > '0') k++;
	return k;
}
