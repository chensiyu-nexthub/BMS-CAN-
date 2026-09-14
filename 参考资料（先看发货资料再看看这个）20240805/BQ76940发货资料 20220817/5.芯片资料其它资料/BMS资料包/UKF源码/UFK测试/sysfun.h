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

/***************************************************************************************
*系统方程： x(k) = sysfun[x(k-1),Q(k-1),U1(k-1)]
*
*           x(k-1)为状态向量  Q(k-1)为系统噪声,U1(k-1)为控制输入
*
****************************************************************************************/

  class SysFun    //系统方程
  {
  public:
	  SysFun(Matrix &x_sigma,Matrix &Q,Matrix &u_x);
      ~SysFun();
	  Matrix SFun();
	   Matrix x_transform;
  private:
	  Matrix x,u_sys;
      int n;
	 
  };
  SysFun::SysFun(Matrix &x_sigma,Matrix &Q,Matrix &u_x)  //认为控制输入项的维数和向量维数相同，
	                                                 //若不等可把其余分量设为零即可
  {
	  x=x_sigma;     //sigma点
      u_sys=u_x;     //控制输入
	  n=x_sigma.RowNo();     //状态向量维数
      x_transform.SetSize(n,2*n+1);
	  x_transform.Null(n,2*n+1);
  }
 Matrix SysFun::SFun() //指定系统方程的具体形式，实际当中根据具体方程重载此函数即可
  {
	 /* 设系统方程为：
	  x1(1,1)=x(1,1)+t*x(3,1)+(t^2/2)*(control/1000-0.00035*(x(3,1)^2));
      x1(2,1)=x(2,1)+t*[0.001*control-0.00035*(x(2,1)^2)];
      x1(3,1)=x(3,1)+t*x(4,1)+(t^2/2)*(0.00392*(x(2,1)^2)-g);
      x1(4,1)=x(4,1)+t*[0.00392*(x(2,1)^2)-g];
	*/


	double t=0.2;
	double g=9.8;

	 for (int i=0;i<(2*n+1);i++ )
	 {
      x_transform(0,i)=(x(0,i)+t*x(1,i)+(t*t*0.5)*(u_sys(0,0)*0.001-0.00035*(x(1,i)*x(1,i))));
	  x_transform(1,i)=(x(1,i)+t*(0.001*u_sys(0,0)-0.00035*(x(1,i)*x(1,i))));
	  x_transform(2,i)=(x(2,i)+t*x(3,i)+(t*t*0.5)*(0.00392*(x(1,i)*x(1,i))-g));
      x_transform(3,i)=(x(3,i)+t*(0.00392*(x(1,i)*x(1,i))-g));

	 }     
	 return x_transform;
  }
 SysFun::~SysFun()
 {
	 x_transform.~matrix ();
 }

#endif   
