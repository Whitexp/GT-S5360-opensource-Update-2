/*.......................................................................................................
. COPYRIGHT (C)  SAMSUNG Electronics CO., LTD (Suwon, Korea). 2009
. All rights are reserved. Reproduction and redistiribution in whole or
. in part is prohibited without the written consent of the copyright owner.
.
.   Developer:
.   Date:
.   Description:
..........................................................................................................
*/
#if !defined(_CAMACQ_S5K5CAGX_H_)
#define _CAMACQ_S5K5CAGX_H_

/* Include */
#include "camacq_api.h"

/* Global */
#undef GLOBAL

#if !defined(_CAMACQ_API_C_)
#define GLOBAL extern
#else
#define GLOBAL
#endif /* _CAMACQ_API_C_ */

/* Include */
#if defined(WIN32)
#include "cmmfile.h"
#endif /* WIN32 */

/* Definition */
#define __S5K5CAGX_EVT0__         // Option             

#define CAMACQ_MAIN_NAME         "cami2c"               // wingi
#define CAMACQ_MAIN_I2C_ID       0x5A                // don't use now.
#define CAMACQ_MAIN_RES_TYPE   	 CAMACQ_SENSOR_MAIN   // main sensor

#define CAMACQ_MAIN_ISPROBED     0
#define CAMACQ_MAIN_CLOCK        0
#define CAMACQ_MAIN_YUVORDER     0
#define CAMACQ_MAIN_V_SYNCPOL    0
#define CAMACQ_MAIN_H_SYNCPOL    0
#define CAMACQ_MAIN_SAMPLE_EDGE  0
#define CAMACQ_MAIN_FULL_RANGE   0

#define CAMACQ_MAIN_RST
#define CAMACQ_MAIN_RST_MUX
#define CAMACQ_MAIN_EN
#define CAMACQ_MAIN_EN_MUX

#define CAMACQ_MAIN_RST_ON          1
#define CAMACQ_MAIN_RST_OFF         0
#define CAMACQ_MAIN_EN_ON           1
#define CAMACQ_MAIN_EN_OFF          0
#define CAMACQ_MAIN_STANDBY_ON      1
#define CAMACQ_MAIN_STANDBY_OFF	    0

#define CAMACQ_MAIN_POWER_CTRL(onoff)

#define CAMACQ_MAIN_2BYTE_SENSOR    0
#define CAMACQ_MAIN_AF              0
#define CAMACQ_MAIN_INT_MODE        1   // 0xAABBCCDD is INT_MODE, 0xAA, 0xBB, 0xCC, 0xDD is ARRAY MODE
#define CAMACQ_MAIN_FS_MODE         0
#define CAMACQ_MAIN_PATH_SET_FILE   "/sdcard/sensor/main/%s.dat"

#if (CAMACQ_MAIN_2BYTE_SENSOR)
#define CAMACQ_MAIN_BURST_MODE 0
#else
#define CAMACQ_MAIN_BURST_MODE 1
#endif /* CAMACQ_MAIN2BYTE_SENSOR */

#define CAMACQ_MAIN_BURST_I2C_TRACE 0

#define CAMACQ_MAIN_BURST_MAX 100

#define CAMACQ_MAIN_REG_FLAG_CNTS 	0x0F12
#define CAMACQ_MAIN_REG_DELAY 		0xFEFE
#define CAMACQ_MAIN_BTM_OF_DATA 	0xFFFFFFFF,
#define CAMACQ_MAIN_END_MARKER 		0xFF
#define CAMACQ_MAIN_REG_SET_SZ 		1 	// {0xFFFFFFFF} is 1, {0xFFFF,0xFFFF} is 2, {0xFF,0XFF} is 2, {0xFF,0xFF,0xFF,0xFF} is 4, {0xFFFF} is 1
#define CAMACQ_MAIN_REG_DAT_SZ 		4   // {0xFFFFFFFF} is 4, {0xFFFF,0xFFFF} is 2, {0xFF,0XFF} is 1, {0xFF,0xFF,0xFF,0xFF} is 1, {0xFFFF} is 2
#define CAMACQ_MAIN_FRATE_MIN  5
#define CAMACQ_MAIN_FRATE_MAX  30

// MACRO FUNCTIONS BEGIN ////////////////////////////////////////////////////////////
#if (CAMACQ_MAIN_2BYTE_SENSOR)
#define CAMACQ_MAIN_EXT_RD_SZ 1
#else
#define CAMACQ_MAIN_EXT_RD_SZ 2
#endif /* CAMACQ_MAIN_2BYTE_SENSOR */

#if CAMACQ_MAIN_2BYTE_SENSOR
#define CAMACQ_MAIN_EXT_REG_IS_BTM_OF_DATA(A)		(((A[0]==CAMACQ_MAIN_END_MARKER) && (A[1]==CAMACQ_MAIN_END_MARKER))? 1:0)
#define CAMACQ_MAIN_EXT_REG_IS_DELAY(A)				((A[0]==CAMACQ_MAIN_REG_DELAY)? 1:0)


#if (CAMACQ_MAIN_FS_MODE==1)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)\
memcpy(dest, &(srce[idx*CAMACQ_MAIN_REG_DAT_SZ*CAMACQ_MAIN_REG_SET_SZ]), CAMACQ_MAIN_REG_DAT_SZ*CAMACQ_MAIN_REG_SET_SZ);
#elif (CAMACQ_MAIN_REG_DAT_SZ==1)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)	dest[0] = (srce[idx][0] & 0xFF); dest[1] = (srce[idx][1] & 0xFF);
#elif (CAMACQ_MAIN_REG_DAT_SZ==2)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)	dest[0] = ((U8)(srce[idx] >> 8) & 0xFF); dest[1] = ((U8)(srce[idx]) & 0xFF);
#endif

#else // CAMACQ_MAIN_2BYTE_SENSOR

#define CAMACQ_MAIN_EXT_REG_IS_BTM_OF_DATA(A)		(((A[0]==CAMACQ_MAIN_END_MARKER) && (A[1]==CAMACQ_MAIN_END_MARKER) && \
(A[2]==CAMACQ_MAIN_END_MARKER) && (A[3]==CAMACQ_MAIN_END_MARKER))? 1:0)
#define CAMACQ_MAIN_EXT_REG_IS_DELAY(A)				(((A[0]==((CAMACQ_MAIN_REG_DELAY>>8) & 0xFF)) && (A[1]==(CAMACQ_MAIN_REG_DELAY & 0xFF)))? 1:0)
#define CAMACQ_MAIN_EXT_REG_IS_CNTS(A)				(((A[0]==((CAMACQ_MAIN_REG_FLAG_CNTS>>8) & 0xFF)) && (A[1]==(CAMACQ_MAIN_REG_FLAG_CNTS & 0xFF)))? 1:0)

#if (CAMACQ_MAIN_FS_MODE==1)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)\
memcpy(dest, &(srce[idx*CAMACQ_MAIN_REG_DAT_SZ*CAMACQ_MAIN_REG_SET_SZ]), CAMACQ_MAIN_REG_DAT_SZ*CAMACQ_MAIN_REG_SET_SZ);
#elif (CAMACQ_MAIN_REG_DAT_SZ==2)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)	dest[0]=((srce[idx][0] >> 8) & 0xFF); dest[1]=(srce[idx][0] & 0xFF); \
dest[2]=((srce[idx][1] >> 8) & 0xFF); dest[3]=(srce[idx][1] & 0xFF);
#elif (CAMACQ_MAIN_REG_DAT_SZ==1)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)	dest[0]=srce[idx][0]; dest[1]=srce[idx][1]; \
dest[2]=srce[idx][2]; dest[3]=srce[idx][3];
#elif (CAMACQ_MAIN_REG_DAT_SZ==4)
#define CAMACQ_MAIN_EXT_REG_GET_DATA(dest,srce,idx)	dest[0] = ((U8)(srce[idx] >> 24) & 0xFF); dest[1] = ((U8)(srce[idx] >> 16) & 0xFF); \
dest[2] = ((U8)(srce[idx] >> 8) & 0xFF); dest[3] = ((U8)(srce[idx]) & 0xFF);
#endif
#endif /* CAMACQ_2BYTE_SENSOR */
// MACRO FUNCTIONS END ///////////////////////////////////////////////////////////


/* DEFINE for sensor regs*/
#if( CAMACQ_MAIN_FS_MODE )
#define CAMACQ_MAIN_REG_INIT            "INIT"
#define CAMACQ_MAIN_REG_PREVIEW         "PREVIEW"
#define CAMACQ_MAIN_REG_SNAPSHOT        "SNAPSHOT"
#define CAMACQ_MAIN_REG_CAMCORDER        "CAMCORDER"

#define CAMACQ_MAIN_REG_WB_AUTO                 "WB_AUTO"
#define CAMACQ_MAIN_REG_WB_DAYLIGHT             "WB_DAYLIGHT"
#define CAMACQ_MAIN_REG_WB_CLOUDY               "WB_CLOUDY"
#define CAMACQ_MAIN_REG_WB_INCANDESCENT         "WB_INCANDESCENT"
#define CAMACQ_MAIN_REG_WB_FLUORESCENT         "WB_FLUORESCENT"

#define CAMACQ_MAIN_REG_SCENE_AUTO              "SCENE_AUTO"
#define CAMACQ_MAIN_REG_SCENE_NIGHT             "SCENE_NIGHT"
#define CAMACQ_MAIN_REG_SCENE_NIGHT_DARK        "SCENE_NIGHT_DARK"
#define CAMACQ_MAIN_REG_SCENE_LANDSCAPE         "SCENE_LANDSCAPE"
#define CAMACQ_MAIN_REG_SCENE_PORTRAIT          "SCENE_PORTRAIT"
#define CAMACQ_MAIN_REG_SCENE_SPORTS            "SCENE_SPORTS"
#define CAMACQ_MAIN_REG_SCENE_INDOOR            "SCENE_INDOOR"
#define CAMACQ_MAIN_REG_SCENE_SUNSET            "SCENE_SUNSET"
#define CAMACQ_MAIN_REG_SCENE_SUNRISE           "SCENE_SUNRISE"    // dawn
#define CAMACQ_MAIN_REG_SCENE_BEACH             "SCENE_BEACH"
#define CAMACQ_MAIN_REG_SCENE_FALLCOLOR         "SCENE_FALLCOLOR"
#define CAMACQ_MAIN_REG_SCENE_FIREWORKS         "SCENE_FIREWORKS"
#define CAMACQ_MAIN_REG_SCENE_CANDLELIGHT       "SCENE_CANDLELIGHT"
#define CAMACQ_MAIN_REG_SCENE_AGAINSTLIGHT      "SCENE_AGAINSTLIGHT"  // backlight

#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_0      "BRIGHTNESS_LEVEL_0"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_1      "BRIGHTNESS_LEVEL_1"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_2      "BRIGHTNESS_LEVEL_2"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_3      "BRIGHTNESS_LEVEL_3"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_4      "BRIGHTNESS_LEVEL_4"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_5      "BRIGHTNESS_LEVEL_5"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_6      "BRIGHTNESS_LEVEL_6"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_7      "BRIGHTNESS_LEVEL_7"
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_8      "BRIGHTNESS_LEVEL_8"

#define CAMACQ_MAIN_REG_METER_MATRIX            "METER_MATRIX"
#define CAMACQ_MAIN_REG_METER_CW                "METER_CW"
#define CAMACQ_MAIN_REG_METER_SPOT              "METER_SPOT"

#define CAMACQ_MAIN_REG_EFFECT_NONE             "EFFECT_NONE"
#define CAMACQ_MAIN_REG_EFFECT_GRAY             "EFFECT_GRAY" // mono, blackwhite
#define CAMACQ_MAIN_REG_EFFECT_NEGATIVE         "EFFECT_NEGATIVE"
#define CAMACQ_MAIN_REG_EFFECT_SEPIA            "EFFECT_SEPIA"

#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_M2              "CONTRAST_M2"
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_M1              "CONTRAST_M1"
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_DEFAULT         "CONTRAST_DEFAULT"
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_P1              "CONTRAST_P1"
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_P2              "CONTRAST_P2"

#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_M2             "SHARPNESS_M2"
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_M1             "SHARPNESS_M1"
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_DEFAULT        "SHARPNESS_DEFAULT"
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_P1             "SHARPNESS_P1"
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_P2             "SHARPNESS_P2"

#define CAMACQ_MAIN_REG_ADJUST_SATURATION_M2            "SATURATION_M2"
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_M1            "SATURATION_M1"
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_DEFAULT       "SATURATION_DEFAULT"
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_P1            "SATURATION_P1"
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_P2            "SATURATION_P2"

#else
#define CAMACQ_MAIN_REG_INIT            reg_main_init
#define CAMACQ_MAIN_REG_PREVIEW         reg_main_preview
#define CAMACQ_MAIN_REG_SNAPSHOT        reg_main_snapshot
#define CAMACQ_MAIN_REG_CAMCORDER      reg_main_camcorder

#define CAMACQ_MAIN_REG_WB_AUTO                 reg_main_wb_auto
#define CAMACQ_MAIN_REG_WB_DAYLIGHT             reg_main_wb_daylight
#define CAMACQ_MAIN_REG_WB_CLOUDY               reg_main_wb_cloudy
#define CAMACQ_MAIN_REG_WB_INCANDESCENT         reg_main_wb_incandescent
#define CAMACQ_MAIN_REG_WB_FLUORESCENT          reg_main_wb_fluorescent


#define CAMACQ_MAIN_REG_SCENE_AUTO              reg_main_scene_auto  // auto, off
#define CAMACQ_MAIN_REG_SCENE_NIGHT             reg_main_scene_night
#define CAMACQ_MAIN_REG_SCENE_NIGHT_DARK        reg_main_scene_night_dark
#define CAMACQ_MAIN_REG_SCENE_LANDSCAPE         reg_main_scene_landscape
#define CAMACQ_MAIN_REG_SCENE_PORTRAIT          reg_main_scene_portrait
#define CAMACQ_MAIN_REG_SCENE_SPORTS            reg_main_scene_sports
#define CAMACQ_MAIN_REG_SCENE_INDOOR            reg_main_scene_indoor
#define CAMACQ_MAIN_REG_SCENE_SUNSET            reg_main_scene_sunset
#define CAMACQ_MAIN_REG_SCENE_SUNRISE           reg_main_scene_sunrise    // dawn
#define CAMACQ_MAIN_REG_SCENE_BEACH             reg_main_scene_beach
#define CAMACQ_MAIN_REG_SCENE_FALLCOLOR         reg_main_scene_fallcolor
#define CAMACQ_MAIN_REG_SCENE_FIREWORKS         reg_main_scene_fireworks
#define CAMACQ_MAIN_REG_SCENE_CANDLELIGHT       reg_main_scene_candlelight
#define CAMACQ_MAIN_REG_SCENE_AGAINSTLIGHT      reg_main_scene_againstlight  // backlight

#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_0      reg_main_brightness_level_0
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_1      reg_main_brightness_level_1
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_2      reg_main_brightness_level_2
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_3      reg_main_brightness_level_3
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_4      reg_main_brightness_level_4
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_5      reg_main_brightness_level_5
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_6      reg_main_brightness_level_6
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_7      reg_main_brightness_level_7
#define CAMACQ_MAIN_REG_BRIGHTNESS_LEVEL_8      reg_main_brightness_level_8

#define CAMACQ_MAIN_REG_METER_MATRIX            reg_main_meter_matrix
#define CAMACQ_MAIN_REG_METER_CW                reg_main_meter_cw
#define CAMACQ_MAIN_REG_METER_SPOT              reg_main_meter_spot

#define CAMACQ_MAIN_REG_EFFECT_NONE             reg_main_effect_none
#define CAMACQ_MAIN_REG_EFFECT_GRAY             reg_main_effect_gray // mono, blackwhite
#define CAMACQ_MAIN_REG_EFFECT_NEGATIVE         reg_main_effect_negative
#define CAMACQ_MAIN_REG_EFFECT_SEPIA            reg_main_effect_sepia

#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_M2              reg_main_adjust_contrast_m2
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_M1              reg_main_adjust_contrast_m1
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_DEFAULT         reg_main_adjust_contrast_default
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_P1              reg_main_adjust_contrast_p1
#define CAMACQ_MAIN_REG_ADJUST_CONTRAST_P2              reg_main_adjust_contrast_p2

#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_M2             reg_main_adjust_sharpness_m2
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_M1             reg_main_adjust_sharpness_m1
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_DEFAULT        reg_main_adjust_sharpness_default
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_P1             reg_main_adjust_sharpness_p1
#define CAMACQ_MAIN_REG_ADJUST_SHARPNESS_P2             reg_main_adjust_sharpness_p2

#define CAMACQ_MAIN_REG_ADJUST_SATURATION_M2            reg_main_adjust_saturation_m2
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_M1            reg_main_adjust_saturation_m1
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_DEFAULT       reg_main_adjust_saturation_default
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_P1            reg_main_adjust_saturation_p1
#define CAMACQ_MAIN_REG_ADJUST_SATURATION_P2            reg_main_adjust_saturation_p2
#endif

#define CAMACQ_MAIN_REG_HIGHLIGHT                reg_main_highlight
#define CAMACQ_MAIN_REG_MIDLIGHT                reg_main_midlight
#define CAMACQ_MAIN_REG_LOWLIGHT                reg_main_lowlight
#define CAMACQ_MAIN_REG_NIGHTSHOT_ON            reg_main_nightshot_on
#define CAMACQ_MAIN_REG_NIGHTSHOT_OFF           reg_main_nightshot_off
#define CAMACQ_MAIN_REG_NIGHTSHOT               reg_main_nightshot

#define CAMACQ_MAIN_REG_WB_TWILIGHT             reg_main_wb_twilight
#define CAMACQ_MAIN_REG_WB_TUNGSTEN             reg_main_wb_tungsten
#define CAMACQ_MAIN_REG_WB_OFF                  reg_main_wb_off
#define CAMACQ_MAIN_REG_WB_HORIZON              reg_main_wb_horizon
#define CAMACQ_MAIN_REG_WB_SHADE                reg_main_wb_shade

#define CAMACQ_MAIN_REG_EFFECT_SOLARIZE         reg_main_effect_solarize
#define CAMACQ_MAIN_REG_EFFECT_POSTERIZE        reg_main_effect_posterize
#define CAMACQ_MAIN_REG_EFFECT_WHITEBOARD       reg_main_effect_whiteboard
#define CAMACQ_MAIN_REG_EFFECT_BLACKBOARD       reg_main_effect_blackboard
#define CAMACQ_MAIN_REG_EFFECT_AQUA             reg_main_effect_aqua
#define CAMACQ_MAIN_REG_EFFECT_SHARPEN          reg_main_effect_sharpen
#define CAMACQ_MAIN_REG_EFFECT_VIVID            reg_main_effect_vivid
#define CAMACQ_MAIN_REG_EFFECT_GREEN            reg_main_effect_green
#define CAMACQ_MAIN_REG_EFFECT_SKETCH           reg_main_effect_sketch


#define CAMACQ_MAIN_REG_FLIP_NONE               reg_main_flip_none
#define CAMACQ_MAIN_REG_FLIP_WATER              reg_main_flip_water
#define CAMACQ_MAIN_REG_FLIP_MIRROR             reg_main_flip_mirror
#define CAMACQ_MAIN_REG_FLIP_WATER_MIRROR       reg_main_flip_water_mirror


#define CAMACQ_MAIN_REG_FPS_FIXED_5             reg_main_fps_fixed_5
#define CAMACQ_MAIN_REG_FPS_FIXED_7             reg_main_fps_fixed_7
#define CAMACQ_MAIN_REG_FPS_FIXED_10            reg_main_fps_fixed_10
#define CAMACQ_MAIN_REG_FPS_FIXED_15            reg_main_fps_fixed_15
#define CAMACQ_MAIN_REG_FPS_FIXED_20            reg_main_fps_fixed_20
#define CAMACQ_MAIN_REG_FPS_FIXED_25            reg_main_fps_fixed_25
#define CAMACQ_MAIN_REG_FPS_VAR_15              reg_main_fps_var_15
#define CAMACQ_MAIN_REG_FPS_FIXED_30            reg_main_fps_fixed_30


#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_0 reg_main_expcompensation_level_0
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_1 reg_main_expcompensation_level_1
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_2 reg_main_expcompensation_level_2
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_3 reg_main_expcompensation_level_3
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_4 reg_main_expcompensation_level_4
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_5 reg_main_expcompensation_level_5
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_6 reg_main_expcompensation_level_6
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_7 reg_main_expcompensation_level_7
#define CAMACQ_MAIN_REG_EXPCOMPENSATION_LEVEL_8 reg_main_expcompensation_level_8

#define CAMACQ_MAIN_REG_SET_AF                  reg_main_set_af  // start af
#define CAMACQ_MAIN_REG_OFF_AF                  reg_main_off_af
#define CAMACQ_MAIN_REG_CHECK_AF                reg_main_check_af
#define CAMACQ_MAIN_REG_RESET_AF                reg_main_reset_af
#define CAMACQ_MAIN_REG_MANUAL_AF               reg_main_manual_af    // normal_af
#define CAMACQ_MAIN_REG_MACRO_AF                reg_main_macro_af
#define CAMACQ_MAIN_REG_RETURN_MANUAL_AF        reg_main_return_manual_af
#define CAMACQ_MAIN_REG_RETURN_MACRO_AF         reg_main_return_macro_af
#define CAMACQ_MAIN_REG_SET_AF_NLUX             reg_main_set_af_nlux
#define CAMACQ_MAIN_REG_SET_AF_LLUX             reg_main_set_af_llux
#define CAMACQ_MAIN_REG_SET_AF_NORMAL_MODE_1    reg_main_set_af_normal_mode_1
#define CAMACQ_MAIN_REG_SET_AF_NORMAL_MODE_2    reg_main_set_af_normal_mode_2
#define CAMACQ_MAIN_REG_SET_AF_NORMAL_MODE_3    reg_main_set_af_normal_mode_3
#define CAMACQ_MAIN_REG_SET_AF_MACRO_MODE_1     reg_main_set_af_macro_mode_1
#define CAMACQ_MAIN_REG_SET_AF_MACRO_MODE_2     reg_main_set_af_macro_mode_2
#define CAMACQ_MAIN_REG_SET_AF_MACRO_MODE_3     reg_main_set_af_macro_mode_3

#define CAMACQ_MAIN_REG_ISO_AUTO                reg_main_iso_auto
#define CAMACQ_MAIN_REG_ISO_50                  reg_main_iso_50
#define CAMACQ_MAIN_REG_ISO_100                 reg_main_iso_100
#define CAMACQ_MAIN_REG_ISO_200                 reg_main_iso_200
#define CAMACQ_MAIN_REG_ISO_400                 reg_main_iso_400
#define CAMACQ_MAIN_REG_ISO_800                 reg_main_iso_800
#define CAMACQ_MAIN_REG_ISO_1600                reg_main_iso_1600
#define CAMACQ_MAIN_REG_ISO_3200                reg_main_iso_3200


#define CAMACQ_MAIN_REG_SCENE_PARTY             reg_main_scene_party
#define CAMACQ_MAIN_REG_SCENE_SNOW              reg_main_scene_snow
#define CAMACQ_MAIN_REG_SCENE_TEXT              reg_main_scene_text

#define CAMACQ_MAIN_REG_QQVGA                   reg_main_qqvga
#define CAMACQ_MAIN_REG_QCIF                    reg_main_qcif
#define CAMACQ_MAIN_REG_QVGA                    reg_main_qvga
#define CAMACQ_MAIN_REG_WQVGA                    reg_main_wqvga
#define CAMACQ_MAIN_REG_CIF                     reg_main_cif
#define CAMACQ_MAIN_REG_VGA                     reg_main_vga
#define CAMACQ_MAIN_REG_WVGA                     reg_main_wvga // 800* 480
#define CAMACQ_MAIN_REG_SVGA                    reg_main_svga
#define CAMACQ_MAIN_REG_SXGA                    reg_main_sxga
#define CAMACQ_MAIN_REG_QXGA                    reg_main_qxga
#define CAMACQ_MAIN_REG_UXGA                    reg_main_uxga
#define CAMACQ_MAIN_REG_FULL_YUV                reg_main_full_yuv
#define CAMACQ_MAIN_REG_CROP_YUV                reg_main_crop_yuv
#define CAMACQ_MAIN_REG_QVGA_V		            reg_main_qvga_v
#define CAMACQ_MAIN_REG_HALF_VGA_V	            reg_main_half_vga_v
#define CAMACQ_MAIN_REG_HALF_VGA		        reg_main_half_vga
#define CAMACQ_MAIN_REG_VGA_V		            reg_main_vga_v
#define CAMACQ_MAIN_REG_5M			            reg_main_5M
#define CAMACQ_MAIN_REG_1080P		            reg_main_1080P
#define CAMACQ_MAIN_REG_720P			        reg_main_720P
#define CAMACQ_MAIN_REG_NOT                     reg_main_not

#define CAMACQ_MAIN_REG_JPEG_5M                 reg_main_jpeg_5m        //2560X1920
#define CAMACQ_MAIN_REG_JPEG_5M_2               reg_main_jpeg_5m_2      // 2592X1944
#define CAMACQ_MAIN_REG_JPEG_W4M                 reg_main_jpeg_w4m      // 2560x1536
#define CAMACQ_MAIN_REG_JPEG_3M                 reg_main_jpeg_3m        // QXGA 2048X1536
#define CAMACQ_MAIN_REG_JPEG_2M                 reg_main_jpeg_2m        // UXGA 1600x1200
#define CAMACQ_MAIN_REG_JPEG_W1_5M               reg_main_jpeg_w1_5m    // 1280x960
#define CAMACQ_MAIN_REG_JPEG_1M                 reg_main_jpeg_1m
#define CAMACQ_MAIN_REG_JPEG_VGA                reg_main_jpeg_vga   //640x480
#define CAMACQ_MAIN_REG_JPEG_WQVGA              reg_main_jpeg_wqvga //420x240
#define CAMACQ_MAIN_REG_JPEG_QVGA               reg_main_jpeg_qvga  //320x240

#define CAMACQ_MAIN_REG_FLICKER_DISABLED        reg_main_flicker_disabled
#define CAMACQ_MAIN_REG_FLICKER_50HZ            reg_main_flicker_50hz
#define CAMACQ_MAIN_REG_FLICKER_60HZ            reg_main_flicker_60hz
#define CAMACQ_MAIN_REG_FLICKER_AUTO            reg_main_flicker_auto

// image quality
#define CAMACQ_MAIN_REG_JPEG_QUALITY_SUPERFINE reg_main_jpeg_quality_superfine
#define CAMACQ_MAIN_REG_JPEG_QUALITY_FINE      reg_main_jpeg_quality_fine
#define CAMACQ_MAIN_REG_JPEG_QUALITY_NORMAL    reg_main_jpeg_quality_normal

// Private Control
#define CAMACQ_MAIN_REG_PRIVCTRL_RETURNPREVIEW  reg_main_priv_ctrl_returnpreview

// Format
#define CAMACQ_MAIN_REG_FMT_YUV422 	            reg_main_fmt_yuv422
#define CAMACQ_MAIN_REG_FMT_JPG		            reg_main_fmt_jpg


// NEW
#define CAMACQ_MAIN_REG_SLEEP                   reg_main_sleep
#define CAMACQ_MAIN_REG_WAKEUP                  reg_main_wakeup

/* Enumeration */

/* Global Value */
GLOBAL const U32 reg_main_sleep[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


GLOBAL const U32 reg_main_wakeup[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_init[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
//****************************************/
//===================================================================
// History
//===================================================================
//20100717 : 1st release
//20100806 : 2nd release for EVT0.1
//20101028 : 3rd release for EVT1

//////////////////////////////////////////////////////////////
////WRITE #awbb_otp_disable  0000 //awb otp use

//==========================================================================================
//-->The below registers are for FACTORY ONLY. if you change them without prior notification,
//   YOU are RESPONSIBLE for the FAILURE that will happen in the future.
//==========================================================================================
//===================================================================
// Reset & Trap and Patch
//===================================================================

// Start of Trap and Patch

//  2010-08-11 13:53:35
0x00100001,
0x10300000,
0x00140001,

0xFEFE000A,	//Dealy 10ms

//ESD ADD
0x00287000,
0x002A0150,
0x0F12AAAA,

// Start of Patch data
0x00287000,
0x002A352C,
0x0F12B510,                                    // 7000352C 
0x0F124A2C,    // 7000352E 
0x0F12213F,    // 70003530 
0x0F12482C,    // 70003532 
0x0F124B2C,    // 70003534 
0x0F122400,    // 70003536 
0x0F12801C,    // 70003538 
0x0F12C004,    // 7000353A 
0x0F126001,                                    // 7000353C 
0x0F12492B,    // 7000353E 
0x0F12482B,    // 70003540 
0x0F12F000,    // 70003542 
0x0F12FBE9,    // 70003544 
0x0F122115,    // 70003546 
0x0F12482A,    // 70003548 
0x0F1201C9,    // 7000354A 
0x0F12F000,    // 7000354C 
0x0F12F88E,    // 7000354E 
0x0F124828,    // 70003550 
0x0F12210B,    // 70003552 
0x0F120189,    // 70003554 
0x0F123018,    // 70003556 
0x0F12F000,    // 70003558 
0x0F12F888,    // 7000355A 
0x0F124825,    // 7000355C 
0x0F124926,    // 7000355E 
0x0F12300C,    // 70003560 
0x0F12F000,    // 70003562 
0x0F12F883,    // 70003564 
0x0F124823,    // 70003566 
0x0F124924,    // 70003568 
0x0F123010,    // 7000356A 
0x0F12F000,    // 7000356C 
0x0F12F87E,    // 7000356E 
0x0F124923,    // 70003570 
0x0F124824,    // 70003572 
0x0F12F000,    // 70003574 
0x0F12FBD0,    // 70003576 
0x0F124923,    // 70003578 
0x0F124824,    // 7000357A 
0x0F12F000,    // 7000357C 
0x0F12FBCC,    // 7000357E 
0x0F124923,    // 70003580 
0x0F124824,    // 70003582 
0x0F12F000,    // 70003584 
0x0F12FBC8,    // 70003586 
0x0F124923,    // 70003588 
0x0F124824,    // 7000358A 
0x0F12F000,    // 7000358C 
0x0F12FBC4,    // 7000358E 
0x0F124923,    // 70003590 
0x0F124824,    // 70003592 
0x0F12F000,    // 70003594 
0x0F12FBC0,    // 70003596 
0x0F124823,    // 70003598 
0x0F124924,    // 7000359A 
0x0F126408,    // 7000359C 
0x0F124924,    // 7000359E 
0x0F124824,    // 700035A0 
0x0F12F000,    // 700035A2 
0x0F12FBB9,    // 700035A4 
0x0F124924,    // 700035A6 
0x0F124824,    // 700035A8 
0x0F12F000,    // 700035AA 
0x0F12FBB5,    // 700035AC 
0x0F124924,    // 700035AE 
0x0F124824,    // 700035B0 
0x0F12F000,    // 700035B2 
0x0F12FBB1,    // 700035B4 
0x0F124924,    // 700035B6 
0x0F124824,    // 700035B8 
0x0F12F000,    // 700035BA 
0x0F12FBAD,    // 700035BC 
0x0F124924,    // 700035BE 
0x0F124824,    // 700035C0 
0x0F12F000,    // 700035C2 
0x0F12FBA9,    // 700035C4 
0x0F124824,    // 700035C6 
0x0F128104,    // 700035C8 
0x0F124924,    // 700035CA 
0x0F124824,    // 700035CC 
0x0F12F000,    // 700035CE 
0x0F12FBA3,    // 700035D0 
0x0F124924,    // 700035D2 
0x0F124824,    // 700035D4 
0x0F12F000,    // 700035D6 
0x0F12FB9F,    // 700035D8 
0x0F12BC10,    // 700035DA 
0x0F12BC08,    // 700035DC 
0x0F124718,    // 700035DE 
0x0F1200BA,    // 700035E0 
0x0F125CC1,    // 700035E2 
0x0F121C08,    // 700035E4 
0x0F127000,    // 700035E6 
0x0F123290,    // 700035E8 
0x0F127000,    // 700035EA 
0x0F12368B,    // 700035EC 
0x0F127000,    // 700035EE 
0x0F12D9E7,    // 700035F0 
0x0F120000,    // 700035F2 
0x0F126FC0,    // 700035F4 
0x0F120000,    // 700035F6 
0x0F120A91,    // 700035F8 
0x0F120000,    // 700035FA 
0x0F1202C9,    // 700035FC 
0x0F120000,    // 700035FE 
0x0F1236C3,    // 70003600 
0x0F127000,    // 70003602 
0x0F12A607,    // 70003604 
0x0F120000,    // 70003606 
0x0F123733,    // 70003608 
0x0F127000,    // 7000360A 
0x0F127C0D,    // 7000360C 
0x0F120000,    // 7000360E 
0x0F12374F,    // 70003610 
0x0F127000,    // 70003612 
0x0F127C2B,    // 70003614 
0x0F120000,    // 70003616 
0x0F12376B,    // 70003618 
0x0F127000,    // 7000361A 
0x0F129E89,    // 7000361C 
0x0F120000,    // 7000361E 
0x0F12394F,    // 70003620 
0x0F127000,    // 70003622 
0x0F12395D,    // 70003624 
0x0F120000,    // 70003626 
0x0F1239DB,    // 70003628 
0x0F127000,    // 7000362A 
0x0F120000,    // 7000362C 
0x0F127000,    // 7000362E 
0x0F123B01,    // 70003630 
0x0F127000,    // 70003632 
0x0F12F903,    // 70003634 
0x0F120000,    // 70003636 
0x0F1237B3,    // 70003638 
0x0F127000,    // 7000363A 
0x0F12495F,    // 7000363C 
0x0F120000,    // 7000363E 
0x0F12380D,    // 70003640 
0x0F127000,    // 70003642 
0x0F12E421,    // 70003644 
0x0F120000,    // 70003646 
0x0F1238BB,    // 70003648 
0x0F127000,    // 7000364A 
0x0F12216D,    // 7000364C 
0x0F120000,    // 7000364E 
0x0F12392F,    // 70003650 
0x0F127000,    // 70003652 
0x0F120179,    // 70003654 
0x0F120001,    // 70003656 
0x0F123FC8,    // 70003658 
0x0F127000,    // 7000365A 
0x0F123CE3,    // 7000365C 
0x0F127000,    // 7000365E 
0x0F1204C9,    // 70003660 
0x0F120000,    // 70003662 
0x0F123C2F,    // 70003664 
0x0F127000,    // 70003666 
0x0F125027,    // 70003668 
0x0F120000,    // 7000366A 
0x0F12B570,    // 7000366C 
0x0F12000D,    // 7000366E 
0x0F124CFF,    // 70003670 
0x0F128821,    // 70003672 
0x0F12F000,    // 70003674 
0x0F12FB58,    // 70003676 
0x0F128820,    // 70003678 
0x0F124AFE,    // 7000367A 
0x0F120081,    // 7000367C 
0x0F125055,    // 7000367E 
0x0F121C40,    // 70003680 
0x0F128020,    // 70003682 
0x0F12BC70,    // 70003684 
0x0F12BC08,    // 70003686 
0x0F124718,    // 70003688 
0x0F126801,    // 7000368A 
0x0F120409,    // 7000368C 
0x0F120C09,    // 7000368E 
0x0F126840,    // 70003690 
0x0F120400,    // 70003692 
0x0F120C00,    // 70003694 
0x0F124AF8,    // 70003696 
0x0F128992,    // 70003698 
0x0F122A00,    // 7000369A 
0x0F12D00D,    // 7000369C 
0x0F122300,    // 7000369E 
0x0F121A80,    // 700036A0 
0x0F12D400,    // 700036A2 
0x0F120003,    // 700036A4 
0x0F120418,    // 700036A6 
0x0F120C00,    // 700036A8 
0x0F124BF4,    // 700036AA 
0x0F121851,    // 700036AC 
0x0F12891B,    // 700036AE 
0x0F12428B,    // 700036B0 
0x0F12D300,    // 700036B2 
0x0F12000B,    // 700036B4 
0x0F120419,    // 700036B6 
0x0F120C09,    // 700036B8 
0x0F124AF1,    // 700036BA 
0x0F128151,    // 700036BC 
0x0F128190,    // 700036BE 
0x0F124770,    // 700036C0 
0x0F12B510,    // 700036C2 
0x0F1248EF,    // 700036C4 
0x0F124CF0,    // 700036C6 
0x0F1288C1,    // 700036C8 
0x0F128061,    // 700036CA 
0x0F122101,    // 700036CC 
0x0F128021,    // 700036CE 
0x0F128840,    // 700036D0 
0x0F12F000,    // 700036D2 
0x0F12FB31,    // 700036D4 
0x0F1288E0,    // 700036D6 
0x0F124AEC,    // 700036D8 
0x0F122800,    // 700036DA 
0x0F12D003,    // 700036DC 
0x0F1249EC,    // 700036DE 
0x0F128849,    // 700036E0 
0x0F122900,    // 700036E2 
0x0F12D009,    // 700036E4 
0x0F122001,    // 700036E6 
0x0F1203C0,    // 700036E8 
0x0F128050,    // 700036EA 
0x0F1280D0,    // 700036EC 
0x0F122000,    // 700036EE 
0x0F128090,    // 700036F0 
0x0F128110,    // 700036F2 
0x0F12BC10,    // 700036F4 
0x0F12BC08,    // 700036F6 
0x0F124718,    // 700036F8 
0x0F128050,    // 700036FA 
0x0F128920,    // 700036FC 
0x0F1280D0,    // 700036FE 
0x0F128960,    // 70003700 
0x0F120400,    // 70003702 
0x0F121400,    // 70003704 
0x0F128090,    // 70003706 
0x0F1289A1,    // 70003708 
0x0F120409,    // 7000370A 
0x0F121409,    // 7000370C 
0x0F128111,    // 7000370E 
0x0F1289E3,    // 70003710 
0x0F128A24,    // 70003712 
0x0F122B00,    // 70003714 
0x0F12D104,    // 70003716 
0x0F1217C3,    // 70003718 
0x0F120F5B,    // 7000371A 
0x0F121818,    // 7000371C 
0x0F1210C0,    // 7000371E 
0x0F128090,    // 70003720 
0x0F122C00,    // 70003722 
0x0F12D1E6,    // 70003724 
0x0F1217C8,    // 70003726 
0x0F120F40,    // 70003728 
0x0F121840,    // 7000372A 
0x0F1210C0,    // 7000372C 
0x0F128110,    // 7000372E 
0x0F12E7E0,    // 70003730 
0x0F12B510,    // 70003732 
0x0F120004,    // 70003734 
0x0F1249D5,    // 70003736 
0x0F122204,    // 70003738 
0x0F126820,    // 7000373A 
0x0F125E8A,    // 7000373C 
0x0F120140,    // 7000373E 
0x0F121A80,    // 70003740 
0x0F120280,    // 70003742 
0x0F128849,    // 70003744 
0x0F12F000,    // 70003746 
0x0F12FAFF,    // 70003748 
0x0F126020,    // 7000374A 
0x0F12E7D2,    // 7000374C 
0x0F12B510,    // 7000374E 
0x0F120004,    // 70003750 
0x0F1249CE,    // 70003752 
0x0F122208,    // 70003754 
0x0F126820,    // 70003756 
0x0F125E8A,    // 70003758 
0x0F120140,    // 7000375A 
0x0F121A80,    // 7000375C 
0x0F120280,    // 7000375E 
0x0F1288C9,    // 70003760 
0x0F12F000,    // 70003762 
0x0F12FAF1,    // 70003764 
0x0F126020,    // 70003766 
0x0F12E7C4,    // 70003768 
0x0F12B510,    // 7000376A 
0x0F120004,    // 7000376C 
0x0F1249C7,    // 7000376E 
0x0F1248C8,    // 70003770 
0x0F12884A,    // 70003772 
0x0F128B43,    // 70003774 
0x0F12435A,    // 70003776 
0x0F122304,    // 70003778 
0x0F125ECB,    // 7000377A 
0x0F120A92,    // 7000377C 
0x0F1218D2,    // 7000377E 
0x0F1202D2,    // 70003780 
0x0F120C12,    // 70003782 
0x0F1288CB,    // 70003784 
0x0F128B80,    // 70003786 
0x0F124343,    // 70003788 
0x0F120A98,    // 7000378A 
0x0F122308,    // 7000378C 
0x0F125ECB,    // 7000378E 
0x0F1218C0,    // 70003790 
0x0F1202C0,    // 70003792 
0x0F120C00,    // 70003794 
0x0F120411,    // 70003796 
0x0F120400,    // 70003798 
0x0F121409,    // 7000379A 
0x0F121400,    // 7000379C 
0x0F121A08,    // 7000379E 
0x0F1249BC,    // 700037A0 
0x0F123980,    // 700037A2 
0x0F126348,    // 700037A4 
0x0F120020,    // 700037A6 
0x0F12C80F,    // 700037A8 
0x0F12F000,    // 700037AA 
0x0F12FAD3,    // 700037AC 
0x0F126020,    // 700037AE 
0x0F12E7A0,    // 700037B0 
0x0F12B510,    // 700037B2 
0x0F124CB8,    // 700037B4 
0x0F1248B9,    // 700037B6 
0x0F1278A1,    // 700037B8 
0x0F122900,    // 700037BA 
0x0F12D101,    // 700037BC 
0x0F1287C1,    // 700037BE 
0x0F12E004,    // 700037C0 
0x0F127AE1,    // 700037C2 
0x0F122900,    // 700037C4 
0x0F12D001,    // 700037C6 
0x0F122101,    // 700037C8 
0x0F1287C1,    // 700037CA 
0x0F12F000,    // 700037CC 
0x0F12FACA,    // 700037CE 
0x0F1249B3,    // 700037D0 
0x0F128B08,    // 700037D2 
0x0F1206C2,    // 700037D4 
0x0F12D50A,    // 700037D6 
0x0F127AA2,    // 700037D8 
0x0F120652,    // 700037DA 
0x0F12D507,    // 700037DC 
0x0F122210,    // 700037DE 
0x0F124390,    // 700037E0 
0x0F128308,    // 700037E2 
0x0F1248AF,    // 700037E4 
0x0F127AE1,    // 700037E6 
0x0F126B00,    // 700037E8 
0x0F12F000,    // 700037EA 
0x0F12FAC3,    // 700037EC 
0x0F1248A2,    // 700037EE 
0x0F1289C0,    // 700037F0 
0x0F122801,    // 700037F2 
0x0F12D109,    // 700037F4 
0x0F1278A0,    // 700037F6 
0x0F122800,    // 700037F8 
0x0F12D006,    // 700037FA 
0x0F127AE0,    // 700037FC 
0x0F122800,    // 700037FE 
0x0F12D003,    // 70003800 
0x0F127AA0,    // 70003802 
0x0F122140,    // 70003804 
0x0F124308,    // 70003806 
0x0F1272A0,    // 70003808 
0x0F12E773,    // 7000380A 
0x0F12B570,    // 7000380C 
0x0F124DA4,    // 7000380E 
0x0F124CA4,    // 70003810 
0x0F128B28,    // 70003812 
0x0F120701,    // 70003814 
0x0F12D507,    // 70003816 
0x0F122108,    // 70003818 
0x0F124388,    // 7000381A 
0x0F128328,    // 7000381C 
0x0F1249A2,    // 7000381E 
0x0F126B20,    // 70003820 
0x0F126B89,    // 70003822 
0x0F12F000,    // 70003824 
0x0F12FAAE,    // 70003826 
0x0F128B28,    // 70003828 
0x0F1206C1,    // 7000382A 
0x0F12D50A,    // 7000382C 
0x0F12499A,    // 7000382E 
0x0F127A8A,    // 70003830 
0x0F120652,    // 70003832 
0x0F12D406,    // 70003834 
0x0F122210,    // 70003836 
0x0F124390,    // 70003838 
0x0F128328,    // 7000383A 
0x0F127AC9,    // 7000383C 
0x0F126B20,    // 7000383E 
0x0F12F000,    // 70003840 
0x0F12FA98,    // 70003842 
0x0F12E71E,    // 70003844 
0x0F12B5F8,    // 70003846 
0x0F124C98,    // 70003848 
0x0F1226FF,    // 7000384A 
0x0F128820,    // 7000384C 
0x0F124D98,    // 7000384E 
0x0F121C76,    // 70003850 
0x0F122702,    // 70003852 
0x0F122803,    // 70003854 
0x0F12D112,    // 70003856 
0x0F128860,    // 70003858 
0x0F122800,    // 7000385A 
0x0F12D10F,    // 7000385C 
0x0F1288E0,    // 7000385E 
0x0F122800,    // 70003860 
0x0F12D10C,    // 70003862 
0x0F12F000,    // 70003864 
0x0F12FA96,    // 70003866 
0x0F122800,    // 70003868 
0x0F12D008,    // 7000386A 
0x0F128F28,    // 7000386C 
0x0F122800,    // 7000386E 
0x0F12D001,    // 70003870 
0x0F1280E6,    // 70003872 
0x0F1280A7,    // 70003874 
0x0F122001,    // 70003876 
0x0F127260,    // 70003878 
0x0F12F000,    // 7000387A 
0x0F12FA93,    // 7000387C 
0x0F128820,    // 7000387E 
0x0F122802,    // 70003880 
0x0F12D10E,    // 70003882 
0x0F128860,    // 70003884 
0x0F122800,    // 70003886 
0x0F12D10B,    // 70003888 
0x0F1288E0,    // 7000388A 
0x0F122800,    // 7000388C 
0x0F12D108,    // 7000388E 
0x0F128F28,    // 70003890 
0x0F122800,    // 70003892 
0x0F12D001,    // 70003894 
0x0F1280E6,    // 70003896 
0x0F1280A7,    // 70003898 
0x0F122001,    // 7000389A 
0x0F127260,    // 7000389C 
0x0F12F000,    // 7000389E 
0x0F12FA81,    // 700038A0 
0x0F1288E0,    // 700038A2 
0x0F122800,    // 700038A4 
0x0F12D006,    // 700038A6 
0x0F121FC1,    // 700038A8 
0x0F1239FD,    // 700038AA 
0x0F12D003,    // 700038AC 
0x0F122001,    // 700038AE 
0x0F12BCF8,    // 700038B0 
0x0F12BC08,    // 700038B2 
0x0F124718,    // 700038B4 
0x0F122000,    // 700038B6 
0x0F12E7FA,    // 700038B8 
0x0F12B570,    // 700038BA 
0x0F124C7D,    // 700038BC 
0x0F128860,    // 700038BE 
0x0F122800,    // 700038C0 
0x0F12D00C,    // 700038C2 
0x0F128820,    // 700038C4 
0x0F124D74,    // 700038C6 
0x0F122800,    // 700038C8 
0x0F12D009,    // 700038CA 
0x0F120029,    // 700038CC 
0x0F1231A0,    // 700038CE 
0x0F127AC9,    // 700038D0 
0x0F122900,    // 700038D2 
0x0F12D004,    // 700038D4 
0x0F127AA8,    // 700038D6 
0x0F122180,    // 700038D8 
0x0F124308,    // 700038DA 
0x0F1272A8,    // 700038DC 
0x0F12E6D1,    // 700038DE 
0x0F122800,    // 700038E0 
0x0F12D003,    // 700038E2 
0x0F12F7FF,    // 700038E4 
0x0F12FFAF,    // 700038E6 
0x0F122800,    // 700038E8 
0x0F12D1F8,    // 700038EA 
0x0F122000,    // 700038EC 
0x0F128060,    // 700038EE 
0x0F128820,    // 700038F0 
0x0F122800,    // 700038F2 
0x0F12D003,    // 700038F4 
0x0F122008,    // 700038F6 
0x0F12F000,    // 700038F8 
0x0F12FA5C,    // 700038FA 
0x0F12E00B,    // 700038FC 
0x0F12486D,    // 700038FE 
0x0F123020,    // 70003900 
0x0F128880,    // 70003902 
0x0F122800,    // 70003904 
0x0F12D103,    // 70003906 
0x0F127AA8,    // 70003908 
0x0F122101,    // 7000390A 
0x0F124308,    // 7000390C 
0x0F1272A8,    // 7000390E 
0x0F122010,    // 70003910 
0x0F12F000,    // 70003912 
0x0F12FA4F,    // 70003914 
0x0F128820,    // 70003916 
0x0F122800,    // 70003918 
0x0F12D1E0,    // 7000391A 
0x0F124856,    // 7000391C 
0x0F1289C0,    // 7000391E 
0x0F122801,    // 70003920 
0x0F12D1DC,    // 70003922 
0x0F127AA8,    // 70003924 
0x0F1221BF,    // 70003926 
0x0F124008,    // 70003928 
0x0F1272A8,    // 7000392A 
0x0F12E6AA,    // 7000392C 
0x0F126800,    // 7000392E 
0x0F124961,    // 70003930 
0x0F128188,    // 70003932 
0x0F124861,    // 70003934 
0x0F122201,    // 70003936 
0x0F128981,    // 70003938 
0x0F124861,    // 7000393A 
0x0F120252,    // 7000393C 
0x0F124291,    // 7000393E 
0x0F12D902,    // 70003940 
0x0F122102,    // 70003942 
0x0F128181,    // 70003944 
0x0F124770,    // 70003946 
0x0F122101,    // 70003948 
0x0F128181,    // 7000394A 
0x0F124770,    // 7000394C 
0x0F12B5F1,    // 7000394E 
0x0F124E51,    // 70003950 
0x0F128834,    // 70003952 
0x0F122C00,    // 70003954 
0x0F12D03C,    // 70003956 
0x0F122001,    // 70003958 
0x0F122C08,    // 7000395A 
0x0F12D000,    // 7000395C 
0x0F122000,    // 7000395E 
0x0F1270B0,    // 70003960 
0x0F124D50,    // 70003962 
0x0F122700,    // 70003964 
0x0F122800,    // 70003966 
0x0F12D009,    // 70003968 
0x0F12F000,    // 7000396A 
0x0F12FA2B,    // 7000396C 
0x0F120028,    // 7000396E 
0x0F1238F0,    // 70003970 
0x0F126328,    // 70003972 
0x0F127AB0,    // 70003974 
0x0F12217E,    // 70003976 
0x0F124008,    // 70003978 
0x0F1272B0,    // 7000397A 
0x0F12E00C,    // 7000397C 
0x0F12484C,    // 7000397E 
0x0F128F00,    // 70003980 
0x0F122800,    // 70003982 
0x0F12D003,    // 70003984 
0x0F12F000,    // 70003986 
0x0F12FA25,    // 70003988 
0x0F124849,    // 7000398A 
0x0F128707,    // 7000398C 
0x0F122000,    // 7000398E 
0x0F12F000,    // 70003990 
0x0F12FA28,    // 70003992 
0x0F12484B,    // 70003994 
0x0F126328,    // 70003996 
0x0F1278B1,    // 70003998 
0x0F120038,    // 7000399A 
0x0F122900,    // 7000399C 
0x0F12D008,    // 7000399E 
0x0F124944,    // 700039A0 
0x0F123920,    // 700039A2 
0x0F128ACA,    // 700039A4 
0x0F122A00,    // 700039A6 
0x0F12D003,    // 700039A8 
0x0F128B09,    // 700039AA 
0x0F122900,    // 700039AC 
0x0F12D000,    // 700039AE 
0x0F122001,    // 700039B0 
0x0F127170,    // 700039B2 
0x0F122C02,    // 700039B4 
0x0F12D102,    // 700039B6 
0x0F12483A,    // 700039B8 
0x0F123860,    // 700039BA 
0x0F126328,    // 700039BC 
0x0F122201,    // 700039BE 
0x0F122C02,    // 700039C0 
0x0F12D000,    // 700039C2 
0x0F122200,    // 700039C4 
0x0F124834,    // 700039C6 
0x0F122110,    // 700039C8 
0x0F12300A,    // 700039CA 
0x0F12F000,    // 700039CC 
0x0F12FA12,    // 700039CE 
0x0F128037,    // 700039D0 
0x0F129900,    // 700039D2 
0x0F120020,    // 700039D4 
0x0F12600C,    // 700039D6 
0x0F12E76A,    // 700039D8 
0x0F12B538,    // 700039DA 
0x0F124837,    // 700039DC 
0x0F124669,    // 700039DE 
0x0F123848,    // 700039E0 
0x0F12F000,    // 700039E2 
0x0F12FA0F,    // 700039E4 
0x0F124A32,    // 700039E6 
0x0F124834,    // 700039E8 
0x0F128F51,    // 700039EA 
0x0F122400,    // 700039EC 
0x0F123020,    // 700039EE 
0x0F122900,    // 700039F0 
0x0F12D00A,    // 700039F2 
0x0F128754,    // 700039F4 
0x0F126941,    // 700039F6 
0x0F126451,    // 700039F8 
0x0F126491,    // 700039FA 
0x0F12466B,    // 700039FC 
0x0F128819,    // 700039FE 
0x0F1287D1,    // 70003A00 
0x0F12885B,    // 70003A02 
0x0F120011,    // 70003A04 
0x0F123140,    // 70003A06 
0x0F12800B,    // 70003A08 
0x0F128F91,    // 70003A0A 
0x0F122900,    // 70003A0C 
0x0F12D002,    // 70003A0E 
0x0F128794,    // 70003A10 
0x0F126940,    // 70003A12 
0x0F126490,    // 70003A14 
0x0F12F000,    // 70003A16 
0x0F12F9FD,    // 70003A18 
0x0F12BC38,    // 70003A1A 
0x0F12BC08,    // 70003A1C 
0x0F124718,    // 70003A1E 
0x0F12B5F8,    // 70003A20 
0x0F124C29,    // 70003A22 
0x0F1289E0,    // 70003A24 
0x0F12F000,    // 70003A26 
0x0F12F9FD,    // 70003A28 
0x0F120006,    // 70003A2A 
0x0F128A20,    // 70003A2C 
0x0F12F000,    // 70003A2E 
0x0F12FA01,    // 70003A30 
0x0F120007,    // 70003A32 
0x0F124821,    // 70003A34 
0x0F124D1E,    // 70003A36 
0x0F123020,    // 70003A38 
0x0F126CA9,    // 70003A3A 
0x0F126940,    // 70003A3C 
0x0F121809,    // 70003A3E 
0x0F120200,    // 70003A40 
0x0F12F000,    // 70003A42 
0x0F12F9FF,    // 70003A44 
0x0F120400,    // 70003A46 
0x0F120C00,    // 70003A48 
0x0F12002A,    // 70003A4A 
0x0F12326E,    // 70003A4C 
0x0F120011,    // 70003A4E 
0x0F12390A,    // 70003A50 
0x0F122305,    // 70003A52 
0x0F12F000,    // 70003A54 
0x0F12F9FC,    // 70003A56 
0x0F124C14,    // 70003A58 
0x0F1261A0,    // 70003A5A 
0x0F128FEB,    // 70003A5C 
0x0F120002,    // 70003A5E 
0x0F120031,    // 70003A60 
0x0F120018,    // 70003A62 
0x0F12F000,    // 70003A64 
0x0F12F9FC,    // 70003A66 
0x0F12466B,    // 70003A68 
0x0F120005,    // 70003A6A 
0x0F128018,    // 70003A6C 
0x0F12E02D,    // 70003A6E 
0x0F123290,    // 70003A70 
0x0F127000,    // 70003A72 
0x0F123294,    // 70003A74 
0x0F127000,    // 70003A76 
0x0F1204A8,    // 70003A78 
0x0F127000,    // 70003A7A 
0x0F1215DC,    // 70003A7C 
0x0F127000,    // 70003A7E 
0x0F125000,    // 70003A80 
0x0F12D000,    // 70003A82 
0x0F12064C,    // 70003A84 
0x0F127000,    // 70003A86 
0x0F12A000,    // 70003A88 
0x0F12D000,    // 70003A8A 
0x0F122468,    // 70003A8C 
0x0F127000,    // 70003A8E 
0x0F1211DC,    // 70003A90 
0x0F127000,    // 70003A92 
0x0F122828,    // 70003A94 
0x0F127000,    // 70003A96 
0x0F121E84,    // 70003A98 
0x0F127000,    // 70003A9A 
0x0F121BE4,    // 70003A9C 
0x0F127000,    // 70003A9E 
0x0F122EA8,    // 70003AA0 
0x0F127000,    // 70003AA2 
0x0F1221A4,    // 70003AA4 
0x0F127000,    // 70003AA6 
0x0F120100,    // 70003AA8 
0x0F127000,    // 70003AAA 
0x0F1231A0,    // 70003AAC 
0x0F127000,    // 70003AAE 
0x0F123F48,    // 70003AB0 
0x0F127000,    // 70003AB2 
0x0F1201E8,    // 70003AB4 
0x0F127000,    // 70003AB6 
0x0F12F2A0,    // 70003AB8 
0x0F12D000,    // 70003ABA 
0x0F122A44,    // 70003ABC 
0x0F127000,    // 70003ABE 
0x0F12F400,    // 70003AC0 
0x0F12D000,    // 70003AC2 
0x0F122024,    // 70003AC4 
0x0F127000,    // 70003AC6 
0x0F121650,    // 70003AC8 
0x0F127000,    // 70003ACA 
0x0F124888,    // 70003ACC 
0x0F1269A2,    // 70003ACE 
0x0F128800,    // 70003AD0 
0x0F120039,    // 70003AD2 
0x0F12F000,    // 70003AD4 
0x0F12F9C4,    // 70003AD6 
0x0F12466B,    // 70003AD8 
0x0F120006,    // 70003ADA 
0x0F128058,    // 70003ADC 
0x0F120021,    // 70003ADE 
0x0F129800,    // 70003AE0 
0x0F12311C,    // 70003AE2 
0x0F12F000,    // 70003AE4 
0x0F12F9C4,    // 70003AE6 
0x0F124981,    // 70003AE8 
0x0F123140,    // 70003AEA 
0x0F12808D,    // 70003AEC 
0x0F1280CE,    // 70003AEE 
0x0F128BA1,    // 70003AF0 
0x0F124880,    // 70003AF2 
0x0F128001,    // 70003AF4 
0x0F128BE1,    // 70003AF6 
0x0F128041,    // 70003AF8 
0x0F128C21,    // 70003AFA 
0x0F128081,    // 70003AFC 
0x0F12E6D7,    // 70003AFE 
0x0F12B5F8,    // 70003B00 
0x0F124E7B,    // 70003B02 
0x0F123E40,    // 70003B04 
0x0F126C70,    // 70003B06 
0x0F126CB1,    // 70003B08 
0x0F120200,    // 70003B0A 
0x0F12F000,    // 70003B0C 
0x0F12F99A,    // 70003B0E 
0x0F120400,    // 70003B10 
0x0F120C00,    // 70003B12 
0x0F122401,    // 70003B14 
0x0F120364,    // 70003B16 
0x0F1242A0,    // 70003B18 
0x0F12D200,    // 70003B1A 
0x0F120004,    // 70003B1C 
0x0F124A74,    // 70003B1E 
0x0F120020,    // 70003B20 
0x0F12323E,    // 70003B22 
0x0F121F91,    // 70003B24 
0x0F122303,    // 70003B26 
0x0F12F000,    // 70003B28 
0x0F12F992,    // 70003B2A 
0x0F120405,    // 70003B2C 
0x0F120C2D,    // 70003B2E 
0x0F124A6F,    // 70003B30 
0x0F120020,    // 70003B32 
0x0F12321A,    // 70003B34 
0x0F120011,    // 70003B36 
0x0F12390A,    // 70003B38 
0x0F122305,    // 70003B3A 
0x0F12F000,    // 70003B3C 
0x0F12F988,    // 70003B3E 
0x0F12496B,    // 70003B40 
0x0F123940,    // 70003B42 
0x0F1264C8,    // 70003B44 
0x0F12496C,    // 70003B46 
0x0F124E6A,    // 70003B48 
0x0F1288C8,    // 70003B4A 
0x0F122701,    // 70003B4C 
0x0F123620,    // 70003B4E 
0x0F122800,    // 70003B50 
0x0F12D009,    // 70003B52 
0x0F124C69,    // 70003B54 
0x0F1238FF,    // 70003B56 
0x0F121E40,    // 70003B58 
0x0F12D00A,    // 70003B5A 
0x0F122804,    // 70003B5C 
0x0F12D01D,    // 70003B5E 
0x0F122806,    // 70003B60 
0x0F12D101,    // 70003B62 
0x0F122000,    // 70003B64 
0x0F1280C8,    // 70003B66 
0x0F1282B7,    // 70003B68 
0x0F122001,    // 70003B6A 
0x0F12F000,    // 70003B6C 
0x0F12F988,    // 70003B6E 
0x0F12E69E,    // 70003B70 
0x0F12000D,    // 70003B72 
0x0F12724F,    // 70003B74 
0x0F122001,    // 70003B76 
0x0F12F000,    // 70003B78 
0x0F12F98A,    // 70003B7A 
0x0F12F000,    // 70003B7C 
0x0F12F990,    // 70003B7E 
0x0F12485B,    // 70003B80 
0x0F123840,    // 70003B82 
0x0F126C81,    // 70003B84 
0x0F126CC0,    // 70003B86 
0x0F124341,    // 70003B88 
0x0F120A08,    // 70003B8A 
0x0F126160,    // 70003B8C 
0x0F1220FF,    // 70003B8E 
0x0F121D40,    // 70003B90 
0x0F1280E8,    // 70003B92 
0x0F124858,    // 70003B94 
0x0F123040,    // 70003B96 
0x0F127707,    // 70003B98 
0x0F12E7E5,    // 70003B9A 
0x0F124856,    // 70003B9C 
0x0F127247,    // 70003B9E 
0x0F1221FF,    // 70003BA0 
0x0F121DC9,    // 70003BA2 
0x0F1280C1,    // 70003BA4 
0x0F12F000,    // 70003BA6 
0x0F12F983,    // 70003BA8 
0x0F124952,    // 70003BAA 
0x0F123940,    // 70003BAC 
0x0F122800,    // 70003BAE 
0x0F12D007,    // 70003BB0 
0x0F12684A,    // 70003BB2 
0x0F120001,    // 70003BB4 
0x0F12436A,    // 70003BB6 
0x0F120010,    // 70003BB8 
0x0F12F000,    // 70003BBA 
0x0F12F943,    // 70003BBC 
0x0F126160,    // 70003BBE 
0x0F12E002,    // 70003BC0 
0x0F126848,    // 70003BC2 
0x0F124368,    // 70003BC4 
0x0F126160,    // 70003BC6 
0x0F128BF0,    // 70003BC8 
0x0F122800,    // 70003BCA 
0x0F12D001,    // 70003BCC 
0x0F12F7FF,    // 70003BCE 
0x0F12FF27,    // 70003BD0 
0x0F122000,    // 70003BD2 
0x0F12F000,    // 70003BD4 
0x0F12F95C,    // 70003BD6 
0x0F124947,    // 70003BD8 
0x0F1220FF,    // 70003BDA 
0x0F121DC0,    // 70003BDC 
0x0F1280C8,    // 70003BDE 
0x0F12E7C2,    // 70003BE0 
0x0F12B5F8,    // 70003BE2 
0x0F122400,    // 70003BE4 
0x0F124D46,    // 70003BE6 
0x0F124846,    // 70003BE8 
0x0F12210E,    // 70003BEA 
0x0F128041,    // 70003BEC 
0x0F122101,    // 70003BEE 
0x0F128001,    // 70003BF0 
0x0F12F000,    // 70003BF2 
0x0F12F965,    // 70003BF4 
0x0F124844,    // 70003BF6 
0x0F128840,    // 70003BF8 
0x0F12F000,    // 70003BFA 
0x0F12F89D,    // 70003BFC 
0x0F124E3C,    // 70003BFE 
0x0F12270D,    // 70003C00 
0x0F12073F,    // 70003C02 
0x0F123E40,    // 70003C04 
0x0F1219E8,    // 70003C06 
0x0F128803,    // 70003C08 
0x0F1200E2,    // 70003C0A 
0x0F121991,    // 70003C0C 
0x0F12804B,    // 70003C0E 
0x0F128843,    // 70003C10 
0x0F1252B3,    // 70003C12 
0x0F128882,    // 70003C14 
0x0F1280CA,    // 70003C16 
0x0F1288C0,    // 70003C18 
0x0F128088,    // 70003C1A 
0x0F123508,    // 70003C1C 
0x0F12042D,    // 70003C1E 
0x0F120C2D,    // 70003C20 
0x0F121C64,    // 70003C22 
0x0F120424,    // 70003C24 
0x0F120C24,    // 70003C26 
0x0F122C07,    // 70003C28 
0x0F12D3EC,    // 70003C2A 
0x0F12E640,    // 70003C2C 
0x0F12B5F0,    // 70003C2E 
0x0F12B085,    // 70003C30 
0x0F126801,    // 70003C32 
0x0F129103,    // 70003C34 
0x0F126881,    // 70003C36 
0x0F12040A,    // 70003C38 
0x0F120C12,    // 70003C3A 
0x0F124933,    // 70003C3C 
0x0F128B89,    // 70003C3E 
0x0F122900,    // 70003C40 
0x0F12D001,    // 70003C42 
0x0F120011,    // 70003C44 
0x0F12E000,    // 70003C46 
0x0F122100,    // 70003C48 
0x0F129102,    // 70003C4A 
0x0F126840,    // 70003C4C 
0x0F120401,    // 70003C4E 
0x0F129803,    // 70003C50 
0x0F120C09,    // 70003C52 
0x0F12F000,    // 70003C54 
0x0F12F93C,    // 70003C56 
0x0F124825,    // 70003C58 
0x0F123040,    // 70003C5A 
0x0F128900,    // 70003C5C 
0x0F122800,    // 70003C5E 
0x0F12D03B,    // 70003C60 
0x0F122100,    // 70003C62 
0x0F124825,    // 70003C64 
0x0F124D2A,    // 70003C66 
0x0F1230C0,    // 70003C68 
0x0F124684,    // 70003C6A 
0x0F124B29,    // 70003C6C 
0x0F124C20,    // 70003C6E 
0x0F1288DA,    // 70003C70 
0x0F123C40,    // 70003C72 
0x0F120048,    // 70003C74 
0x0F1200D7,    // 70003C76 
0x0F12193E,    // 70003C78 
0x0F12197F,    // 70003C7A 
0x0F12183F,    // 70003C7C 
0x0F125A36,    // 70003C7E 
0x0F128AFF,    // 70003C80 
0x0F12437E,    // 70003C82 
0x0F1200B6,    // 70003C84 
0x0F120C37,    // 70003C86 
0x0F121906,    // 70003C88 
0x0F123680,    // 70003C8A 
0x0F128177,    // 70003C8C 
0x0F121C52,    // 70003C8E 
0x0F1200D2,    // 70003C90 
0x0F121914,    // 70003C92 
0x0F121952,    // 70003C94 
0x0F121812,    // 70003C96 
0x0F125A24,    // 70003C98 
0x0F128AD2,    // 70003C9A 
0x0F124354,    // 70003C9C 
0x0F1200A2,    // 70003C9E 
0x0F120C12,    // 70003CA0 
0x0F128272,    // 70003CA2 
0x0F12891C,    // 70003CA4 
0x0F12895B,    // 70003CA6 
0x0F124367,    // 70003CA8 
0x0F12435A,    // 70003CAA 
0x0F121943,    // 70003CAC 
0x0F123340,    // 70003CAE 
0x0F1289DB,    // 70003CB0 
0x0F129C02,    // 70003CB2 
0x0F1218BA,    // 70003CB4 
0x0F124363,    // 70003CB6 
0x0F1218D2,    // 70003CB8 
0x0F120212,    // 70003CBA 
0x0F120C12,    // 70003CBC 
0x0F12466B,    // 70003CBE 
0x0F12521A,    // 70003CC0 
0x0F124663,    // 70003CC2 
0x0F127DDB,    // 70003CC4 
0x0F12435A,    // 70003CC6 
0x0F129B03,    // 70003CC8 
0x0F120252,    // 70003CCA 
0x0F120C12,    // 70003CCC 
0x0F12521A,    // 70003CCE 
0x0F121C49,    // 70003CD0 
0x0F120409,    // 70003CD2 
0x0F120C09,    // 70003CD4 
0x0F122904,    // 70003CD6 
0x0F12D3C8,    // 70003CD8 
0x0F12B005,    // 70003CDA 
0x0F12BCF0,    // 70003CDC 
0x0F12BC08,    // 70003CDE 
0x0F124718,    // 70003CE0 
0x0F12B510,    // 70003CE2 
0x0F12F7FF,    // 70003CE4 
0x0F12FF7D,    // 70003CE6 
0x0F12F000,    // 70003CE8 
0x0F12F8FA,    // 70003CEA 
0x0F12E502,    // 70003CEC 
0x0F120000,    // 70003CEE 
0x0F123F88,    // 70003CF0 
0x0F127000,    // 70003CF2 
0x0F122A24,    // 70003CF4 
0x0F127000,    // 70003CF6 
0x0F1231A0,    // 70003CF8 
0x0F127000,    // 70003CFA 
0x0F122A64,    // 70003CFC 
0x0F127000,    // 70003CFE 
0x0F12A006,    // 70003D00 
0x0F120000,    // 70003D02 
0x0F12A000,    // 70003D04 
0x0F12D000,    // 70003D06 
0x0F12064C,    // 70003D08 
0x0F127000,    // 70003D0A 
0x0F1207C4,    // 70003D0C 
0x0F127000,    // 70003D0E 
0x0F1207E8,    // 70003D10 
0x0F127000,    // 70003D12 
0x0F121FA0,    // 70003D14 
0x0F127000,    // 70003D16 
0x0F124778,    // 70003D18 
0x0F1246C0,    // 70003D1A 
0x0F12C000,    // 70003D1C 
0x0F12E59F,    // 70003D1E 
0x0F12FF1C,    // 70003D20 
0x0F12E12F,    // 70003D22 
0x0F121F63,    // 70003D24 
0x0F120001,    // 70003D26 
0x0F124778,    // 70003D28 
0x0F1246C0,    // 70003D2A 
0x0F12C000,    // 70003D2C 
0x0F12E59F,    // 70003D2E 
0x0F12FF1C,    // 70003D30 
0x0F12E12F,    // 70003D32 
0x0F121EDF,    // 70003D34 
0x0F120001,    // 70003D36 
0x0F124778,    // 70003D38 
0x0F1246C0,    // 70003D3A 
0x0F12C000,    // 70003D3C 
0x0F12E59F,    // 70003D3E 
0x0F12FF1C,    // 70003D40 
0x0F12E12F,    // 70003D42 
0x0F12FDAF,    // 70003D44 
0x0F120000,    // 70003D46 
0x0F124778,    // 70003D48 
0x0F1246C0,    // 70003D4A 
0x0F12F004,    // 70003D4C 
0x0F12E51F,    // 70003D4E 
0x0F122328,    // 70003D50 
0x0F120001,    // 70003D52 
0x0F124778,    // 70003D54 
0x0F1246C0,    // 70003D56 
0x0F12C000,    // 70003D58 
0x0F12E59F,    // 70003D5A 
0x0F12FF1C,    // 70003D5C 
0x0F12E12F,    // 70003D5E 
0x0F129E89,    // 70003D60 
0x0F120000,    // 70003D62 
0x0F124778,    // 70003D64 
0x0F1246C0,    // 70003D66 
0x0F12C000,    // 70003D68 
0x0F12E59F,    // 70003D6A 
0x0F12FF1C,    // 70003D6C 
0x0F12E12F,    // 70003D6E 
0x0F12495F,    // 70003D70 
0x0F120000,    // 70003D72 
0x0F124778,    // 70003D74 
0x0F1246C0,    // 70003D76 
0x0F12C000,    // 70003D78 
0x0F12E59F,    // 70003D7A 
0x0F12FF1C,    // 70003D7C 
0x0F12E12F,    // 70003D7E 
0x0F12E403,    // 70003D80 
0x0F120000,    // 70003D82 
0x0F124778,    // 70003D84 
0x0F1246C0,    // 70003D86 
0x0F12C000,    // 70003D88 
0x0F12E59F,    // 70003D8A 
0x0F12FF1C,    // 70003D8C 
0x0F12E12F,    // 70003D8E 
0x0F1224B3,    // 70003D90 
0x0F120001,    // 70003D92 
0x0F124778,    // 70003D94 
0x0F1246C0,    // 70003D96 
0x0F12C000,    // 70003D98 
0x0F12E59F,    // 70003D9A 
0x0F12FF1C,    // 70003D9C 
0x0F12E12F,    // 70003D9E 
0x0F12EECD,    // 70003DA0 
0x0F120000,    // 70003DA2 
0x0F124778,    // 70003DA4 
0x0F1246C0,    // 70003DA6 
0x0F12C000,    // 70003DA8 
0x0F12E59F,    // 70003DAA 
0x0F12FF1C,    // 70003DAC 
0x0F12E12F,    // 70003DAE 
0x0F12F049,    // 70003DB0 
0x0F120000,    // 70003DB2 
0x0F124778,    // 70003DB4 
0x0F1246C0,    // 70003DB6 
0x0F12C000,    // 70003DB8 
0x0F12E59F,    // 70003DBA 
0x0F12FF1C,    // 70003DBC 
0x0F12E12F,    // 70003DBE 
0x0F1212DF,    // 70003DC0 
0x0F120000,    // 70003DC2 
0x0F124778,    // 70003DC4 
0x0F1246C0,    // 70003DC6 
0x0F12C000,    // 70003DC8 
0x0F12E59F,    // 70003DCA 
0x0F12FF1C,    // 70003DCC 
0x0F12E12F,    // 70003DCE 
0x0F12F05B,    // 70003DD0 
0x0F120000,    // 70003DD2 
0x0F124778,    // 70003DD4 
0x0F1246C0,    // 70003DD6 
0x0F12C000,    // 70003DD8 
0x0F12E59F,    // 70003DDA 
0x0F12FF1C,    // 70003DDC 
0x0F12E12F,    // 70003DDE 
0x0F12F07B,    // 70003DE0 
0x0F120000,    // 70003DE2 
0x0F124778,    // 70003DE4 
0x0F1246C0,    // 70003DE6 
0x0F12C000,    // 70003DE8 
0x0F12E59F,    // 70003DEA 
0x0F12FF1C,    // 70003DEC 
0x0F12E12F,    // 70003DEE 
0x0F12FE6D,    // 70003DF0 
0x0F120000,    // 70003DF2 
0x0F124778,    // 70003DF4 
0x0F1246C0,    // 70003DF6 
0x0F12C000,    // 70003DF8 
0x0F12E59F,    // 70003DFA 
0x0F12FF1C,    // 70003DFC 
0x0F12E12F,    // 70003DFE 
0x0F123295,    // 70003E00 
0x0F120000,    // 70003E02 
0x0F124778,    // 70003E04 
0x0F1246C0,    // 70003E06 
0x0F12C000,    // 70003E08 
0x0F12E59F,    // 70003E0A 
0x0F12FF1C,    // 70003E0C 
0x0F12E12F,    // 70003E0E 
0x0F12234F,    // 70003E10 
0x0F120000,    // 70003E12 
0x0F124778,    // 70003E14 
0x0F1246C0,    // 70003E16 
0x0F12C000,    // 70003E18 
0x0F12E59F,    // 70003E1A 
0x0F12FF1C,    // 70003E1C 
0x0F12E12F,    // 70003E1E 
0x0F124521,    // 70003E20 
0x0F120000,    // 70003E22 
0x0F124778,    // 70003E24 
0x0F1246C0,    // 70003E26 
0x0F12C000,    // 70003E28 
0x0F12E59F,    // 70003E2A 
0x0F12FF1C,    // 70003E2C 
0x0F12E12F,    // 70003E2E 
0x0F127C0D,    // 70003E30 
0x0F120000,    // 70003E32 
0x0F124778,    // 70003E34 
0x0F1246C0,    // 70003E36 
0x0F12C000,    // 70003E38 
0x0F12E59F,    // 70003E3A 
0x0F12FF1C,    // 70003E3C 
0x0F12E12F,    // 70003E3E 
0x0F127C2B,    // 70003E40 
0x0F120000,    // 70003E42 
0x0F124778,    // 70003E44 
0x0F1246C0,    // 70003E46 
0x0F12F004,    // 70003E48 
0x0F12E51F,    // 70003E4A 
0x0F1224C4,    // 70003E4C 
0x0F120001,    // 70003E4E 
0x0F124778,    // 70003E50 
0x0F1246C0,    // 70003E52 
0x0F12C000,    // 70003E54 
0x0F12E59F,    // 70003E56 
0x0F12FF1C,    // 70003E58 
0x0F12E12F,    // 70003E5A 
0x0F123183,    // 70003E5C 
0x0F120000,    // 70003E5E 
0x0F124778,    // 70003E60 
0x0F1246C0,    // 70003E62 
0x0F12C000,    // 70003E64 
0x0F12E59F,    // 70003E66 
0x0F12FF1C,    // 70003E68 
0x0F12E12F,    // 70003E6A 
0x0F12302F,    // 70003E6C 
0x0F120000,    // 70003E6E 
0x0F124778,    // 70003E70 
0x0F1246C0,    // 70003E72 
0x0F12C000,    // 70003E74 
0x0F12E59F,    // 70003E76 
0x0F12FF1C,    // 70003E78 
0x0F12E12F,    // 70003E7A 
0x0F12EF07,    // 70003E7C 
0x0F120000,    // 70003E7E 
0x0F124778,    // 70003E80 
0x0F1246C0,    // 70003E82 
0x0F12C000,    // 70003E84 
0x0F12E59F,    // 70003E86 
0x0F12FF1C,    // 70003E88 
0x0F12E12F,    // 70003E8A 
0x0F1248FB,    // 70003E8C 
0x0F120000,    // 70003E8E 
0x0F124778,    // 70003E90 
0x0F1246C0,    // 70003E92 
0x0F12C000,    // 70003E94 
0x0F12E59F,    // 70003E96 
0x0F12FF1C,    // 70003E98 
0x0F12E12F,    // 70003E9A 
0x0F12F0B1,    // 70003E9C 
0x0F120000,    // 70003E9E 
0x0F124778,    // 70003EA0 
0x0F1246C0,    // 70003EA2 
0x0F12C000,    // 70003EA4 
0x0F12E59F,    // 70003EA6 
0x0F12FF1C,    // 70003EA8 
0x0F12E12F,    // 70003EAA 
0x0F12EEDF,    // 70003EAC 
0x0F120000,    // 70003EAE 
0x0F124778,    // 70003EB0 
0x0F1246C0,    // 70003EB2 
0x0F12C000,    // 70003EB4 
0x0F12E59F,    // 70003EB6 
0x0F12FF1C,    // 70003EB8 
0x0F12E12F,    // 70003EBA 
0x0F12AEF1,    // 70003EBC 
0x0F120000,    // 70003EBE 
0x0F124778,    // 70003EC0 
0x0F1246C0,    // 70003EC2 
0x0F12C000,    // 70003EC4 
0x0F12E59F,    // 70003EC6 
0x0F12FF1C,    // 70003EC8 
0x0F12E12F,    // 70003ECA 
0x0F12FD21,    // 70003ECC 
0x0F120000,    // 70003ECE 
0x0F124778,    // 70003ED0 
0x0F1246C0,    // 70003ED2 
0x0F12C000,    // 70003ED4 
0x0F12E59F,    // 70003ED6 
0x0F12FF1C,    // 70003ED8 
0x0F12E12F,    // 70003EDA 
0x0F125027,    // 70003EDC 
0x0F120000,    // 70003EDE 
0x0F124778,    // 70003EE0 
0x0F1246C0,    // 70003EE2 
0x0F12C000,    // 70003EE4 
0x0F12E59F,    // 70003EE6 
0x0F12FF1C,    // 70003EE8 
0x0F12E12F,    // 70003EEA 
0x0F1204C9,    // 70003EEC 
0x0F120000,    // 70003EEE 
// End of Patch Data(Last : 70003EEEh)
// Total Size 2500 (0x09C4)
// Addr : 352C , Size : 2498(9C2h) 
0x10000001,

//TNP_USER_MBCV_CONTROL	
//TNP_AWB_MODUL_COMP	
//TNP_SINGLE_FRAME_CAPTURE	// "M_REG_ELF_Reserved3"  register was used for Option register					
//TNP_CAPTURE_DONE_INFO		// "Mon_DBG_GenInfo[15]" register was used for Capture Infor
//TNP_5CC_SENSOR_TUNE	

0x0028D000,
0x002A0070,
0x0F120007,// clks_src_gf_force_enable

//driving current chnage 4mA -> 8mA
0x0028D000,                           
0x002A1082,
0x0F1203FF, // 
0x0F1203FF, //
0x002A1088,
0x0F120FFF, 

//MBCV Control
0x00287000,
0x002A04B4,
0x0F120064,

// AFIT by Normalized Brightness Tuning parameter
0x00287000,
0x002A3302,
0x0F120000,	// on/off AFIT by NB option

0x0F120005,	// NormBR[0]
0x0F120019,	// NormBR[1]
0x0F120130,	// NormBR[2]
0x0F120360,	// NormBR[3]
0x0F120460,	// NormBR[4]

///////////////////////////////////////////////////////////////////////////////////
// Analog & APS settings///////////////////////////////////////////////////////////
// This register is for FACTORY ONLY. If you change it without prior notification//
// YOU are RESPONSIBLE for the FAILURE that will happen in the future//////////////
///////////////////////////////////////////////////////////////////////////////////

//========================================================================================
// 5CC EVT1 analog register setting
// '10.07.14. Initial Draft
// '10.07.24. 0xE404=0000 -> 1FC0 (Depedestal 0 -> -64d)
// '10.08.16. 0xF410=0001 -> 0000 (for SHBN)
// '10.08.25. 0xF438=0020 -> 0002 (VTGSL=2.96V) by APS
//            0xF43A=0020 -> 0001 (VRG=2.83V) by APS
// '10.09.28. 0xF402=1F02 -> 3F02 ([13]: pixel bias powerdown according to HADR) for Darkshading
//		    0xF416=0000 -> 0001 (AAC_EN enable) for Darkshading
// '10.11.15. 0xF40C=0002@>=AGx2, 0001@<AGx2 by T&P (Updated comment only)
// '10.12.07. 0xE412=0000 -> 0008 (for HFPN, by APS)
//		    0xE414=0000 -> 0008 (for HFPN, by APS)
//		    0xE416=0000 -> 0010 (for HFPN, by APS)
//		    0xE418=0000 -> 0010 (for HFPN, by APS)
//========================================================================================

//============================= Analog & APS Control =====================================
0x0028D000,
0x002AF2AC,
0x0F120100,	// analog gain; 0200 x16, 0100 x8, 0080 x4, 0040 x2, 0020 x1
0x002AF400,
0x0F12001D,	// ldb_en[4], ld_en[3], clp_en[2](N/A), smp_en[1], dshut_en[0]
0x0F123F02,	// cds_test[15:0]; refer to the ATOP_TEST_INFORMATION.

0x002AF40A,
0x0F120054,	// adc_sat[7:0]=84d (500mV)
0x0F120002,	// ms[2:0]; 2h@Normal, 2h@PLA, 1h@CNT.AVG
0x0F120008,	// rmp_option[7:0]; [3]SL_Low_PWR_SAVE On
0x0F120000,	// msoff_en; No MS if gain gain is lower than x2
0x0F1200A4,	// rmp_init[7:0]

0x002AF416,
0x0F120001,	// dbs_option[11:4], dbs_mode[3:2], dbs_bist_en[1], aac_en[0]

0x002AF41E,
0x0F120065,	// comp2_bias[7:4], comp1_bias[3:0]

0x002AF422,
0x0F120005,	// pix_bias[3:0]

0x002AF426,
0x0F1200D4,	// clp_lvl[7:0]

0x002AF42A,
0x0F120001,	// ref_option[7:0]; [4]OB_PIX monit en, [3]Clamp monit en, [2]Monit amp en, [1]Clamp power-down, [0]CDS power-down during SL=low

0x002AF42E,
0x0F120406,	// fb_lv[11:10], pd_fblv[9], capa_ctrl_en[8], pd_inrush_ctrl[7], pd_reg_ntg[6], pd_reg_tgsl[5], pd_reg_rg[4], pd_reg_pix[3], pd_ncp_rosc[2], pd_cp_rosc[1], pd_cp[0]

0x002AF434,
0x0F120003,	// dbr_clk_sel[1:0]; PLL_mode=3h, ROSC_mode=0h
0x0F120004,	// reg_tune_pix[7:0]
0x0F120002,	// reg_tune_tgsl[7:0] (2.96V)
0x0F120001,	// reg_tune_rg[7:0] (2.83V)
0x0F120004,	// reg_tune_ntg[7:0]

0x002AF446,
0x0F120000,	// blst_en_cintr[15:0]

0x002AF466,
0x0F120000,	// srx_en[0]

0x002A0054,
0x0F120028,	// pll_pd[10](0:enable, 1:disable), div_clk_en[0](0:enable, 1:disable)
0x0F128888,	// div_dbr[7:4]

0x002AF132,
0x0F120206,	// tgr_frame_decription 4
0x002AF152,
0x0F120206,	// tgr_frame_decription 7
0x002AF1A2,
0x0F120200,	// tgr_frame_params_descriptor_3
0x002AF1B2,
0x0F120202,	// tgr_frame_params_descriptor_6
//===========================================================================================

//============================= Line-ADLC Tuning ============================================
0x002AE412,
0x0F120008,	// adlc_tune_offset_gr[7:0]
0x0F120008,	// adlc_tune_offset_r[7:0]
0x0F120010,	// adlc_tune_offset_b[7:0]
0x0F120010,	// adlc_tune_offset_gb[7:0]
0x002AE42E,
0x0F120004,	// adlc_qec[2:0]
//===========================================================================================

//===================================================================
// AWB white locus setting - Have to be written after TnP
//===================================================================
0x00287000,
0x002A1014,
0x0F120132,	//0138	//awbb_IntcR
0x0F12010A,	//011C	//awbb_IntcB

//===================================================================
// Flash setting
//===================================================================

0x002A018C,
0x0F120001,	//REG_TC_IPRM_AuxConfig	// bit[0] : Flash is in use, bit[1] : Mechanical shutter is in use // 0 : do not use, 1 : use
0x0F120003,	//REG_TC_IPRM_AuxPolarity	// bit[0] : Flash polarity (1 is active high), bit[1] : Mechanical shutter polarity (1 is active high)
0x0F120003,	//REG_TC_IPRM_AuxGpios	//1-4 : Flash GPIO number, If GPIO number is overaped with AF GPIO, F/W could be stop.

//===================================================================
// 1-H timing setting
//===================================================================
0x002A1686,
0x0F12005C,	//senHal_uAddColsBin
0x0F12005C,	//senHal_uAddColsNoBin
0x0F12085C,	//senHal_uMinColsHorBin
0x0F12005C,	//senHal_uMinColsNoHorBin
0x0F12025A,	//senHal_uMinColsAddAnalogBin

//===================================================================
// Forbidden area setting
//===================================================================
0x002A1844,
0x0F120000,	//senHal_bSRX    //SRX off

0x002A1680,
0x0F120002,	//senHal_NExpLinesCheckFine	//0004    //extend Forbidden area line

//===================================================================
// Preview subsampling mode
//===================================================================
0x002A18F8,
0x0F120001,	//senHal_bAACActiveWait2Start
0x002A18F6,
0x0F120001,	//senHal_bAlwaysAAC
0x002A182C,
0x0F120001,	//senHal_bSenAAC
0x002A0EE4,
0x0F120001,	//setot_bUseDigitalHbin
0x002A1674,
0x0F120002,	//senHal_SenBinFactor	// 2:2x2, 4:4x4
0x0F120002,	//senHal_SamplingType	// 0:Full, 1:digital, 2:PLA, 3:CA
0x0F120000,	//senHal_SamplingMode	// 0:2x2,	1:4x4

//===================================================================
// PLL setting for Max frequency (EVT0.1) 2010.08.05 - Do not remove
//===================================================================
0x002A19AE,
0x0F12EA60,	//pll_uMaxSysFreqKhz
0x0F127530,	//pll_uMaxPVIFreq4KH
0x002A19C2,
0x0F127530,	//pll_uMaxMIPIFreq4KH
0x002A0244,
0x0F127530,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x002A0336,
0x0F127530,	//REG_0TC_CCFG_usMaxOut4KHzRate

//===================================================================
// Init Parameters
//===================================================================
//MCLK
0x002A0188,
0x0F125DC0,	//REG_TC_IPRM_InClockLSBs
0x0F120000,	//REG_TC_IPRM_InClockMSBs
0x002A01B2,
0x0F120002,	//REG_TC_IPRM_UseNPviClocks
0x0F120000,	//REG_TC_IPRM_UseNMipiClocks
0x002A01B8,
0x0F120000,	//REG_TC_IPRM_bBlockInternalPllCalc	//1:pll bypass

//SCLK & PCLK
0x0F1232C8,	//REG_TC_IPRM_OpClk4KHz_0	//52Mhz
0x0F1234BC,	//REG_TC_IPRM_MinOutRate4KHz_0	//54Mhz
0x0F1234BC,	//REG_TC_IPRM_MaxOutRate4KHz_0	//54Mhz

//SCLK & PCLK
0x0F121F40,	//REG_TC_IPRM_OpClk4KHz_1	//32Mhz
0x0F1234BC,	//REG_TC_IPRM_MinOutRate4KHz_1	//54Mhz
0x0F1234BC,	//REG_TC_IPRM_MaxOutRate4KHz_1	//54Mhz

0x002A01CC,
0x0F120001,	//REG_TC_IPRM_InitParamsUpdated

//===================================================================
// Input Width & Height
//===================================================================
0x002A01F6,
0x0F120800,	//REG_TC_GP_PrevReqInputWidth		//Sensor Crop Width	2048
0x0F120600,	//REG_TC_GP_PrevReqInputHeight	//Sensor Crop Height 1536
0x0F120000,	//REG_TC_GP_PrevInputWidthOfs		//Sensor HOffset 0
0x0F120000,	//REG_TC_GP_PrevInputHeightOfs	//Sensor VOffset 0
0x0F120800,	//REG_TC_GP_CapReqInputWidth		//Sensor Crop Width	2048
0x0F120600,	//REG_TC_GP_CapReqInputHeight		//Sensor Crop Height 1536
0x0F120000,	//REG_TC_GP_CapInputWidthOfs		//Sensor HOffset 0
0x0F120000,	//REG_TC_GP_CapInputHeightOfs		//Sensor VOffset 0

0x002A0216,
0x0F120001,	//REG_TC_GP_bUseReqInputInPre
0x0F120001,	//REG_TC_GP_bUseReqInputInCap

//===================================================================
// Preview 0 640x480 system 52M PCLK 54M (NORMAL PREVIEW)
//===================================================================
0x002A023E,
0x0F120280,	//REG_0TC_PCFG_usWidth
0x0F1201E0,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234BC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234BC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A024C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask

0x002A0254,
0x0F120000,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10
0x0F12029A,	//REG_0TC_PCFG_usMinFrTimeMsecMult10

0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120000,	//REG_0TC_PCFG_uPrevMirror
0x0F120000,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation

//===================================================================
// Preview 1 640x480 system 52M PCLK 54M (15FPS CAMCORDER)
//===================================================================
0x002A026E,
0x0F120280,	//REG_0TC_PCFG_usWidth
0x0F1201E0,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234BC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234BC,	//REG_0TC_PCFG_usMinOut4KHzRate
               
0x002A027C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask
               
0x002A0284,
0x0F120000,	//REG_0TC_PCFG_uClockInd
0x0F120001,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F12029A,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F12029A,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
               
0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120000,	//REG_0TC_PCFG_uPrevMirror
0x0F120000,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation

//===================================================================
// Preview 2 640x480 system 52M PCLK 54M (NIGHT PREVIEW)
//===================================================================
0x002A029E,
0x0F120280,	//REG_0TC_PCFG_usWidth
0x0F1201E0,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234BC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234BC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A02AC,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask
               
0x002A02B4,
0x0F120000,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F1209C4,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014E 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS; 09C4 - 4FPS
0x0F12029A,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014E 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
               
0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120000,	//REG_0TC_PCFG_uPrevMirror
0x0F120000,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation

//===================================================================
// Capture 0 2048x1536 system 52M PCLK 54M (NORMAL CAPTURE)
//===================================================================

0x002A032E,
0x0F120000,	//REG_0TC_CCFG_uCaptureMode

0x0F120800,	//REG_0TC_CCFG_usWidth
0x0F120600,	//REG_0TC_CCFG_usHeight
0x0F120005,	//REG_0TC_CCFG_Format
0x0F1234BC,	//REG_0TC_CCFG_usMaxOut4KHzRate
0x0F1234BC,	//REG_0TC_CCFG_usMinOut4KHzRate

0x002A033E,
0x0F120042,	//REG_0TC_CCFG_PVIMask
0x0F120010,	//REG_0TC_CCFG_OIFMask

0x002A0346,
0x0F120000,	//REG_0TC_CCFG_uClockInd
0x0F120001,	//REG_0TC_CCFG_usFrTimeType
0x0F120002,	//REG_0TC_CCFG_FrRateQualityType
0x0F120535,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10
0x0F120535,	//REG_0TC_CCFG_usMinFrTimeMsecMult10
0x0F120000,	//REG_0TC_CCFG_bSmearOutput
0x0F120000,	//REG_0TC_CCFG_sSaturation
0x0F120000,	//REG_0TC_CCFG_sSharpBlur
0x0F120000,	//REG_0TC_CCFG_sColorTemp
0x0F120000,	//REG_0TC_CCFG_uDeviceGammaIndex

0x002A0426,
0x0F120055,	//REG_TC_BRC_usCaptureQuality

//===================================================================   
// Capture 1 2048x1536 system 52M PCLK 54M               
//===================================================================   
                                                                        
0x002A035A,                                                       
0x0F120000,	//REG_0TC_CCFG_uCaptureMode                           
                                                                        
0x0F120800,	//REG_0TC_CCFG_usWidth                                
0x0F120600,	//REG_0TC_CCFG_usHeight                               
0x0F120005,	//REG_0TC_CCFG_Format //jpeg mode                     
0x0F1234BC,	//REG_0TC_CCFG_usMaxOut4KHzRate                       
0x0F1234BC,	//REG_0TC_CCFG_usMinOut4KHzRate                       
                                                                        
0x002A036A,                                                       
0x0F120042,	//REG_0TC_CCFG_PVIMask                                
0x0F120010,	//REG_0TC_CCFG_OIFMask                                
0x0F1203C0,	//REG_0TC_CCFG_usJpegPacketSize                       
                                                                        
0x002A0372,                                                       
0x0F120000,	//REG_0TC_CCFG_uClockInd                              
0x0F120001,	//REG_0TC_CCFG_usFrTimeType                           
0x0F120002,	//REG_0TC_CCFG_FrRateQualityType                      
0x0F122710,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10                  
0x0F122710,	//REG_0TC_CCFG_usMinFrTimeMsecMult10                  
0x0F120000,	//REG_0TC_CCFG_bSmearOutput
0x0F120000,	//REG_0TC_CCFG_sSaturation
0x0F120000,	//REG_0TC_CCFG_sSharpBlur
0x0F120000,	//REG_0TC_CCFG_sColorTemp
0x0F120000,	//REG_0TC_CCFG_uDeviceGammaIndex
                   
//===================================================================
// Capture 2 2048x1536 system 52M PCLK 54M (NIGHT CAPTURE)
//===================================================================

0x002A0386,
0x0F120000,	//REG_0TC_CCFG_uCaptureMode

0x0F120800,	//REG_0TC_CCFG_usWidth
0x0F120600,	//REG_0TC_CCFG_usHeight
0x0F120005,	//REG_0TC_CCFG_Format //jpeg mode
0x0F1234BC,	//REG_0TC_CCFG_usMaxOut4KHzRate
0x0F1234BC,	//REG_0TC_CCFG_usMinOut4KHzRate

0x002A0396,
0x0F120042,	//REG_0TC_CCFG_PVIMask
0x0F120010,	//REG_0TC_CCFG_OIFMask
0x0F1203C0,	//REG_0TC_CCFG_usJpegPacketSize

0x002A039E,
0x0F120000,	//REG_0TC_CCFG_uClockInd
0x0F120001,	//REG_0TC_CCFG_usFrTimeType
0x0F120002,	//REG_0TC_CCFG_FrRateQualityType
0x0F121388,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10
0x0F121388,	//REG_0TC_CCFG_usMinFrTimeMsecMult10
0x0F120000,	//REG_0TC_CCFG_bSmearOutput
0x0F120000,	//REG_0TC_CCFG_sSaturation
0x0F120000,	//REG_0TC_CCFG_sSharpBlur
0x0F120000,	//REG_0TC_CCFG_sColorTemp
0x0F120000,	//REG_0TC_CCFG_uDeviceGammaIndex

0x002A0426,
0x0F120055,	//REG_TC_BRC_usCaptureQuality

//PREVIEW
0x002A0208,
0x0F120000,	//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,	//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

0xFEFE0064,	//p100	//Delay 100ms


//===================================================================
// AFC
//===================================================================
//Auto
0x002A0F08,
0x0F120001,	//AFC_Default60Hz   01:60hz 00:50Hz
0x002A04A4,
0x0F12067F,	//REG_TC_DBG_AutoAlgEnBits, 065f : Manual AFC on   067f : Manual AFC off

//===================================================================
// Shading (AF module)
//===================================================================
// TVAR_ash_pGAS_high

0x002A0D22,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F120000,
0x0F12000F,
0x0F12000F,
0x0F120F00,
0x0F120F00,
0x0F120000,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120000,
0x0F120F00,
0x0F120000,
0x0F120F0F,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F120F00,
0x0F120F00,
0x0F120000,
0x0F12000F,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120000,
0x0F120F00,
0x0F120000,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F120000,
0x0F120000,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F120000,
0x0F12000F,
0x0F12000F,
0x0F12000F,
0x0F120000,
0x0F120000,
0x0F120F00,
0x0F120F00,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F12000F,
0x0F120000,
0x0F12000F,
0x0F120F0F,
0x0F120F00,
0x0F120F00,
0x0F120F00,
0x0F12000F,
0x0F12000F,
0x0F12000F,
0x0F120000,
0x0F120F00,
0x0F120F00,

// TVAR_ash_pGAS_low
0x0F12780C,
0x0F12F971,
0x0F12F2E8,
0x0F122053,
0x0F12E51C,
0x0F120934,
0x0F12E8FA,
0x0F12F15D,
0x0F120E2F,
0x0F12F683,
0x0F1203E9,
0x0F12032F,
0x0F12FA73,
0x0F120B76,
0x0F12F7BD,
0x0F12018A,
0x0F120219,
0x0F12FAC2,
0x0F1220E2,
0x0F12F9BC,
0x0F120257,
0x0F1207B8,
0x0F12F736,
0x0F12051A,
0x0F12E54F,
0x0F120496,
0x0F12FC3B,
0x0F12FA39,
0x0F120619,
0x0F120013,
0x0F120840,
0x0F12FF12,
0x0F1201EA,
0x0F120293,
0x0F12FDB7,
0x0F12FCDF,
0x0F128CAD,
0x0F12F880,
0x0F12ED13,
0x0F122CE6,
0x0F12DA3F,
0x0F120CFB,
0x0F12E6C4,
0x0F12E637,
0x0F1217BE,
0x0F12EEA9,
0x0F120C4A,
0x0F12FFE6,
0x0F12F24D,
0x0F12135D,
0x0F12F48E,
0x0F1200E6,
0x0F12000A,
0x0F12FB8E,
0x0F122F87,
0x0F12F6FE,
0x0F120379,
0x0F1205EA,
0x0F12FAC4,
0x0F120652,
0x0F12E1B5,
0x0F1202BF,
0x0F12F691,
0x0F1208BF,
0x0F12FC3E,
0x0F12FBD4,
0x0F120135,
0x0F120481,
0x0F120562,
0x0F12F698,
0x0F1202C1,
0x0F1202F8,
0x0F12739E,
0x0F12F752,
0x0F12F447,
0x0F1221B9,
0x0F12DEC6,
0x0F12103C,
0x0F12E9E5,
0x0F12F517,
0x0F120C04,
0x0F12F671,
0x0F120534,
0x0F12019E,
0x0F1200AF,
0x0F120843,
0x0F12F779,
0x0F1203A6,
0x0F12FF57,
0x0F12FCEE,
0x0F121AD0,
0x0F12FC1A,
0x0F12030D,
0x0F12061A,
0x0F12F5F7,
0x0F1206BA,
0x0F12E5F5,
0x0F120017,
0x0F12F9F8,
0x0F120053,
0x0F1203E5,
0x0F12013E,
0x0F1209B5,
0x0F1202FE,
0x0F12060F,
0x0F12F848,
0x0F120810,
0x0F12F4D2,
0x0F127C5A,
0x0F12F51C,
0x0F12F68F,
0x0F121CD0,
0x0F12E82B,
0x0F1207EA,
0x0F12E890,
0x0F12F3EE,
0x0F120A06,
0x0F12FC57,
0x0F12FDEB,
0x0F120615,
0x0F12017F,
0x0F1206C2,
0x0F12F970,
0x0F1202EF,
0x0F12FE5F,
0x0F12FE98,
0x0F121A59,
0x0F12FD3A,
0x0F120451,
0x0F12FC2C,
0x0F1206DD,
0x0F12FA5E,
0x0F12EA73,
0x0F12028C,
0x0F12F7D8,
0x0F120922,
0x0F12F4C2,
0x0F120AD6,
0x0F1203D8,
0x0F120101,
0x0F1203B1,
0x0F12FD21,
0x0F120248,
0x0F12FA84,

0x002A04A8,
0x0F120001,	//REG_TC_DBG_ReInitCmd


//===================================================================
// Shading - Alpha
//===================================================================

0x002A07E8,
0x0F1200BC,	//TVAR_ash_AwbAshCord_0_	//HOR
0x0F1200ED,	//TVAR_ash_AwbAshCord_1_	//INCA
0x0F120101,	//TVAR_ash_AwbAshCord_2_	//WW
0x0F12012D,	//TVAR_ash_AwbAshCord_3_	//CW
0x0F120166,	//TVAR_ash_AwbAshCord_4_	//D50
0x0F120184,	//TVAR_ash_AwbAshCord_5_	//D65
0x0F1201A0,	//TVAR_ash_AwbAshCord_6_	//D75

0x002A07FE,
0x0F124000,	//TVAR_ash_GASAlpha_0__0_
0x0F124000,	//TVAR_ash_GASAlpha_0__1_
0x0F124000,	//TVAR_ash_GASAlpha_0__2_
0x0F124000,	//TVAR_ash_GASAlpha_0__3_
0x0F123E00,	//TVAR_ash_GASAlpha_1__0_
0x0F124000,	//TVAR_ash_GASAlpha_1__1_
0x0F124000,	//TVAR_ash_GASAlpha_1__2_
0x0F124000,	//TVAR_ash_GASAlpha_1__3_
0x0F123E00,	//TVAR_ash_GASAlpha_2__0_
0x0F124000,	//TVAR_ash_GASAlpha_2__1_
0x0F124000,	//TVAR_ash_GASAlpha_2__2_
0x0F124000,	//TVAR_ash_GASAlpha_2__3_
0x0F123200,	//TVAR_ash_GASAlpha_3__0_
0x0F124000,	//TVAR_ash_GASAlpha_3__1_
0x0F124000,	//TVAR_ash_GASAlpha_3__2_
0x0F123C00,	//TVAR_ash_GASAlpha_3__3_
0x0F123600,	//TVAR_ash_GASAlpha_4__0_
0x0F124000,	//TVAR_ash_GASAlpha_4__1_
0x0F124000,	//TVAR_ash_GASAlpha_4__2_
0x0F124000,	//TVAR_ash_GASAlpha_4__3_
0x0F123A00,	//TVAR_ash_GASAlpha_5__0_
0x0F124000,	//TVAR_ash_GASAlpha_5__1_
0x0F124000,	//TVAR_ash_GASAlpha_5__2_
0x0F124000,	//TVAR_ash_GASAlpha_5__3_
0x0F123F00,	//TVAR_ash_GASAlpha_6__0_
0x0F124000,	//TVAR_ash_GASAlpha_6__1_
0x0F124000,	//TVAR_ash_GASAlpha_6__2_
0x0F124000,	//TVAR_ash_GASAlpha_6__3_

0x002A0836,
0x0F124000,	//TVAR_ash_GASOutdoorAlpha_0_
0x0F124000,	//TVAR_ash_GASOutdoorAlpha_1_
0x0F124000,	//TVAR_ash_GASOutdoorAlpha_2_
0x0F124000,	//TVAR_ash_GASOutdoorAlpha_3_

//===================================================================
// Gamma
//===================================================================
//	param_start	SARR_usGammaLutRGBIndoor
0x002A0660,
0x0F120000, //saRR_usDualGammaLutRGBIndoor[0][0]
0x0F120008, //saRR_usDualGammaLutRGBIndoor[0][1]
0x0F120015, //saRR_usDualGammaLutRGBIndoor[0][2]
0x0F120032, //saRR_usDualGammaLutRGBIndoor[0][3]
0x0F12006C, //saRR_usDualGammaLutRGBIndoor[0][4]
0x0F1200D0, //saRR_usDualGammaLutRGBIndoor[0][5]
0x0F120129, //saRR_usDualGammaLutRGBIndoor[0][6]
0x0F120151, //saRR_usDualGammaLutRGBIndoor[0][7]
0x0F120174, //saRR_usDualGammaLutRGBIndoor[0][8]
0x0F1201AA, //saRR_usDualGammaLutRGBIndoor[0][9]
0x0F1201D7, //saRR_usDualGammaLutRGBIndoor[0][10]
0x0F1201FE, //saRR_usDualGammaLutRGBIndoor[0][11]
0x0F120221, //saRR_usDualGammaLutRGBIndoor[0][12]
0x0F120252, //saRR_usDualGammaLutRGBIndoor[0][13]
0x0F120281, //saRR_usDualGammaLutRGBIndoor[0][14]
0x0F1202E1, //saRR_usDualGammaLutRGBIndoor[0][15]
0x0F120345, //saRR_usDualGammaLutRGBIndoor[0][16]
0x0F12039C, //saRR_usDualGammaLutRGBIndoor[0][17]
0x0F1203D9, //saRR_usDualGammaLutRGBIndoor[0][18]
0x0F1203FF, //saRR_usDualGammaLutRGBIndoor[0][19]
0x0F120000, //saRR_usDualGammaLutRGBIndoor[1][0]
0x0F120008, //saRR_usDualGammaLutRGBIndoor[1][1]
0x0F120015, //saRR_usDualGammaLutRGBIndoor[1][2]
0x0F120032, //saRR_usDualGammaLutRGBIndoor[1][3]
0x0F12006C, //saRR_usDualGammaLutRGBIndoor[1][4]
0x0F1200D0, //saRR_usDualGammaLutRGBIndoor[1][5]
0x0F120129, //saRR_usDualGammaLutRGBIndoor[1][6]
0x0F120151, //saRR_usDualGammaLutRGBIndoor[1][7]
0x0F120174, //saRR_usDualGammaLutRGBIndoor[1][8]
0x0F1201AA, //saRR_usDualGammaLutRGBIndoor[1][9]
0x0F1201D7, //saRR_usDualGammaLutRGBIndoor[1][10]
0x0F1201FE, //saRR_usDualGammaLutRGBIndoor[1][11]
0x0F120221, //saRR_usDualGammaLutRGBIndoor[1][12]
0x0F120252, //saRR_usDualGammaLutRGBIndoor[1][13]
0x0F120281, //saRR_usDualGammaLutRGBIndoor[1][14]
0x0F1202E1, //saRR_usDualGammaLutRGBIndoor[1][15]
0x0F120345, //saRR_usDualGammaLutRGBIndoor[1][16]
0x0F12039C, //saRR_usDualGammaLutRGBIndoor[1][17]
0x0F1203D9, //saRR_usDualGammaLutRGBIndoor[1][18]
0x0F1203FF, //saRR_usDualGammaLutRGBIndoor[1][19]
0x0F120000, //saRR_usDualGammaLutRGBIndoor[2][0]
0x0F120008, //saRR_usDualGammaLutRGBIndoor[2][1]
0x0F120015, //saRR_usDualGammaLutRGBIndoor[2][2]
0x0F120032, //saRR_usDualGammaLutRGBIndoor[2][3]
0x0F12006C, //saRR_usDualGammaLutRGBIndoor[2][4]
0x0F1200D0, //saRR_usDualGammaLutRGBIndoor[2][5]
0x0F120129, //saRR_usDualGammaLutRGBIndoor[2][6]
0x0F120151, //saRR_usDualGammaLutRGBIndoor[2][7]
0x0F120174, //saRR_usDualGammaLutRGBIndoor[2][8]
0x0F1201AA, //saRR_usDualGammaLutRGBIndoor[2][9]
0x0F1201D7, //saRR_usDualGammaLutRGBIndoor[2][10]
0x0F1201FE, //saRR_usDualGammaLutRGBIndoor[2][11]
0x0F120221, //saRR_usDualGammaLutRGBIndoor[2][12]
0x0F120252, //saRR_usDualGammaLutRGBIndoor[2][13]
0x0F120281, //saRR_usDualGammaLutRGBIndoor[2][14]
0x0F1202E1, //saRR_usDualGammaLutRGBIndoor[2][15]
0x0F120345, //saRR_usDualGammaLutRGBIndoor[2][16]
0x0F12039C, //saRR_usDualGammaLutRGBIndoor[2][17]
0x0F1203D9, //saRR_usDualGammaLutRGBIndoor[2][18]
0x0F1203FF, //saRR_usDualGammaLutRGBIndoor[2][19]


0x0F120000,	//saRR_usDualGammaLutRGBOutdoor[0][0]
0x0F120008,	//saRR_usDualGammaLutRGBOutdoor[0][1]
0x0F120013,	//saRR_usDualGammaLutRGBOutdoor[0][2]
0x0F12002C,	//saRR_usDualGammaLutRGBOutdoor[0][3]
0x0F12005C,	//saRR_usDualGammaLutRGBOutdoor[0][4]
0x0F1200BB,	//saRR_usDualGammaLutRGBOutdoor[0][5]
0x0F120109,	//saRR_usDualGammaLutRGBOutdoor[0][6]
0x0F12012C,	//saRR_usDualGammaLutRGBOutdoor[0][7]
0x0F12014C,	//saRR_usDualGammaLutRGBOutdoor[0][8]
0x0F120185,	//saRR_usDualGammaLutRGBOutdoor[0][9]
0x0F1201B9,	//saRR_usDualGammaLutRGBOutdoor[0][10]
0x0F1201E8,	//saRR_usDualGammaLutRGBOutdoor[0][11]
0x0F120210,	//saRR_usDualGammaLutRGBOutdoor[0][12]
0x0F12024C,	//saRR_usDualGammaLutRGBOutdoor[0][13]
0x0F120280,	//saRR_usDualGammaLutRGBOutdoor[0][14]
0x0F1202D4,	//saRR_usDualGammaLutRGBOutdoor[0][15]
0x0F120324,	//saRR_usDualGammaLutRGBOutdoor[0][16]
0x0F12036C,	//saRR_usDualGammaLutRGBOutdoor[0][17]
0x0F1203AC,	//saRR_usDualGammaLutRGBOutdoor[0][18]
0x0F1203E6,	//saRR_usDualGammaLutRGBOutdoor[0][19]
0x0F120000,	//saRR_usDualGammaLutRGBOutdoor[1][0]
0x0F120008,	//saRR_usDualGammaLutRGBOutdoor[1][1]
0x0F120013,	//saRR_usDualGammaLutRGBOutdoor[1][2]
0x0F12002C,	//saRR_usDualGammaLutRGBOutdoor[1][3]
0x0F12005C,	//saRR_usDualGammaLutRGBOutdoor[1][4]
0x0F1200BB,	//saRR_usDualGammaLutRGBOutdoor[1][5]
0x0F120109,	//saRR_usDualGammaLutRGBOutdoor[1][6]
0x0F12012C,	//saRR_usDualGammaLutRGBOutdoor[1][7]
0x0F12014C,	//saRR_usDualGammaLutRGBOutdoor[1][8]
0x0F120185,	//saRR_usDualGammaLutRGBOutdoor[1][9]
0x0F1201B9,	//saRR_usDualGammaLutRGBOutdoor[1][10]
0x0F1201E8,	//saRR_usDualGammaLutRGBOutdoor[1][11]
0x0F120210,	//saRR_usDualGammaLutRGBOutdoor[1][12]
0x0F12024C,	//saRR_usDualGammaLutRGBOutdoor[1][13]
0x0F120280,	//saRR_usDualGammaLutRGBOutdoor[1][14]
0x0F1202D4,	//saRR_usDualGammaLutRGBOutdoor[1][15]
0x0F120324,	//saRR_usDualGammaLutRGBOutdoor[1][16]
0x0F12036C,	//saRR_usDualGammaLutRGBOutdoor[1][17]
0x0F1203AC,	//saRR_usDualGammaLutRGBOutdoor[1][18]
0x0F1203E6,	//saRR_usDualGammaLutRGBOutdoor[1][19]
0x0F120000,	//saRR_usDualGammaLutRGBOutdoor[2][0]
0x0F120008,	//saRR_usDualGammaLutRGBOutdoor[2][1]
0x0F120013,	//saRR_usDualGammaLutRGBOutdoor[2][2]
0x0F12002C,	//saRR_usDualGammaLutRGBOutdoor[2][3]
0x0F12005C,	//saRR_usDualGammaLutRGBOutdoor[2][4]
0x0F1200BB,	//saRR_usDualGammaLutRGBOutdoor[2][5]
0x0F120109,	//saRR_usDualGammaLutRGBOutdoor[2][6]
0x0F12012C,	//saRR_usDualGammaLutRGBOutdoor[2][7]
0x0F12014C,	//saRR_usDualGammaLutRGBOutdoor[2][8]
0x0F120185,	//saRR_usDualGammaLutRGBOutdoor[2][9]
0x0F1201B9,	//saRR_usDualGammaLutRGBOutdoor[2][10]
0x0F1201E8,	//saRR_usDualGammaLutRGBOutdoor[2][11]
0x0F120210,	//saRR_usDualGammaLutRGBOutdoor[2][12]
0x0F12024C,	//saRR_usDualGammaLutRGBOutdoor[2][13]
0x0F120280,	//saRR_usDualGammaLutRGBOutdoor[2][14]
0x0F1202D4,	//saRR_usDualGammaLutRGBOutdoor[2][15]
0x0F120324,	//saRR_usDualGammaLutRGBOutdoor[2][16]
0x0F12036C,	//saRR_usDualGammaLutRGBOutdoor[2][17]
0x0F1203AC,	//saRR_usDualGammaLutRGBOutdoor[2][18]
0x0F1203E6,	//saRR_usDualGammaLutRGBOutdoor[2][19]


//===================================================================
// AE - shutter
//===================================================================
//****************************************/
// AE 2009 03 08 - based on TN
//****************************************/
//============================================================
// Frame rate setting
//============================================================
// How to set
// 1. Exposure value
// dec2hex((1 / (frame rate you want(ms))) * 100d * 5d)
//
//
// 2. Analog Digital gain
// dec2hex((Analog gain you want) * 256d)
//              Ex1) Simple Caculation for x3.25?:   3.25x256 = 832[dec] = 0340[hex]
//============================================================
//MBR
0x002A01DE,
0x0F120000,	//REG_TC_bUseMBR	//MBR off
//MBR off is needed to prevent a shorter integration time when the scene has blurring in Night shot

//AE_Target
0x002A1308,
0x0F120040,	//TVAR_ae_BrAve
0x002A130E,
0x0F12000F,	//ae_StatMode
//ae_StatMode bit[3] BLC has to be bypassed to prevent AE weight change, especially backlight scene

//AE_state
0x002A04EE,
0x0F12010E,	//#lt_uLimitHigh
0x0F1200F5,	//#lt_uLimitLow

//For 60Hz
0x002A0504,
0x0F123415,	//#lt_uMaxExp1
0x002A0508,
0x0F12681F,	//#lt_uMaxExp2
0x002A050C,
0x0F128227,	//#lt_uMaxExp3
0x002A0510,
0x0F12C350,	//#lt_uMaxExp4

0x002A0514,
0x0F123415,	//#lt_uCapMaxExp1
0x002A0518,
0x0F12681F,	//#lt_uCapMaxExp2
0x002A051C,
0x0F128227,	//#lt_uCapMaxExp3
0x002A0520,
0x0F12C350,	//#lt_uCapMaxExp4

0x002A0524,
0x0F1201D0,	//#lt_uMaxAnGain1
0x0F1201D0,	//#lt_uMaxAnGain2
0x0F1202C0,	//#lt_uMaxAnGain3
0x0F120880,	//#lt_uMaxAnGain4

0x0F120100,	//#lt_uMaxDigGain
0x0F128000,	//#lt_uMaxTotGain  Total-gain is limited by #lt_uMaxTotGain

0x0F1201D0,	//#lt_uCapMaxAnGain1
0x0F1201D0,	//#lt_uCapMaxAnGain2
0x0F1202C0,	//#lt_uCapMaxAnGain3
0x0F120710,	//#lt_uCapMaxAnGain4

0x0F120100,	//#lt_uCapMaxDigGain
0x0F128000,	//#lt_uCapMaxTotGain  Total-gain is limited by #lt_uMaxTotGain

//===================================================================
//AE - Weights
//===================================================================
0x002A1316,
0x0F120000,	//ae_WeightTbl_16[0]
0x0F120000,	//ae_WeightTbl_16[1]
0x0F120000,	//ae_WeightTbl_16[2]
0x0F120000,	//ae_WeightTbl_16[3]
0x0F120101,	//ae_WeightTbl_16[4]
0x0F120101,	//ae_WeightTbl_16[5]
0x0F120101,	//ae_WeightTbl_16[6]
0x0F120101,	//ae_WeightTbl_16[7]
0x0F120101,	//ae_WeightTbl_16[8]
0x0F120201,	//ae_WeightTbl_16[9]
0x0F120102,	//ae_WeightTbl_16[10]
0x0F120101,	//ae_WeightTbl_16[11]
0x0F120101,	//ae_WeightTbl_16[12]
0x0F120202,	//ae_WeightTbl_16[13]
0x0F120202,	//ae_WeightTbl_16[14]
0x0F120101,	//ae_WeightTbl_16[15]
0x0F120101,	//ae_WeightTbl_16[16]
0x0F120202,	//ae_WeightTbl_16[17]
0x0F120202,	//ae_WeightTbl_16[18]
0x0F120101,	//ae_WeightTbl_16[19]
0x0F120201,	//ae_WeightTbl_16[20]
0x0F120202,	//ae_WeightTbl_16[21]
0x0F120202,	//ae_WeightTbl_16[22]
0x0F120102,	//ae_WeightTbl_16[23]
0x0F120201,	//ae_WeightTbl_16[24]
0x0F120202,	//ae_WeightTbl_16[25]
0x0F120202,	//ae_WeightTbl_16[26]
0x0F120102,	//ae_WeightTbl_16[27]
0x0F120101,	//ae_WeightTbl_16[28]
0x0F120101,	//ae_WeightTbl_16[29]
0x0F120101,	//ae_WeightTbl_16[30]
0x0F120101,	//ae_WeightTbl_16[31]



//===================================================================
//AWB-BASIC setting
//===================================================================
0x002A1018,
0x0F1202A7,	//awbb_GLocusR
0x0F120343,	//awbb_GLocusB
0x002A0FFC,
0x0F12036C,	//awbb_CrclLowT_R_c
0x002A1000,
0x0F12011D,	//awbb_CrclLowT_B_c
0x002A1004,
0x0F1262C1,	//awbb_CrclLowT_Rad_c
0x002A1034,
0x0F1205F0,	//awbb_GamutWidthThr1
0x0F1201F4,	//awbb_GamutHeightThr1
0x0F12006C,	//awbb_GamutWidthThr2
0x0F120038,	//awbb_GamutHeightThr2
0x002A1020,
0x0F12000C,	//awbb_MinNumOfFinalPatches
0x0F12001E,	//awbb_MinNumOfLowBrFinalPatches
0x0F120046,	//awbb_MinNumOfLowBr0_FinalPatches
0x002A291A,
0x0F120006,	// #Mon_AWB_ByPassMode // [0]Outdoor [1]LowBr [2]LowTemp

0x002A11C2,
0x0F120000,	//awbb_RGainOff
0x0F120000,	//awbb_BGainOff
0x0F120000,	//awbb_GGainOff
0x0F1200C2,	//awbb_Alpha_Comp_Mode
0x0F120002,	//awbb_Rpl_InvalidOutDoor
0x0F120001,	//awbb_UseGrThrCorr
0x0F1200E4,	//awbb_Use_Filters
0x0F12053C,	//awbb_GainsInit[0]
0x0F120400,	//awbb_GainsInit[1]
0x0F12055C,	//awbb_GainsInit[2]
0x0F120008,	//001E awbb_WpFilterMinThr
0x0F120160,	//0190 awbb_WpFilterMaxThr
0x0F1200A0,	//awbb_WpFilterCoef
0x0F120004,	//awbb_WpFilterSize
0x0F120001,	//awbb_otp_disable

//===================================================================
//AWB-Zone
//===================================================================
//	param_start	awbb_IndoorGrZones_m_BGrid
0x002A0F28,
0x0F1203A2,	//03C0	//03C0	//awbb_IndoorGrZones_m_BGrid[0]  03A2
0x0F1203C4,	//03E2	//03E2	//awbb_IndoorGrZones_m_BGrid[1]  03C4
0x0F12033C,	//0356	//0356	//awbb_IndoorGrZones_m_BGrid[2]  033C
0x0F1203E2,	//03FC	//03FC	//awbb_IndoorGrZones_m_BGrid[3]  03E2
0x0F120304,	//031E	//031E	//awbb_IndoorGrZones_m_BGrid[4]  0304
0x0F1203E4,	//03FE	//03FE	//awbb_IndoorGrZones_m_BGrid[5]  03E4
0x0F1202D6,	//02F0	//02F0	//awbb_IndoorGrZones_m_BGrid[6]  02D6
0x0F1203D6,	//03F0	//03F0	//awbb_IndoorGrZones_m_BGrid[7]  03D6
0x0F1202B0,	//02CA	//02CA	//awbb_IndoorGrZones_m_BGrid[8]  02B0
0x0F1203B2,	//03CC	//03CC	//awbb_IndoorGrZones_m_BGrid[9]  03B2
0x0F120290,	//02A8	//02A8	//awbb_IndoorGrZones_m_BGrid[10] 0290
0x0F120362,	//037A	//037A	//awbb_IndoorGrZones_m_BGrid[11] 0362
0x0F120266,	//0288	//0288	//awbb_IndoorGrZones_m_BGrid[12] 0266
0x0F120322,	//033C	//033C	//awbb_IndoorGrZones_m_BGrid[13] 0322
0x0F120248,	//0266	//0266	//awbb_IndoorGrZones_m_BGrid[14] 0248
0x0F1202E4,	//030A	//031E	//awbb_IndoorGrZones_m_BGrid[15] 02DE
0x0F120226,	//0246	//0246	//awbb_IndoorGrZones_m_BGrid[16] 0226
0x0F1202B6,	//02EC	//0300	//awbb_IndoorGrZones_m_BGrid[17] 02BE
0x0F120200,	//0228	//0228	//awbb_IndoorGrZones_m_BGrid[18] 0200
0x0F120294,	//02CE	//02E8	//awbb_IndoorGrZones_m_BGrid[19] 02A6
0x0F1201E4,	//020E	//020E	//awbb_IndoorGrZones_m_BGrid[20] 01E4
0x0F12028C,	//02B0	//02CA	//awbb_IndoorGrZones_m_BGrid[21] 0296
0x0F1201C6,	//01F8	//01F8	//awbb_IndoorGrZones_m_BGrid[22] 01C6
0x0F120288,	//0292	//02B0	//awbb_IndoorGrZones_m_BGrid[23] 0286
0x0F1201B6,	//01E8	//01E8	//awbb_IndoorGrZones_m_BGrid[24] 01B6
0x0F12027C,	//0276	//0296	//awbb_IndoorGrZones_m_BGrid[25] 0276
0x0F1201A8,	//01DC	//01DC	//awbb_IndoorGrZones_m_BGrid[26] 01A8
0x0F12026C,	//0256	//027A	//awbb_IndoorGrZones_m_BGrid[27] 0266
0x0F1201AE,	//01E0	//01E0	//awbb_IndoorGrZones_m_BGrid[28] 01AE
0x0F120256,	//0238	//0252	//awbb_IndoorGrZones_m_BGrid[29] 0250
0x0F1201BA,	//01EC	//01F2	//awbb_IndoorGrZones_m_BGrid[30] 01BA
0x0F120238,	//020E	//0226	//awbb_IndoorGrZones_m_BGrid[31] 0238
0x0F1201D2,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[32] 01D2
0x0F120206,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[33] 0206
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[34] 0000
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[35] 0000
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[36] 0000
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[37] 0000
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[38] 0000
0x0F120000,	//0000	//0000	//awbb_IndoorGrZones_m_BGrid[39] 0000
//	param_end	awbb_IndoorGrZones_m_BGrid

0x0F120005,	//awbb_IndoorGrZones_m_Grid 0005
0x002A0F80, //                          0F80
0x0F1200E4,	//awbb_IndoorGrZones_m_Boff 00E6
0x002A0F7C, //                          0F7C
0x0F120011, //                          0010
//	param_start	awbb_OutdoorGrZones_m_BGrid
0x002A0F84,
0x0F12028C,	//awbb_OutdoorGrZones_m_BGrid[0]   028C   028C
0x0F1202A0,	//awbb_OutdoorGrZones_m_BGrid[1]   02A0   02A0
0x0F120276,	//awbb_OutdoorGrZones_m_BGrid[2]   0276   0276
0x0F1202B2,	//awbb_OutdoorGrZones_m_BGrid[3]   02B2   02B4
0x0F120262,	//awbb_OutdoorGrZones_m_BGrid[4]   0262   0262
0x0F1202BC,	//awbb_OutdoorGrZones_m_BGrid[5]   02BC   02BE
0x0F120250,	//awbb_OutdoorGrZones_m_BGrid[6]   0250   0250
0x0F1202BE,	//awbb_OutdoorGrZones_m_BGrid[7]   02BE   02C4
0x0F120240,	//awbb_OutdoorGrZones_m_BGrid[8]   0240   0240
0x0F1202BE,	//awbb_OutdoorGrZones_m_BGrid[9]   02BE   02C2
0x0F120232,	//awbb_OutdoorGrZones_m_BGrid[10]  0232   0232
0x0F1202B6,	//awbb_OutdoorGrZones_m_BGrid[11]  02B6   02BC
0x0F120226,	//awbb_OutdoorGrZones_m_BGrid[12]  0226   0226
0x0F1202A8,	//awbb_OutdoorGrZones_m_BGrid[13]  02A8   02B0
0x0F12021A,	//awbb_OutdoorGrZones_m_BGrid[14]  021A   021A
0x0F12029C,	//awbb_OutdoorGrZones_m_BGrid[15]  029C   02A2
0x0F120214,	//awbb_OutdoorGrZones_m_BGrid[16]  0214   0214
0x0F12028C,	//awbb_OutdoorGrZones_m_BGrid[17]  028C   0294
0x0F120216,	//awbb_OutdoorGrZones_m_BGrid[18]  0216   0216
0x0F12027A,	//awbb_OutdoorGrZones_m_BGrid[19]  027A   0282
0x0F120226,	//awbb_OutdoorGrZones_m_BGrid[20]  0226   0226
0x0F12025A,	//awbb_OutdoorGrZones_m_BGrid[21]  025A   025A
0x0F120000,	//awbb_OutdoorGrZones_m_BGrid[22]  0000   0000
0x0F120000,	//awbb_OutdoorGrZones_m_BGrid[23]  0000   0000
//	param_end	awbb_OutdoorGrZones_m_BGrid

0x0F120004,	//awbb_OutdoorGrZones_m_Gri
0x002A0FB8,
0x0F12000B,	//awbb_OutdoorGrZones_ZInfo_m_GridSz
0x002A0FBC,
0x0F1201E2,	//awbb_OutdoorGrZones_m_Bof

//	param_start	awbb_LowBrGrZones_m_BGrid
0x002A0FC0,
0x0F1203B2,	//awbb_LowBrGrZones_m_BGrid[0]
0x0F12044E,	//awbb_LowBrGrZones_m_BGrid[1]
0x0F120330,	//awbb_LowBrGrZones_m_BGrid[2]
0x0F120454,	//awbb_LowBrGrZones_m_BGrid[3]
0x0F1202CC,	//awbb_LowBrGrZones_m_BGrid[4]
0x0F120414,	//awbb_LowBrGrZones_m_BGrid[5]
0x0F12026E,	//awbb_LowBrGrZones_m_BGrid[6]
0x0F1203D0,	//awbb_LowBrGrZones_m_BGrid[7]
0x0F120226,	//awbb_LowBrGrZones_m_BGrid[8]
0x0F120362,	//awbb_LowBrGrZones_m_BGrid[9]
0x0F1201F0,	//awbb_LowBrGrZones_m_BGrid[10]
0x0F120312,	//awbb_LowBrGrZones_m_BGrid[11]
0x0F1201CE,	//awbb_LowBrGrZones_m_BGrid[12]
0x0F1202CC,	//awbb_LowBrGrZones_m_BGrid[13]
0x0F1201B2,	//awbb_LowBrGrZones_m_BGrid[14]
0x0F12029E,	//awbb_LowBrGrZones_m_BGrid[15]
0x0F1201AC,	//awbb_LowBrGrZones_m_BGrid[16]
0x0F120278,	//awbb_LowBrGrZones_m_BGrid[17]
0x0F1201B6,	//awbb_LowBrGrZones_m_BGrid[18]
0x0F120248,	//awbb_LowBrGrZones_m_BGrid[19]
0x0F120000,	//awbb_LowBrGrZones_m_BGrid[20]
0x0F120000,	//awbb_LowBrGrZones_m_BGrid[21]
0x0F120000,	//awbb_LowBrGrZones_m_BGrid[22]
0x0F120000,	//awbb_LowBrGrZones_m_BGrid[23]

//	param_end	awbb_LowBrGrZones_m_BGrid
0x0F120006,	//awbb_LowBrGrZones_m_GridStep
0x002A0FF4,
0x0F12000A,	//awbb_LowBrGrZones_ZInfo_m_GridSz
0x002A0FF8,
0x0F1200C2,	//awbb_LowBrGrZones_m_Boffs



//===================================================================
//AWB Scene Detection
//===================================================================

0x002A1098,
0x0F12FE82,	//awbb_SCDetectionMap_SEC_StartR_B
0x0F12001E,	//awbb_SCDetectionMap_SEC_StepR_B
0x0F1209C4,	//awbb_SCDetectionMap_SEC_SunnyNB
0x0F120122,	//awbb_SCDetectionMap_SEC_StepNB
0x0F1200E4,	//awbb_SCDetectionMap_SEC_LowTempR_B
0x0F120096,	//awbb_SCDetectionMap_SEC_SunnyNBZone
0x0F12000E,	//awbb_SCDetectionMap_SEC_LowTempR_BZone

0x002A105C,
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_0__0_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_0__2_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_0__4_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_1__1_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_1__3_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_2__0_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_2__2_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_2__4_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_3__1_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_3__3_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_4__0_
0x0F120000,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_4__2_
0x0F120500,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_4__4_
0x0F125555,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_5__1_
0x0F125455,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_5__3_
0x0F12AA55,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_6__0_
0x0F12AAAA,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_6__2_
0x0F12BF54,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_6__4_
0x0F12FFFF,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_7__1_
0x0F1254FE,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_7__3_
0x0F12FF6F,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_8__0_
0x0F12FEFF,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_8__2_
0x0F121B54,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_8__4_
0x0F12FFFF,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_9__1_
0x0F1254FE,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_9__3_
0x0F12FF06,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_10__0_
0x0F12FEFF,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_10__2_
0x0F120154,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_10__4_
0x0F12BFBF,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_11__1_
0x0F1254BE,	//#awbb_SCDetectionMap_SEC_SceneDetectionMap_11__3_


//===================================================================
//AWB - GridCorrection
//===================================================================

0x002A11E0,
0x0F120002,	//awbb_GridEnable

0x002A11A8,
0x0F1202C8, //02C8	//awbb_GridConst_1[0]
0x0F120325, //0325	//awbb_GridConst_1[1]
0x0F12038F, //038F	//awbb_GridConst_1[2]

0x0F120F8E, //0F8E	//awbb_GridConst_2[0]
0x0F1210B3, //10B3	//awbb_GridConst_2[1]
0x0F121136, //1136	//awbb_GridConst_2[2]
0x0F121138, //1138	//awbb_GridConst_2[3]
0x0F12119A, //118E	//awbb_GridConst_2[4]
0x0F12123E, //123E	//awbb_GridConst_2[5]

0x0F1200A7, //00A7	//awbb_GridCoeff_R_1
0x0F1200C2, //00C2	//awbb_GridCoeff_B_1
0x0F1200BD, //00BD	//awbb_GridCoeff_R_2
0x0F1200AC, //00AC	//awbb_GridCoeff_B_2

0x002A1118,
0x0F120000,     //0000	//awbb_GridCorr_R[0][0] 0050
0x0F120000,     //0000	//awbb_GridCorr_R[0][1] 0032
0x0F120032,     //0032	//awbb_GridCorr_R[0][2] 0032
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[0][3] FFEC
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[0][4] FFEC
0x0F12001E,     //0050	//awbb_GridCorr_R[0][5] 0050
0x0F120000,     //0000	//awbb_GridCorr_R[1][0] 0050
0x0F120000,     //0000	//awbb_GridCorr_R[1][1] 0032
0x0F120032,     //0032	//awbb_GridCorr_R[1][2] 0032
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[1][3] FFEC
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[1][4] FFEC
0x0F12001E,     //0050	//awbb_GridCorr_R[1][5] 0050
0x0F120000,     //0000	//awbb_GridCorr_R[2][0] 0050
0x0F120000,     //0000	//awbb_GridCorr_R[2][1] 0032
0x0F120032,     //0032	//awbb_GridCorr_R[2][2] 0032
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[2][3] FFEC
0x0F12FFE7,     //FFEC	//awbb_GridCorr_R[2][4] FFEC
0x0F12001E,     //0050	//awbb_GridCorr_R[2][5] 0050
0x0F12FFCE,     //FFCE	//awbb_GridCorr_B[0][0] FF9C
0x0F12FFCE,	//FFCE	//awbb_GridCorr_B[0][1] FFD8
0x0F12FFEC,     //FFEC	//awbb_GridCorr_B[0][2] FFEC
0x0F12FF47,     //FF97	//awbb_GridCorr_B[0][3] FF97
0x0F12FF47,     //FF97	//awbb_GridCorr_B[0][4] FF97
0x0F12FDA8,	//FDA8	//awbb_GridCorr_B[0][5] FDA8
0x0F12FFCE,     //FFCE	//awbb_GridCorr_B[1][0] FF9C
0x0F12FFCE,	//FFCE	//awbb_GridCorr_B[1][1] FFD8
0x0F12FFEC,     //FFEC	//awbb_GridCorr_B[1][2] FFEC
0x0F12FF47,     //FF97	//awbb_GridCorr_B[1][3] FF97
0x0F12FF47,     //FF97	//awbb_GridCorr_B[1][4] FF97
0x0F12FDA8,	//FDA8	//awbb_GridCorr_B[1][5] FDA8
0x0F12FFCE,     //FFCE	//awbb_GridCorr_B[2][0] FF9C
0x0F12FFCE,	//FFCE	//awbb_GridCorr_B[2][1] FFD8
0x0F12FFEC,     //FFEC	//awbb_GridCorr_B[2][2] FFEC
0x0F12FF47,     //FF97	//awbb_GridCorr_B[2][3] FF97
0x0F12FF47,     //FF97	//awbb_GridCorr_B[2][4] FF97
0x0F12FDA8,	//FDA8	//awbb_GridCorr_B[2][5] FDA8

0x002A1160,
0x0F12FFF4,	//awbb_GridCorr_R_Out[0][0]   0000
0x0F12FFF4,	//awbb_GridCorr_R_Out[0][1]   0000
0x0F120000,	//awbb_GridCorr_R_Out[0][2]   0000
0x0F120000,	//awbb_GridCorr_R_Out[0][3]   0000
0x0F120000,	//awbb_GridCorr_R_Out[0][4]   0000
0x0F120000,	//awbb_GridCorr_R_Out[0][5]   0000
0x0F12FFF4,	//awbb_GridCorr_R_Out[1][0]   0000
0x0F12FFF4,	//awbb_GridCorr_R_Out[1][1]   0000
0x0F120000,	//awbb_GridCorr_R_Out[1][2]   0000
0x0F120000,	//awbb_GridCorr_R_Out[1][3]   0000
0x0F120000,	//awbb_GridCorr_R_Out[1][4]   0000
0x0F120000,	//awbb_GridCorr_R_Out[1][5]   0000
0x0F12FFF4,	//awbb_GridCorr_R_Out[2][0]   0000
0x0F12FFF4,	//awbb_GridCorr_R_Out[2][1]   0000
0x0F120000,	//awbb_GridCorr_R_Out[2][2]   0000
0x0F120000,	//awbb_GridCorr_R_Out[2][3]   0000
0x0F120000,	//awbb_GridCorr_R_Out[2][4]   0000
0x0F120000,	//awbb_GridCorr_R_Out[2][5]   0000
0x0F12FFF2,	//awbb_GridCorr_B_Out[0][0]   FFE2
0x0F12FFF2,	//awbb_GridCorr_B_Out[0][1]   FFE2
0x0F120000,	//awbb_GridCorr_B_Out[0][2]   0000
0x0F120000,	//awbb_GridCorr_B_Out[0][3]   0000
0x0F120000,	//awbb_GridCorr_B_Out[0][4]   0000
0x0F120000,	//awbb_GridCorr_B_Out[0][5]   0000
0x0F12FFF2,	//awbb_GridCorr_B_Out[1][0]   FFE2
0x0F12FFF2,	//awbb_GridCorr_B_Out[1][1]   FFE2
0x0F120000,	//awbb_GridCorr_B_Out[1][2]   0000
0x0F120000,	//awbb_GridCorr_B_Out[1][3]   0000
0x0F120000,	//awbb_GridCorr_B_Out[1][4]   0000
0x0F120000,	//awbb_GridCorr_B_Out[1][5]   0000
0x0F12FFF2,	//awbb_GridCorr_B_Out[2][0]   FFE2
0x0F12FFF2,	//awbb_GridCorr_B_Out[2][1]   FFE2
0x0F120000,	//awbb_GridCorr_B_Out[2][2]   0000
0x0F120000,	//awbb_GridCorr_B_Out[2][3]   0000
0x0F120000,	//awbb_GridCorr_B_Out[2][4]   0000
0x0F120000,	//awbb_GridCorr_B_Out[2][5]   0000


//===================================================================
// CCM
//===================================================================
0x002A07D2,
0x0F1200C0,	//SARR_AwbCcmCord_0_
0x0F1200E0,	//SARR_AwbCcmCord_1_
0x0F120110,	//SARR_AwbCcmCord_2_
0x0F120139,	//SARR_AwbCcmCord_3_
0x0F120166,	//SARR_AwbCcmCord_4_
0x0F12019F,	//SARR_AwbCcmCord_5_

//	param_start	TVAR_wbt_pBaseCcms
0x002A07C4,
0x0F124000,	//TVAR_wbt_pBaseCcms
0x0F127000,

0x002A4000,
0x0F1201EA,	//01E2	//TVAR_wbt_pBaseCcms[0] 
0x0F12FFAC,	//FF9A	//TVAR_wbt_pBaseCcms[1] 
0x0F12FFE3,	//FFE8	//TVAR_wbt_pBaseCcms[2] 
0x0F12FF45,	//FF45	//TVAR_wbt_pBaseCcms[3] 
0x0F120140,	//0140	//TVAR_wbt_pBaseCcms[4] 
0x0F12FF4F,	//FF4F	//TVAR_wbt_pBaseCcms[5] 
0x0F12FFC3,	//FFC3	//TVAR_wbt_pBaseCcms[6] 
0x0F12FFD5,	//FFD5	//TVAR_wbt_pBaseCcms[7] 
0x0F120173,	//0173	//TVAR_wbt_pBaseCcms[8] 
0x0F120137,	//0128	//TVAR_wbt_pBaseCcms[9] 
0x0F1200C2,	//00EA	//TVAR_wbt_pBaseCcms[10]
0x0F12FEC1,	//FEA8	//TVAR_wbt_pBaseCcms[11]
0x0F1200C8,	//00C8	//TVAR_wbt_pBaseCcms[12]
0x0F12FF49,	//FF49	//TVAR_wbt_pBaseCcms[13]
0x0F12014B,	//014B	//TVAR_wbt_pBaseCcms[14]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[15]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[16]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[17]

0x0F1201EA,	//01E2	//TVAR_wbt_pBaseCcms[18]
0x0F12FFAC,	//FF9A	//TVAR_wbt_pBaseCcms[19]
0x0F12FFE3,	//FFE8	//TVAR_wbt_pBaseCcms[20]
0x0F12FF45,	//FF45	//TVAR_wbt_pBaseCcms[21]
0x0F120140,	//0140	//TVAR_wbt_pBaseCcms[22]
0x0F12FF4F,	//FF4F	//TVAR_wbt_pBaseCcms[23]
0x0F12FFC3,	//FFC3	//TVAR_wbt_pBaseCcms[24]
0x0F12FFD5,	//FFD5	//TVAR_wbt_pBaseCcms[25]
0x0F120173,	//0173	//TVAR_wbt_pBaseCcms[26]
0x0F120137,	//0128	//TVAR_wbt_pBaseCcms[27]
0x0F1200C2,	//00EA	//TVAR_wbt_pBaseCcms[28]
0x0F12FEC1,	//FEA8	//TVAR_wbt_pBaseCcms[29]
0x0F1200C8,	//00C8	//TVAR_wbt_pBaseCcms[30]
0x0F12FF49,	//FF49	//TVAR_wbt_pBaseCcms[31]
0x0F12014B,	//014B	//TVAR_wbt_pBaseCcms[32]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[33]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[34]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[35]

0x0F1201EA,	//01E2	//TVAR_wbt_pBaseCcms[36]
0x0F12FFAC,	//FF9A	//TVAR_wbt_pBaseCcms[37]
0x0F12FFE3,	//FFE8	//TVAR_wbt_pBaseCcms[38]
0x0F12FF45,	//FF45	//TVAR_wbt_pBaseCcms[39]
0x0F120140,	//0140	//TVAR_wbt_pBaseCcms[40]
0x0F12FF4F,	//FF4F	//TVAR_wbt_pBaseCcms[41]
0x0F12FFC3,	//FFC3	//TVAR_wbt_pBaseCcms[42]
0x0F12FFD5,	//FFD5	//TVAR_wbt_pBaseCcms[43]
0x0F120173,	//0173	//TVAR_wbt_pBaseCcms[44]
0x0F120137,	//0128	//TVAR_wbt_pBaseCcms[45]
0x0F1200C2,	//00EA	//TVAR_wbt_pBaseCcms[46]
0x0F12FEC1,	//FEA8	//TVAR_wbt_pBaseCcms[47]
0x0F1200C8,	//00C8	//TVAR_wbt_pBaseCcms[48]
0x0F12FF49,	//FF49	//TVAR_wbt_pBaseCcms[49]
0x0F12014B,	//014B	//TVAR_wbt_pBaseCcms[50]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[51]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[52]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[53]

0x0F1201EA,	//01E2	//TVAR_wbt_pBaseCcms[54]
0x0F12FFAC,	//FF9A	//TVAR_wbt_pBaseCcms[55]
0x0F12FFE3,	//FFE8	//TVAR_wbt_pBaseCcms[56]
0x0F12FF45,	//FF45	//TVAR_wbt_pBaseCcms[57]
0x0F120140,	//0140	//TVAR_wbt_pBaseCcms[58]
0x0F12FF4F,	//FF4F	//TVAR_wbt_pBaseCcms[59]
0x0F12FFC3,	//FFC3	//TVAR_wbt_pBaseCcms[60]
0x0F12FFD5,	//FFD5	//TVAR_wbt_pBaseCcms[61]
0x0F120173,	//0173	//TVAR_wbt_pBaseCcms[62]
0x0F120137,	//0128	//TVAR_wbt_pBaseCcms[63]
0x0F1200C2,	//00EA	//TVAR_wbt_pBaseCcms[64]
0x0F12FEC1,	//FEA8	//TVAR_wbt_pBaseCcms[65]
0x0F1200C8,	//00C8	//TVAR_wbt_pBaseCcms[66]
0x0F12FF49,	//FF49	//TVAR_wbt_pBaseCcms[67]
0x0F12014B,	//014B	//TVAR_wbt_pBaseCcms[68]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[69]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[70]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[71]

0x0F12011D,	//00F4 //TVAR_wbt_pBaseCcms[72]
0x0F12FFA7,	//FFC2	//TVAR_wbt_pBaseCcms[73]
0x0F12FFEC,	//FFFA	//TVAR_wbt_pBaseCcms[74]
0x0F12FF0D,	//FF3E	//TVAR_wbt_pBaseCcms[75]
0x0F120193,	//0163	//TVAR_wbt_pBaseCcms[76]
0x0F12FF34,	//FF5F	//TVAR_wbt_pBaseCcms[77]
0x0F12FFCE,	//FFCE	//TVAR_wbt_pBaseCcms[78]
0x0F12FFDF,	//FFDF	//TVAR_wbt_pBaseCcms[79]
0x0F12015E,	//015E	//TVAR_wbt_pBaseCcms[80]
0x0F120095,	//0095	//TVAR_wbt_pBaseCcms[81]
0x0F120096,	//0096	//TVAR_wbt_pBaseCcms[82]
0x0F12FF0B,	//FF0B	//TVAR_wbt_pBaseCcms[83]
0x0F1200C3,	//00C3	//TVAR_wbt_pBaseCcms[84]
0x0F12FF5C,	//FF5C	//TVAR_wbt_pBaseCcms[85]
0x0F12013D,	//013D	//TVAR_wbt_pBaseCcms[86]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[87]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[88]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[89]

0x0F12011D,	//00F4 //TVAR_wbt_pBaseCcms[90]
0x0F12FFA7,	//FFC2	//TVAR_wbt_pBaseCcms[91]
0x0F12FFEC,	//FFFA	//TVAR_wbt_pBaseCcms[92]
0x0F12FF0D,	//FF3E	//TVAR_wbt_pBaseCcms[93]
0x0F120193,	//0163	//TVAR_wbt_pBaseCcms[94]
0x0F12FF34,	//FF5F	//TVAR_wbt_pBaseCcms[95]
0x0F12FFCE,	//FFCE	//TVAR_wbt_pBaseCcms[96]
0x0F12FFDF,	//FFDF	//TVAR_wbt_pBaseCcms[97]
0x0F12015E,	//015E	//TVAR_wbt_pBaseCcms[98]
0x0F120095,	//0095	//TVAR_wbt_pBaseCcms[99]
0x0F120096,	//0096	//TVAR_wbt_pBaseCcms[100]
0x0F12FF0B,	//FF0B	//TVAR_wbt_pBaseCcms[101]
0x0F1200C3,	//00C3	//TVAR_wbt_pBaseCcms[102]
0x0F12FF5C,	//FF5C	//TVAR_wbt_pBaseCcms[103]
0x0F12013D,	//013D	//TVAR_wbt_pBaseCcms[104]
0x0F12FF68,	//FF68	//TVAR_wbt_pBaseCcms[105]
0x0F120109,	//0109	//TVAR_wbt_pBaseCcms[106]
0x0F1200F4,	//00F4	//TVAR_wbt_pBaseCcms[107]
//	param_end	TVAR_wbt_pBasecms


0x002A07CC,
0x0F1240D8,	//#TVAR_wbt_pOutdoorCcm
0x0F127000,

//	param_start	TVAR_wbt_pOutdoorCcm
0x002A40D8,
0x0F1201F5,	//01F6	//01F8	//TVAR_wbt_pOutdoorCcm[0]  01F6 01F2
0x0F12FFA5,	//FF9F	//FFAF	//TVAR_wbt_pOutdoorCcm[1]  FF9F FFA1
0x0F12FFE0,	//FFE5	//FFD3	//TVAR_wbt_pOutdoorCcm[2]  FFE5 FFE7
0x0F12FEC7,	//FED2	//FEC4	//TVAR_wbt_pOutdoorCcm[3]  FED2 FEC7
0x0F12018F,	//0193	//0191	//TVAR_wbt_pOutdoorCcm[4]  0193 018F
0x0F12FF32,	//FF23	//FF33	//TVAR_wbt_pOutdoorCcm[5]  FF23 FF32
0x0F12FFFD,	//FFF7	//FFED	//TVAR_wbt_pOutdoorCcm[6]  FFF7 0002
0x0F120011,	//000C	//0017	//TVAR_wbt_pOutdoorCcm[7]  000C 000C
0x0F120207,	//0211	//0210	//TVAR_wbt_pOutdoorCcm[8]  0211 0207
0x0F120105,	//00F1	//00E3	//TVAR_wbt_pOutdoorCcm[9]  00FF 00FC
0x0F1200F1,	//00FA	//0107	//TVAR_wbt_pOutdoorCcm[10] 00EC 00E9
0x0F12FF39,	//FF2E	//FF2F	//TVAR_wbt_pOutdoorCcm[11] FF2E FF34
0x0F120238,	//0220	//0220	//TVAR_wbt_pOutdoorCcm[12] 0220 0238
0x0F12000D,	//FFE7	//FFE7	//TVAR_wbt_pOutdoorCcm[13] FFE7 000D
0x0F1201BC,	//01A1	//01A1	//TVAR_wbt_pOutdoorCcm[14] 01A1 01BC
0x0F12FED1,	//FEC7	//FEC8	//TVAR_wbt_pOutdoorCcm[15] FEC7 FED1
0x0F120167,	//016D	//017D	//TVAR_wbt_pOutdoorCcm[16] 016D 0167
0x0F12014E,	//0153	//0142	//TVAR_wbt_pOutdoorCcm[17] 0153 014E
//	param_end	TVAR_wbt_pOutdoorCcm


0x002A2A64,
0x0F120001,	//#MVAR_AAIO_bFIT
0x002A2A68,
0x0F120001,	//#MVAR_AAIO_bAutoCCMandASH
0x002A2A3C,
0x0F1201DD,	//#Mon_AAIO_PrevFrmData_NormBr



//===================================================================
// AFIT
//===================================================================

//	param_start	afit_uNoiseIndInDoor
0x002A085C,
0x0F120040,	//0049	//#afit_uNoiseIndInDoor_0_
0x0F120045,	//005F	//#afit_uNoiseIndInDoor_1_
0x0F1200CB,	//00CB	//#afit_uNoiseIndInDoor_2_
0x0F1201C0,	//01E0	//#afit_uNoiseIndInDoor_3_
0x0F120200,	//0220	//#afit_uNoiseIndInDoor_4_

0x002A08C0,
0x0F120007,	//700008C0	//AFIT16_BRIGHTNESS
0x0F120000,	//700008C2  //AFIT16_CONTRAST                                                                                        
0x0F120000,	//700008C4  //AFIT16_SATURATION                                                                                      
0x0F120000,	//700008C6  //AFIT16_SHARP_BLUR                                                                                
0x0F120000,	//700008C8  //AFIT16_GLAMOUR                                                                                         
0x0F1200C1,	//700008CA  //AFIT16_sddd8a_edge_high                                                                                
0x0F120000,	//700008CC        
0x0F1203FF,	//700008CE  //AFIT16_Demosaicing_iSatVal                                                                             
0x0F12009C,	//700008D0  //AFIT16_Sharpening_iReduceEdgeThresh                                                                    
0x0F12017C,	//700008D2  //AFIT16_demsharpmix1_iRGBOffset                                                                         
0x0F1203FF,	//700008D4  //AFIT16_demsharpmix1_iDemClamp                                                                          
0x0F12000C,	//700008D6  //AFIT16_demsharpmix1_iLowThreshold                                                                      
0x0F120010,	//700008D8  //AFIT16_demsharpmix1_iHighThreshold                                                                     
0x0F12012C,	//700008DA  //AFIT16_demsharpmix1_iLowBright                                                                         
0x0F1203E8,	//700008DC  //AFIT16_demsharpmix1_iHighBright                                                                        
0x0F120046,	//700008DE  //AFIT16_demsharpmix1_iLowSat                                                                            
0x0F12005A,	//700008E0  //AFIT16_demsharpmix1_iHighSat                                                                           
0x0F120070,	//700008E2  //AFIT16_demsharpmix1_iTune                                                                              
0x0F120010,	//700008E4	//AFIT16_demsharpmix1_iHystThLow
0x0F120010,	//700008E6	//AFIT16_demsharpmix1_iHystThHigh
0x0F1201F4,	//700008E8	//AFIT16_demsharpmix1_iHystCenter
0x0F12003C,	//700008EA  //AFIT16_Sharpening_iLowSharpClamp                                                                       
0x0F120008,	//700008EC  //AFIT16_Sharpening_iHighSharpClamp                                                                      
0x0F12003C,	//700008EE  //AFIT16_Sharpening_iLowSharpClamp_Bin                                                                   
0x0F12001E,	//700008F0  //AFIT16_Sharpening_iHighSharpClamp_Bin                                                                  
0x0F12003C,	//700008F2  //AFIT16_Sharpening_iLowSharpClamp_sBin                                                                  
0x0F12001E,	//700008F4  //AFIT16_Sharpening_iHighSharpClamp_sBin                                                                 
0x0F120A24,	//700008F6  //AFIT8_sddd8a_edge_low [7:0], AFIT8_sddd8a_repl_thresh [15:8]                                           
0x0F121701,	//700008F8  //AFIT8_sddd8a_repl_force [7:0], AFIT8_sddd8a_sat_level [15:8]                                           
0x0F120229,	//700008FA  //AFIT8_sddd8a_sat_thr[7:0], AFIT8_sddd8a_sat_mpl [15:8]                                                 
0x0F121403,	//700008FC  //AFIT8_sddd8a_sat_noise[7:0], AFIT8_sddd8a_iMaxSlopeAllowed [15:8]                                      
0x0F120004,	//700008FE  //AFIT8_sddd8a_iHotThreshHigh[7:0], AFIT8_sddd8a_iHotThreshLow [15:8]                                    
0x0F120300,	//70000900  //AFIT8_sddd8a_iColdThreshHigh[7:0], AFIT8_sddd8a_iColdThreshLow [15:8]                                  
0x0F120000,	//70000902  //AFIT8_sddd8a_AddNoisePower1[7:0], AFIT8_sddd8a_AddNoisePower2 [15:8]                                   
0x0F1202FF,	//70000904  //AFIT8_sddd8a_iSatSat[7:0], AFIT8_sddd8a_iRadialTune [15:8]                                             
0x0F1209E8,	//70000906  //AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]                                      
0x0F121414,	//70000908  //AFIT8_sddd8a_iLowMaxSlopeAllowed [7:0], AFIT8_sddd8a_iHighMaxSlopeAllowed [15:8]                       
0x0F120301,	//7000090A  //AFIT8_sddd8a_iLowSlopeThresh[7:0], AFIT8_sddd8a_iHighSlopeThresh [15:8]                                
0x0F120007,	//7000090C  //AFIT8_sddd8a_iSquaresRounding [7:0]                                                                    
0x0F124000,	//7000090E        
0x0F127803,	//70000910        
0x0F123C50,	//70000912        
0x0F12003C,	//70000914        
0x0F121E80,	//70000916  //AFIT8_Demosaicing_iCentGrad[7:0], AFIT8_Demosaicing_iMonochrom[15:8]                                   
0x0F121E08,	//70000918  //AFIT8_Demosaicing_iDecisionThresh[7:0], AFIT8_Demosaicing_iDesatThresh[15:8]                           
0x0F12000A,	//7000091A  //AFIT8_Demosaicing_iEnhThresh[7:0], AFIT8_Demosaicing_iGRDenoiseVal[15:8]                               
0x0F120000,	//7000091C  //AFIT8_Demosaicing_iGBDenoiseVal[7:0], AFIT8_Demosaicing_iNearGrayDesat[15:8]                           
0x0F12120A,	//7000091E  //AFIT8_Demosaicing_iDFD_ReduceCoeff[7:0], AFIT8_Sharpening_iMSharpen[15:8]                              
0x0F120F00,	//70000920  //AFIT8_Sharpening_iMShThresh[7:0], AFIT8_Sharpening_iWSharpen[15:8]                                     
0x0F120200,	//70000922  //AFIT8_Sharpening_iWShThresh[7:0], AFIT8_Sharpening_nSharpWidth[15:8]                                   
0x0F12FF00,	//70000924  //AFIT8_Sharpening_iReduceNegative[7:0], AFIT8_Sharpening_iShDespeckle[15:8]                             
0x0F120200,	//70000926  //AFIT8_demsharpmix1_iRGBMultiplier[7:0], AFIT8_demsharpmix1_iFilterPower[15:8]                          
0x0F121B11,	//70000928  //AFIT8_demsharpmix1_iBCoeff[7:0], AFIT8_demsharpmix1_iGCoeff[15:8]                                      
0x0F120000,	//7000092A  //AFIT8_demsharpmix1_iWideMult[7:0], AFIT8_demsharpmix1_iNarrMult[15:8]                                  
0x0F120009,	//7000092C  //AFIT8_demsharpmix1_iHystFalloff[7:0], AFIT8_demsharpmix1_iHystMinMult[15:8]                            
0x0F120406,	//7000092E  //AFIT8_demsharpmix1_iHystWidth[7:0], AFIT8_demsharpmix1_iHystFallLow[15:8]                              
0x0F120605,	//70000930  //AFIT8_demsharpmix1_iHystFallHigh[7:0], AFIT8_demsharpmix1_iHystTune[15:8]                              
0x0F120307,	//70000932        
0x0F120609,	//70000934        
0x0F122C07,	//70000936        
0x0F12142C,	//70000938        
0x0F120718,	//7000093A	//[15:8]iUVNRStrengthL, [7:0]iMaxThreshH
0x0F128007,	//7000093C	//[7:0]iUVNRStrengthH, AFIT8_byr_cgras_iShadingPower[15:8]
0x0F120880,	//7000093E  //AFIT8_RGBGamma2_iLinearity [7:0],  AFIT8_RGBGamma2_iDarkReduce [15:8]
0x0F120B50,	//70000940  //AFIT8_ccm_oscar_iSaturation[7:0],   AFIT8_RGB2YUV_iYOffset [15:8]
0x0F120080,	//70000942  //AFIT8_RGB2YUV_iRGBGain [7:0],   AFIT8_RGB2YUV_iSaturation [15:8]                                       
0x0F120101,	//70000944  //AFIT8_sddd8a_iClustThresh_H [7:0],  AFIT8_sddd8a_iClustThresh_C [15:8]                                 
0x0F120707,	//70000946  //AFIT8_sddd8a_iClustMulT_H [7:0],   AFIT8_sddd8a_iClustMulT_C [15:8]                                    
0x0F124601,	//70000948  //AFIT8_sddd8a_nClustLevel_H [7:0],   AFIT8_sddd8a_DispTH_Low [15:8]                                     
0x0F12C844,	//7000094A  //AFIT8_sddd8a_DispTH_High [7:0],   AFIT8_sddd8a_iDenThreshLow [15:8]
0x0F1250C8,	//7000094C  //AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]
0x0F120500,	//7000094E  //AFIT8_Demosaicing_iEdgeDesatThrLow [7:0],   AFIT8_Demosaicing_iEdgeDesatThrHigh [15:8]                 
0x0F120003,	//70000950  //AFIT8_Demosaicing_iEdgeDesatLimit[7:0],  AFIT8_Demosaicing_iDemSharpenLow [15:8]                       
0x0F121C01,	//70000952  //AFIT8_Demosaicing_iDemSharpenHigh[7:0],   AFIT8_Demosaicing_iDemSharpThresh [15:8]                     
0x0F120714,	//70000954  //AFIT8_Demosaicing_iDemShLowLimit [7:0],   AFIT8_Demosaicing_iDespeckleForDemsharp [15:8]               
0x0F121464,	//70000956  //AFIT8_Demosaicing_iDemBlurLow[7:0],   AFIT8_Demosaicing_iDemBlurHigh [15:8]                            
0x0F125A04,	//70000958  //AFIT8_Demosaicing_iDemBlurRange[7:0],   AFIT8_Sharpening_iLowSharpPower [15:8]                         
0x0F123C1E,	//7000095A  //AFIT8_Sharpening_iHighSharpPower[7:0],   AFIT8_Sharpening_iLowShDenoise [15:8]                         
0x0F12400F,	//7000095C  //AFIT8_Sharpening_iHighShDenoise [7:0],   AFIT8_Sharpening_iReduceEdgeMinMult [15:8]                    
0x0F120204,	//7000095E  //AFIT8_Sharpening_iReduceEdgeSlope [7:0],  AFIT8_demsharpmix1_iWideFiltReduce [15:8]                    
0x0F121403,	//70000960  //AFIT8_demsharpmix1_iNarrFiltReduce [7:0],  AFIT8_sddd8a_iClustThresh_H_Bin [15:8]                      
0x0F120114,	//70000962  //AFIT8_sddd8a_iClustThresh_C_Bin [7:0],   AFIT8_sddd8a_iClustMulT_H_Bin [15:8]                          
0x0F120101,	//70000964  //AFIT8_sddd8a_iClustMulT_C_Bin [7:0],   AFIT8_sddd8a_nClustLevel_H_Bin [15:8]                           
0x0F124446,	//70000966  //AFIT8_sddd8a_DispTH_Low_Bin [7:0],   AFIT8_sddd8a_DispTH_High_Bin [15:8]                               
0x0F12646E,	//70000968  //AFIT8_sddd8a_iDenThreshLow_Bin [7:0],   AFIT8_sddd8a_iDenThreshHigh_Bin [15:8]                         
0x0F120028,	//7000096A  //AFIT8_Demosaicing_iEdgeDesat_Bin[7:0],   AFIT8_Demosaicing_iEdgeDesatThrLow_Bin [15:8]                 
0x0F12030A,	//7000096C  //AFIT8_Demosaicing_iEdgeDesatThrHigh_Bin [7:0],  AFIT8_Demosaicing_iEdgeDesatLimit_Bin [15:8]           
0x0F120000,	//7000096E  //AFIT8_Demosaicing_iDemSharpenLow_Bin [7:0],  AFIT8_Demosaicing_iDemSharpenHigh_Bin [15:8]              
0x0F12141E,	//70000970  //AFIT8_Demosaicing_iDemSharpThresh_Bin [7:0],  AFIT8_Demosaicing_iDemShLowLimit_Bin [15:8]              
0x0F12FF07,	//70000972  //AFIT8_Demosaicing_iDespeckleForDemsharp_Bin [7:0],  AFIT8_Demosaicing_iDemBlurLow_Bin [15:8]           
0x0F120432,	//70000974  //AFIT8_Demosaicing_iDemBlurHigh_Bin [7:0],  AFIT8_Demosaicing_iDemBlurRange_Bin [15:8]                  
0x0F120000,	//70000976  //AFIT8_Sharpening_iLowSharpPower_Bin [7:0],  AFIT8_Sharpening_iHighSharpPower_Bin [15:8]                
0x0F120F0F,	//70000978  //AFIT8_Sharpening_iLowShDenoise_Bin [7:0],  AFIT8_Sharpening_iHighShDenoise_Bin [15:8]                  
0x0F120440,	//7000097A  //AFIT8_Sharpening_iReduceEdgeMinMult_Bin [7:0],  AFIT8_Sharpening_iReduceEdgeSlope_Bin [15:8]           
0x0F120302,	//7000097C  //AFIT8_demsharpmix1_iWideFiltReduce_Bin [7:0],  AFIT8_demsharpmix1_iNarrFiltReduce_Bin [15:8]           
0x0F121414,	//7000097E  //AFIT8_sddd8a_iClustThresh_H_sBin[7:0],   AFIT8_sddd8a_iClustThresh_C_sBin [15:8]                       
0x0F120101,	//70000980  //AFIT8_sddd8a_iClustMulT_H_sBin [7:0],   AFIT8_sddd8a_iClustMulT_C_sBin [15:8]                          
0x0F124601,	//70000982  //AFIT8_sddd8a_nClustLevel_H_sBin [7:0],   AFIT8_sddd8a_DispTH_Low_sBin [15:8]                           
0x0F126E44,	//70000984  //AFIT8_sddd8a_DispTH_High_sBin [7:0],   AFIT8_sddd8a_iDenThreshLow_sBin [15:8]                          
0x0F122864,	//70000986  //AFIT8_sddd8a_iDenThreshHigh_sBin[7:0],   AFIT8_Demosaicing_iEdgeDesat_sBin [15:8]                      
0x0F120A00,	//70000988  //AFIT8_Demosaicing_iEdgeDesatThrLow_sBin [7:0],  AFIT8_Demosaicing_iEdgeDesatThrHigh_sBin [15:8]        
0x0F120003,	//7000098A  //AFIT8_Demosaicing_iEdgeDesatLimit_sBin [7:0],  AFIT8_Demosaicing_iDemSharpenLow_sBin [15:8]            
0x0F121E00,	//7000098C  //AFIT8_Demosaicing_iDemSharpenHigh_sBin [7:0],  AFIT8_Demosaicing_iDemSharpThresh_sBin [15:8]           
0x0F120714,	//7000098E  //AFIT8_Demosaicing_iDemShLowLimit_sBin [7:0],  AFIT8_Demosaicing_iDespeckleForDemsharp_sBin [15:8]      
0x0F1232FF,	//70000990  //AFIT8_Demosaicing_iDemBlurLow_sBin [7:0],  AFIT8_Demosaicing_iDemBlurHigh_sBin [15:8]                  
0x0F120004,	//70000992  //AFIT8_Demosaicing_iDemBlurRange_sBin [7:0],  AFIT8_Sharpening_iLowSharpPower_sBin [15:8]               
0x0F120F00,	//70000994  //AFIT8_Sharpening_iHighSharpPower_sBin [7:0],  AFIT8_Sharpening_iLowShDenoise_sBin [15:8]               
0x0F12400F,	//70000996  //AFIT8_Sharpening_iHighShDenoise_sBin [7:0],  AFIT8_Sharpening_iReduceEdgeMinMult_sBin [15:8]           
0x0F120204,	//70000998  //AFIT8_Sharpening_iReduceEdgeSlope_sBin [7:0],  AFIT8_demsharpmix1_iWideFiltReduce_sBin [15:8]          
0x0F120003,	//7000099A  //AFIT8_demsharpmix1_iNarrFiltReduce_sBin [7:0]                                                          
0x0F120001,	//7000099C        
0x0F120000,	//7000099E	//AFIT16_BRIGHTNESS
0x0F120000,	//700009A0  //AFIT16_CONTRAST                                                                                        
0x0F120000,	//700009A2  //AFIT16_SATURATION                                                                                      
0x0F120000,	//700009A4  //AFIT16_SHARP_BLUR                                                                                      
0x0F120000,	//700009A6  //AFIT16_GLAMOUR                                                                                         
0x0F1200C1,	//700009A8  //AFIT16_sddd8a_edge_high                                                                                
0x0F120000,	//700009AA        
0x0F1203FF,	//700009AC  //AFIT16_Demosaicing_iSatVal                                                                             
0x0F12009C,	//700009AE  //AFIT16_Sharpening_iReduceEdgeThresh                                                                    
0x0F12017C,	//700009B0  //AFIT16_demsharpmix1_iRGBOffset                                                                         
0x0F1203FF,	//700009B2  //AFIT16_demsharpmix1_iDemClamp                                                                          
0x0F12000C,	//700009B4  //AFIT16_demsharpmix1_iLowThreshold                                                                      
0x0F120010,	//700009B6  //AFIT16_demsharpmix1_iHighThreshold                                                                     
0x0F12012C,	//700009B8  //AFIT16_demsharpmix1_iLowBright                                                                         
0x0F1203E8,	//700009BA  //AFIT16_demsharpmix1_iHighBright                                                                        
0x0F120046,	//700009BC  //AFIT16_demsharpmix1_iLowSat                                                                            
0x0F12005A,	//700009BE  //AFIT16_demsharpmix1_iHighSat                                                                           
0x0F120070,	//700009C0  //AFIT16_demsharpmix1_iTune                                                                              
0x0F120001,	//700009C2	//AFIT16_demsharpmix1_iHystThLow
0x0F120000,	//700009C4	//AFIT16_demsharpmix1_iHystThHigh
0x0F120320,	//700009C6	//AFIT16_demsharpmix1_iHystCenter
0x0F12006E,	//700009C8  //AFIT16_Sharpening_iLowSharpClamp                                                                       
0x0F120014,	//700009CA  //AFIT16_Sharpening_iHighSharpClamp                                                                      
0x0F12003C,	//700009CC  //AFIT16_Sharpening_iLowSharpClamp_Bin                                                                   
0x0F12001E,	//700009CE  //AFIT16_Sharpening_iHighSharpClamp_Bin                                                                  
0x0F12003C,	//700009D0  //AFIT16_Sharpening_iLowSharpClamp_sBin                                                                  
0x0F12001E,	//700009D2  //AFIT16_Sharpening_iHighSharpClamp_sBin                                                                 
0x0F120A24,	//700009D4  //AFIT8_sddd8a_edge_low [7:0], AFIT8_sddd8a_repl_thresh [15:8]                                           
0x0F121701,	//700009D6  //AFIT8_sddd8a_repl_force [7:0], AFIT8_sddd8a_sat_level [15:8]                                           
0x0F120229,	//700009D8  //AFIT8_sddd8a_sat_thr[7:0], AFIT8_sddd8a_sat_mpl [15:8]                                                 
0x0F121403,	//700009DA  //AFIT8_sddd8a_sat_noise[7:0], AFIT8_sddd8a_iMaxSlopeAllowed [15:8]                                      
0x0F120004,	//700009DC  //AFIT8_sddd8a_iHotThreshHigh[7:0], AFIT8_sddd8a_iHotThreshLow [15:8]                                    
0x0F120300,	//700009DE  //AFIT8_sddd8a_iColdThreshHigh[7:0], AFIT8_sddd8a_iColdThreshLow [15:8]                                  
0x0F120000,	//700009E0  //AFIT8_sddd8a_AddNoisePower1[7:0], AFIT8_sddd8a_AddNoisePower2 [15:8]                                   
0x0F1202FF,	//700009E2  //AFIT8_sddd8a_iSatSat[7:0], AFIT8_sddd8a_iRadialTune [15:8]                                             
0x0F1205E8,	//700009E4  //AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]                                      
0x0F121414,	//700009E6  //AFIT8_sddd8a_iLowMaxSlopeAllowed [7:0], AFIT8_sddd8a_iHighMaxSlopeAllowed [15:8]                       
0x0F120301,	//700009E8  //AFIT8_sddd8a_iLowSlopeThresh[7:0], AFIT8_sddd8a_iHighSlopeThresh [15:8]                                
0x0F120007,	//700009EA  //AFIT8_sddd8a_iSquaresRounding [7:0]                                                                    
0x0F122000,	//700009EC        
0x0F125003,	//700009EE        
0x0F123228,	//700009F0        
0x0F120032,	//700009F2        
0x0F121E80,	//700009F4  //AFIT8_Demosaicing_iCentGrad[7:0], AFIT8_Demosaicing_iMonochrom[15:8]                                   
0x0F121E08,	//700009F6  //AFIT8_Demosaicing_iDecisionThresh[7:0], AFIT8_Demosaicing_iDesatThresh[15:8]                           
0x0F12000A,	//700009F8  //AFIT8_Demosaicing_iEnhThresh[7:0], AFIT8_Demosaicing_iGRDenoiseVal[15:8]                               
0x0F120000,	//700009FA  //AFIT8_Demosaicing_iGBDenoiseVal[7:0], AFIT8_Demosaicing_iNearGrayDesat[15:8]                           
0x0F12120A,	//700009FC  //AFIT8_Demosaicing_iDFD_ReduceCoeff[7:0], AFIT8_Sharpening_iMSharpen[15:8]                              
0x0F121400,	//700009FE  //AFIT8_Sharpening_iMShThresh[7:0], AFIT8_Sharpening_iWSharpen[15:8]                                     
0x0F120200,	//70000A00  //AFIT8_Sharpening_iWShThresh[7:0], AFIT8_Sharpening_nSharpWidth[15:8]                                   
0x0F12FF00,	//70000A02  //AFIT8_Sharpening_iReduceNegative[7:0], AFIT8_Sharpening_iShDespeckle[15:8]                             
0x0F120200,	//70000A04  //AFIT8_demsharpmix1_iRGBMultiplier[7:0], AFIT8_demsharpmix1_iFilterPower[15:8]                          
0x0F121B11,	//70000A06  //AFIT8_demsharpmix1_iBCoeff[7:0], AFIT8_demsharpmix1_iGCoeff[15:8]                                      
0x0F120000,	//70000A08  //AFIT8_demsharpmix1_iWideMult[7:0], AFIT8_demsharpmix1_iNarrMult[15:8]                                  
0x0F120009,	//70000A0A  //AFIT8_demsharpmix1_iHystFalloff[7:0], AFIT8_demsharpmix1_iHystMinMult[15:8]                            
0x0F120406,	//70000A0C  //AFIT8_demsharpmix1_iHystWidth[7:0], AFIT8_demsharpmix1_iHystFallLow[15:8]                              
0x0F120605,	//70000A0E  //AFIT8_demsharpmix1_iHystFallHigh[7:0], AFIT8_demsharpmix1_iHystTune[15:8]                              
0x0F120307,	//70000A10        
0x0F120609,	//70000A12        
0x0F122C07,	//70000A14        
0x0F12142C,	//70000A16        
0x0F120518,	//70000A18	//[15:8]iUVNRStrengthL, [7:0]iMaxThreshH
0x0F128005,	//70000A1A	//[7:0]iUVNRStrengthH, AFIT8_byr_cgras_iShadingPower[15:8]
0x0F120580,	//70000A1C  //AFIT8_RGBGamma2_iLinearity [7:0],  AFIT8_RGBGamma2_iDarkReduce [15:8]                                  
0x0F120080,	//70000A1E  //AFIT8_ccm_oscar_iSaturation[7:0],   AFIT8_RGB2YUV_iYOffset [15:8]                                      
0x0F120080,	//70000A20  //AFIT8_RGB2YUV_iRGBGain [7:0],   AFIT8_RGB2YUV_iSaturation [15:8]                                       
0x0F120101,	//70000A22  //AFIT8_sddd8a_iClustThresh_H [7:0],  AFIT8_sddd8a_iClustThresh_C [15:8]                                 
0x0F120707,	//70000A24  //AFIT8_sddd8a_iClustMulT_H [7:0],   AFIT8_sddd8a_iClustMulT_C [15:8]                                    
0x0F124B01,	//70000A26  //AFIT8_sddd8a_nClustLevel_H [7:0],   AFIT8_sddd8a_DispTH_Low [15:8]                                     
0x0F12494B,	//70000A28  //AFIT8_sddd8a_DispTH_High [7:0],   AFIT8_sddd8a_iDenThreshLow [15:8]                                    
0x0F125044,	//70000A2A  //AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]                                
0x0F120500,	//70000A2C  //AFIT8_Demosaicing_iEdgeDesatThrLow [7:0],   AFIT8_Demosaicing_iEdgeDesatThrHigh [15:8]                 
0x0F120503,	//70000A2E  //AFIT8_Demosaicing_iEdgeDesatLimit[7:0],  AFIT8_Demosaicing_iDemSharpenLow [15:8]                       
0x0F120D02,	//70000A30  //AFIT8_Demosaicing_iDemSharpenHigh[7:0],   AFIT8_Demosaicing_iDemSharpThresh [15:8]                     
0x0F12071E,	//70000A32  //AFIT8_Demosaicing_iDemShLowLimit [7:0],   AFIT8_Demosaicing_iDespeckleForDemsharp [15:8]               
0x0F121432,	//70000A34  //AFIT8_Demosaicing_iDemBlurLow[7:0],   AFIT8_Demosaicing_iDemBlurHigh [15:8]                            
0x0F125A01,	//70000A36  //AFIT8_Demosaicing_iDemBlurRange[7:0],   AFIT8_Sharpening_iLowSharpPower [15:8]                         
0x0F12281E,	//70000A38  //AFIT8_Sharpening_iHighSharpPower[7:0],   AFIT8_Sharpening_iLowShDenoise [15:8]                         
0x0F12200F,	//70000A3A  //AFIT8_Sharpening_iHighShDenoise [7:0],   AFIT8_Sharpening_iReduceEdgeMinMult [15:8]                    
0x0F120204,	//70000A3C  //AFIT8_Sharpening_iReduceEdgeSlope [7:0],  AFIT8_demsharpmix1_iWideFiltReduce [15:8]                    
0x0F121E03,	//70000A3E  //AFIT8_demsharpmix1_iNarrFiltReduce [7:0],  AFIT8_sddd8a_iClustThresh_H_Bin [15:8]                      
0x0F12011E,	//70000A40  //AFIT8_sddd8a_iClustThresh_C_Bin [7:0],   AFIT8_sddd8a_iClustMulT_H_Bin [15:8]                          
0x0F120101,	//70000A42  //AFIT8_sddd8a_iClustMulT_C_Bin [7:0],   AFIT8_sddd8a_nClustLevel_H_Bin [15:8]                           
0x0F123A3C,	//70000A44  //AFIT8_sddd8a_DispTH_Low_Bin [7:0],   AFIT8_sddd8a_DispTH_High_Bin [15:8]                               
0x0F12585A,	//70000A46  //AFIT8_sddd8a_iDenThreshLow_Bin [7:0],   AFIT8_sddd8a_iDenThreshHigh_Bin [15:8]                         
0x0F120028,	//70000A48  //AFIT8_Demosaicing_iEdgeDesat_Bin[7:0],   AFIT8_Demosaicing_iEdgeDesatThrLow_Bin [15:8]                 
0x0F12030A,	//70000A4A  //AFIT8_Demosaicing_iEdgeDesatThrHigh_Bin [7:0],  AFIT8_Demosaicing_iEdgeDesatLimit_Bin [15:8]           
0x0F120000,	//70000A4C  //AFIT8_Demosaicing_iDemSharpenLow_Bin [7:0],  AFIT8_Demosaicing_iDemSharpenHigh_Bin [15:8]              
0x0F12141E,	//70000A4E  //AFIT8_Demosaicing_iDemSharpThresh_Bin [7:0],  AFIT8_Demosaicing_iDemShLowLimit_Bin [15:8]              
0x0F12FF07,	//70000A50  //AFIT8_Demosaicing_iDespeckleForDemsharp_Bin [7:0],  AFIT8_Demosaicing_iDemBlurLow_Bin [15:8]           
0x0F120432,	//70000A52  //AFIT8_Demosaicing_iDemBlurHigh_Bin [7:0],  AFIT8_Demosaicing_iDemBlurRange_Bin [15:8]                  
0x0F120000,	//70000A54  //AFIT8_Sharpening_iLowSharpPower_Bin [7:0],  AFIT8_Sharpening_iHighSharpPower_Bin [15:8]                
0x0F120F0F,	//70000A56  //AFIT8_Sharpening_iLowShDenoise_Bin [7:0],  AFIT8_Sharpening_iHighShDenoise_Bin [15:8]                  
0x0F120440,	//70000A58  //AFIT8_Sharpening_iReduceEdgeMinMult_Bin [7:0],  AFIT8_Sharpening_iReduceEdgeSlope_Bin [15:8]           
0x0F120302,	//70000A5A  //AFIT8_demsharpmix1_iWideFiltReduce_Bin [7:0],  AFIT8_demsharpmix1_iNarrFiltReduce_Bin [15:8]           
0x0F121E1E,	//70000A5C  //AFIT8_sddd8a_iClustThresh_H_sBin[7:0],   AFIT8_sddd8a_iClustThresh_C_sBin [15:8]                       
0x0F120101,	//70000A5E  //AFIT8_sddd8a_iClustMulT_H_sBin [7:0],   AFIT8_sddd8a_iClustMulT_C_sBin [15:8]                          
0x0F123C01,	//70000A60  //AFIT8_sddd8a_nClustLevel_H_sBin [7:0],   AFIT8_sddd8a_DispTH_Low_sBin [15:8]                           
0x0F125A3A,	//70000A62  //AFIT8_sddd8a_DispTH_High_sBin [7:0],   AFIT8_sddd8a_iDenThreshLow_sBin [15:8]                          
0x0F122858,	//70000A64  //AFIT8_sddd8a_iDenThreshHigh_sBin[7:0],   AFIT8_Demosaicing_iEdgeDesat_sBin [15:8]                      
0x0F120A00,	//70000A66  //AFIT8_Demosaicing_iEdgeDesatThrLow_sBin [7:0],  AFIT8_Demosaicing_iEdgeDesatThrHigh_sBin [15:8]        
0x0F120003,	//70000A68  //AFIT8_Demosaicing_iEdgeDesatLimit_sBin [7:0],  AFIT8_Demosaicing_iDemSharpenLow_sBin [15:8]            
0x0F121E00,	//70000A6A  //AFIT8_Demosaicing_iDemSharpenHigh_sBin [7:0],  AFIT8_Demosaicing_iDemSharpThresh_sBin [15:8]           
0x0F120714,	//70000A6C  //AFIT8_Demosaicing_iDemShLowLimit_sBin [7:0],  AFIT8_Demosaicing_iDespeckleForDemsharp_sBin [15:8]      
0x0F1232FF,	//70000A6E  //AFIT8_Demosaicing_iDemBlurLow_sBin [7:0],  AFIT8_Demosaicing_iDemBlurHigh_sBin [15:8]                  
0x0F120004,	//70000A70  //AFIT8_Demosaicing_iDemBlurRange_sBin [7:0],  AFIT8_Sharpening_iLowSharpPower_sBin [15:8]               
0x0F120F00,	//70000A72  //AFIT8_Sharpening_iHighSharpPower_sBin [7:0],  AFIT8_Sharpening_iLowShDenoise_sBin [15:8]               
0x0F12400F,	//70000A74  //AFIT8_Sharpening_iHighShDenoise_sBin [7:0],  AFIT8_Sharpening_iReduceEdgeMinMult_sBin [15:8]           
0x0F120204,	//70000A76  //AFIT8_Sharpening_iReduceEdgeSlope_sBin [7:0],  AFIT8_demsharpmix1_iWideFiltReduce_sBin [15:8]          
0x0F120003,	//70000A78  //AFIT8_demsharpmix1_iNarrFiltReduce_sBin [7:0]                                                          
0x0F120001,	//70000A7A        
0x0F120000,	//70000A7C	//AFIT16_BRIGHTNESS
0x0F120000,	//70000A7E  //AFIT16_CONTRAST                                                                                        
0x0F120000,	//70000A80  //AFIT16_SATURATION                                                                                      
0x0F120000,	//70000A82  //AFIT16_SHARP_BLUR                                                                                      
0x0F120000,	//70000A84  //AFIT16_GLAMOUR                                                                                         
0x0F1200C1,	//70000A86  //AFIT16_sddd8a_edge_high                                                                                
0x0F120000,	//70000A88        
0x0F1203FF,	//70000A8A  //AFIT16_Demosaicing_iSatVal                                                                             
0x0F12009E,	//70000A8C  //AFIT16_Sharpening_iReduceEdgeThresh                                                                    
0x0F12017C,	//70000A8E  //AFIT16_demsharpmix1_iRGBOffset                                                                         
0x0F1203FF,	//70000A90  //AFIT16_demsharpmix1_iDemClamp                                                                          
0x0F12000C,	//70000A92  //AFIT16_demsharpmix1_iLowThreshold                                                                      
0x0F120010,	//70000A94  //AFIT16_demsharpmix1_iHighThreshold                                                                     
0x0F12012C,	//70000A96  //AFIT16_demsharpmix1_iLowBright                                                                         
0x0F1203E8,	//70000A98  //AFIT16_demsharpmix1_iHighBright                                                                        
0x0F120046,	//70000A9A  //AFIT16_demsharpmix1_iLowSat                                                                            
0x0F12005A,	//70000A9C  //AFIT16_demsharpmix1_iHighSat                                                                           
0x0F120070,	//70000A9E  //AFIT16_demsharpmix1_iTune                                                                              
0x0F120001,	//70000AA0	//AFIT16_demsharpmix1_iHystThLow
0x0F120000,	//70000AA2	//AFIT16_demsharpmix1_iHystThHigh
0x0F120320,	//70000AA4	//AFIT16_demsharpmix1_iHystCenter
0x0F12008C,	//70000AA6  //AFIT16_Sharpening_iLowSharpClamp                                                                       
0x0F120014,	//70000AA8  //AFIT16_Sharpening_iHighSharpClamp                                                                      
0x0F12003C,	//70000AAA  //AFIT16_Sharpening_iLowSharpClamp_Bin                                                                   
0x0F12001E,	//70000AAC  //AFIT16_Sharpening_iHighSharpClamp_Bin                                                                  
0x0F12003C,	//70000AAE  //AFIT16_Sharpening_iLowSharpClamp_sBin                                                                  
0x0F12001E,	//70000AB0  //AFIT16_Sharpening_iHighSharpClamp_sBin                                                                 
0x0F120A24,	//70000AB2  //AFIT8_sddd8a_edge_low [7:0], AFIT8_sddd8a_repl_thresh [15:8]                                           
0x0F121701,	//70000AB4  //AFIT8_sddd8a_repl_force [7:0], AFIT8_sddd8a_sat_level [15:8]                                           
0x0F120229,	//70000AB6  //AFIT8_sddd8a_sat_thr[7:0], AFIT8_sddd8a_sat_mpl [15:8]                                                 
0x0F121403,	//70000AB8  //AFIT8_sddd8a_sat_noise[7:0], AFIT8_sddd8a_iMaxSlopeAllowed [15:8]                                      
0x0F120004,	//70000ABA  //AFIT8_sddd8a_iHotThreshHigh[7:0], AFIT8_sddd8a_iHotThreshLow [15:8]                                    
0x0F120300,	//70000ABC  //AFIT8_sddd8a_iColdThreshHigh[7:0], AFIT8_sddd8a_iColdThreshLow [15:8]                                  
0x0F120000,	//70000ABE  //AFIT8_sddd8a_AddNoisePower1[7:0], AFIT8_sddd8a_AddNoisePower2 [15:8]                                   
0x0F1202FF,	//70000AC0  //AFIT8_sddd8a_iSatSat[7:0], AFIT8_sddd8a_iRadialTune [15:8]                                             
0x0F1205DE,	//70000AC2  //AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]                                      
0x0F121414,	//70000AC4  //AFIT8_sddd8a_iLowMaxSlopeAllowed [7:0], AFIT8_sddd8a_iHighMaxSlopeAllowed [15:8]                       
0x0F120301,	//70000AC6  //AFIT8_sddd8a_iLowSlopeThresh[7:0], AFIT8_sddd8a_iHighSlopeThresh [15:8]                                
0x0F120007,	//70000AC8  //AFIT8_sddd8a_iSquaresRounding [7:0]                                                                    
0x0F121000,	//70000ACA        
0x0F122803,	//70000ACC        
0x0F12261E,	//70000ACE        
0x0F120026,	//70000AD0        
0x0F121E80,	//70000AD2  //AFIT8_Demosaicing_iCentGrad[7:0], AFIT8_Demosaicing_iMonochrom[15:8]                                   
0x0F121E08,	//70000AD4  //AFIT8_Demosaicing_iDecisionThresh[7:0], AFIT8_Demosaicing_iDesatThresh[15:8]                           
0x0F12010A,	//70000AD6  //AFIT8_Demosaicing_iEnhThresh[7:0], AFIT8_Demosaicing_iGRDenoiseVal[15:8]                               
0x0F120001,	//70000AD8  //AFIT8_Demosaicing_iGBDenoiseVal[7:0], AFIT8_Demosaicing_iNearGrayDesat[15:8]                           
0x0F123C0A,	//70000ADA  //AFIT8_Demosaicing_iDFD_ReduceCoeff[7:0], AFIT8_Sharpening_iMSharpen[15:8]                              
0x0F122300,	//70000ADC  //AFIT8_Sharpening_iMShThresh[7:0], AFIT8_Sharpening_iWSharpen[15:8]                                     
0x0F120200,	//70000ADE  //AFIT8_Sharpening_iWShThresh[7:0], AFIT8_Sharpening_nSharpWidth[15:8]                                   
0x0F12FF00,	//70000AE0  //AFIT8_Sharpening_iReduceNegative[7:0], AFIT8_Sharpening_iShDespeckle[15:8]                             
0x0F120200,	//70000AE2  //AFIT8_demsharpmix1_iRGBMultiplier[7:0], AFIT8_demsharpmix1_iFilterPower[15:8]                          
0x0F121B11,	//70000AE4  //AFIT8_demsharpmix1_iBCoeff[7:0], AFIT8_demsharpmix1_iGCoeff[15:8]                                      
0x0F120000,	//70000AE6  //AFIT8_demsharpmix1_iWideMult[7:0], AFIT8_demsharpmix1_iNarrMult[15:8]                                  
0x0F120009,	//70000AE8  //AFIT8_demsharpmix1_iHystFalloff[7:0], AFIT8_demsharpmix1_iHystMinMult[15:8]                            
0x0F120406,	//70000AEA  //AFIT8_demsharpmix1_iHystWidth[7:0], AFIT8_demsharpmix1_iHystFallLow[15:8]                              
0x0F120605,	//70000AEC  //AFIT8_demsharpmix1_iHystFallHigh[7:0], AFIT8_demsharpmix1_iHystTune[15:8]                              
0x0F120307,	//70000AEE        
0x0F120609,	//70000AF0        
0x0F121C07,	//70000AF2        
0x0F121014,	//70000AF4        
0x0F120510,	//70000AF6	//[15:8]iUVNRStrengthL, [7:0]iMaxThreshH
0x0F128005,	//70000AF8	//[7:0]iUVNRStrengthH, AFIT8_byr_cgras_iShadingPower[15:8]
0x0F120080,	//70000AFA  //AFIT8_RGBGamma2_iLinearity [7:0],  AFIT8_RGBGamma2_iDarkReduce [15:8]                                  
0x0F120080,	//70000AFC  //AFIT8_ccm_oscar_iSaturation[7:0],   AFIT8_RGB2YUV_iYOffset [15:8]                                      
0x0F120080,	//70000AFE  //AFIT8_RGB2YUV_iRGBGain [7:0],   AFIT8_RGB2YUV_iSaturation [15:8]                                       
0x0F120101,	//70000B00  //AFIT8_sddd8a_iClustThresh_H [7:0],  AFIT8_sddd8a_iClustThresh_C [15:8]                                 
0x0F120707,	//70000B02  //AFIT8_sddd8a_iClustMulT_H [7:0],   AFIT8_sddd8a_iClustMulT_C [15:8]                                    
0x0F124B01,	//70000B04  //AFIT8_sddd8a_nClustLevel_H [7:0],   AFIT8_sddd8a_DispTH_Low [15:8]                                     
0x0F122A4B,	//70000B06  //AFIT8_sddd8a_DispTH_High [7:0],   AFIT8_sddd8a_iDenThreshLow [15:8]                                    
0x0F125020,	//70000B08  //AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]                                
0x0F120500,	//70000B0A  //AFIT8_Demosaicing_iEdgeDesatThrLow [7:0],   AFIT8_Demosaicing_iEdgeDesatThrHigh [15:8]                 
0x0F121C03,	//70000B0C  //AFIT8_Demosaicing_iEdgeDesatLimit[7:0],  AFIT8_Demosaicing_iDemSharpenLow [15:8]                       
0x0F120D0C,	//70000B0E  //AFIT8_Demosaicing_iDemSharpenHigh[7:0],   AFIT8_Demosaicing_iDemSharpThresh [15:8]                     
0x0F120823,	//70000B10  //AFIT8_Demosaicing_iDemShLowLimit [7:0],   AFIT8_Demosaicing_iDespeckleForDemsharp [15:8]               
0x0F121428,	//70000B12  //AFIT8_Demosaicing_iDemBlurLow[7:0],   AFIT8_Demosaicing_iDemBlurHigh [15:8]                            
0x0F126401,	//70000B14  //AFIT8_Demosaicing_iDemBlurRange[7:0],   AFIT8_Sharpening_iLowSharpPower [15:8]
0x0F12282D,	//70000B16  //AFIT8_Sharpening_iHighSharpPower[7:0],   AFIT8_Sharpening_iLowShDenoise [15:8]
0x0F122012,	//70000B18  //AFIT8_Sharpening_iHighShDenoise [7:0],   AFIT8_Sharpening_iReduceEdgeMinMult [15:8]                    
0x0F120204,	//70000B1A  //AFIT8_Sharpening_iReduceEdgeSlope [7:0],  AFIT8_demsharpmix1_iWideFiltReduce [15:8]                    
0x0F122803,	//70000B1C  //AFIT8_demsharpmix1_iNarrFiltReduce [7:0],  AFIT8_sddd8a_iClustThresh_H_Bin [15:8]                      
0x0F120128,	//70000B1E  //AFIT8_sddd8a_iClustThresh_C_Bin [7:0],   AFIT8_sddd8a_iClustMulT_H_Bin [15:8]                          
0x0F120101,	//70000B20  //AFIT8_sddd8a_iClustMulT_C_Bin [7:0],   AFIT8_sddd8a_nClustLevel_H_Bin [15:8]                           
0x0F122224,	//70000B22  //AFIT8_sddd8a_DispTH_Low_Bin [7:0],   AFIT8_sddd8a_DispTH_High_Bin [15:8]                               
0x0F123236,	//70000B24  //AFIT8_sddd8a_iDenThreshLow_Bin [7:0],   AFIT8_sddd8a_iDenThreshHigh_Bin [15:8]                         
0x0F120028,	//70000B26  //AFIT8_Demosaicing_iEdgeDesat_Bin[7:0],   AFIT8_Demosaicing_iEdgeDesatThrLow_Bin [15:8]                 
0x0F12030A,	//70000B28  //AFIT8_Demosaicing_iEdgeDesatThrHigh_Bin [7:0],  AFIT8_Demosaicing_iEdgeDesatLimit_Bin [15:8]           
0x0F120410,	//70000B2A  //AFIT8_Demosaicing_iDemSharpenLow_Bin [7:0],  AFIT8_Demosaicing_iDemSharpenHigh_Bin [15:8]              
0x0F12141E,	//70000B2C  //AFIT8_Demosaicing_iDemSharpThresh_Bin [7:0],  AFIT8_Demosaicing_iDemShLowLimit_Bin [15:8]              
0x0F12FF07,	//70000B2E  //AFIT8_Demosaicing_iDespeckleForDemsharp_Bin [7:0],  AFIT8_Demosaicing_iDemBlurLow_Bin [15:8]           
0x0F120432,	//70000B30  //AFIT8_Demosaicing_iDemBlurHigh_Bin [7:0],  AFIT8_Demosaicing_iDemBlurRange_Bin [15:8]                  
0x0F124050,	//70000B32  //AFIT8_Sharpening_iLowSharpPower_Bin [7:0],  AFIT8_Sharpening_iHighSharpPower_Bin [15:8]                
0x0F120F0F,	//70000B34  //AFIT8_Sharpening_iLowShDenoise_Bin [7:0],  AFIT8_Sharpening_iHighShDenoise_Bin [15:8]                  
0x0F120440,	//70000B36  //AFIT8_Sharpening_iReduceEdgeMinMult_Bin [7:0],  AFIT8_Sharpening_iReduceEdgeSlope_Bin [15:8]           
0x0F120302,	//70000B38  //AFIT8_demsharpmix1_iWideFiltReduce_Bin [7:0],  AFIT8_demsharpmix1_iNarrFiltReduce_Bin [15:8]           
0x0F122828,	//70000B3A  //AFIT8_sddd8a_iClustThresh_H_sBin[7:0],   AFIT8_sddd8a_iClustThresh_C_sBin [15:8]                       
0x0F120101,	//70000B3C  //AFIT8_sddd8a_iClustMulT_H_sBin [7:0],   AFIT8_sddd8a_iClustMulT_C_sBin [15:8]                          
0x0F122401,	//70000B3E  //AFIT8_sddd8a_nClustLevel_H_sBin [7:0],   AFIT8_sddd8a_DispTH_Low_sBin [15:8]                           
0x0F123622,	//70000B40  //AFIT8_sddd8a_DispTH_High_sBin [7:0],   AFIT8_sddd8a_iDenThreshLow_sBin [15:8]                          
0x0F122832,	//70000B42  //AFIT8_sddd8a_iDenThreshHigh_sBin[7:0],   AFIT8_Demosaicing_iEdgeDesat_sBin [15:8]                      
0x0F120A00,	//70000B44  //AFIT8_Demosaicing_iEdgeDesatThrLow_sBin [7:0],  AFIT8_Demosaicing_iEdgeDesatThrHigh_sBin [15:8]        
0x0F121003,	//70000B46  //AFIT8_Demosaicing_iEdgeDesatLimit_sBin [7:0],  AFIT8_Demosaicing_iDemSharpenLow_sBin [15:8]            
0x0F121E04,	//70000B48  //AFIT8_Demosaicing_iDemSharpenHigh_sBin [7:0],  AFIT8_Demosaicing_iDemSharpThresh_sBin [15:8]           
0x0F120714,	//70000B4A  //AFIT8_Demosaicing_iDemShLowLimit_sBin [7:0],  AFIT8_Demosaicing_iDespeckleForDemsharp_sBin [15:8]      
0x0F1232FF,	//70000B4C  //AFIT8_Demosaicing_iDemBlurLow_sBin [7:0],  AFIT8_Demosaicing_iDemBlurHigh_sBin [15:8]                  
0x0F125004,	//70000B4E  //AFIT8_Demosaicing_iDemBlurRange_sBin [7:0],  AFIT8_Sharpening_iLowSharpPower_sBin [15:8]               
0x0F120F40,	//70000B50  //AFIT8_Sharpening_iHighSharpPower_sBin [7:0],  AFIT8_Sharpening_iLowShDenoise_sBin [15:8]               
0x0F12400F,	//70000B52  //AFIT8_Sharpening_iHighShDenoise_sBin [7:0],  AFIT8_Sharpening_iReduceEdgeMinMult_sBin [15:8]           
0x0F120204,	//70000B54  //AFIT8_Sharpening_iReduceEdgeSlope_sBin [7:0],  AFIT8_demsharpmix1_iWideFiltReduce_sBin [15:8]          
0x0F120003,	//70000B56  //AFIT8_demsharpmix1_iNarrFiltReduce_sBin [7:0]                                                          
0x0F120001,	//70000B58        
0x0F120000,	//70000B5A	//AFIT16_BRIGHTNESS
0x0F120000,	//70000B5C  //AFIT16_CONTRAST                                                                                        
0x0F120000,	//70000B5E  //AFIT16_SATURATION                                                                                      
0x0F120000,	//70000B60  //AFIT16_SHARP_BLUR                                                                                      
0x0F120000,	//70000B62  //AFIT16_GLAMOUR                                                                                         
0x0F1200C1,	//70000B64  //AFIT16_sddd8a_edge_high                                                                                
0x0F120000,	//70000B66        
0x0F1203FF,	//70000B68  //AFIT16_Demosaicing_iSatVal                                                                             
0x0F12009E,	//70000B6A  //AFIT16_Sharpening_iReduceEdgeThresh                                                                    
0x0F12017C,	//70000B6C  //AFIT16_demsharpmix1_iRGBOffset                                                                         
0x0F1203FF,	//70000B6E  //AFIT16_demsharpmix1_iDemClamp                                                                          
0x0F12000C,	//70000B70  //AFIT16_demsharpmix1_iLowThreshold                                                                      
0x0F120010,	//70000B72  //AFIT16_demsharpmix1_iHighThreshold                                                                     
0x0F1200C8,	//70000B74  //AFIT16_demsharpmix1_iLowBright                                                                         
0x0F1203E8,	//70000B76  //AFIT16_demsharpmix1_iHighBright                                                                        
0x0F120046,	//70000B78  //AFIT16_demsharpmix1_iLowSat                                                                            
0x0F120050,	//70000B7A  //AFIT16_demsharpmix1_iHighSat                                                                           
0x0F120070,	//70000B7C  //AFIT16_demsharpmix1_iTune                                                                              
0x0F120001,	//70000B7E	//AFIT16_demsharpmix1_iHystThLow
0x0F120000,	//70000B80	//AFIT16_demsharpmix1_iHystThHigh
0x0F120320,	//70000B82	//AFIT16_demsharpmix1_iHystCenter
0x0F12008C,	//70000B84  //AFIT16_Sharpening_iLowSharpClamp                                                                       
0x0F120014,	//70000B86  //AFIT16_Sharpening_iHighSharpClamp                                                                      
0x0F12002D,	//70000B88  //AFIT16_Sharpening_iLowSharpClamp_Bin                                                                   
0x0F120019,	//70000B8A  //AFIT16_Sharpening_iHighSharpClamp_Bin                                                                  
0x0F12002D,	//70000B8C  //AFIT16_Sharpening_iLowSharpClamp_sBin                                                                  
0x0F120019,	//70000B8E  //AFIT16_Sharpening_iHighSharpClamp_sBin                                                                 
0x0F120A24,	//70000B90  //AFIT8_sddd8a_edge_low [7:0], AFIT8_sddd8a_repl_thresh [15:8]                                           
0x0F121701,	//70000B92  //AFIT8_sddd8a_repl_force [7:0], AFIT8_sddd8a_sat_level [15:8]                                           
0x0F120229,	//70000B94  //AFIT8_sddd8a_sat_thr[7:0], AFIT8_sddd8a_sat_mpl [15:8]                                                 
0x0F121403,	//70000B96  //AFIT8_sddd8a_sat_noise[7:0], AFIT8_sddd8a_iMaxSlopeAllowed [15:8]                                      
0x0F120004,	//70000B98  //AFIT8_sddd8a_iHotThreshHigh[7:0], AFIT8_sddd8a_iHotThreshLow [15:8]                                    
0x0F120300,	//70000B9A  //AFIT8_sddd8a_iColdThreshHigh[7:0], AFIT8_sddd8a_iColdThreshLow [15:8]                                  
0x0F120000,	//70000B9C  //AFIT8_sddd8a_AddNoisePower1[7:0], AFIT8_sddd8a_AddNoisePower2 [15:8]                                   
0x0F1202FF,	//70000B9E  //AFIT8_sddd8a_iSatSat[7:0], AFIT8_sddd8a_iRadialTune [15:8]                                             
0x0F1205DE,	//70000BA0  //AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]                                      
0x0F121414,	//70000BA2  //AFIT8_sddd8a_iLowMaxSlopeAllowed [7:0], AFIT8_sddd8a_iHighMaxSlopeAllowed [15:8]                       
0x0F120301,	//70000BA4  //AFIT8_sddd8a_iLowSlopeThresh[7:0], AFIT8_sddd8a_iHighSlopeThresh [15:8]                                
0x0F120007,	//70000BA6  //AFIT8_sddd8a_iSquaresRounding [7:0]                                                                    
0x0F121000,	//70000BA8        
0x0F122303,	//70000BAA        
0x0F12231A,	//70000BAC        
0x0F120023,	//70000BAE        
0x0F121E80,	//70000BB0  //AFIT8_Demosaicing_iCentGrad[7:0], AFIT8_Demosaicing_iMonochrom[15:8]                                   
0x0F121E08,	//70000BB2  //AFIT8_Demosaicing_iDecisionThresh[7:0], AFIT8_Demosaicing_iDesatThresh[15:8]                           
0x0F12010A,	//70000BB4  //AFIT8_Demosaicing_iEnhThresh[7:0], AFIT8_Demosaicing_iGRDenoiseVal[15:8]                               
0x0F120001,	//70000BB6  //AFIT8_Demosaicing_iGBDenoiseVal[7:0], AFIT8_Demosaicing_iNearGrayDesat[15:8]                           
0x0F123C0A,	//70000BB8  //AFIT8_Demosaicing_iDFD_ReduceCoeff[7:0], AFIT8_Sharpening_iMSharpen[15:8]                              
0x0F122300,	//70000BBA  //AFIT8_Sharpening_iMShThresh[7:0], AFIT8_Sharpening_iWSharpen[15:8]                                     
0x0F120200,	//70000BBC  //AFIT8_Sharpening_iWShThresh[7:0], AFIT8_Sharpening_nSharpWidth[15:8]                                   
0x0F12FF00,	//70000BBE  //AFIT8_Sharpening_iReduceNegative[7:0], AFIT8_Sharpening_iShDespeckle[15:8]                             
0x0F120200,	//70000BC0  //AFIT8_demsharpmix1_iRGBMultiplier[7:0], AFIT8_demsharpmix1_iFilterPower[15:8]                          
0x0F121E10,	//70000BC2  //AFIT8_demsharpmix1_iBCoeff[7:0], AFIT8_demsharpmix1_iGCoeff[15:8]                                      
0x0F120000,	//70000BC4  //AFIT8_demsharpmix1_iWideMult[7:0], AFIT8_demsharpmix1_iNarrMult[15:8]                                  
0x0F120009,	//70000BC6  //AFIT8_demsharpmix1_iHystFalloff[7:0], AFIT8_demsharpmix1_iHystMinMult[15:8]                            
0x0F120406,	//70000BC8  //AFIT8_demsharpmix1_iHystWidth[7:0], AFIT8_demsharpmix1_iHystFallLow[15:8]                              
0x0F120705,	//70000BCA  //AFIT8_demsharpmix1_iHystFallHigh[7:0], AFIT8_demsharpmix1_iHystTune[15:8]                              
0x0F120306,	//70000BCC        
0x0F120509,	//70000BCE        
0x0F122806,	//70000BD0        
0x0F121428,	//70000BD2        
0x0F120518,	//70000BD4	//[15:8]iUVNRStrengthL, [7:0]iMaxThreshH
0x0F128005,	//70000BD6	//[7:0]iUVNRStrengthH, AFIT8_byr_cgras_iShadingPower[15:8]
0x0F120080,	//70000BD8  //AFIT8_RGBGamma2_iLinearity [7:0],  AFIT8_RGBGamma2_iDarkReduce [15:8]                                  
0x0F120080,	//70000BDA  //AFIT8_ccm_oscar_iSaturation[7:0],   AFIT8_RGB2YUV_iYOffset [15:8]                                      
0x0F120080,	//70000BDC  //AFIT8_RGB2YUV_iRGBGain [7:0],   AFIT8_RGB2YUV_iSaturation [15:8]                                       
0x0F120101,	//70000BDE  //AFIT8_sddd8a_iClustThresh_H [7:0],  AFIT8_sddd8a_iClustThresh_C [15:8]                                 
0x0F120707,	//70000BE0  //AFIT8_sddd8a_iClustMulT_H [7:0],   AFIT8_sddd8a_iClustMulT_C [15:8]                                    
0x0F124B01,	//70000BE2  //AFIT8_sddd8a_nClustLevel_H [7:0],   AFIT8_sddd8a_DispTH_Low [15:8]                                     
0x0F122A4B,	//70000BE4  //AFIT8_sddd8a_DispTH_High [7:0],   AFIT8_sddd8a_iDenThreshLow [15:8]                                    
0x0F125020,	//70000BE6  //AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]                                
0x0F120500,	//70000BE8  //AFIT8_Demosaicing_iEdgeDesatThrLow [7:0],   AFIT8_Demosaicing_iEdgeDesatThrHigh [15:8]                 
0x0F121C03,	//70000BEA  //AFIT8_Demosaicing_iEdgeDesatLimit[7:0],  AFIT8_Demosaicing_iDemSharpenLow [15:8]                       
0x0F120D0C,	//70000BEC  //AFIT8_Demosaicing_iDemSharpenHigh[7:0],   AFIT8_Demosaicing_iDemSharpThresh [15:8]                     
0x0F120823,	//70000BEE  //AFIT8_Demosaicing_iDemShLowLimit [7:0],   AFIT8_Demosaicing_iDespeckleForDemsharp [15:8]               
0x0F121428,	//70000BF0  //AFIT8_Demosaicing_iDemBlurLow[7:0],   AFIT8_Demosaicing_iDemBlurHigh [15:8]                            
0x0F126401,	//70000BF2  //AFIT8_Demosaicing_iDemBlurRange[7:0],   AFIT8_Sharpening_iLowSharpPower [15:8]                         
0x0F12282D,	//70000BF4  //AFIT8_Sharpening_iHighSharpPower[7:0],   AFIT8_Sharpening_iLowShDenoise [15:8]
0x0F122012,	//70000BF6  //AFIT8_Sharpening_iHighShDenoise [7:0],   AFIT8_Sharpening_iReduceEdgeMinMult [15:8]                    
0x0F120204,	//70000BF8  //AFIT8_Sharpening_iReduceEdgeSlope [7:0],  AFIT8_demsharpmix1_iWideFiltReduce [15:8]                    
0x0F123C03,	//70000BFA  //AFIT8_demsharpmix1_iNarrFiltReduce [7:0],  AFIT8_sddd8a_iClustThresh_H_Bin [15:8]                      
0x0F12013C,	//70000BFC  //AFIT8_sddd8a_iClustThresh_C_Bin [7:0],   AFIT8_sddd8a_iClustMulT_H_Bin [15:8]                          
0x0F120101,	//70000BFE  //AFIT8_sddd8a_iClustMulT_C_Bin [7:0],   AFIT8_sddd8a_nClustLevel_H_Bin [15:8]                           
0x0F121C1E,	//70000C00  //AFIT8_sddd8a_DispTH_Low_Bin [7:0],   AFIT8_sddd8a_DispTH_High_Bin [15:8]                               
0x0F121E22,	//70000C02  //AFIT8_sddd8a_iDenThreshLow_Bin [7:0],   AFIT8_sddd8a_iDenThreshHigh_Bin [15:8]                         
0x0F120028,	//70000C04  //AFIT8_Demosaicing_iEdgeDesat_Bin[7:0],   AFIT8_Demosaicing_iEdgeDesatThrLow_Bin [15:8]                 
0x0F12030A,	//70000C06  //AFIT8_Demosaicing_iEdgeDesatThrHigh_Bin [7:0],  AFIT8_Demosaicing_iEdgeDesatLimit_Bin [15:8]           
0x0F120214,	//70000C08  //AFIT8_Demosaicing_iDemSharpenLow_Bin [7:0],  AFIT8_Demosaicing_iDemSharpenHigh_Bin [15:8]              
0x0F120E14,	//70000C0A  //AFIT8_Demosaicing_iDemSharpThresh_Bin [7:0],  AFIT8_Demosaicing_iDemShLowLimit_Bin [15:8]              
0x0F12FF06,	//70000C0C  //AFIT8_Demosaicing_iDespeckleForDemsharp_Bin [7:0],  AFIT8_Demosaicing_iDemBlurLow_Bin [15:8]           
0x0F120432,	//70000C0E  //AFIT8_Demosaicing_iDemBlurHigh_Bin [7:0],  AFIT8_Demosaicing_iDemBlurRange_Bin [15:8]                  
0x0F124052,	//70000C10  //AFIT8_Sharpening_iLowSharpPower_Bin [7:0],  AFIT8_Sharpening_iHighSharpPower_Bin [15:8]                
0x0F12150C,	//70000C12  //AFIT8_Sharpening_iLowShDenoise_Bin [7:0],  AFIT8_Sharpening_iHighShDenoise_Bin [15:8]                  
0x0F120440,	//70000C14  //AFIT8_Sharpening_iReduceEdgeMinMult_Bin [7:0],  AFIT8_Sharpening_iReduceEdgeSlope_Bin [15:8]           
0x0F120302,	//70000C16  //AFIT8_demsharpmix1_iWideFiltReduce_Bin [7:0],  AFIT8_demsharpmix1_iNarrFiltReduce_Bin [15:8]           
0x0F123C3C,	//70000C18  //AFIT8_sddd8a_iClustThresh_H_sBin[7:0],   AFIT8_sddd8a_iClustThresh_C_sBin [15:8]                       
0x0F120101,	//70000C1A  //AFIT8_sddd8a_iClustMulT_H_sBin [7:0],   AFIT8_sddd8a_iClustMulT_C_sBin [15:8]                          
0x0F121E01,	//70000C1C  //AFIT8_sddd8a_nClustLevel_H_sBin [7:0],   AFIT8_sddd8a_DispTH_Low_sBin [15:8]                           
0x0F12221C,	//70000C1E  //AFIT8_sddd8a_DispTH_High_sBin [7:0],   AFIT8_sddd8a_iDenThreshLow_sBin [15:8]                          
0x0F12281E,	//70000C20  //AFIT8_sddd8a_iDenThreshHigh_sBin[7:0],   AFIT8_Demosaicing_iEdgeDesat_sBin [15:8]                      
0x0F120A00,	//70000C22  //AFIT8_Demosaicing_iEdgeDesatThrLow_sBin [7:0],  AFIT8_Demosaicing_iEdgeDesatThrHigh_sBin [15:8]        
0x0F121403,	//70000C24  //AFIT8_Demosaicing_iEdgeDesatLimit_sBin [7:0],  AFIT8_Demosaicing_iDemSharpenLow_sBin [15:8]            
0x0F121402,	//70000C26  //AFIT8_Demosaicing_iDemSharpenHigh_sBin [7:0],  AFIT8_Demosaicing_iDemSharpThresh_sBin [15:8]           
0x0F12060E,	//70000C28  //AFIT8_Demosaicing_iDemShLowLimit_sBin [7:0],  AFIT8_Demosaicing_iDespeckleForDemsharp_sBin [15:8]      
0x0F1232FF,	//70000C2A  //AFIT8_Demosaicing_iDemBlurLow_sBin [7:0],  AFIT8_Demosaicing_iDemBlurHigh_sBin [15:8]                  
0x0F125204,	//70000C2C  //AFIT8_Demosaicing_iDemBlurRange_sBin [7:0],  AFIT8_Sharpening_iLowSharpPower_sBin [15:8]               
0x0F120C40,	//70000C2E  //AFIT8_Sharpening_iHighSharpPower_sBin [7:0],  AFIT8_Sharpening_iLowShDenoise_sBin [15:8]               
0x0F124015,	//70000C30  //AFIT8_Sharpening_iHighShDenoise_sBin [7:0],  AFIT8_Sharpening_iReduceEdgeMinMult_sBin [15:8]           
0x0F120204,	//70000C32  //AFIT8_Sharpening_iReduceEdgeSlope_sBin [7:0],  AFIT8_demsharpmix1_iWideFiltReduce_sBin [15:8]          
0x0F120003,	//70000C34  //AFIT8_demsharpmix1_iNarrFiltReduce_sBin [7:0]                                                          
0x0F120001,	//70000C36        
0x0F120000,	//0000	//70000C38	//AFIT16_BRIGHTNESS
0x0F120000,	//0000	//70000C3A	//AFIT16_CONTRAST
0x0F120000,	//0000	//70000C3C	//AFIT16_SATURATION
0x0F120000,	//0000	//70000C3E	//AFIT16_SHARP_BLUR
0x0F120000,	//0000	//70000C40	//AFIT16_GLAMOUR
0x0F1200C1,	//00C1	//70000C42	//AFIT16_sddd8a_edge_high
0x0F120000,	//0000	//70000C44
0x0F1203FF,	//03FF	//70000C46	//AFIT16_Demosaicing_iSatVal
0x0F12009C,	//0008	//70000C48	//AFIT16_Sharpening_iReduceEdgeThresh
0x0F120251,	//017C	//70000C4A	//AFIT16_demsharpmix1_iRGBOffset
0x0F1203FF,	//03FF	//70000C4C	//AFIT16_demsharpmix1_iDemClamp
0x0F12000C,	//000C	//70000C4E	//AFIT16_demsharpmix1_iLowThreshold
0x0F120010,	//0010	//70000C50	//AFIT16_demsharpmix1_iHighThreshold
0x0F120032,	//0032	//70000C52	//AFIT16_demsharpmix1_iLowBright
0x0F12028A,	//028A	//70000C54	//AFIT16_demsharpmix1_iHighBright
0x0F120032,	//0032	//70000C56	//AFIT16_demsharpmix1_iLowSat
0x0F1201F4,	//01F4	//70000C58	//AFIT16_demsharpmix1_iHighSat
0x0F120070,	//0070	//70000C5A	//AFIT16_demsharpmix1_iTune
0x0F120002,	//0002	//70000C5C	//AFIT16_demsharpmix1_iHystThLow
0x0F120000,	//0000	//70000C5E	//AFIT16_demsharpmix1_iHystThHigh
0x0F120320,	//0320	//70000C60	//AFIT16_demsharpmix1_iHystCenter
0x0F120044,	//0070	//70000C62	//AFIT16_Sharpening_iLowSharpClamp
0x0F120014,	//0014	//70000C64	//AFIT16_Sharpening_iHighSharpClamp
0x0F120046,	//0046	//70000C66	//AFIT16_Sharpening_iLowSharpClamp_Bin
0x0F120019,	//0019	//70000C68	//AFIT16_Sharpening_iHighSharpClamp_Bin
0x0F120046,	//0046	//70000C6A	//AFIT16_Sharpening_iLowSharpClamp_sBin
0x0F120019,	//0019	//70000C6C	//AFIT16_Sharpening_iHighSharpClamp_sBin
0x0F120A24,	//0A24	//70000C6E	//AFIT8_sddd8a_edge_low [7:0], AFIT8_sddd8a_repl_thresh [15:8]
0x0F121701,	//1701	//70000C70	//AFIT8_sddd8a_repl_force [7:0], AFIT8_sddd8a_sat_level [15:8]
0x0F120229,	//0229	//70000C72	//AFIT8_sddd8a_sat_thr[7:0], AFIT8_sddd8a_sat_mpl [15:8]
0x0F120503,	//0503	//70000C74	//AFIT8_sddd8a_sat_noise[7:0], AFIT8_sddd8a_iMaxSlopeAllowed [15:8]
0x0F12080F,	//0101	//70000C76	//AFIT8_sddd8a_iHotThreshHigh[7:0], AFIT8_sddd8a_iHotThreshLow [15:8]
0x0F120808,	//0101	//70000C78	//AFIT8_sddd8a_iColdThreshHigh[7:0], AFIT8_sddd8a_iColdThreshLow [15:8]
0x0F120000,	//0000	//70000C7A	//AFIT8_sddd8a_AddNoisePower1[7:0], AFIT8_sddd8a_AddNoisePower2 [15:8]
0x0F1200FF,	//02FF	//70000C7C	//AFIT8_sddd8a_iSatSat[7:0], AFIT8_sddd8a_iRadialTune [15:8]
0x0F120396, //12D 	//0396	//70000C7E	//AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]
0x0F121414,	//1414	//70000C80	//AFIT8_sddd8a_iLowMaxSlopeAllowed [7:0], AFIT8_sddd8a_iHighMaxSlopeAllowed [15:8]
0x0F120301,	//0301	//70000C82	//AFIT8_sddd8a_iLowSlopeThresh[7:0], AFIT8_sddd8a_iHighSlopeThresh [15:8]
0x0F120007,	//0007	//70000C84	//AFIT8_sddd8a_iSquaresRounding [7:0]                        
0x0F121000,	//1000	//70000C86        
0x0F122003,	//2003	//70000C88        
0x0F121020,	//1020	//70000C8A        
0x0F120010,	//0010	//70000C8C        
0x0F121EFF,	//1E80	//70000C8E	//AFIT8_Demosaicing_iCentGrad[7:0], AFIT8_Demosaicing_iMonochrom[15:8]
0x0F121E06,	//1E06	//70000C90	//AFIT8_Demosaicing_iDecisionThresh[7:0], AFIT8_Demosaicing_iDesatThresh[15:8]
0x0F12060A,	//030C	//70000C92	//AFIT8_Demosaicing_iEnhThresh[7:0], AFIT8_Demosaicing_iGRDenoiseVal[15:8]
0x0F120306,	//0103	//70000C94	//AFIT8_Demosaicing_iGBDenoiseVal[7:0], AFIT8_Demosaicing_iNearGrayDesat[15:8]
0x0F128B0A,	//5A0A	//70000C96	//AFIT8_Demosaicing_iDFD_ReduceCoeff[7:0], AFIT8_Sharpening_iMSharpen[15:8]
0x0F122837,	//2D00	//70000C98	//AFIT8_Sharpening_iMShThresh[7:0], AFIT8_Sharpening_iWSharpen[15:8]
0x0F120110,	//0100	//70000C9A	//AFIT8_Sharpening_iWShThresh[7:0], AFIT8_Sharpening_nSharpWidth[15:8]
0x0F12FF00,	//FF00	//70000C9C	//AFIT8_Sharpening_iReduceNegative[7:0], AFIT8_Sharpening_iShDespeckle[15:8]
0x0F120200,	//0200	//70000C9E	//AFIT8_demsharpmix1_iRGBMultiplier[7:0], AFIT8_demsharpmix1_iFilterPower[15:8]
0x0F121E10,	//1E10	//70000CA0	//AFIT8_demsharpmix1_iBCoeff[7:0], AFIT8_demsharpmix1_iGCoeff[15:8]
0x0F120000,	//0000	//70000CA2	//AFIT8_demsharpmix1_iWideMult[7:0], AFIT8_demsharpmix1_iNarrMult[15:8]
0x0F120009,	//0009	//70000CA4	//AFIT8_demsharpmix1_iHystFalloff[7:0], AFIT8_demsharpmix1_iHystMinMult[15:8]
0x0F120406,	//0406	//70000CA6	//AFIT8_demsharpmix1_iHystWidth[7:0], AFIT8_demsharpmix1_iHystFallLow[15:8]
0x0F120705,	//0705	//70000CA8	//AFIT8_demsharpmix1_iHystFallHigh[7:0], AFIT8_demsharpmix1_iHystTune[15:8]
0x0F120305,	//0305	//70000CAA        
0x0F120609,	//0609	//70000CAC        
0x0F122C07,	//2C07	//70000CAE        
0x0F12142C,	//142C	//70000CB0        
0x0F120B18,	//0B18	//70000CB2	//[15:8]iUVNRStrengthL, [7:0]iMaxThreshH        
0x0F12800B,	//800B	//70000CB4	//[7:0]iUVNRStrengthH, AFIT8_byr_cgras_iShadingPower[15:8]        
0x0F120080,	//0080	//70000CB6	//AFIT8_RGBGamma2_iLinearity [7:0],  AFIT8_RGBGamma2_iDarkReduce [15:8]
0x0F120080,	//0080	//70000CB8	//AFIT8_ccm_oscar_iSaturation[7:0],   AFIT8_RGB2YUV_iYOffset [15:8]
0x0F120080,	//0080	//70000CBA	//AFIT8_RGB2YUV_iRGBGain [7:0],   AFIT8_RGB2YUV_iSaturation [15:8]
0x0F125050,	//0101	//70000CBC	//AFIT8_sddd8a_iClustThresh_H [7:0],  AFIT8_sddd8a_iClustThresh_C [15:8]
0x0F120101,	//0A0A	//70000CBE	//AFIT8_sddd8a_iClustMulT_H [7:0],   AFIT8_sddd8a_iClustMulT_C [15:8]
0x0F123201,	//3201	//70000CC0	//AFIT8_sddd8a_nClustLevel_H [7:0],   AFIT8_sddd8a_DispTH_Low [15:8]
0x0F121832,	//1428	//70000CC2	//AFIT8_sddd8a_DispTH_High [7:0],   AFIT8_sddd8a_iDenThreshLow [15:8]
0x0F12100C, //210C	//100C	//70000CC4	//AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]
0x0F120A00,	//0500	//70000CC6	//AFIT8_Demosaicing_iEdgeDesatThrLow [7:0],   AFIT8_Demosaicing_iEdgeDesatThrHigh [15:8]
0x0F121E04,	//1E02	//70000CC8	//AFIT8_Demosaicing_iEdgeDesatLimit[7:0],  AFIT8_Demosaicing_iDemSharpenLow [15:8]
0x0F120A08,	//040C	//70000CCA	//AFIT8_Demosaicing_iDemSharpenHigh[7:0],   AFIT8_Demosaicing_iDemSharpThresh [15:8]
0x0F12070C,	//0828	//70000CCC	//AFIT8_Demosaicing_iDemShLowLimit [7:0],   AFIT8_Demosaicing_iDespeckleForDemsharp [15:8]
0x0F123264,	//5064	//70000CCE	//AFIT8_Demosaicing_iDemBlurLow[7:0],   AFIT8_Demosaicing_iDemBlurHigh [15:8]
0x0F125A02,	//4605	//70000CD0	//AFIT8_Demosaicing_iDemBlurRange[7:0],   AFIT8_Sharpening_iLowSharpPower [15:8]
0x0F121040,	//1E68	//70000CD2	//AFIT8_Sharpening_iHighSharpPower[7:0],   AFIT8_Sharpening_iLowShDenoise [15:8]
0x0F124012,	//201E	//70000CD4	//AFIT8_Sharpening_iHighShDenoise [7:0],   AFIT8_Sharpening_iReduceEdgeMinMult [15:8]
0x0F120604,	//0604	//70000CD6	//AFIT8_Sharpening_iReduceEdgeSlope [7:0],  AFIT8_demsharpmix1_iWideFiltReduce [15:8]
0x0F124606,	//4606	//70000CD8	//AFIT8_demsharpmix1_iNarrFiltReduce [7:0],  AFIT8_sddd8a_iClustThresh_H_Bin [15:8]
0x0F120146,	//0146	//70000CDA	//AFIT8_sddd8a_iClustThresh_C_Bin [7:0],   AFIT8_sddd8a_iClustMulT_H_Bin [15:8]
0x0F120101,	//0101	//70000CDC	//AFIT8_sddd8a_iClustMulT_C_Bin [7:0],   AFIT8_sddd8a_nClustLevel_H_Bin [15:8]
0x0F121C18,	//1C18	//70000CDE	//AFIT8_sddd8a_DispTH_Low_Bin [7:0],   AFIT8_sddd8a_DispTH_High_Bin [15:8]
0x0F121819,	//1819	//70000CE0	//AFIT8_sddd8a_iDenThreshLow_Bin [7:0],   AFIT8_sddd8a_iDenThreshHigh_Bin [15:8]
0x0F120028,	//0028	//70000CE2	//AFIT8_Demosaicing_iEdgeDesat_Bin[7:0],   AFIT8_Demosaicing_iEdgeDesatThrLow_Bin [15:8]
0x0F12030A,	//030A	//70000CE4	//AFIT8_Demosaicing_iEdgeDesatThrHigh_Bin [7:0],  AFIT8_Demosaicing_iEdgeDesatLimit_Bin [15:8]
0x0F120514,	//0514	//70000CE6	//AFIT8_Demosaicing_iDemSharpenLow_Bin [7:0],  AFIT8_Demosaicing_iDemSharpenHigh_Bin [15:8]
0x0F120C14,	//0C14	//70000CE8	//AFIT8_Demosaicing_iDemSharpThresh_Bin [7:0],  AFIT8_Demosaicing_iDemShLowLimit_Bin [15:8]
0x0F12FF05,	//FF05	//70000CEA	//AFIT8_Demosaicing_iDespeckleForDemsharp_Bin [7:0],  AFIT8_Demosaicing_iDemBlurLow_Bin [15:8]
0x0F120432,	//0432	//70000CEC	//AFIT8_Demosaicing_iDemBlurHigh_Bin [7:0],  AFIT8_Demosaicing_iDemBlurRange_Bin [15:8]
0x0F124052,	//4052	//70000CEE	//AFIT8_Sharpening_iLowSharpPower_Bin [7:0],  AFIT8_Sharpening_iHighSharpPower_Bin [15:8]
0x0F121514,	//1514	//70000CF0	//AFIT8_Sharpening_iLowShDenoise_Bin [7:0],  AFIT8_Sharpening_iHighShDenoise_Bin [15:8]
0x0F120440,	//0440	//70000CF2	//AFIT8_Sharpening_iReduceEdgeMinMult_Bin [7:0],  AFIT8_Sharpening_iReduceEdgeSlope_Bin [15:8]
0x0F120302,	//0302	//70000CF4	//AFIT8_demsharpmix1_iWideFiltReduce_Bin [7:0],  AFIT8_demsharpmix1_iNarrFiltReduce_Bin [15:8]
0x0F124646,	//4646	//70000CF6	//AFIT8_sddd8a_iClustThresh_H_sBin[7:0],   AFIT8_sddd8a_iClustThresh_C_sBin [15:8]
0x0F120101,	//0101	//70000CF8	//AFIT8_sddd8a_iClustMulT_H_sBin [7:0],   AFIT8_sddd8a_iClustMulT_C_sBin [15:8]
0x0F121801,	//1801	//70000CFA	//AFIT8_sddd8a_nClustLevel_H_sBin [7:0],   AFIT8_sddd8a_DispTH_Low_sBin [15:8]
0x0F12191C,	//191C	//70000CFC	//AFIT8_sddd8a_DispTH_High_sBin [7:0],   AFIT8_sddd8a_iDenThreshLow_sBin [15:8]
0x0F122818,	//2818	//70000CFE	//AFIT8_sddd8a_iDenThreshHigh_sBin[7:0],   AFIT8_Demosaicing_iEdgeDesat_sBin [15:8]
0x0F120A00,	//0A00	//70000D00	//AFIT8_Demosaicing_iEdgeDesatThrLow_sBin [7:0],  AFIT8_Demosaicing_iEdgeDesatThrHigh_sBin [15:8]
0x0F121403,	//1403	//70000D02	//AFIT8_Demosaicing_iEdgeDesatLimit_sBin [7:0],  AFIT8_Demosaicing_iDemSharpenLow_sBin [15:8]
0x0F121405,	//1405	//70000D04	//AFIT8_Demosaicing_iDemSharpenHigh_sBin [7:0],  AFIT8_Demosaicing_iDemSharpThresh_sBin [15:8]
0x0F12050C,	//050C	//70000D06	//AFIT8_Demosaicing_iDemShLowLimit_sBin [7:0],  AFIT8_Demosaicing_iDespeckleForDemsharp_sBin [15:8]
0x0F1232FF,	//32FF	//70000D08	//AFIT8_Demosaicing_iDemBlurLow_sBin [7:0],  AFIT8_Demosaicing_iDemBlurHigh_sBin [15:8]
0x0F125204,	//5204	//70000D0A	//AFIT8_Demosaicing_iDemBlurRange_sBin [7:0],  AFIT8_Sharpening_iLowSharpPower_sBin [15:8]
0x0F121440,	//1440	//70000D0C	//AFIT8_Sharpening_iHighSharpPower_sBin [7:0],  AFIT8_Sharpening_iLowShDenoise_sBin [15:8]
0x0F124015,	//4015	//70000D0E	//AFIT8_Sharpening_iHighShDenoise_sBin [7:0],  AFIT8_Sharpening_iReduceEdgeMinMult_sBin [15:8]
0x0F120204,	//0204	//70000D10	//AFIT8_Sharpening_iReduceEdgeSlope_sBin [7:0],  AFIT8_demsharpmix1_iWideFiltReduce_sBin [15:8]
0x0F120003,	//0003	//70000D12	//AFIT8_demsharpmix1_iNarrFiltReduce_sBin [7:0]
0x0F120001,	//0001	//70000D14

0x0F12BA7A,	//70000D16
0x0F124FDE,	//70000D18
0x0F12137F,	//70000D1A
0x0F123BDE,	//70000D1C
0x0F12A102,	//70000D1E
0x0F1200B5,	//70000D20

//===================================================================
// Brightness setting
//===================================================================
0x002A1300,
0x0F12019D,
0x002A1306,
0x0F120280,

CAMACQ_MAIN_BTM_OF_DATA


}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_fmt_jpg[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_fmt_yuv422[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA PREVIEW(640*480 / FULL)
//==========================================================
GLOBAL const U32 reg_main_preview[]
#if defined(_CAMACQ_API_C_)
={

//PREVIEW
0xFCFCD000,
0x00287000,

0x002A085C,
0x0F120040,	//0049	//#afit_uNoiseIndInDoor_0_
0x0F120045,	//005F	//#afit_uNoiseIndInDoor_1_

0x002A0C7E,
0x0F120396,	//70000C7E	//AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]

0x002A0CC4,
0x0F12100C,	//70000CC4	//AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]

//PREVIEW
0x002A0208,
0x0F120000,	//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,	//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// SNAPSHOT
//==========================================================

GLOBAL const U32 reg_main_snapshot[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,

0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120001,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

0xFEFE0082,   //160ms 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_highlight[]
#if defined(_CAMACQ_API_C_)
={
//High_capture
0xFCFCD000,
0x00287000,

0x002A0C7E,
0x0F12032D,	//70000C7E	//AFIT8_sddd8a_iRadialLimit [7:0], AFIT8_sddd8a_iRadialPower [15:8]

0x002A0CC4,
0x0F12210E,	//70000CC4	//AFIT8_sddd8a_iDenThreshHigh[7:0],   AFIT8_Demosaicing_iEdgeDesat [15:8]

0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120001,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

0xFEFE0042, //Dealy 66ms
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// MIDLIGHT SNAPSHOT =======> Flash Low light snapshot
//==========================================================
GLOBAL const U32 reg_main_midlight[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// LOWLIGHT SNAPSHOT
//==========================================================
GLOBAL const U32 reg_main_lowlight[]
#if defined(_CAMACQ_API_C_)
={
//Low_capture
0xFCFCD000,
0x00287000,

0x002A085C,
0x0F12004A,	//0049	//#afit_uNoiseIndInDoor_0_
0x0F12004E,	//005F	//#afit_uNoiseIndInDoor_1_


0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120001,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

0xFEFE0082, //Dealy 130ms
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;



/*****************************************************************/
/*********************** N I G H T  M O D E **********************/
/*****************************************************************/

//==========================================================
// CAMERA NIGHTMODE ON
//==========================================================
GLOBAL const U32 reg_main_nightshot_on[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_NIGHT (Night/Center/Br0/Auto/Sharp0/Sat0)
// ==========================================================
0xFCFCD000,
0x00287000,  

0x002A025A,
0x0F1209C4,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10

0x002A034C,
0x0F121388,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10
0x0F121388,	//REG_0TC_CCFG_usMinFrTimeMsecMult10

0x002A01CC,
0x0F120001,       //REG_TC_IPRM_InitParamsUpdated

0x002A0504,
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,
0x0F121A80,		//lt_uMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,

0x002A0514,
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,
0x0F121A80,		//lt_uCapMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,

0x002A0524,
0x0F1201D0,		//lt_uMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202C0,		//lt_uMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uMaxAnGain4		0710h	= 1808d	= x7.0625

0x0F120100,		//lt_uMaxDigGain
0x0F128000,		//lt_uMaxTotGain

0x0F1201D0,		//lt_uCapMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uCapMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202C0,		//lt_uCapMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uCapMaxAnGain4		0710h	= 1808d	= x7.0625

0x0F120100,		//lt_uCapMaxDigGain
0x0F128000,		//lt_uCapMaxTotGain

0x002A08E4,
0x0F120000,	//AFIT16_demsharpmix1_iHystThLow
0x0F120000,	//AFIT16_demsharpmix1_iHystThHigh
0x002A0940,
0x0F121080,	//[15:8] AFIT8_RGB2YUV_iYOffset, [7:0] AFIT8_ccm_oscar_iSaturation

//PREVIEW
0x002A0208,
0x0F120000,	//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,	//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA NIGHTMODE OFF
//==========================================================
GLOBAL const U32 reg_main_nightshot_off[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_OFF
// ==========================================================
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120001,		//Mon_AAIO_bAWB		0: AWB OFF, 1: AWB ON

//	01. Portait / Landscape / Text / Fall Color Off

0x00287000,
0x002A1308,
0x0F120040,  //TVAR_ae_BrAve
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F120000,		//REG_TC_UserSaturation
0x0F120000,		//REG_TC_UserSharpBlur

// metering center

0x002A1316,		//ae_WeightTbl_16

0x0F120000,
0x0F120000,
0x0F120000,
0x0F120000,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120201,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,


//	02. Night / Firework Off

0x00287000,
0x002A025A,
0x0F1203E8,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10
0x0F12014E,	//REG_0TC_PCFG_usMinFrTimeMsecMult10

0x002A034C,
0x0F120535,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10
0x0F12029A,	//REG_0TC_CCFG_usMinFrTimeMsecMult10

0x00287000,
0x002A0504,
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,
0x0F12C350,		//lt_uMaxExp4		C350h = 50000d = 125.0000ms
0x0F120000,

0x002A0514,
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,
0x0F12C350,		//lt_uCapMaxExp4		C350h = 50000d = 125.0000ms
0x0F120000,

0x002A0524,
0x0F1201D0,		//lt_uMaxAnGain1
0x0F1201D0,		//lt_uMaxAnGain2
0x0F1202C0,		//lt_uMaxAnGain3
0x0F120880,		//lt_uMaxAnGain4

0x0F120100,		//lt_uMaxDigGain
0x0F128000,		//lt_uMaxTotGain

0x0F1201D0,		//lt_uCapMaxAnGain1
0x0F1201D0,		//lt_uCapMaxAnGain2
0x0F1202C0,		//lt_uCapMaxAnGain3
0x0F120710,		//lt_uCapMaxAnGain4

0x0F120100,		//lt_uCapMaxDigGain
0x0F128000,		//lt_uCapMaxTotGain

0x002A08E4,
0x0F120010,		//AFIT16_demsharpmix1_iHystThLow
0x0F120010,		//AFIT16_demsharpmix1_iHystThHigh
0x002A0940,
0x0F120B50,		//[15:8] AFIT8_RGB2YUV_iYOffset, [7:0] AFIT8_ccm_oscar_iSaturation


//	03. ISO Auto

0x002A04A4,
0x0F12067F,		//REG_TC_DBG_AutoAlgEnBits		Auto Algorithm Enable
0x002A048C,
0x0F120001,		//REG_SF_USER_FlickerQuant		0:No AFC, 1:50Hz, 2:60Hz
0x0F120001,		//REG_SF_USER_FlickerQuantChanged

0x002A05EA,
0x0F120100,		//lt_bUseSecISODgain

0x002A0486,
0x0F120000,		//REG_SF_USER_IsoType		0:OFF 3:ISO
0x002A048A,
0x0F120001,		//REG_SF_USER_IsoChanged

0x002A3302,
0x0F120000,		//AFIT by Normalized Brightness Tunning Parameter

//PREVIEW
0x002A0208,
0x0F120000,		//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,		//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,		//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,		//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,		//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,		//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged
CAMACQ_MAIN_BTM_OF_DATA

}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// NIGHT-MODE SNAPSHOT
//==========================================================
GLOBAL const U32 reg_main_nightshot[]
#if defined(_CAMACQ_API_C_)
={
//Night_capture
0xFCFCD000,
0x00287000,

0x002A0210,
0x0F120000,	//REG_TC_GP_ActiveCapConfig
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120001,	//REG_TC_GP_EnableCapture
0x0F120001,	//REG_TC_GP_EnableCaptureChanged

0xFEFE012C,   //300ms 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_AUTO (1/10)
//==========================================================
GLOBAL const U32 reg_main_wb_auto[]
#if defined(_CAMACQ_API_C_)
={
//MWB_AUTO
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120001,		//Mon_AAIO_bAWB		AWB ON
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_DAYLIGHT (2/10)
//==========================================================
GLOBAL const U32 reg_main_wb_daylight[]
#if defined(_CAMACQ_API_C_)
={
//MWB_Sunny
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0F120620,		//REG_SF_USER_Rgain
0x0F120001,		//REG_SF_USER_RgainChanged
0x0F120400,		//REG_SF_USER_Ggain
0x0F120001,		//REG_SF_USER_GgainChanged
0x0F120540,		//REG_SF_USER_Bgain
0x0F120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_CLOUDY  (3/10)
//==========================================================
GLOBAL const U32 reg_main_wb_cloudy[]
#if defined(_CAMACQ_API_C_)
={
//MWB_Cloudy
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0F1207A0,               //REG_SF_USER_Rgain
0x0F120001,		//REG_SF_USER_RgainChanged
0x0F120400,		//REG_SF_USER_Ggain
0x0F120001,		//REG_SF_USER_GgainChanged
0x0F120498,               //REG_SF_USER_Bgain
0x0F120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_INCANDESCENT (4/10)
//==========================================================
GLOBAL const U32 reg_main_wb_incandescent[]
#if defined(_CAMACQ_API_C_)
={
//MWB_incandescent
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0F1203C0,		//REG_SF_USER_Rgain
0x0F120001,		//REG_SF_USER_RgainChanged
0x0F120400,		//REG_SF_USER_Ggain
0x0F120001,		//REG_SF_USER_GgainChanged
0x0F120980,		//REG_SF_USER_Bgain
0x0F120001,		//REG_SF_USER_BgainChaged

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (5/10)
//==========================================================
GLOBAL const U32 reg_main_wb_fluorescent[]
#if defined(_CAMACQ_API_C_)
={
//MWB_Flourscent
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0F120560,		//REG_SF_USER_Rgain
0x0F120001,		//REG_SF_USER_RgainChanged
0x0F120400,		//REG_SF_USER_Ggain
0x0F120001,		//REG_SF_USER_GgainChanged
0x0F1208A0,		//REG_SF_USER_Bgain
0x0F120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (6/10)
//==========================================================
GLOBAL const U32 reg_main_wb_twilight[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (7/10)
//==========================================================
GLOBAL const U32 reg_main_wb_tungsten[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (8/10)
//==========================================================
GLOBAL const U32 reg_main_wb_off[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (9/10)
//==========================================================
GLOBAL const U32 reg_main_wb_horizon[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_WB_FLUORESCENT (10/10)
//==========================================================
GLOBAL const U32 reg_main_wb_shade[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_EFFECT_OFF (1/13)
//==========================================================
GLOBAL const U32 reg_main_effect_none[]
#if defined(_CAMACQ_API_C_)
={
//EF_NOMAL
0xFCFCD000,
0x00287000,  

0x002A0648,                                        
0x0F120001,		//skl_af_bPregmOff	Pre/Post Gamma On

0x002A01E2,
0x0F120000,		//REG_TC_GP_SpecialEffects	00:Normal Mode

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_MONO (2/13)
//==========================================================
GLOBAL const U32 reg_main_effect_gray[]
#if defined(_CAMACQ_API_C_)
={
//EF_MONO
0xFCFCD000,
0x00287000,
0x002A01E2,
0x0F120001,		//REG_TC_GP_SpecialEffects	01:Mono Mode
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_NEGATIVE (3/13)
//==========================================================
GLOBAL const U32 reg_main_effect_negative[]
#if defined(_CAMACQ_API_C_)
={
//EF_Negative
0xFCFCD000,
0x00287000,

0x002A0648,                                        
0x0F120001,		//skl_af_bPregmOff	Pre/Post Gamma On

0x002A01E2,
0x0F120003,		//REG_TC_GP_SpecialEffects	03:Negative Mode

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_SOLARIZE (4/13)
//==========================================================
GLOBAL const U32 reg_main_effect_solarize[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_SEPIA (5/13)
//==========================================================
GLOBAL const U32 reg_main_effect_sepia[]
#if defined(_CAMACQ_API_C_)
={
//EF_sepia
0xFCFCD000,
0x00287000,
0x002A0648,                                        
0x0F120001,		//skl_af_bPregmOff	Pre/Post Gamma On
0x002A01E2,
0x0F120004,		//REG_TC_GP_SpecialEffects	04:Sepia Mode
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_POSTERIZE (6/13)
//==========================================================
GLOBAL const U32 reg_main_effect_posterize[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_WHITEBOARD (7/13)
//==========================================================
GLOBAL const U32 reg_main_effect_whiteboard[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_BLACKBOARD (8/13)
//==========================================================
GLOBAL const U32 reg_main_effect_blackboard[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_AQUA (9/13)
//==========================================================
GLOBAL const U32 reg_main_effect_aqua[]
#if defined(_CAMACQ_API_C_)
={
//EF_Aqua
0xFCFCD000,
0x00287000,
0x002A01E2,
0x0F120005,		//REG_TC_GP_SpecialEffects	05:Aqua Mode
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_SHARPEN (10/13)
//==========================================================
GLOBAL const U32 reg_main_effect_sharpen[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_AQUA (11/13)
//==========================================================
GLOBAL const U32 reg_main_effect_vivid[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_GREEN (12/13)
//==========================================================
GLOBAL const U32 reg_main_effect_green[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EFFECT_SKETCH (13/13)
//==========================================================
GLOBAL const U32 reg_main_effect_sketch[]
#if defined(_CAMACQ_API_C_)
={
//EF_sketch
0xFCFCD000,
0x00287000,
0x002A01E2,
0x0F120008,		//REG_TC_GP_SpecialEffects	08:Sketch Mode
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_AEC_MATRIX_METERING (2/2)
//==========================================================
GLOBAL const U32 reg_main_meter_matrix[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A1316,		//ae_WeightTbl_16

0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101,  
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_AEC_CENTERWEIGHTED_METERING (2/2)
//==========================================================
GLOBAL const U32 reg_main_meter_cw[]
#if defined(_CAMACQ_API_C_)
={
//Center   
0xFCFCD000,
0x00287000,             
0x002A1316,		//ae_WeightTbl_16

0x0F120000,
0x0F120000,
0x0F120000,
0x0F120000,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120201,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101, 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_AEC_SPOT_METERING (1/2)
//==========================================================
GLOBAL const U32 reg_main_meter_spot[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A1316,		//ae_WeightTbl_16
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120101, 
0x0F120101, 
0x0F120000, 
0x0F120000, 
0x0F120f01, 
0x0F12010f, 
0x0F120000, 
0x0F120000, 
0x0F120f01, 
0x0F12010f, 
0x0F120000, 
0x0F120000, 
0x0F120101, 
0x0F120101, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_AEC_FRAME_AVERAGE (2/2)
//==========================================================
GLOBAL const U32 reg_main_meter_frame[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A1316,		//ae_WeightTbl_16
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101, 
0x0F120101,  
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FLIP_NONE (1/4)
//==========================================================
GLOBAL const U32 reg_main_flip_none[]
#if defined(_CAMACQ_API_C_)
={
0x002A023E,
0x0F120400,	//REG_0TC_PCFG_usWidth
0x0F120300,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234CC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234AC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A024C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask

0x002A0254,
0x0F120001,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F120298,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS

0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120000,	//REG_0TC_PCFG_uPrevMirror
0x0F120000,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FLIP_VERTICAL (2/4)
//==========================================================
GLOBAL const U32 reg_main_flip_water[]
#if defined(_CAMACQ_API_C_)
={
0x002A023E,
0x0F120400,	//REG_0TC_PCFG_usWidth
0x0F120300,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234CC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234AC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A024C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask

0x002A0254,
0x0F120001,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F120298,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS

0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120002,	//REG_0TC_PCFG_uPrevMirror // 0x0F120002,	//REG_0TC_PCFG_uPrevMirror
0x0F120002,	//REG_0TC_PCFG_uCaptureMirror // 0x0F120002,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FLIP_HORIZ (3/4)
//==========================================================
GLOBAL const U32 reg_main_flip_mirror[]
#if defined(_CAMACQ_API_C_)
={
0x002A023E,
0x0F120400,	//REG_0TC_PCFG_usWidth
0x0F120300,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234CC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234AC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A024C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask

0x002A0254,
0x0F120001,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F120298,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS

0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120001,	//REG_0TC_PCFG_uPrevMirror // 0x0F120001,	//REG_0TC_PCFG_uPrevMirror
0x0F120001,	//REG_0TC_PCFG_uCaptureMirror // 0x0F120001,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_FLIP_SYMMETRIC (4/4)
//==========================================================
GLOBAL const U32 reg_main_flip_water_mirror[]
#if defined(_CAMACQ_API_C_)
={
0x002A023E,
0x0F120400,	//REG_0TC_PCFG_usWidth
0x0F120300,	//REG_0TC_PCFG_usHeight
0x0F120005,	//REG_0TC_PCFG_Format
0x0F1234CC,	//REG_0TC_PCFG_usMaxOut4KHzRate
0x0F1234AC,	//REG_0TC_PCFG_usMinOut4KHzRate

0x002A024C,
0x0F120042,	//REG_0TC_PCFG_PVIMask
0x0F120010,	//REG_0TC_PCFG_OIFMask

0x002A0254,
0x0F120001,	//REG_0TC_PCFG_uClockInd
0x0F120000,	//REG_0TC_PCFG_usFrTimeType
0x0F120001,	//REG_0TC_PCFG_FrRateQualityType
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F120298,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS

0x0F120000,	//REG_0TC_PCFG_bSmearOutput
0x0F120000,	//REG_0TC_PCFG_sSaturation
0x0F120000,	//REG_0TC_PCFG_sSharpBlur
0x0F120000,	//REG_0TC_PCFG_sColorTemp
0x0F120000,	//REG_0TC_PCFG_uDeviceGammaIndex
0x0F120003,	//REG_0TC_PCFG_uPrevMirror // 0x0F120003,	//REG_0TC_PCFG_uPrevMirror
0x0F120003,	//REG_0TC_PCFG_uCaptureMirror // 0x0F120003,	//REG_0TC_PCFG_uCaptureMirror
0x0F120000,	//REG_0TC_PCFG_uRotation
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;



//==========================================================
// CAMERA_CAMCORDER
//==========================================================
GLOBAL const U32 reg_main_camcorder[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A028A,
0x0F12029A,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10	//max frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS
0x0F12029A,	//REG_0TC_PCFG_usMinFrTimeMsecMult10	//min frame time : 30fps 014D 15fps 029a; a6a - 3.75 fps; 0535 - 7.5FPS

//PREVIEW
0x002A0208,
0x0F120001,  //REG_TC_GP_ActivePrevConfig
0x002A0210,  
0x0F120000,  //REG_TC_GP_ActiveCapConfig
0x002A020C,  
0x0F120001,  //REG_TC_GP_PrevOpenAfterChange
0x002A01F4,  
0x0F120001,  //REG_TC_GP_NewConfigSync
0x002A020A,  
0x0F120001,  //REG_TC_GP_PrevConfigChanged
0x002A0212,  
0x0F120001,  //REG_TC_GP_CapConfigChanged
0x002A01E8,  
0x0F120000,  //REG_TC_GP_EnableCapture
0x0F120001,  //REG_TC_GP_EnableCaptureChanged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_FRAMERATE_5FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_5[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_FRAMERATE_7FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_7[]
#if defined(_CAMACQ_API_C_)
={


CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_10FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_10[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_15FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_15[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_20FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_20[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_25FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_25[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_30FPS
//==========================================================
GLOBAL const U32 reg_main_fps_fixed_30[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_FRAMERATE_AUTO_MAX15(5~15fps)
//==========================================================
GLOBAL const U32 reg_main_fps_var_15[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_BRIGHTNESS_LEVEL1 (1/9) :
//==========================================================
GLOBAL const U32 reg_main_brightness_level_0[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F12FF70,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL2 (2/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_1[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F12FF94,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL3 (3/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_2[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F12FFB8,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL4 (4/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_3[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000,
0x002A01D0,
0x0F12FFDC,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL5 (5/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_4[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL6 (6/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_5[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120020,		//REG_TC_UserBrightness 
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL7 (7/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_6[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120040,		//REG_TC_UserBrightness  
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL8 (8/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_7[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D0,
0x0F120060,		//REG_TC_UserBrightness
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_BRIGHTNESS_LEVEL9 (9/9)
//==========================================================
GLOBAL const U32 reg_main_brightness_level_8[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,                  
0x00287000,                  
0x002A01D0,                  
0x0F120080,		//REG_TC_UserBrightness
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL1 (1/9) :
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_0[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL2 (2/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_1[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL3 (3/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_2[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL4 (4/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_3[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL5 (5/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_4[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL6 (6/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_5[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL7 (7/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_6[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL8 (8/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_7[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_EXPOSURE_COMPENSATION_LEVEL9 (9/9)
//==========================================================
GLOBAL const U32 reg_main_expcompensation_level_8[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_AF
//==========================================================
GLOBAL const U32 reg_main_reset_af []
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_nlux []
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_llux []
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af[] // start_af
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_off_af[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


GLOBAL const U32 reg_main_check_af[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_manual_af[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_macro_af[]
#if defined(_CAMACQ_API_C_)
={
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_return_manual_af[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_return_macro_af[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_normal_mode_1[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_normal_mode_2[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_normal_mode_3[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_macro_mode_1[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_macro_mode_2[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_set_af_macro_mode_3[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ISO_AUTO
//==========================================================
GLOBAL const U32 reg_main_iso_auto[]
#if defined(_CAMACQ_API_C_)
={
//ISO_AUTO
0xFCFCD000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ISO_50
//==========================================================
GLOBAL const U32 reg_main_iso_50[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ISO_100
//==========================================================
GLOBAL const U32 reg_main_iso_100[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ISO_200
//==========================================================
GLOBAL const U32 reg_main_iso_200[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ISO_400
//==========================================================
GLOBAL const U32 reg_main_iso_400[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ISO_800
//==========================================================
GLOBAL const U32 reg_main_iso_800[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ISO_1600
//==========================================================
GLOBAL const U32 reg_main_iso_1600[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ISO_3200
//==========================================================
GLOBAL const U32 reg_main_iso_3200[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_AUTO (OFF)
//==========================================================
GLOBAL const U32 reg_main_scene_auto[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_AUTO
// ==========================================================
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0F120001,		//Mon_AAIO_bAWB		0: AWB OFF, 1: AWB ON

//	01. Portait / Landscape / Text / Fall Color Off

0x00287000,
0x002A1308,
0x0F120040,  //TVAR_ae_BrAve
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F120000,		//REG_TC_UserSaturation
0x0F120000,		//REG_TC_UserSharpBlur

// metering center

0x002A1316,		//ae_WeightTbl_16

0x0F120000,
0x0F120000,
0x0F120000,
0x0F120000,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120201,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120101,
0x0F120202,
0x0F120202,
0x0F120101,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120201,
0x0F120202,
0x0F120202,
0x0F120102,
0x0F120101,
0x0F120101,
0x0F120101,
0x0F120101,


//	02. Night / Firework Off

0x00287000,

0x002A025A,
0x0F120535,	//REG_0TC_PCFG_usMaxFrTimeMsecMult10
0x0F12029A,	//REG_0TC_PCFG_usMinFrTimeMsecMult10

0x002A034C,
0x0F120535,	//REG_0TC_CCFG_usMaxFrTimeMsecMult10
0x0F120535,	//REG_0TC_CCFG_usMinFrTimeMsecMult10

0x00287000,
0x002A0504,
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,
0x0F12C350,		//lt_uMaxExp4		C350h = 50000d = 125.0000ms
0x0F120000,

0x002A0514,
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,
0x0F12C350,		//lt_uCapMaxExp4		C350h = 50000d = 125.0000ms
0x0F120000,

0x002A0524,
0x0F1201D0,		//lt_uMaxAnGain1
0x0F1201D0,		//lt_uMaxAnGain2
0x0F1202C0,		//lt_uMaxAnGain3
0x0F120880,		//lt_uMaxAnGain4

0x0F120100,		//lt_uMaxDigGain
0x0F128000,		//lt_uMaxTotGain

0x0F1201D0,		//lt_uCapMaxAnGain1
0x0F1201D0,		//lt_uCapMaxAnGain2
0x0F1202C0,		//lt_uCapMaxAnGain3
0x0F120710,		//lt_uCapMaxAnGain4

0x0F120100,		//lt_uCapMaxDigGain
0x0F128000,		//lt_uCapMaxTotGain

0x002A08E4,
0x0F120010,		//AFIT16_demsharpmix1_iHystThLow
0x0F120010,		//AFIT16_demsharpmix1_iHystThHigh
0x002A0940,
0x0F120B50,		//[15:8] AFIT8_RGB2YUV_iYOffset, [7:0] AFIT8_ccm_oscar_iSaturation


//	03. ISO Auto

0x002A04A4,
0x0F12067F,		//REG_TC_DBG_AutoAlgEnBits		Auto Algorithm Enable
0x002A048C,
0x0F120001,		//REG_SF_USER_FlickerQuant		0:No AFC, 1:50Hz, 2:60Hz
0x0F120001,		//REG_SF_USER_FlickerQuantChanged

0x002A05EA,
0x0F120100,		//lt_bUseSecISODgain

0x002A0486,
0x0F120000,		//REG_SF_USER_IsoType		0:OFF 3:ISO
0x002A048A,
0x0F120001,		//REG_SF_USER_IsoChanged

0x002A3302,
0x0F120000,		//AFIT by Normalized Brightness Tunning Parameter

//PREVIEW
0x002A0208,
0x0F120000,		//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,		//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,		//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,		//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,		//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,		//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	  //REG_TC_GP_EnableCapture
0x0F120001,	  //REG_TC_GP_EnableCaptureChanged

0xFEFE0064,	//Dealy 100ms

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_NIGHT
//==========================================================
GLOBAL const U32 reg_main_scene_night[]
#if defined(_CAMACQ_API_C_)
={

// ==========================================================
// 	CAMERA_SCENE_NIGHT (ISO:Night/Metering:Center/0 EV/WB:Auto/Sharp:0/Sat:0)
// ==========================================================

0xFCFCD000,                                             
0x00287000,                                             
                                                              
0x002A01CC,                                             
0x0F120001,   //REG_TC_IPRM_InitParamsUpdated 

0x002A025A,
0x0F1209C4,   //REG_2TC_PCFG_usMaxFrTimeMsecMult10	  4fps
0x0F12029A,   //REG_2TC_PCFG_usMinFrTimeMsecMult10	 15fps

0x002A034C,
0x0F121388,   //REG_2TC_CCFG_usMaxFrTimeMsecMult10			1388h = 500.0ms =  2fps
0x0F121388,   //REG_2TC_CCFG_usMinFrTimeMsecMult10			1388h = 500.0ms =  2fps

0x002A0504,                                             
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,                                             
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,                                             
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,                                             
0x0F121A80,		//lt_uMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,                                             
                                                              
0x002A0514,                                             
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,                                             
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,                                             
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,                                             
0x0F121A80,		//lt_uCapMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,                                             
                                                              
0x002A0524,                                             
0x0F1201D0,		//lt_uMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202E0,		//lt_uMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uMaxAnGain4		0710h	= 1808d	= x7.0625
                                                              
0x0F120100,		//lt_uMaxDigGain                          
0x0F128000,		//lt_uMaxTotGain                          
                                                              
0x0F1201D0,		//lt_uCapMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uCapMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202E0,		//lt_uCapMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uCapMaxAnGain4		0710h	= 1808d	= x7.0625
                                                              
0x0F120100,		//lt_uCapMaxDigGain                       
0x0F128000,		//lt_uCapMaxTotGain                       
                                                              
0x002A08E4,                                             
0x0F120000,	//AFIT16_demsharpmix1_iHystThLow            
0x0F120000,	//AFIT16_demsharpmix1_iHystThHigh           
0x002A0940,                                             
0x0F120480,	//[15:8] AFIT8_RGB2YUV_iYOffset, [7:0] AFIT8_ccm_oscar_iSaturation
                                                              
// Preview                                                    
0x002A0208,                                             
0x0F120000,	//REG_TC_GP_ActivePrevConfig                
0x002A0210,                                             
0x0F120000,	//REG_TC_GP_ActiveCapConfig                 
0x002A020C,                                             
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange             
0x002A01F4,                                             
0x0F120001,	//REG_TC_GP_NewConfigSync                   
0x002A020A,                                             
0x0F120001,	//REG_TC_GP_PrevConfigChanged               
0x002A0212,                                             
0x0F120001,	//REG_TC_GP_CapConfigChanged                
0x002A01E8,                                             
0x0F120000,	//REG_TC_GP_EnableCapture                   
0x0F120001,	//REG_TC_GP_EnableCaptureChanged            

0xFEFE00C8,	//Dealy 200ms
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


GLOBAL const U32 reg_main_scene_night_dark[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_NIGHT (ISO:Night/Metering:Center/0 EV/WB:Auto/Sharp:0/Sat:0)
// ==========================================================

0xFCFCD000,                                             
0x00287000,                                             
                                                              
0x002A01CC,                                             
0x0F120001,   //REG_TC_IPRM_InitParamsUpdated 

0x002A025A,
0x0F1209C4,   //REG_2TC_PCFG_usMaxFrTimeMsecMult10	  4fps
0x0F12029A,   //REG_2TC_PCFG_usMinFrTimeMsecMult10	 15fps

0x002A034C,
0x0F121388,   //REG_2TC_CCFG_usMaxFrTimeMsecMult10			1388h = 500.0ms =  2fps
0x0F121388,   //REG_2TC_CCFG_usMinFrTimeMsecMult10			1388h = 500.0ms =  2fps

0x002A0504,                                             
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,                                             
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,                                             
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,                                             
0x0F121A80,		//lt_uMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,                                             
                                                              
0x002A0514,                                             
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,                                             
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,                                             
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,                                             
0x0F121A80,		//lt_uCapMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,                                             
                                                              
0x002A0524,                                             
0x0F1201D0,		//lt_uMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202E0,		//lt_uMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uMaxAnGain4		0710h	= 1808d	= x7.0625
                                                              
0x0F120100,		//lt_uMaxDigGain                          
0x0F128000,		//lt_uMaxTotGain                          
                                                              
0x0F1201D0,		//lt_uCapMaxAnGain1		0180h	= 0384d	= x1.5000
0x0F1201D0,		//lt_uCapMaxAnGain2		0180h	= 0384d	= x1.5000
0x0F1202E0,		//lt_uCapMaxAnGain3		0250h	= 0592d	= x2.3125
0x0F120800,		//lt_uCapMaxAnGain4		0710h	= 1808d	= x7.0625
                                                              
0x0F120100,		//lt_uCapMaxDigGain                       
0x0F128000,		//lt_uCapMaxTotGain                       
                                                              
0x002A08E4,                                             
0x0F120000,	//AFIT16_demsharpmix1_iHystThLow            
0x0F120000,	//AFIT16_demsharpmix1_iHystThHigh           
0x002A0940,                                             
0x0F120480,	//[15:8] AFIT8_RGB2YUV_iYOffset, [7:0] AFIT8_ccm_oscar_iSaturation
                                                              
// Preview                                                    
0x002A0208,                                             
0x0F120000,	//REG_TC_GP_ActivePrevConfig                
0x002A0210,                                             
0x0F120000,	//REG_TC_GP_ActiveCapConfig                 
0x002A020C,                                             
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange             
0x002A01F4,                                             
0x0F120001,	//REG_TC_GP_NewConfigSync                   
0x002A020A,                                             
0x0F120001,	//REG_TC_GP_PrevConfigChanged               
0x002A0212,                                             
0x0F120001,	//REG_TC_GP_CapConfigChanged                
0x002A01E8,                                             
0x0F120000,	//REG_TC_GP_EnableCapture                   
0x0F120001,	//REG_TC_GP_EnableCaptureChanged            

0xFEFE00C8,	//Dealy 200ms

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_LANDSCAPE
//==========================================================
GLOBAL const U32 reg_main_scene_landscape[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F12001E,		//REG_TC_UserSaturation
0x0F12000A,		//REG_TC_UserSharpBlur
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_SUNSET
//==========================================================
GLOBAL const U32 reg_main_scene_sunset[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0f120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0f120600,		//REG_SF_USER_Rgain
0x0f120001,		//REG_SF_USER_RgainChanged
0x0f120400,		//REG_SF_USER_Ggain
0x0f120001,		//REG_SF_USER_GgainChanged
0x0f120526,		//REG_SF_USER_Bgain
0x0f120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA

}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_PORTRAIT
//==========================================================
GLOBAL const U32 reg_main_scene_portrait[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F120000,		//REG_TC_UserSaturation
0x0F12FFF6,		//REG_TC_UserSharpBlur
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_SUNRISE
//==========================================================
GLOBAL const U32 reg_main_scene_sunrise[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0f120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0f120555,		//REG_SF_USER_Rgain
0x0f120001,		//REG_SF_USER_RgainChanged
0x0f120400,		//REG_SF_USER_Ggain
0x0f120001,		//REG_SF_USER_GgainChanged
0x0f1207F6,		//REG_SF_USER_Bgain
0x0f120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_INDOOR // == PARTY
//==========================================================
GLOBAL const U32 reg_main_scene_indoor[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_PARTY_INDOOR (ISO:200/Metering:Center/0 EV/WB:Auto/Sharp:0/Sat:+1)
// ==========================================================

0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F12001E,		//REG_TC_UserSaturation
0x0F120000,		//REG_TC_UserSharpBlur

0x002A04A4,
0x0F12065F,		//REG_TC_DBG_AutoAlgEnBits		Auto Flicker Off
0x002A048C,
0x0F120000,		//REG_SF_USER_FlickerQuant		0:No AFC, 1:50Hz, 2:60Hz
0x0F120001,		//REG_SF_USER_FlickerQuantChanged

0x002A0504,
0x0F12C350,		//lt_uMaxExp1		C350h = 50000d = 125.0000ms
0x002A0508,
0x0F12C350,		//lt_uMaxExp2		C350h = 50000d = 125.0000ms
0x002A050C,
0x0F12C350,		//lt_uMaxExp3		C350h = 50000d = 125.0000ms
0x002A0510,
0x0F12C350,		//lt_uMaxExp4		C350h = 50000d = 125.0000ms

0x002A0514,
0x0F12C350,		//lt_uCapMaxExp1		C350h = 50000d = 125.0000ms
0x002A0518,
0x0F12C350,		//lt_uCapMaxExp2		C350h = 50000d = 125.0000ms
0x002A051C,
0x0F12C350,		//lt_uCapMaxExp3		C350h = 50000d = 125.0000ms
0x002A0520,
0x0F12C350,		//lt_uCapMaxExp4		C350h = 50000d = 125.0000ms

0x002A05EA,
0x0F120100,		//lt_bUseSecISODgain

// ISO Gain
0x002A0486,
0x0F120003,		//REG_SF_USER_IsoType		0:OFF 3:ISO
0x0F120400,		//REG_SF_USER_IsoVal
0x0F120001,		//REG_SF_USER_IsoChanged

// AFIT by Normalized Brightness Tuning parameter
0x002A3302,
0x0F120001,		//AFIT by Normalized Brightness Tunning Parameter

//Preview
0x002A0208,
0x0F120000,	//REG_TC_GP_ActivePrevConfig
0x002A020C,
0x0F120001,	//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,	//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,	//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,	//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,	//REG_TC_GP_EnablePreview
0x0F120001,	//REG_TC_GP_EnablePreviewChanged 

0xFEFE0064,	//Dealy 100ms
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_PARTY // == INDOOR
//==========================================================
GLOBAL const U32 reg_main_scene_party[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_SPORTS
//==========================================================
GLOBAL const U32 reg_main_scene_sports[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,

0x002A0504,
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,
0x0F123415,		//lt_uMaxExp2		3415h = 13333d =  33.3325ms
0x002A050C,
0x0F123415,		//lt_uMaxExp3		3415h = 13333d =  33.3325ms
0x002A0510,
0x0F123415,		//lt_uMaxExp4		3415h = 13333d =  33.3325ms

0x002A0514,
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,
0x0F123415,		//lt_uCapMaxExp2		3415h = 13333d =  33.3325ms
0x002A051C,
0x0F123415,		//lt_uCapMaxExp3		3415h = 13333d =  33.3325ms
0x002A0520,
0x0F123415,		//lt_uCapMaxExp4		3415h = 13333d =  33.3325ms
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_BEACH
//==========================================================
GLOBAL const U32 reg_main_scene_beach[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120014,		//REG_TC_UserBrightness
0x002A01D4,
0x0F12001E,		//REG_TC_UserSaturation
0x0F120000,		//REG_TC_UserSharpBlur

0x002A04A4,
0x0F12065F,		//REG_TC_DBG_AutoAlgEnBits		Auto Flicker Off
0x002A048C,
0x0F120000,		//REG_SF_USER_FlickerQuant		0:No AFC, 1:50Hz, 2:60Hz
0x0F120001,		//REG_SF_USER_FlickerQuantChanged

0x002A0504,
0x0F12C350,		//lt_uMaxExp1		C350h = 50000d = 125.0000ms
0x002A0508,
0x0F12C350,		//lt_uMaxExp2		C350h = 50000d = 125.0000ms
0x002A050C,
0x0F12C350,		//lt_uMaxExp3		C350h = 50000d = 125.0000ms
0x002A0510,
0x0F12C350,		//lt_uMaxExp4		C350h = 50000d = 125.0000ms

0x002A0514,
0x0F12C350,		//lt_uCapMaxExp1		C350h = 50000d = 125.0000ms
0x002A0518,
0x0F12C350,		//lt_uCapMaxExp2		C350h = 50000d = 125.0000ms
0x002A051C,
0x0F12C350,		//lt_uCapMaxExp3		C350h = 50000d = 125.0000ms
0x002A0520,
0x0F12C350,		//lt_uCapMaxExp4		C350h = 50000d = 125.0000ms

0x002A05EA,
0x0F120100,		//lt_bUseSecISODgain

// ISO Gain
0x002A0486,
0x0F120003,		//REG_SF_USER_IsoType		0:OFF 3:ISO
0x0F120100,		//REG_SF_USER_IsoVal
0x0F120001,		//REG_SF_USER_IsoChanged

// AFIT by Normalized Brightness Tuning parameter
0x002A3302,
0x0F120001,		//AFIT by Normalized Brightness Tunning Parameter

//Preview
0x002A0208,
0x0F120000,		//REG_TC_GP_ActivePrevConfig
0x002A020C,
0x0F120001,		//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,		//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,		//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,		//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,		//REG_TC_GP_EnablePreview
0x0F120001,		//REG_TC_GP_EnablePreviewChanged 

0xFEFE0064,	//Dealy 100ms

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_SNOW
//==========================================================
GLOBAL const U32 reg_main_scene_snow[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_FALLCOLOR
//==========================================================
GLOBAL const U32 reg_main_scene_fallcolor[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F120032,		//REG_TC_UserSaturation
0x0F12000A,		//REG_TC_UserSharpBlur
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_FIREWORKS
//==========================================================
GLOBAL const U32 reg_main_scene_fireworks[]
#if defined(_CAMACQ_API_C_)
={
// ==========================================================
// 	CAMERA_SCENE_FIREWORK (ISO:50/Metering:Center/0 EV/WB:Auto/Sharp:0/Sat:0)
// ==========================================================

0xFCFCD000,
0x00287000, 

0x002A034C,
0x0F122710,   //REG_2TC_CCFG_usMaxFrTimeMsecMult10			2710h = 1000.0ms =  1fps
0x0F122710,   //REG_2TC_CCFG_usMinFrTimeMsecMult10			2710h = 1000.0ms =  1fps

0x002A01CC,
0x0F120001,   //REG_TC_IPRM_InitParamsUpdated

0x002A0504,
0x0F123415,		//lt_uMaxExp1		3415h = 13333d =  33.3325ms
0x002A0508,
0x0F12681F,		//lt_uMaxExp2		681Fh = 26655d =  66.6375ms
0x002A050C,
0x0F128227,		//lt_uMaxExp3		8227h = 33319d =  83.2975ms
0x002A0510,
0x0F121A80,		//lt_uMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,

0x002A0514,
0x0F123415,		//lt_uCapMaxExp1		3415h = 13333d =  33.3325ms
0x002A0518,
0x0F12681F,		//lt_uCapMaxExp2		681Fh = 26655d =  66.6375ms
0x002A051C,
0x0F128227,		//lt_uCapMaxExp3		8227h = 33319d =  83.2975ms
0x002A0520,
0x0F121A80,		//lt_uCapMaxExp4		00061A80h = 400000d =  1000ms
0x0F120006,

0x002A04A4,
0x0F12065F,		//REG_TC_DBG_AutoAlgEnBits		Auto Flicker Off
0x002A048C,
0x0F120000,		//REG_SF_USER_FlickerQuant		0:No AFC, 1:50Hz, 2:60Hz
0x0F120001,		//REG_SF_USER_FlickerQuantChanged

0x002A05EA,
0x0F120100,		//lt_bUseSecISODgain

// ISO Gain
0x002A0486,
0x0F120003,		//REG_SF_USER_IsoType		0:OFF 3:ISO
0x0F120100,		//REG_SF_USER_IsoVal
0x0F120001,		//REG_SF_USER_IsoChanged

// AFIT by Normalized Brightness Tuning parameter
0x002A3302,
0x0F120001,		//AFIT by Normalized Brightness Tunning Parameter

//Firework Mode Preview
0x002A0208,
0x0F120000,		//REG_TC_GP_ActivePrevConfig
0x002A0210,
0x0F120000,		//REG_TC_GP_ActiveCapConfig
0x002A020C,
0x0F120001,		//REG_TC_GP_PrevOpenAfterChange
0x002A01F4,
0x0F120001,		//REG_TC_GP_NewConfigSync
0x002A020A,
0x0F120001,		//REG_TC_GP_PrevConfigChanged
0x002A0212,
0x0F120001,		//REG_TC_GP_CapConfigChanged
0x002A01E8,
0x0F120000,		//REG_TC_GP_EnablePreview
0x0F120001,		//REG_TC_GP_EnablePreviewChanged 

0xFEFE0064,	//Dealy 100ms

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_CANDLELIGHT
//==========================================================
GLOBAL const U32 reg_main_scene_candlelight[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A2A62,
0x0f120000,		//Mon_AAIO_bAWB		AWB OFF

0x002A0470,
0x0f120600,		//REG_SF_USER_Rgain
0x0f120001,		//REG_SF_USER_RgainChanged
0x0f120400,		//REG_SF_USER_Ggain
0x0f120001,		//REG_SF_USER_GgainChanged
0x0f120526,		//REG_SF_USER_Bgain
0x0f120001,		//REG_SF_USER_BgainChaged
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_AGAINSTLIGHT (BACKLight??)
//==========================================================
GLOBAL const U32 reg_main_scene_againstlight[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A1316,		//ae_WeightTbl_16

0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120101, 
0x0F120101, 
0x0F120000, 
0x0F120000, 
0x0F120f01, 
0x0F12010f, 
0x0F120000, 
0x0F120000, 
0x0F120f01, 
0x0F12010f, 
0x0F120000, 
0x0F120000, 
0x0F120101, 
0x0F120101, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000, 
0x0F120000,
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_SCENE_TEXT
//==========================================================
GLOBAL const U32 reg_main_scene_text[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000,
0x00287000,
0x002A01D0,
0x0F120000,		//REG_TC_UserBrightness
0x002A01D4,
0x0F120000,		//REG_TC_UserSaturation
0x0F120014,		//REG_TC_UserSharpBlur
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_CONTRAST_M2
//==========================================================
GLOBAL const U32 reg_main_adjust_contrast_m2[]
#if defined(_CAMACQ_API_C_)
={

//-4
0xFCFCD000, 
0x00287000, 
0x002A01D2,
0x0F12FF80,		//REG_TC_UserContrast

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_CONTRAST_M1
//==========================================================
GLOBAL const U32 reg_main_adjust_contrast_m1[]
#if defined(_CAMACQ_API_C_)
={
//-2
0xFCFCD000, 
0x00287000, 
0x002A01D2,
0x0F12FFC0,		//REG_TC_UserContrast
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_CONTRAST_DEFAULT
//==========================================================
GLOBAL const U32 reg_main_adjust_contrast_default[]
#if defined(_CAMACQ_API_C_)
={
// 0
0xFCFCD000, 
0x00287000, 
0x002A01D2,
0x0F120000,		//REG_TC_UserContrast

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_CONTRAST_P1
//==========================================================
GLOBAL const U32 reg_main_adjust_contrast_p1[]
#if defined(_CAMACQ_API_C_)
={
//+2
0xFCFCD000, 
0x00287000, 
0x002A01D2,
0x0F120040,		//REG_TC_UserContrast

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_CONTRAST_P2
//==========================================================
GLOBAL const U32 reg_main_adjust_contrast_p2[]
#if defined(_CAMACQ_API_C_)
={

//+4
0xFCFCD000, 
0x00287000, 
0x002A01D2,
0x0F120080,		//REG_TC_UserContrast

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SHARPNESS_M2
//==========================================================
GLOBAL const U32 reg_main_adjust_sharpness_m2[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D6,
0x0F12FFC0,		//REG_TC_UserContrast
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SHARPNESS_M1
//==========================================================
GLOBAL const U32 reg_main_adjust_sharpness_m1[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D6,
0x0F12FFE0,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ADJUST_SHARPNESS_DEFAULT
//==========================================================
GLOBAL const U32 reg_main_adjust_sharpness_default[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D6,
0x0F120000,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ADJUST_SHARPNESS_P1
//==========================================================
GLOBAL const U32 reg_main_adjust_sharpness_p1[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D6,
0x0F120020,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA	
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_ADJUST_SHARPNESS_P2
//==========================================================
GLOBAL const U32 reg_main_adjust_sharpness_p2[]
#if defined(_CAMACQ_API_C_)
={
0xFCFCD000, 
0x00287000, 
0x002A01D6,
0x0F120040,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SATURATION_M2
//==========================================================
GLOBAL const U32 reg_main_adjust_saturation_m2[]
#if defined(_CAMACQ_API_C_)
={
	//-2
0xFCFCD000, 
0x00287000, 
0x002A01D4,
0x0F12FF80,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SATURATION_M1
//==========================================================
GLOBAL const U32 reg_main_adjust_saturation_m1[]
#if defined(_CAMACQ_API_C_)
={
	//-1
0xFCFCD000, 
0x00287000, 
0x002A01D4,
0x0F12FFC0,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SATURATION_DEFAULT
//==========================================================
GLOBAL const U32 reg_main_adjust_saturation_default[]
#if defined(_CAMACQ_API_C_)
={
	// 0
0xFCFCD000, 
0x00287000, 
0x002A01D4,
0x0F120000,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SATURATION_P1
//==========================================================
GLOBAL const U32 reg_main_adjust_saturation_p1[]
#if defined(_CAMACQ_API_C_)
={
	//+1
0xFCFCD000, 
0x00287000, 
0x002A01D4,
0x0F120040,		//REG_TC_UserContrast
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_ADJUST_SATURATION_P2
//==========================================================
GLOBAL const U32 reg_main_adjust_saturation_p2[]
#if defined(_CAMACQ_API_C_)
={
//+4
0xFCFCD000, 
0x00287000, 
0x002A01D4,
0x0F120080,		//REG_TC_UserContrast
CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_output_qqvga
//==========================================================
GLOBAL const U32 reg_main_qqvga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_qcif
//==========================================================
GLOBAL const U32 reg_main_qcif[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_qvga
//==========================================================
GLOBAL const U32 reg_main_qvga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_wqvga
//==========================================================
GLOBAL const U32 reg_main_wqvga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_cif
//==========================================================
GLOBAL const U32 reg_main_cif[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_vga
//==========================================================
GLOBAL const U32 reg_main_vga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_wvga 800 * 480
//==========================================================
GLOBAL const U32 reg_main_wvga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_svga
//==========================================================
GLOBAL const U32 reg_main_svga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_sxga
//==========================================================
GLOBAL const U32 reg_main_sxga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_qxga
//==========================================================
GLOBAL const U32 reg_main_qxga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_qxga
//==========================================================
GLOBAL const U32 reg_main_uxga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_full_yuv
//==========================================================
GLOBAL const U32 reg_main_full_yuv[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_output_cropped_yuv
//==========================================================
GLOBAL const U8 reg_main_crop_yuv[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;


//==========================================================
// CAMERA_JPEG_output_5M
//==========================================================
GLOBAL const U32 reg_main_jpeg_5m[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_5M_2
//==========================================================
GLOBAL const U32 reg_main_jpeg_5m_2[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_4M
//==========================================================
GLOBAL const U32 reg_main_jpeg_w4m[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_3M
//==========================================================
GLOBAL const U32 reg_main_jpeg_3m[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_2M
//==========================================================
GLOBAL const U32 reg_main_jpeg_2m[]
#if defined(_CAMACQ_API_C_)
={


CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_w1_5M
//==========================================================
GLOBAL const U32 reg_main_jpeg_w1_5m[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_1M
//==========================================================
GLOBAL const U32 reg_main_jpeg_1m[]
#if defined(_CAMACQ_API_C_)
={


CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_VGA
//==========================================================
GLOBAL const U32 reg_main_jpeg_vga[]
#if defined(_CAMACQ_API_C_)
={


CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_WQVGA
//==========================================================
GLOBAL const U32 reg_main_jpeg_wqvga[]
#if defined(_CAMACQ_API_C_)
={


CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

//==========================================================
// CAMERA_JPEG_output_QVGA
//==========================================================
GLOBAL const U32 reg_main_jpeg_qvga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_qvga_v[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_half_vga_v[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_half_vga[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_vga_v[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_5M[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_1080P[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_720P[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_not[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_flicker_disabled[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_flicker_50hz[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_flicker_60hz[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_flicker_auto[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_jpeg_quality_superfine[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_jpeg_quality_fine[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_jpeg_quality_normal[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_priv_ctrl_returnpreview[]
#if defined(_CAMACQ_API_C_)
={

CAMACQ_MAIN_BTM_OF_DATA

}
#endif /* _CAMACQ_API_C_ */
;

//BYKIM_DTP
GLOBAL const U32 reg_main_dtp_on[]
#if defined(_CAMACQ_API_C_)
={
	0xFCFCD000,
	0x0028D000,
	0x002AB054,
	0x0F120001,
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

GLOBAL const U32 reg_main_dtp_off[]
#if defined(_CAMACQ_API_C_)
={
	0xFCFCD000,
	0x0028D000,
	0x002AB054,
	0x0F120000,
	CAMACQ_MAIN_BTM_OF_DATA
}
#endif /* _CAMACQ_API_C_ */
;

#undef GLOBAL
#endif /* _CAMACQ_SR200PC10_H_ */
