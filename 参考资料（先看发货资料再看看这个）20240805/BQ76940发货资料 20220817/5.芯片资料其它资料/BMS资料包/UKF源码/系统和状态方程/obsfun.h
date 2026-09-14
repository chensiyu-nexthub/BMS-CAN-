#ifndef _OBSFUN_H		
#define _OBSFUN_H

#include "matrix.h"

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

//********************************************************************************
//观测方程
  class ObsFun
  {
  public:
	  ObsFun(Matrix x_sigma,int mo);
	  Matrix OFun();
	  ~ObsFun();
	  Matrix y_transform;
  private:
      Matrix y,u_obs;
      int n,m;
	  
      
  };
  ObsFun::ObsFun(Matrix x_sigma,int m0)//这里以一维观测维实例
  {
	  m=m0; //观测向量维数
	  n=x_sigma.RowNo();   //状态向量维数
	  y=x_sigma;     //sigma点
      y_transform.SetSize(m,2*n+1);
	  y_transform.Null(m,2*n+1);
  }
  Matrix ObsFun::OFun()
  {
	  for(int i=0;i<y.ColNo();i++)
	  {
		 // y(k)=0.25*x(k)^2+ v      %观测方程

		  y_transform(0,i)=(y(0,i)*y(0,i))/4;
	  }
	  return y_transform;
  }
  ObsFun::~ObsFun()
  {
	  y_transform.~matrix();
  }

#endif 
