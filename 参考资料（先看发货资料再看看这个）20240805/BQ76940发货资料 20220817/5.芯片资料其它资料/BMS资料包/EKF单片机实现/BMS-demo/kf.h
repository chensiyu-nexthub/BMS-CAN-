
#ifndef _KF_H
#define _KF_H

#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "Matrix.h"  //矩阵的相关运算

//接口
/*********************************************************************************/
void KalmanFilter(void(*sysfun)(double*,double*,double*),   //系统方程
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
				  ) ;        
         
//其中 X0 和P0 每一次都需要 Result_est 和 Result_cov来更新，即第一步迭代由
//用户自己指定x0 p0,然后算出结果Result_cov 和Result_est,第二次迭代时用
//Result_cov 和Result_est代替X0 和P0 ，如此循环下去


/***********************************************************************************/

//各个功能函数



/***********************************************************************************/
//实现第一步： 预测   x'(k,k-1)=Ax'(k-1) 
//a为系统状态转移矩阵 x为系统的状态向量 n为状态向量的维数  Prex为预测的状态值

void PreEst(void(*sysfun)(double *,double *,double *),
			 double *x,double *u,double *PreX);


/*********************************************************************************/
//实现第二步：求一步预测误差方差阵P(k,k-1)
// 其中 P(k,k-1)=AP(k-1)A' +BQB'
// a为（n*n）阶系统转移矩阵，B为（n*s）阶噪声转移矩阵，
//P为（n*n）前一时刻的系统哦噪声阵,Q为（s*s）为系统噪声阵
//n 和s 分别为系统方程维数 、系统噪声转移阵的维数
//prep存结果
void PreCOV(double *a,
			double *p, 
			double *b,
			double *q,
			int n,int s,
			double *PreP);

/**********************************************************************************/

//实现第三步 ：获得滤波增益kg
//Kg= p*h' *(p*h*p')~     h' 代表h的转置  (p*h*p')~为p*h*p'的逆

//PreP为第二步算出的PreP
//h为观测方程的转移矩阵 r为观测噪声 
//n是p的维数 m是h的维数
// Result_Kg保存结果
void K_Gain(double *p,
			double *h,
			double *r,
			int n,int m,
			double *Result_Kg);

/*********************************************************************************/


//实现第四步：状态估计
//h是观测转移阵 kg为第三步算出的滤波增益 p_x为第一步算出的预测值 z为观测值
//n为系统方程的维数 m为观测方程维数
//Result_Est保存结果
void Est(double *h,
		 double *kg,
		 double *p_x,
		 double *z,
		 int n,int m,
		 double *Result_Est);// x维数是n 设为n*1

/*********************************************************************************/


//实现第五步：更新误差方差阵
//kg由第三步算出 h为观测方程的转移阵 PreP为第二步算出的预测误差阵
//n m 分别为系统和观测方程维数】
//Result_Cov保存结果
void Cov(double *kg,
		 double *h,
		 double *p_p,
		 int n,int m,
		 double *Result_Cov);

/*********************************************************************************/

#endif
