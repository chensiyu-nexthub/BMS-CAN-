/*
 * Event_Test.c
 *
 *  Created on: May 11, 2010
 *      Author: Administrator
 */
#include "Event.h"
/***********Add event one time and fetch it one time,do this loop for many times.***********/
//Single test:
//1.First define event array size only 5, then test this code below.
#include "Stdio.h"
void Test_Event_SingleTest()
{
	int event;
	printf("Start:\n");
	Event_AddEvent(1);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(2);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(3);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(4);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(5);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(6);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(7);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(8);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(9);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
	Event_AddEvent(10);
	while(Event_IsEmpty()!=1)
	{
		event=Event_GetEvent();
		printf("%d",event);
	}
}
