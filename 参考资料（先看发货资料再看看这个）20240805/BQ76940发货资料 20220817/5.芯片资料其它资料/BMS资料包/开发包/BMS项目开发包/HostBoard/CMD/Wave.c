/*
 * Wave.c
 *
 *  Created on: May 8, 2010
 *      Author: Administrator
 */

#include "Wave.h"
#include "XC22xxMREGS.h"


void Wave_Change_1()
{
	PWAVE_00(0);
}
void Wave_Change_2()
{
	PWAVE_03(0);
}
void Wave_Change_3()
{
	PWAVE_00(1);
}
void Wave_Change_4()
{

	PWAVE_01(1);
}
void Wave_Change_5()
{

	PWAVE_03(1);
}
void Wave_Change_6()
{
	PWAVE_05(1);
}
void Wave_Change_7()
{
	PWAVE_06(1);
}
void Wave_Change_8()
{
	PWAVE_07(1);
}
void Wave_Change_9()
{

	PWAVE_07(4);
}
void Wave_Change_10()
{

	PWAVE_06(4);
}
void Wave_Change_11()
{

	PWAVE_05(4);
}
void Wave_Change_12()
{

	PWAVE_04(4);
}
void Wave_Change_13()
{

	PWAVE_03(4);
}
void Wave_Change_14()
{

	PWAVE_02(4);
}
void Wave_Change_15()
{
	PWAVE_01(4);
}
void Wave_Change_16()
{
	PWAVE_00(4);
}
void Wave_Test()
{

	//Test_Event_SingleTest();
#ifdef TEST_Wave
	unsigned int i=0;
	while(1)
	{
	P11_OUT_P2=1;
	P11_IOCR02=0x80;
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	P11_OUT_P2=0;
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();
	for(i=0;i<0xffff;i++)
		__nop();

	}
#endif
#ifdef TEST_2
	while(1)
	{

		Wave_Change_1();
		Wave_Change_2();
		Wave_Change_3();
		Wave_Change_4();
		Wave_Change_5();
		Wave_Change_6();
		Wave_Change_7();
		Wave_Change_8();
		Wave_Change_9();
		Wave_Change_10();
		Wave_Change_11();
		Wave_Change_12();
		Wave_Change_13();
		Wave_Change_14();
		Wave_Change_15();
		Wave_Change_16();
/*
		PWAVE_00(0);
		PWAVE_01(0);
		PWAVE_02(0);
		PWAVE_03(0);
		PWAVE_04(0);
		PWAVE_05(0);
		PWAVE_06(0);
		PWAVE_07(0);

		PWAVE_00(1);
		PWAVE_01(1);
		PWAVE_02(1);
		PWAVE_03(1);
		PWAVE_04(1);
		PWAVE_05(1);
		PWAVE_06(1);
		PWAVE_07(1);
		PWAVE_00(2);
		PWAVE_01(2);
		PWAVE_02(2);
		PWAVE_03(2);
		PWAVE_04(2);
		PWAVE_05(2);
		PWAVE_06(2);
		PWAVE_07(2);
		PWAVE_08(2);
		PWAVE_09(2);
		PWAVE_10(2);
		PWAVE_11(2);
		PWAVE_12(2);

		PWAVE_00(3);
		PWAVE_01(3);
		PWAVE_02(3);
		PWAVE_03(3);
		PWAVE_04(3);
	    PWAVE_05(3);
		PWAVE_06(3);
		PWAVE_07(3);

		PWAVE_00(4);
		PWAVE_01(4);
		PWAVE_02(4);
		PWAVE_03(4);
		PWAVE_04(4);
		PWAVE_05(4);
		PWAVE_06(4);
		PWAVE_07(4);
		PWAVE_00(6);
		PWAVE_01(6);
		PWAVE_02(6);
		PWAVE_03(6);

		PWAVE_00(7);
		PWAVE_01(7);
		PWAVE_02(7);
		PWAVE_03(7);
		PWAVE_04(7);
		PWAVE_00(8);
		PWAVE_01(8);
		PWAVE_02(8);
		PWAVE_03(8);
		PWAVE_04(8);
		PWAVE_05(8);
		PWAVE_06(8);

		PWAVE_00(9);
		PWAVE_01(9);
		PWAVE_02(9);
		PWAVE_03(9);
		PWAVE_04(9);
		PWAVE_05(9);
		PWAVE_06(9);
		PWAVE_07(9);

		PWAVE_00(10);
		PWAVE_01(10);
		PWAVE_02(10);
		PWAVE_03(10);
		PWAVE_04(10);
		PWAVE_05(10);
		PWAVE_06(10);
		PWAVE_07(10);
		PWAVE_08(10);
		PWAVE_09(10);
		PWAVE_10(10);
		PWAVE_11(10);
		PWAVE_12(10);
		PWAVE_13(10);
		PWAVE_14(10);
		PWAVE_15(10);

		PWAVE_00(11);
		PWAVE_01(11);
		PWAVE_02(11);
		PWAVE_03(11);
		PWAVE_04(11);
		PWAVE_05(11);
*/

	}
#endif
}
