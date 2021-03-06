/*
** Copyright (c) 2013 Silicon Laboratories, Inc.
** 2013-09-27 09:29:01
**
** Si3217x ProSLIC API Configuration Tool Version 2.11.0
*/


#ifndef SI3217X_CONSTANTS_H
#define SI3217X_CONSTANTS_H

/** Ringing Presets */
enum {
	RING_F20_45VRMS_0VDC_LPR,
	RING_F20_45VRMS_0VDC_BAL,
	RING_F20_45VRMS_0VDC_LPR_ON1000_OFF4000,
	RING_F25_48VRMS_0VDC_LPR_ON1000_OFF4500
};

/** DC_Feed Presets */
enum {
	DCFEED_48V_20MA,
	DCFEED_48V_25MA,
	DCFEED_48V_38MA
};

/** Impedance Presets */
enum {
	ZSYN_600_0_0_30_0,
	ZSYN_270_750_150_30_0,
	ZSYN_370_620_310_30_0,
	ZSYN_220_820_120_30_0,
	ZSYN_600_0_1000_30_0,
	ZSYN_200_680_100_30_0,
	ZSYN_220_820_115_30_0,
	ZSYN_600_0_0_54_0,
	ZSYN_250_750_150_54_0,
	ZSYN_200_680_100_54_0,
	ZSYN_220_820_115_54_0,
};

/** FSK Presets */
enum {
	DEFAULT_FSK
};

/** Pulse_Metering Presets */
enum {
	DEFAULT_PULSE_METERING
};

/** Tone Presets */
enum {
	TONE_NONE,
	TONE_F425,
	TONE_F425_M4,
	TONE_F425_ON200_OFF200,
	TONE_F425_ON1000_OFF4500,
	TONE_F425_ON500_OFF500,
	TONE_F425_ON240_OFF240,
	TONE_F440,
	TONE_F440_F350,
	TONE_F440_F350_ON100_OFF100,
	TONE_F450,
	/*TONE_F450_ON350_OFF350*/
	TONE_F480_F620_ON500_OFF500,
	TONE_F450_ON700_OFF700,
	/*TONE_F450_ON1000_OFF4000*/
	TONE_F440_F480_ON2000_OFF4000,
	TONE_F480_F620_ON200_OFF200,
	TONE_F480_F620_ON300_OFF200,
	TONE_F480_F620_ON300_OFF300,
	TONE_F600_ON100_OFF100,
	TONE_F932,
	TONE_F950,
	TONE_F1397,
	TONE_F1760,
	TONE_F2130_F2750,
	TONE_F2750,
	TONE_F2900,
	TONE_DTMF_0,
	TONE_DTMF_1,
	TONE_DTMF_2,
	TONE_DTMF_3,
	TONE_DTMF_4,
	TONE_DTMF_5,
	TONE_DTMF_6,
	TONE_DTMF_7,
	TONE_DTMF_8,
	TONE_DTMF_9,
	TONE_DTMF_A,
	TONE_DTMF_B,
	TONE_DTMF_C,
	TONE_DTMF_D,
	TONE_MAX,
};

/** PCM Presets */
enum {
	ULAW_PCM,
	LIN16_PCM,
	WB_LIN16_PCM,
};

#endif
