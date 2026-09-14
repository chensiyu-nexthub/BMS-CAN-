#ifndef _UKF_H		//ukf变换
#define _UKF_H

#include <iostream>
#include "matrix.h"
#include "obsfun.h"
#include "sysfun.h"
#include "sigmapoint.h"
using namespace std;

//***********************************************************************************************

class UKF
{
public:
	 UKF();
     UKF(Matrix sigma,Matrix w0_m,Matrix w0_c,Matrix y0,Matrix u_sys,Matrix Q_,Matrix R_);
	~UKF();
	Matrix Process();//整个滤波处理过程

    Matrix x_est;//滤波后的估计值
	Matrix p_est;//更新的误差方差阵
   
private:
	int n,m;//状态向量和观测向量的维数
	Matrix U_sys,Q,R;//系统方程中的控制输入项及相应的噪声方差阵
    Matrix y; //观测值	 
	Matrix x_sigma;//sigma点
	Matrix w_m,w_c;//相应的权值
};


UKF::UKF(Matrix sigma,Matrix w0_m,Matrix w0_c,Matrix y0,Matrix u_sys,Matrix Q_,Matrix R_)
   {    
		y=y0;//观测值
		x_sigma=sigma;//sigma点
		w_m=w0_m;w_c=w0_c;
        U_sys=u_sys;
	    Q=Q_;
	    R=R_;
		n=Q.RowNo();m=R.RowNo();//维数

		//分配空间并初始化为0
		x_est.SetSize(n,1);x_est.Null(n,1);
		p_est.SetSize(n,n);p_est.Null(n,n);		
   }

Matrix UKF::Process()
{ 
	
   Matrix px(n,n),py(m,m),pxy(n,m); //初始化x y 各自的方差
   px.Null(n,n),py.Null(m,m),pxy.Null(n,m);
   Matrix x_mean(n,1),y_mean(m,1);//初始化平均值
   x_mean.Null(n,1),y_mean.Null(m,1);


   SysFun fun1(x_sigma,U_sys); //将sigma点分别代入系统和观测方程得到变换后的值
   ObsFun fun2(x_sigma,m);
   fun1.SFun();
   fun2.OFun();

   for(int i=1;i<(2*n+2);i++) //计算均值
   {   
	  x_mean+=((fun1.x_transform.GetCol(i)*=(w_m(0,i-1))));  
      y_mean+=((fun2.y_transform.GetCol(i)*=(w_m(0,i-1))));	   
   }
    
    for (i=0;i<(2*n+1);i++)  //x y的方差
    {
     px+=(((fun1.x_transform.GetCol(i+1)-x_mean)*(~(fun1.x_transform.GetCol(i+1)-x_mean)))*=(w_c(0,i)));
     py+=(((fun2.y_transform.GetCol(i+1)-y_mean)*(~(fun2.y_transform.GetCol(i+1)-y_mean)))*=(w_c(0,i)));
    }

     px+=Q;
     py+=R;
	 
     for (i=1;i<(2*n+2);i++)    //计算xy的协方差
     {
	 pxy+=(((fun1.x_transform.GetCol(i)-x_mean)*(~(fun2.y_transform.GetCol(i)-y_mean)))*=(w_c(0,i-1)));
     }               
           
	 Matrix Kk(n,m);//计算滤波增益
     Kk.Null(n,m);
     Kk=(pxy*(!py));
     x_est=((Kk*(y-y_mean))+=x_mean);//状态估计
	 p_est=(px-(Kk*py)*(~Kk));    //更新误差方差阵 
     
	 px.~matrix();py.~matrix();pxy.~matrix();
	 x_mean.~matrix();y_mean.~matrix();

     return x_est,p_est;
}
  UKF::~UKF()
   {	
	   x_est.~matrix();
	   p_est.~matrix();
	   U_sys.~matrix();
	   Q.~matrix();R.~matrix();
       y.~matrix();	 
	   x_sigma.~matrix();
	   w_m.~matrix();w_c.~matrix();
   }
//****************************************************************************************  
#endif


