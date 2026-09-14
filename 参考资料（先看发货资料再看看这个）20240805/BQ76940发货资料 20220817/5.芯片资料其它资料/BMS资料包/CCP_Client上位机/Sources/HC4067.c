#include "HC4067.h"
#include "MC9S08DZ60.h"
static UINT8 g_ubHC4067InitFlag=0;
UINT8 g_ubHC4067Channel=0;
void HC4067_vInit() 
{

  HC4067_SELECT_BIT=1;
  HC4067_SELECT_CONTROL=1; 
  PTGD_PTGD2=0;
 	PTGD_PTGD3=0;
  PTFD_PTFD0=0;	
  PTFD_PTFD1=0;

}

void HC4067_vRelease()
{
  HC4067_SELECT_BIT=1;
  HC4067_SELECT_CONTROL=1; 
}
void HC4067_vSelect() 
{ 
  PTGDD_PTGDD2=1;  //VS_1
	PTGDD_PTGDD3=1;  //VS_2
	PTFDD_PTFDD0=1;  //VS_3
	PTFDD_PTFDD1=1;  //VS_4  
  HC4067_SELECT_CONTROL=1;
  
  PTGD_PTGD2=0;
 	PTGD_PTGD3=0;
  PTFD_PTFD0=0;	
  PTFD_PTFD1=0; 
  
 	if((g_ubHC4067Channel&1)!=0)
	{
	    PTGD_PTGD2=1;	
	}
	if((g_ubHC4067Channel&2)!=0)
	{
	    PTGD_PTGD3=1;	
	}
	if((g_ubHC4067Channel&4)!=0)
	{
	    PTFD_PTFD0=1;	
	}
	if((g_ubHC4067Channel&8)!=0)
	{
	    PTFD_PTFD1=1;	
	}
	HC4067_SELECT_BIT=0;

}
