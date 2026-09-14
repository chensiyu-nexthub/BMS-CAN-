
#ifndef RELEASE_H
#define RELEASE_H

#define V1_DS PTBD_PTBD1
#define V2_DS PTCD_PTCD2
#define V3_DS PTAD_PTAD0
#define V4_DS PTCD_PTCD1
#define V5_DS PTBD_PTBD0
#define V6_DS PTCD_PTCD0
#define V7_DS PTFD_PTFD7
#define V8_DS PTDD_PTDD5
#define V9_DS PTDD_PTDD4
#define V10_DS PTDD_PTDD3
#define V11_DS PTDD_PTDD2
#define V12_DS PTFD_PTFD2
#define V13_DS PTFD_PTFD3
#define V14_DS PTGD_PTGD4
#define V15_DS PTGD_PTGD5
#define V16_DS PTDD_PTDD1

#define V1_Control	    PTBDD_PTBDD1
#define V2_Control      PTCDD_PTCDD2
#define V3_Control      PTADD_PTADD0
#define V4_Control      PTCDD_PTCDD1
#define V5_Control      PTBDD_PTBDD0
#define V6_Control	    PTCDD_PTCDD0
#define V7_Control      PTFDD_PTFDD7
#define V8_Control      PTDDD_PTDDD5
#define V9_Control      PTDDD_PTDDD4
#define V10_Control     PTDDD_PTDDD3
#define V11_Control     PTDDD_PTDDD2
#define V12_Control     PTFDD_PTFDD2
#define V13_Control     PTFDD_PTFDD3
#define V14_Control     PTGDD_PTGDD4
#define V15_Control     PTGDD_PTGDD5
#define V16_Control     PTDDD_PTDDD1

	void Release_vInit();
	void Release_vNotReleaseAll();
	void Release_vReleaseBattery();
  void Release_vDelay();
#endif