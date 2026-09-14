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
	Sigma(Matrix x0,double alpha0,double beta0,double kappa0);
	void SigmaSet(Matrix x0,Matrix p0);
	void SigmaFun();
	~Sigma();
	Matrix sigma;//sigma点
	Matrix w_m,w_c;
private:
	Matrix p,x;
	Matrix tmp0,tmp1;//临时矩阵
	double alpha,beta,kappa,lambda;
	int n;
};
Sigma::Sigma(Matrix x0,double alpha0=0.85,double beta0=2,double kappa0=0)
{
   //初始化各项参数 
   alpha=alpha0;
   beta=beta0;
   kappa=kappa0;

   n=x0.RowNo();//获得向量维数

   sigma.SetSize(n,2*n+1);sigma.Null(); //初始化sigma矩阵并赋初值0
   w_m.SetSize(1,2*n+1),w_c.SetSize(1,2*n+1);
   w_m.Null();w_c.Null();
   
   tmp0.SetSize(n,1);tmp1.SetSize(n,1);//初始化临时矩阵
   

}

  void Sigma::SigmaSet(Matrix x0,Matrix p0)
  {
   x=x0;
   p=p0;
   
   //清零
   sigma.Null();
   w_m.Null();
   w_c.Null(); 
   tmp0.Null();
   tmp1.Null();
  
}
  void Sigma::SigmaFun()
{
	    	
	lambda=(alpha*alpha)*(n+kappa)-n; //计算lambda
       
	p*=(n+lambda);                 
    if (p.Det()<0.0)
    {
	 REPORT_ERROR( "sigmapoint::P: P 矩阵可能负定，停止计算!");
    }

	p=p.Cholesky();//cholesky分解求矩阵方根
        
    sigma.AddCol(x,1,1);
		
    w_m(0,0)=lambda/(n+lambda);
	w_c(0,0)=lambda/(n+lambda)+(1-alpha*alpha+beta);

    double weight=1/(2*(n+lambda));

    for (int i=2;i<n+2;i++)
	{
		  
	tmp0=x+p.GetCol(i-1);
	sigma.AddCol(tmp0,i,1);	 // 求sigma点
		  
	tmp1=x-p.GetCol(i-1);
	sigma.AddCol(tmp1,i+n,1);

    w_m(0,i-1)=weight;        //求权值
    w_m(0,i-1+n)=weight;
           
    w_c(0,i-1)=weight;
	w_c(0,i-1+n)=weight;

	}
		
}
Sigma::~Sigma()
{
    sigma.~matrix();
    w_m.~matrix();
	w_c.~matrix();
	x.~matrix();
	p.~matrix();
	tmp0.~matrix();
	tmp1.~matrix();

}

#endif 