#include "Matrix.h"

//计算矩阵A（m*n）和B（n*k）的乘积，结果保存在C（m*k）中
//a-为m*n的数组
//b-为n*k的数组
//c-为m*k的数组，存放结果

 void Mult (double *a,double *b,int m,int n,int k,double *c)
{ 
int i,j,l,u;
    for (i=0; i<m; i++)
{
  for (j=0; j<k; j++)
  { 
   u=i*k+j; 
   c[u]=0.0;
   for (l=0; l<n; l++)
   {
    c[u]+=a[i*n+l]*b[l*k+j];
   }
  }
}
    return;
} 
 
//计算矩阵A（m*n）和B（k*n）的转置矩阵BT（n*k）的乘积，结果保存在C（m*k）中
//添加的函数，非原书程序
//a-长度为m*n的数组
//b-长度为k*n的数组
//c-长度为m*k的数组，存放结果
void Mult_T(double *a,double *b,int m,int n,int k,double *c)
{ 
int i,j,l,u;
    for (i=0; i<m; i++)
{
  for (j=0; j<k; j++)
  { 
   u=i*k+j; 
   c[u]=0.0;
   for (l=0; l<n; l++)
   {
    c[u]+=a[i*n+l]*b[j*n+l];
   }
  }
}
    return;
} 

//实矩阵求逆
//a-长度为n*n的数组, n*n矩阵
//n 矩阵的维数

double Inv(double *a,int n)
{ 
int *is,*js,i,j,k,l,u,v;
double d,p;
is=(int *)malloc(sizeof(int)*n);
js=(int *)malloc(sizeof(int)*n);
for (k=0; k<=n-1; k++)
{ 
  d=0.0;
  for (i=k; i<=n-1; i++)
  {
   for (j=k; j<=n-1; j++)
   { 
    l=i*n+j; p=fabs(a[l]);
    if (p>d) 
    { 
     d=p; 
     is[k]=i; 
     js[k]=j;
    }
   }
  }
  if (d+1.0==1.0)
  { 
   free(is); 
   free(js); 
   printf("err**not inv\n");
   return(0);
  }
  if (is[k]!=k)
  {
   for (j=0; j<=n-1; j++)
   { 
    u=k*n+j; v=is[k]*n+j;
    p=a[u]; a[u]=a[v]; a[v]=p;
   }
  }
  if (js[k]!=k)
  {
   for (i=0; i<=n-1; i++)
   { 
    u=i*n+k; v=i*n+js[k];
    p=a[u]; a[u]=a[v]; a[v]=p;
   }
  }
  l=k*n+k;
  a[l]=1.0/a[l];
  for (j=0; j<=n-1; j++)
  {
   if (j!=k)
   { 
    u=k*n+j; a[u]=a[u]*a[l];
   }
  }
  for (i=0; i<=n-1; i++)
  {
   if (i!=k)
   {
    for (j=0; j<=n-1; j++)
    {
     if (j!=k)
     { 
      u=i*n+j;
      a[u]=a[u]-a[i*n+k]*a[k*n+j];
     }
    }
   }
  }
  for (i=0; i<=n-1; i++)
  {
   if (i!=k)
   { 
    u=i*n+k; a[u]=-a[u]*a[l];
   }
   
  }
  
}
for (k=n-1; k>=0; k--)
{ 
  if (js[k]!=k)
  {
   for (j=0; j<=n-1; j++)
   { 
    u=k*n+j; v=js[k]*n+j;
    p=a[u]; a[u]=a[v]; a[v]=p;
   }
  }
  if (is[k]!=k)
  {
   for (i=0; i<=n-1; i++)
   { 
    u=i*n+k; v=i*n+is[k];
    p=a[u]; a[u]=a[v]; a[v]=p;
   }
  }
}
free(is); 
free(js); 
return(1);
}