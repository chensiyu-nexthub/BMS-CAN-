#ifndef _SIGMAPOINT_H		//求sigma点
#define _SIGMAPOINT_H

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

//****************************************************************************
//  定义sigma
class Sigma
{
public:
	Sigma(Matrix x0,Matrix p0,double alpha0,double beta0,double kappa0);
	Matrix SigmaFun();
	~Sigma();
	Matrix sigma;//sigma点
	Matrix w_m,w_c;
private:
	Matrix x,p;
	double alpha,beta,kappa,lambda;
	int n;
};
Sigma::Sigma(Matrix x0,Matrix p0,double alpha0=0.85,double beta0=2,double kappa0=0)
{
   x=x0;//初始化各项参数
   p=p0;
   alpha=alpha0;
   beta=beta0;
   kappa=kappa0;
   n=x.RowNo();//获得向量维数
   sigma.SetSize(n,2*n+1);sigma.Null(n,2*n+1); //初始化sigma矩阵并赋初值0
   w_m.SetSize(1,2*n+1),w_c.SetSize(1,2*n+1);
   w_m.Null(1,2*n+1),w_c.Null(1,2*n+1);
   
   
}

Matrix Sigma::SigmaFun()
{
	    Matrix temp(n,1);
		temp.Null(n,1);//初始化临时矩阵
		
	    lambda=(alpha*alpha)*(n+kappa)-n; //计算lambda
		p*=(n+lambda);                    
		 
		p=p.Cholesky();//cholesky分解求矩阵方根
       
       sigma.AddCol(x,1,1);w_m(0,0)=lambda/(n+lambda);

	 	w_c(0,0)=lambda/(n+lambda)+(1-alpha*alpha+beta);

		for (int i=2;i<n+2;i++)
		{
		  temp.AddCol(p,1,i-1);
		  temp+=x;
		  sigma.AddCol(temp,i,1);	 // 求sigma点
		}
		for ( i=n+2;i<(2*n+2);i++)
		{
		  temp.AddCol(p,1,i-1-n);
		  temp-=x;
		  sigma.AddCol(temp,i,1);	  
		}   
		for (i=1;i<(2*n+1);i++)     //求权值
		{
			w_m(0,i)=1/(2*(n+lambda));
			w_c(0,i)=1/(2*(n+lambda));
		}
       return sigma,w_m,w_c;	
}
Sigma::~Sigma()
{
	sigma.~matrix();
    w_m.~matrix();w_c.~matrix();
	x.~matrix();
	p.~matrix();

}

#endif 