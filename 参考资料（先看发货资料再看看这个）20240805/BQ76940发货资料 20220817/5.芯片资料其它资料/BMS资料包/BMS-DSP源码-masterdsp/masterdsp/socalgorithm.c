#include "socalgorithm.h"

/*开路电压法计算SOC
soc=f(v,t,l)即soc为电压、温度、以及老化程度的函数，这里暂时不考虑老化因素*/
float OCV(float voltage,float temp)
{
	int i,j;
	float soc,p[3];
	float t[]={-13,22,40};
	float v[3][11]={
	{1.101,1.191,1.208,1.230,1.248,1.257,1.276,1.295,1.313,1.331,1.350},
	{1.12,1.222,1.241,1.265,1.285,1.296,1.317,1.338,1.358,1.379,1.4},
	{1.133,1.244,1.265,1.292,1.314,1.326,1.350,1.373,1.395,1.419,1.443},
	};
	for(i=0;i<3;i++)
	{
		if(voltage<=v[i][0])p[i]=0;
		else if(voltage>=v[i][11])p[i]=1;
		else 
		for(j=0;j<11;j++)
		{
			if((voltage-v[i][j]>=0)&(voltage-v[i][j+1]<=0))
				p[i]=(v[i][j+1]-voltage)/(v[i][j+1]-v[i][j])*0.1*j+(voltage-v[i][j])/(v[i][j+1]-v[i][j])*0.1*(j+1);
				break;
		}
	}
	for(i=0;i<3;i++)
		if((temp-t[i]>=0)&(temp-t[i+1]<=0))break;
	soc	=(t[i+1]-temp)/(t[i+1]-t[i])*p[i]+(temp-t[i])/(t[i+1]-t[i])*p[i+1]; 
	return soc;	
}
/*float OCVTable(float voltage,float temp)
{
	float soc;
	float soctable[][]={{},{},{}...};
	int m,n;//m、n分别表示电压、温度对应的下标
	m=
}*/
//累积电量
float Deltasoc(float current,float temp)
{
	float delta;
	delta=DELTAT*current*Efficiency(current,temp)/Cap(temp);
	return delta;
}
//折算库仑效率
float Efficiency(float current,float temp)
{
	float eff,effs=0.98;
	int k,j,i[]={-160,-120,-80,-40,-20,0,20,40,80,120,160,};
	float effc[]={0.94,0.95,0.95,0.965,0.97,0.98,0.97,0.93,0.92,0.91,0.87,};
	int t[]={-10,0,25,};
	float kt0,kt[]={0.85,0.9,1};
	if(current<i[0])
		eff=0.9;
	else if(current>i[10])
		eff=0.8;
	else
		for(j=0;j<10;j++)
		{
			if((current>=i[j])&(current<=i[j+1]))
				eff=(i[j+1]-current)/(i[j+1]-i[j])*effc[j]+(current-i[j])/(i[j+1]-i[j])*effc[j+1];
				break;
		}
	if(current>0)eff=effs/eff;
	if(temp<t[0])
		kt0=0.8;
	else if(temp>t[3])
		kt0=0.8;
	else
		for(k=0;k<3;k++)
		{
			if((temp>=t[k])&(temp<=t[k+1]))
				kt0=(t[k+1]-temp)/(t[k+1]-t[k])*kt[k]+(temp-t[k])/(t[k+1]-t[k])*kt[k+1];
				break;
		}
	eff=eff*kt0;
	return eff;
}
//可变容量
float Cap(float temp)
{
	float Ca;
	int i,t[]={-20,-10,0,25,40,60};
	float kct[]={0.9,0.955,0.98,1,1.005,0.94};
	if(temp<t[0])
		Ca=0.8;
	else if(temp>t[5])
		Ca=0.8;
	else
		for(i=0;i<5;i++)
		{
			if((temp>=t[i])&(temp<=t[i+1]))
				Ca=(t[i+1]-temp)/(t[i+1]-t[i])*kct[i]+(temp-t[i])/(t[i+1]-t[i])*kct[i+1];
				break;
		}
	return Ca*C3;
}