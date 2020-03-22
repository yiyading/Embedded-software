#include "./barrel.h"
#include "./char2int.h"
#include<stdio.h>

int main()
{
	int if_input = 1;
	int i, k, len;
	int a[] = {2,0,6,7,4,6,4,11,56,89,100,1,1};
	char scan_num[100];
	int c[50]={0};

	//scanf("%[^/n]", scan_num);
	if( if_input ==1 )		
	{
		fgets(scan_num, 100, stdin);
        	k= char2int(scan_num, c);
		len = k;
	}
	else	
		len = sizeof(a)/sizeof(int);
	int b[ len ];

	if(if_input == 1)
		barrel(c, len,b,0);
	else 
		barrel(a, len,  b, 1);
	for(i =0; i< (sizeof(b)/sizeof(int)); i ++)
	{
		printf("%d, ", b[i]);
	}
	return 0;
}
