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

/********************************************************************************
* y(k) = ObsFun[x(k),n(k),U2(k)]
*********************************************************************************/
//观测方程
  class ObsFun
  {
  public:
	  ObsFun(Matrix x_sigma,Matrix R );
	  Matrix OFun();
	  ~ObsFun();
	  Matrix y_transform;
  private:
      Matrix xtrans,u_obs;
      int n,m;
	  
      
  };
  ObsFun::ObsFun(Matrix x_transform,Matrix R)//这里以二维观测为实例
  {
	  m=R.RowNo(); //观测向量维数
	  n=x_transform.RowNo();   //状态向量维数
	  xtrans=x_transform;     //sigma点
      y_transform.SetSize(m,2*n+1);
	  y_transform.Null(m,2*n+1);
  }
  Matrix ObsFun::OFun()
  {
	  for(int i=0;i<xtrans.ColNo();i++)
	  {
	  // x2(1,1)=atan(x(3,1)/x(1,1));
      // x2(2,1)=sqrt(x(1,1)^2+x(3,1)^2);      %观测方程

	  y_transform(0,i)=atan(xtrans(2,i)/xtrans(0,i));
      y_transform(1,i)=sqrt(xtrans(0,i)*xtrans(0,i)+xtrans(2,i)*xtrans(2,i));

	  }

	  return y_transform;
  }
  ObsFun::~ObsFun()
  {
	  y_transform.~matrix();
  }

#endif 
