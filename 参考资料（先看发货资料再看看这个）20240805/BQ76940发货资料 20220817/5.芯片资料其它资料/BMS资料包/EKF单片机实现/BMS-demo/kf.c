
#include "kf.h"

void PreEst(void(*sysfun)(double *,double *,double *),
			 double *x,double *u,double *PreX)
{
    sysfun(x,u,PreX);
    return; 		
}


void PreCOV(double *a,
			double *p, 
			double *b,
			double *q,
			int n,int s,
			double *PreP)
{
    int i,j;
    double *temp1,*temp2,*temp3,*temp4;
	temp1=(double *)malloc(sizeof(double)*n*n);
	temp2=(double *)malloc(sizeof(double)*n*n);
	temp3=(double *)malloc(sizeof(double)*n*s);
	temp4=(double *)malloc(sizeof(double)*n*n);
	
    Mult(a,p,n,n,n,temp1);  //Mult 实现两个矩阵相乘并把结果赋给最后那个参数
    Mult_T(temp1,a,n,n,n,temp2);//Mult_T实现一个矩阵与另一个的转置相乘并把结果赋给最后那个参数

    Mult(b,q,n,s,s,temp3);

    Mult_T(temp3,b,n,s,n,temp4);

	for (i=0;i<n;i++)
	{
	for (j=0;j<n;j++)
	{
     PreP[i*n+j]=temp2[i*n+j]+temp4[i*n+j];
	}
	}
	
   // PreP=temp2+temp4;
    free(temp1);
    free(temp2);
    free(temp3);
    free(temp4);
	return ;
}
  

void K_Gain(double *p,
			double *h,
			double *r,
			int n,int m,
			double *Result_Kg)
{
	int i,j;
    double *temp5,*temp6,*temp7;  
	temp5=(double *)malloc(sizeof(double)*n*m);
	temp6=(double *)malloc(sizeof(double)*m*n);
	temp7=(double *)malloc(sizeof(double)*m*m);
    

//计算P乘H的转置
	 
     Mult_T(p,h,n,n,m,temp5);
	 
//求 h*p*h'+r

	 Mult(h,p,m,n,n,temp6);
	 Mult_T(temp6,h,m,n,m,temp7);
	 
	for (i=0;i<m;i++)
	{for (j=0;j<m;j++)
	{temp7[i*m+j]+=r[i*m+j];
	}
	}	

//求h*p*h'+r的逆
    Inv(temp7,m);	 
//结果
    Mult(temp5,temp7,n,m,m, Result_Kg);
    free(temp5);
    free(temp6);
    free(temp7);
   
  return ;
}




void Est(double *h,
		 double *kg,
		 double *p_x,
		 double *z,
		 int n,int m,
		 double *Result_Est)// x维数是n 设为n*1
{
	int i;
	double *temp8,*temp9;
	temp8=(double *)malloc(sizeof(double)*m);
	temp9=(double *)malloc(sizeof(double)*n);

 //计算z-h*x(k,k-1)
    Mult(h,p_x,m,n,1,temp8);

	for(i=0;i<m;i++) 
	{    
	temp8[i]=z[i]-temp8[i];
	}

//再与kg相乘

    Mult(kg,temp8,n,m,1,temp9);

    for (i=0;i<n;i++)
    {
	Result_Est[i]=p_x[i]+temp9[i] ;
    }

    free(temp8);
    free(temp9);
	return ;
	 
}

void Cov(double *kg,
		 double *h,
		 double *p_p,
		 int n,int m,
		 double *Result_Cov)
{
      int i;
	  double *e=(double *)malloc(sizeof(double)*n*n);
	  double *temp10=(double *)malloc(sizeof(double)*n*n);
      Mult(kg,h,n,m,n,temp10);

  //构造一个单位阵
     for (i=0;i<n*n;i++)
	 {
	 e[i]=0.0;
	 }
     for (i=0;i<n*n;i=i+n+1)
	 {
	  e[i]=1.0;
	 }

  //和单位阵相减
	for (i=0;i<n*n;i++)
	{
	temp10[i]=e[i]-temp10[i];
	}
		
//结果
	Mult(temp10,p_p,n,n,n,Result_Cov);
	
    free(temp10);
	free(e);
	return  ;
}

void KalmanFilter(void(*sysfun)(double *,double *,double *),   //系统方程
				  void(*geta)(double *,double *),//系统转移阵A
				  void(*geth)(double *,double *),   //观测转移矩阵H
				  double *B,   //b为噪声转移阵
				  double *X0,  //滤波初始值
				  double *P0,  //初始噪声方差       
				  double *Q,   //系统噪声
				  double *R,   //观测噪声
				  double *U,   //控制向量
				  int n,int m,int s,   // n m分别为维数 s为噪声转移的维数                  
				  double *guance,     //guance为此刻的观测值
				  double *Result_cov, //保存每一步更新的方差
				  double *Result_est  // result_est 保存结果	
				  )
                    
{
   double *PreX=(double *)malloc(sizeof(double)*n);
   double *PreP=(double *)malloc(sizeof(double)*n*n);
   double *Result_Kg=(double *)malloc(sizeof(double)*n*m);
   double *A=(double *)malloc(sizeof(double)*n*n);
   double *H=(double *)malloc(sizeof(double)*m*n);   

   PreEst(sysfun,X0,U,PreX);  //预测 x(k,k-1)
   
   geta(PreX,A);
   geth(PreX,H);

   PreCOV(A,P0,B,Q,n,s,PreP); //一步预测误差方差阵p(k,k-1)
  
   K_Gain(PreP,H,R,n,m,Result_Kg);// 获得滤波增益矩阵 kg
    
   Est(H,Result_Kg,PreX,guance,n,m,Result_est);// 状态估计 得到x(k)
	   
   Cov(Result_Kg,H,PreP,n,m,Result_cov);//更新误差方差阵     

   free(PreX);
   free(PreP);
   free(A);
   free(H);
   free(Result_Kg);
}