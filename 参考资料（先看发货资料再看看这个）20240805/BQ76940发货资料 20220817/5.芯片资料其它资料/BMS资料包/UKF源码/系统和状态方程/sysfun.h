#ifndef _SYSFUN_H		
#define _SYSFUN_H

#include "matrix.h"
#include <math.h>
#define pi 3.14159

#ifndef _NO_NAMESPACE
using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

#ifndef _NO_EXCEPTION
#  define TRYBEGIN()	try {
#  define CATCHERROR()	} catch (const STD::exception& e) { \
						cerr << "Error: " << e.what() << endl; }
#else
#  define TRYBEGIN()
#  define CATCHERROR()
#endif

//***************************************************************************************
  class SysFun    //系统方程
  {
  public:
	  SysFun(Matrix x_sigma,Matrix u_x);
      ~SysFun();
	  Matrix SFun();
	   Matrix x_transform;
  private:
	  Matrix x,u_sys;
      int n;
	 double t;
	 
  };
  SysFun::SysFun(Matrix x_sigma,Matrix u_x)  //认为控制输入项的维数和向量维数相同，
	                                               //若不等可把其余分量设为零即可
  {
	  x=x_sigma;     //sigma点
      u_sys=u_x;     //控制输入
	  t=t0
	  n=x_sigma.RowNo();     //状态向量维数
      x_transform.SetSize(n,2*n+1);
	  x_transform.Null(n,2*n+1);
  }
 Matrix SysFun::SFun() //指定系统方程的具体形式,这里以二维做实例，实际中重载此函数即可
  {
	 for (int i=0;i<(2*n+1);i++ )
	 {
		 //x(k+1)=0.25*x(k)^2+W  %状态方程
		 
          x_transform(0,i)=0.25*(x(0,i)*x(0,i));
		 
	 }     
	 return x_transform;
  }
 SysFun::~SysFun()
 {
	 x_transform.~matrix ();
 }

#endif   
