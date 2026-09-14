
#include "SearchTable.h"
TemptureTable* m_searchTableData;
void SearchTable_Init( )
{
	static TemptureTable table[]={
{-30,3722},
{-29,3716},
{-28,3711},
{-27,3705},
{-26,3699},
{-25,3693},
{-24,3686},
{-23,3680},
{-22,3673},
{-21,3666},
{-20,3659},
{-19,3651},
{-18,3644},
{-17,3635},
{-16,3626},
{-15,3617},
{-14,3608},
{-13,3598},
{-12,3587},
{-11,3577},
{-10,3566},
{-9,3555},
{-8,3543},
{-7,3532},
{-6,3520},
{-5,3508},
{-4,3496},
{-3,3484},
{-2,3470},
{-1,3456},
{0,3442},
{1,3426},
{2,3411},
{3,3394},
{4,3378},
{5,3361},
{6,3344},
{7,3326},
{8,3309},
{9,3291},
{10,3271},
{11,3246},
{12,3223},
{13,3206},
{14,3186},
{15,3166},
{16,3149},
{17,3126},
{18,3105},
{19,3082},
{20,3056},
{21,3032},
{22,3007},
{23,2980},
{24,2955},
{25,2924},
{26,2895},
{27,2870},
{28,2841},
{29,2812},
{30,2781},
{31,2749},
{32,2721},
{33,2688},
{34,2664},
{35,2641},
{36,2612},
{37,2588},
{38,2560},
{39,2531},
{40,2500},
{41,2474},
{42,2443},
{43,2412},
{44,2382},
{45,2351},
{46,2318},
{47,2286},
{48,2255},
{49,2228},
{50,2197},
{51,2161},
{52,2134},
{53,2105},
{54,2076},
{55,2048},
{56,2019},
{57,1990},
{58,1961},
{59,1932},
{60,1902},
{61,1874},
{62,1845},
{63,1815},
{64,1785},
{65,1755},
{66,1724},
{67,1694},
{68,1663},
{69,1633},
{70,1604},
{71,1576},
{72,1548},
{73,1522},
{74,1497},
{75,1474},
{76,1446},
{77,1419},
{78,1392},
{79,1366},
{80,1340},
{81,1315},
{82,1289},
{83,1265},
{84,1240},
{85,1216},
{86,1193},
{87,1170},
{88,1147},
{89,1125},
{90,1103},
{91,1074},
{92,1048},
{93,1024},
{94,1002},
{95,981},
{96,962},
{97,944},
{98,926},
{99,909},
{100,893},
{101,876},
{102,859},
{103,842},
{104,824},
{105,806},
{106,789},
{107,772},
{108,756},
{109,739},
{110,723}
		};
m_searchTableData=table;

}
int SearchTable_GetTempture ( uword ADCValue)
{
	int top_index=0,middle_index=71,bottom_index=140;
	int i;
	uword top_value,middle_value,bottom_value;

	if(ADCValue>3722||ADCValue<723)
	return -1000;
	
  if(ADCValue>=2474) {
  for(i=1;i<=71;i++) {
  if(m_searchTableData[i].value<=ADCValue&&ADCValue<=m_searchTableData[i-1].value)
  return m_searchTableData[i].tempture;
  }
  
  } 
  if(ADCValue<2474) {
  for(i=71;i<=140;i++) {
  if(m_searchTableData[i].value<=ADCValue&&ADCValue<=m_searchTableData[i-1].value)
  return m_searchTableData[i].tempture;
  }
  }
  return  -1000;
  
/*	while(1)
	{
		top_value=m_searchTableData[top_index].value;
		middle_value=m_searchTableData[middle_index].value;
		bottom_value=m_searchTableData[bottom_index].value;
		
		if(top_value==ADCValue)
			return m_searchTableData[top_index].tempture;
		if(middle_value==ADCValue)
			return m_searchTableData[middle_index].tempture;
		if(bottom_value==ADCValue)
			return m_searchTableData[bottom_index].tempture;
		

		if(middle_value<ADCValue&&ADCValue<top_value)
		{
			if(middle_index-top_index<=1)
				return m_searchTableData[middle_index].tempture;
			bottom_index=middle_index;
			middle_index=(bottom_index+top_index)>>1;

			continue;
		}
		else if(bottom_value<ADCValue&&ADCValue<middle_value)
		{
			if(bottom_index-middle_value<=1)
				return m_searchTableData[middle_index].tempture;
			top_index=middle_index;
			middle_index=(bottom_index+top_index)>>1;

			continue;
		}

	} */
}

