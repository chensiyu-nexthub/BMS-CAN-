#include "kf.h"

#include <stdio.h>
#include <stdlib.h>

static double t=0.2,g=9.8;
int n=4,m=2,s=1;

void sysfun(double *x, double *u,double *prex)//系统方程
 {
 prex[0]=x[0]+t*x[1]+(t*t*0.5)*(u[0]*0.001-0.00035*(x[1]*x[1]));
 prex[1]=x[1]+t*(0.001*u[1]-0.00035*(x[1]*x[1]));
 prex[2]=x[2]+t*x[3]+(t*t*0.5)*(0.00392*(x[1]*x[1])-g);
 prex[3]=x[3]+t*(0.00392*(x[1]*x[1])-g);
 }

void generA(double *prex,double *A)//系统的雅可比矩阵
{
   int i=0;
     for (i=0;i<n*n;i++)
	 {
	 A[i]=0.0;
	 }

	 A[0]=1;
	 A[1]=t+(t*t/2)*(-0.00035*2*prex[1]);
	 A[5]=1+t*(-0.00035*2*prex[1]);
	 A[9]=(t*t/2)*(0.00392*2*prex[1]);
	 A[13]=t*0.00392*2*prex[2];
}

void generH(double *prex,double *H)//观测的雅可比矩阵
{
   double temp=(prex[2]/prex[0]);
   double temp1=prex[0]*prex[0];
   double temp2=prex[0]*prex[0]+prex[1]*prex[1];

   int i=0;
   for (i=0;i<m*n;i++)
   {
	 H[i]=0.0;
   }

  H[0]=(1/(1+temp*temp))*(-prex[2]/temp1);
  H[2]=(1/(1+temp*temp))*(prex[0]/temp1);
  H[4]=(1/(2*sqrt(temp2)))*2*prex[0];
  H[6]=(1/(2*sqrt(temp2)))*2*prex[2];

}

int  main()
{

       //测试数据	
	double b[4]={1,1,1,1};//噪声转移阵

	double q[16]={0.0001, 0, 0, 0,//噪声误差阵
		           0, 0.0001, 0, 0,
			       0, 0, 0.0001, 0,
			       0, 0, 0, 0.0001};
	double r[4]={0.0001,0,0,2500};//观测噪声初始值
	double x[4]={-2800,100,160,10};//状态量的滤波初值
	double p[16]={100, 0, 0, 0,    //噪声阵的初始值
		           0, 100, 0, 0,
			       0, 0, 100, 0,
			       0, 0, 0, 100};
    double yv[2]={0.0,0.0};//用来存观测值
	double kongzhi=0.0;//用来存储控制向量
    
	int i=0,j=0, count=0;
	double *Result_Est=(double *)malloc(sizeof(double)*n);
	double *Result_Cov=(double *)malloc(sizeof(double)*n*n); 
	void(*myfun)(double [],double [],double [])=&sysfun;
    void(*geta)(double [],double [])=&generA;
	void(*geth)(double [],double [])=&generH;

    FILE *pfile,*presult,*pkongzhi;
    if (!(pfile=fopen("guance1.txt","rt")))//打开观测值
     {
		printf("读文件没打开\n");
     }
    if (!(pkongzhi=fopen("kongzhi.txt","rt")))//打开控制向量
     {
		printf("读控制量文件没打开\n");
     }
     
    if (!(presult=fopen("result.txt","w+")))//保存结果的文件
     {
		printf("写文件没打开\n");
     }

	fprintf(presult,"%f %20f %20f %20f\n",x[0],x[1],x[2],x[3]);
    printf("\n\n滤波开始。。。。\n\n");
	while (!feof(pfile)&&!feof(pkongzhi))
     {        
      fscanf(pfile,"%lf %lf",&yv[0],&yv[1]);//读每一步的观测值
      fscanf(pkongzhi,"%lf",&kongzhi);//读每一步的控制向量

	  KalmanFilter(myfun,	  
		           geta,
				   geth,
				   b,
				   x,
				   p,
				   q,
				   r,
				   &kongzhi,
				   n,m,s,
				   yv,
				   Result_Cov,
				   Result_Est);

      p[0]=Result_Cov[0]; //更新噪声阵，用作下一次迭代
      x[0]=Result_Est[0];   //更新状态x[k],用作下一次迭代

      fprintf(presult,"%f %20f %20f %20f\n",x[0],x[1],x[2],x[3]);//将每一步的滤波结果写入文件

	  t=t+2*3.1415926/200.0;//系统方程里要用到的一个变量

      count++; 
    }

	printf("\n\n滤波结束,结果已保存到文件中.......\n\n");
    free(Result_Est);
    free(Result_Cov);
	return 0;

}









