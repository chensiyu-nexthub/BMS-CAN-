#include <iostream>
#include "matrix.h" //矩阵操作的头文件
#include "obsfun.h"  //观测方程
#include "sysfun.h"  //系统方城
#include "sigmapoint.h" //求sigma点
#include "ukf.h"       //ukf滤波算法
#include <fstream>
#include <cstdio>
#include <ctime>
using namespace std;



/************************************************************************/
/*      测试                                                            */
/************************************************************************/

int main()
{
	clock_t start, finish;
    start = clock();

    //假定初始值如下
	Matrix x0(4,1);
	Matrix p0(4,4),Q(4,4),R(2,2);
    double a[4]={-2800,100,160,10};
	double b[16]={100, 0, 0, 0,
		           0, 100, 0, 0,
			       0, 0, 100, 0,
			       0, 0, 0, 100};
    double c[16]={0.0001, 0, 0, 0,
		           0, 0.0001, 0, 0,
			       0, 0, 0.0001, 0,
			       0, 0, 0, 0.0001};
	double d[4]={0.0001,0,0,2500};
	x0=a;p0=b;Q=c;R=d;
    
	Matrix measure(2,50001),kongzhi(2,50001);
	measure.Null();kongzhi.Null();
    

    ifstream stream0("guance.txt");
	ifstream stream1("kongzhi.txt");
    FILE *stream2; 
	stream2=fopen("result.txt","w+");
	
    if( !stream0 && !stream1 && stream2 == NULL)
	{
	 printf( "The file  was not opened\n" );
	 return false;
	}

	fprintf(stream2,"%f %20f %20f %20f\n",x0(0,0),x0(1,0),x0(2,0),x0(3,0));

    int i=0,i1=0,i2=0;
	double temp0=0.0;
	double temp1=0.0;int j=0;

    while( !stream0.eof()&!stream1.eof()  )
	{
       stream0>>temp0;
	 if ((i+1)%2==0)
	 { measure(1,i1)=temp0;i1++;}
     else
	 {measure(0,i2)=temp0; i2++;}

	i++;

	 stream1>>temp1;
	 kongzhi(0,j)=temp1;
	 j++;

   }
  
   
 Matrix control(2,1),guance(2,1);
 control.Null();guance.Null();


/*滤波处理过程********************************************************************/

  TRYBEGIN()  // 异常处理
  {
	 //定义实例
     Sigma sigmapoint(x0,0.85,2,0);

     UKF ss(Q,R);
	
     for(int k=0;k<49999;k++) //指定滤波次数
	 {

    
    //获得观测值
	guance(0,0)=measure(0,k+1);guance(1,0)=measure(1,k+1);
    control(0,0)=kongzhi(0,k); 
    

    //先求sigma点
	sigmapoint.SigmaSet(x0,p0);
    sigmapoint.SigmaFun();

    //ukf滤波
	ss.UkfSet(sigmapoint.sigma,sigmapoint.w_m,
		      sigmapoint.w_c,guance,control,Q,R);
	ss.Process();

    p0=ss.p_est; x0=ss.x_est;  //更新x0,p0用于下一次循环
		
    fprintf(stream2,"%f %20f %20f %20f\n",x0(0,0),x0(1,0),x0(2,0),x0(3,0));


   }
  }
  CATCHERROR() //异常   

  fclose(stream2);
  stream0.close();
  stream1.close();
/*********************************************************************************/

   finish = clock();
   
   double duration = (double)(finish - start) / CLOCKS_PER_SEC;
   std::cout<<"程序运行了："<<duration<<"  秒！\n"<<"\n";

   system("PAUSE");
   return 0;

}


