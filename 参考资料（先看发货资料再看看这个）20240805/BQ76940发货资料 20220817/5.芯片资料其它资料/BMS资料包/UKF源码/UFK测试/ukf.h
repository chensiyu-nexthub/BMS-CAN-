#ifndef _UKF_H		
#define _UKF_H
#include "matrix.h"
#include "obsfun.h"
#include "sysfun.h"
#include "sigmapoint.h"

//***********************************************************************************************

class UKF
{
public:
	 UKF(Matrix &_Q,Matrix &_R);
	~UKF();
	void UkfSet(Matrix &sigma,Matrix &w0_m,Matrix &w0_c,Matrix &y0,Matrix &u_sys,Matrix &Q_,Matrix &R_);//设置各项参数
	void Process();//整个滤波处理过程

    Matrix p_est;//更新的误差方差阵
    Matrix x_est;//滤波后的估计值

private:
	int n,m;//状态向量和观测向量的维数
	Matrix U_sys,Q,R;//系统方程中的控制输入项及相应的噪声方差阵
     	 
	Matrix x_sigma;//sigma点
	Matrix w_m,w_c;//相应的权值
	Matrix y; //观测值
	Matrix px,py,pxy; //x y 各自的方差
    Matrix x_mean,y_mean;//平均值
	Matrix Kk;//滤波增益
	Matrix tmpx0,tmpx1,tmpy0,tmpy1,tmpxy;//临时矩阵

};


UKF::UKF(Matrix &_Q,Matrix &_R)
   {    
	n=_Q.RowNo();m=_R.RowNo();//取得矩阵维数
		
   	x_est.SetSize(n,1);//指定矩阵大小
	p_est.SetSize(n,n);
	px.SetSize(n,n);py.SetSize(m,m);pxy.SetSize(n,m);   //指定矩阵大小 初始化为0
    x_mean.SetSize(n,1);y_mean.SetSize(m,1);
    Kk.SetSize(n,m);//滤波增益
		
    tmpx0.SetSize(n,1);tmpx1.SetSize(n,n);
	tmpy0.SetSize(m,1);tmpy1.SetSize(n,m);
    tmpxy.SetSize(n,m);
		
   }

void UKF::UkfSet(Matrix &sigma,Matrix &w0_m,Matrix &w0_c,Matrix &y0,Matrix &u_sys,Matrix &Q_,Matrix &R_)
{
  x_sigma=sigma;//sigma点
  w_m=w0_m;w_c=w0_c;//权指
  y=y0;//观测值
  U_sys=u_sys; //控制输入
  Q=Q_; //系统方差
  R=R_; //观测方差
        
  //清零
  x_mean.Null();y_mean.Null();
  px.Null();py.Null();
  pxy.Null();Kk.Null();
  x_est.Null();p_est.Null();

  tmpx0.Null();tmpx1.Null();
  tmpy0.Null();tmpy1.Null();
  tmpxy.Null();
}

void UKF::Process()
{  
	
   SysFun fun1(x_sigma,Q,U_sys); //将sigma点分别代入系统和观测方程得到变换后的值
   fun1.SFun();
   ObsFun fun2(fun1.x_transform,R);
   fun2.OFun();
   
   for(int i=1;i<(2*n+2);i++) //计算均值
   {   
	  x_mean+=((fun1.x_transform.GetCol(i)*w_m(0,i-1)));  
      y_mean+=((fun2.y_transform.GetCol(i)*w_m(0,i-1)));	   
   }
   
    for (i=0;i<(2*n+1);i++)  //x y的方差
    {
     
    tmpx0=fun1.x_transform.GetCol(i+1)-x_mean;//x 的方差
	tmpx1=tmpx0*(~tmpx0);
	px+=tmpx1*w_c(0,i);
    
    tmpy0=fun2.y_transform.GetCol(i+1)-y_mean;//y 的方差
	tmpy1=tmpy0*(~tmpy0);
	py+=tmpy1*w_c(0,i);

	tmpxy=tmpx0*(~tmpy0);//计算xy的协方差
	pxy+=tmpxy*w_c(0,i);

    }

     px+=Q;
     py+=R;
      
     Kk=(pxy*(!py));//计算滤波增益	 
     x_est=(x_mean+Kk*(y-y_mean));//状态估计
	 p_est=(px-(Kk*py)*(~Kk));  //更新误差方差阵    
	 
}
  UKF::~UKF()
   {	
	 x_est.~matrix();
	 p_est.~matrix();
	 U_sys.~matrix();
	 Q.~matrix();
	 R.~matrix();
     y.~matrix();	 
	 x_sigma.~matrix();
	 w_m.~matrix();
	 w_c.~matrix();
	 
   }

#endif 