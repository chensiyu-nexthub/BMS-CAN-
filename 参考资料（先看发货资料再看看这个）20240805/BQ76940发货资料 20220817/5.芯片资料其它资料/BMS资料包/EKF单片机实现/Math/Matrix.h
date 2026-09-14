#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//计算矩阵A（m*n）和B（n*k）的乘积，结果保存在C（m*k）中
//a-为m*n的数组
//b-为n*k的数组
//c-为m*k的数组，存放结果

void Mult (double *a,double *b,int m,int n,int k,double *c);
 
//计算矩阵A（m*n）和B（k*n）的转置矩阵BT（n*k）的乘积，结果保存在C（m*k）中
//添加的函数，非原书程序
//a-长度为m*n的数组
//b-长度为k*n的数组
//c-长度为m*k的数组，存放结果
void Mult_T(double *a,double *b,int m,int n,int k,double *c);

//实矩阵求逆
//a-长度为n*n的数组, n*n矩阵
//n 矩阵的维数

double Inv(double *a,int n);

#endif

