#ifndef _socalgorithm_h
#define _socalgorithm_h

//定义额定容量C3
#define C3 27
//定义间隔时间DELTAT
#define DELTAT	0.1/3600 //100ms

/*开路电压法计算SOC,soc=f(v,t,l)
即soc为电压、温度、以及老化程度的函数，这里暂时不考虑老化因素*/
float OCV(float voltage,float temp);
/*float OCVTable(float voltage,float temp)*/
//电量累计deltasoc
float Deltasoc(float current,float temp);
//折算库仑效率
float Efficiency(float current,float temp);
//电池可变容量，不考虑老化的影响
float Cap(float temp);

#endif