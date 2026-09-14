#include <stdio.h>
#include <stdlib.h>
int main()
{ 
	double temp=0.0;

	FILE *pfile;
	pfile=fopen("guance.txt","rt");
	if (pfile==NULL)	
	{
	printf("文件打开失败\n");
	}
	
	while (!feof(pfile))
	{

	fscanf(pfile,"%lf\n",&temp);
	printf("%lf\n",temp);

	}
}

