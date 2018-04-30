
/************************************************************
*   Project Name:  lt8619
*   File Name:     Scaler.h
*   Build Date:    2015-06-18
*   Version£º
*   Copyright:     Lontium Semi.
************************************************************/


#ifndef __SCALER_H__
#define __SCALER_H__

//#define  _640x480_Ouput_
//#define  _854x480_Ouput_
//#define  _800x480_Ouput_
//#define _864x480_Ouput_
//#define _1024x600_Ouput_
//#define _1024x768_Ouput_
//#define _1280x720_Ouput_
#define _1280x800_Ouput_
//#define _1920x1080_Ouput_

#define _LVDS_Outout_
//#define _RGB_Outout_

#ifdef _640x480_Ouput_
  #define Panel_H_Active  640
  #define Panel_V_Active  480
  #define Panel_Pixel_CLK  2520 // 3375 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   2520 // 3375 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  800
  #define Panel_V_Total  525
  #define Panel_H_BackPorch  40 // 112-6
  #define Panel_V_BackPorch  25
  #define Panel_H_SyncWidth  96
  #define Panel_V_SyncWidth  02
#endif /* _640x480_Ouput_ */

#ifdef _800x480_Ouput_
  #define Panel_H_Active  800
  #define Panel_V_Active  480
  #define Panel_Pixel_CLK  3325 // 3375 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   3325 // 3375 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1056
  #define Panel_V_Total  525
  #define Panel_H_BackPorch  16 // 112-6
  #define Panel_V_BackPorch  10
  #define Panel_H_SyncWidth  30
  #define Panel_V_SyncWidth  13
#endif /* _800x480_Ouput_ */

#ifdef _854x480_Ouput_
  #define Panel_H_Active  854
  #define Panel_V_Active  480
  #define Panel_Pixel_CLK  3639 // 3375 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   3639 // 3375 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1088
  #define Panel_V_Total  517
  #define Panel_H_BackPorch  106 // 112-6
  #define Panel_V_BackPorch  23
  #define Panel_H_SyncWidth  112
  #define Panel_V_SyncWidth  8
#endif /* _854x480_Ouput_ */

#ifdef _864x480_Ouput_
  #define Panel_H_Active  864
  #define Panel_V_Active  480
  #define Panel_Pixel_CLK  3639 // 3375 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass  3639// 3375 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total    1088
  #define Panel_V_Total    517
  #define Panel_H_BackPorch  96 // 112-6-10
  #define Panel_V_BackPorch  23
  #define Panel_H_SyncWidth  112
  #define Panel_V_SyncWidth  8
#endif /* _864x480_Ouput_ */

#ifdef _1024x600_Ouput_
  #define Panel_H_Active  1064
  #define Panel_V_Active  600
  /*
  #define Panel_Pixel_CLK  5125 // 3375 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   5125 // 3375 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1344
  #define Panel_V_Total  635
  #define Panel_H_BackPorch  128 // 112-6
  #define Panel_V_BackPorch  20
  #define Panel_H_SyncWidth  32
  #define Panel_V_SyncWidth  3
  //*/
  #define Panel_Pixel_CLK  5125 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   5125 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1344
  #define Panel_V_Total  750
  #define Panel_H_BackPorch  140 
  #define Panel_V_BackPorch  23
  #define Panel_H_SyncWidth  70
  #define Panel_V_SyncWidth  20
#endif /* _1024x600_Ouput_ */

#ifdef _1024x768_Ouput_
  #define Panel_H_Active  1024
  #define Panel_V_Active  768
  #define Panel_Pixel_CLK  6500 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   6500 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1344
  #define Panel_V_Total  806
  #define Panel_H_BackPorch  160 
  #define Panel_V_BackPorch  29
  #define Panel_H_SyncWidth  136
  #define Panel_V_SyncWidth  6
#endif

#ifdef _1280x720_Ouput_
  #define Panel_H_Active  1280
  #define Panel_V_Active  720
  #define Panel_Pixel_CLK  7425 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   7425 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total  1650
  #define Panel_V_Total  750
  #define Panel_H_BackPorch  220
  #define Panel_V_BackPorch  20
  #define Panel_H_SyncWidth  40
  #define Panel_V_SyncWidth  5
#endif

#ifdef _1280x800_Ouput_
  #define Panel_H_Active  1280
  #define Panel_V_Active  800
  #define Panel_Pixel_CLK  7110
  #define Panel_Pixel_CLK_Bypass   7110
  #define Panel_H_Total  1440
  #define Panel_V_Total  823
//  #define Panel_H_BackPorch  20 
//  #define Panel_V_BackPorch  10
//  #define Panel_H_SyncWidth  5
//  #define Panel_V_SyncWidth  1
  #define Panel_H_BackPorch  80
  #define Panel_V_BackPorch  14
  #define Panel_H_SyncWidth  32
  #define Panel_V_SyncWidth  6
#endif

#ifdef _1920x1080_Ouput_
  #define Panel_H_Active  1920
  #define Panel_V_Active  1080
  #define Panel_Pixel_CLK  14850 // 148.5MHZ
  #define Panel_Pixel_CLK_Bypass   20000 // 200MHz  2500 // 25MHZ
  #define Panel_H_Total    2200
  #define Panel_V_Total    1125
  #define Panel_H_BackPorch  148
  #define Panel_V_BackPorch  36
  #define Panel_H_SyncWidth  44
  #define Panel_V_SyncWidth  5
#endif

#endif 


