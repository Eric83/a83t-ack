/************************************************************
*   Project Name:  lt8619
*   File Name:     variable.h
*   Build Date:    2014-10-20
*   Version£º
*   Copyright:     Lontium Semi.
************************************************************/

#ifndef     __VARIABLE_H
#define     __VARIABLE_H

//typedef unsigned long  u32;
//typedef unsigned short u16;
//typedef unsigned char  u8;
//typedef unsigned int uint;
typedef unsigned char  uchar;

/*****************************defined by yc start***************************/

typedef  enum
{
    INPUT_HDMI = 0,
    INPUT_MHL
}
InputMode;


typedef  enum
{
    MHL_24BIT_MODE = 0,
    MHL_PACKEDPIXEL_MODE
}
MHLPixelMode;


typedef enum {
  DISABLE = 0,
  ENABLE = !DISABLE
}
FunctionalState;

/*****************************defined by yc end*****************************/

#define    Read    1
#define    Write    0

//Input source define 
#define    NOINPUT          0x00
#define    VGAINPUT        0x01
#define    HDMIINPUT        0x02
#define    MHLINPUT        0x03

//Keypad define
#define    KEY_ESC          0x10
#define    KEY_OK          0x20
#define    KEY_DOWN        0x30
#define    KEY_UP          0x40

#if 1
//VESA Mode

#define    VESA_640x480_60        0x01
#define    VESA_800x600_60        0x02
#define    VESA_1024x768_60      0x03
#define    VESA_1280x768_60      0x04
#define    VESA_1280x800_60      0x05
#define    VESA_1280x960_60      0x06
#define    VESA_1280x1024_60      0x07
#define    VESA_1360x768_60      0x08
#define    VESA_1440x900_60      0x09
#define    VESA_1600x1200_60      0x0a
#define    VESA_1920x1440_60      0x0b
    
//just for CurrentVideoMode and LastVideoMode
#define     VIDEO_NOSYNC          0xFF
#define     VIDEO_NOInput          0xFF

/*
#define    HDMI_640x480_60_Mode      0x00
#define    HDMI_720x480_60_Mode      0x01
#define    HDMI_848x480_60_Mode      0x02
#define    HDMI_800x600_60_Mode      0x03
#define    HDMI_1024x768_60_Mode      0x04
#define    HDMI_1280x720_60_Mode      0x05
#define    HDMI_1280x768_60_Mode      0x06
#define    HDMI_1280x800_60_Mode      0x07
#define    HDMI_1280x960_60_Mode      0x08
#define    HDMI_1280x1024_60_Mode      0x09
#define    HDMI_1360x768_60_Mode      0x0a
#define    HDMI_1440x900_60_Mode      0x0b
#define    HDMI_1600x900_60_Mode      0x0c
#define    HDMI_1600x1200_60_Mode      0x0d
#define    HDMI_1920x1080_60_Mode      0x0e

#define    MHL_720x480_60_Mode          0x01
#define    MHL_1280x720_60_Mode        0x05
//*/
#define    HDMI_640x480_60_Mode        0x00
#define    HDMI_720x480_60_Mode        (HDMI_640x480_60_Mode+1)  //1
#define    HDMI_800x480_60_Mode        (HDMI_720x480_60_Mode+1)  //2

#define    HDMI_800x600_60_Mode        (HDMI_800x480_60_Mode+1)  //3
#define    HDMI_848x480_60_Mode        (HDMI_800x600_60_Mode+1)  //4

#define    HDMI_1024x600_60_Mode      (HDMI_848x480_60_Mode+1)    //5

#define    HDMI_1024x768_60_Mode      (HDMI_1024x600_60_Mode+1)   //6
//#define    HDMI_1152x864_75_Mode      (HDMI_1024x768_60_Mode+1)

#define    HDMI_1280x720_60_Mode        (HDMI_1024x768_60_Mode+1) //7
#define    HDMI_1280x768_60_68MHz_Mode      (HDMI_1280x720_60_Mode+1)
#define    HDMI_1280x768_60_79Mhz_Mode      (HDMI_1280x768_60_68MHz_Mode+1)
#define    HDMI_1280x800_60_71MHz_Mode      (HDMI_1280x768_60_79Mhz_Mode+1)
#define    HDMI_1280x800_60_83MHz_Mode      (HDMI_1280x800_60_71MHz_Mode+1)
#define    HDMI_1280x960_60_Mode        (HDMI_1280x800_60_83MHz_Mode+1)
#define    HDMI_1280x1024_60_Mode        (HDMI_1280x960_60_Mode+1)
#define    HDMI_1360x768_60_Mode        (HDMI_1280x1024_60_Mode+1)
#define    HDMI_1366x768_60_72MHz_Mode      (HDMI_1360x768_60_Mode+1)
#define    HDMI_1366x768_60_85MHz_Mode      (HDMI_1366x768_60_72MHz_Mode+1)

//#define    HDMI_1400x1050_60_Mode        (HDMI_1366x768_60_Mode+1)

#define    HDMI_1440x900_60_88MHz_Mode      (HDMI_1366x768_60_85MHz_Mode+1)
#define    HDMI_1440x900_60_106MHz_Mode    (HDMI_1440x900_60_88MHz_Mode+1)
#define    HDMI_1600x900_60_Mode        (HDMI_1440x900_60_106MHz_Mode+1)
#define    HDMI_1600x1200_60_Mode        (HDMI_1600x900_60_Mode+1)
//#define    HDMI_1680x1050_60_Mode        (HDMI_1600x1200_60_Mode+1)
//#define    HDMI_1792x1344_60_Mode        (HDMI_1680x1050_60_Mode+1)
//#define    HDMI_1856x1392_60_Mode        (HDMI_1792x1344_60_Mode+1)

#define    HDMI_1920x1080_60_Mode        (HDMI_1600x1200_60_Mode+1)

#define    HDMI_1280x720_50_Mode        (HDMI_1920x1080_60_Mode+1)
#define    HDMI_1920x1080_50_Mode        (HDMI_1280x720_50_Mode+1)
#define    HDMI_720x576_50_Mode          (HDMI_1920x1080_50_Mode+1)
#define    HDMI_1152x864_60_Mode        (HDMI_720x576_50_Mode+1)

#define    Resolution_Num            (HDMI_1152x864_60_Mode+1)

#define    MHL_720x480_60_Mode          HDMI_720x480_60_Mode
#define    MHL_1280x720_60_Mode        HDMI_1280x720_60_Mode

//CEA Mode

#define    CEA_640x480P_60_4_3        0x0c
#define    CEA_720x480P_60_4_3        0x0d
#define    CEA_720x480P_60_16_9      0x0e
#define    CEA_1280x720P_60_16_9      0x0f
#define    CEA_1920x1080i_60_16_9      0x10
#define    CEA_720x480i_60_4_3        0x11
#define    CEA_720x480i_60_16_9      0x12
#define    CEA_1920x1080P_60_16_9      0x13
#define    CEA_720x576P_50_4_3        0x14
#define    CEA_720x576P_50_16_9      0x15
#define    CEA_1280x720P_50_16_9      0x16
#define    CEA_1920x1080i_50_16_9      0x17
#define    CEA_720x576i_50_4_3        0x18
#define    CEA_720x576i_50_16_9      0x19
#define    CEA_1920x1080P_50_16_9      0x1a
#define    CEA_1920x1080P_24_16_9      0x1b

//just for CurrentVideoMode and LastVideoMode

#define    CEA_640x480P_60_4_3_Mode      0x30
#define    CEA_720x480P_60_4_3_Mode      0x31
#define    CEA_720x480P_60_16_9_Mode      0x32
#define    CEA_1280x720P_60_16_9_Mode      0x33
#define    CEA_1920x1080i_60_16_9_Mode      0x34
#define    CEA_720x480i_60_4_3_Mode      0x35
#define    CEA_720x480i_60_16_9_Mode      0x36
#define    CEA_1920x1080P_60_16_9_Mode      0x37
#define    CEA_720x576P_50_4_3_Mode      0x38
#define    CEA_720x576P_50_16_9_Mode      0x39
#define    CEA_1280x720P_50_16_9_Mode      0x3A
#define    CEA_1920x1080i_50_16_9_Mode      0x3b
#define    CEA_720x576i_50_4_3_Mode      0x3c
#define    CEA_720x576i_50_16_9_Mode      0x3d
#define    CEA_1920x1080P_50_16_9_Mode      0x3e
#define    CEA_1920x1080P_24_16_9_Mode      0x3f 

#endif

#define    Menu_Root                0
#define    Menu_MainImageAutoAdjust          1  
#define    Menu_ImageAutoOfMain            2
#define    Menu_MainContrastLight            3
#define    Menu_MainContrastCtrl            4
#define    Menu_MainLightCtrl              5
#define    Menu_MainAudioAdjust            6
#define    Menu_AudioAdjustVolumn          7
#define    Menu_MainColorAdjust            8
#define    Menu_ColorAdjust9300k            9
#define    Menu_ColorAdjust6500k            10
#define    Menu_ColorAdjust5400k            11
#define    Menu_MainMessage            12
#define    Menu_Message              13
#define    Menu_MainImageHandleAdjust        14
#define    Menu_ImageHorVerPosition          15
#define    Menu_ImageHorPositionAdjust        16
#define    Menu_ImageVerPositionAdjust        17
#define    Menu_ImageHandleHorWidth          18
#define    Menu_HorWidth              19
#define    Menu_ImageHandleTinyAdjust        20
#define    Menu_ImageTiny              21
#define    Menu_MainSetMenu            22
#define    Menu_SetMenuLanguageChoice        23
#define    Menu_LanguageChoiceEng          24
#define    Menu_LanguageChoiceSimpleChinese      25
#define    Menu_Source_Input            26    
#define    Menu_Source_Input_HDMI          27
#define    Menu_Source_Input_Vga          28
#define    Menu_MenuPosition            29
#define    Menu_MenuHorPosition            30
#define    Menu_MenuVerPosition            31
#define    Menu_MenuDisplayTime            32
#define    Menu_DisplayTimeMenuAdjust        33
#define    Menu_MenuBackColor            34
#define    Menu_MenuBackColorOpen          35
#define    Menu_MenuBackColorClose          36
#define    Menu_MainMenuResetConfig          37
#define    Menu_ResetConfig              38
#define    Menu_ImageAutoAdjust            39
#define    Menu_Contrast              40
#define    Menu_Light                41

//define  adjust

#define    AdjustMode1        0x14
#define    AdjustMode2        0x24
#define    AdjustMode3        0x34
#define    AdjustMode4        0x44
#define    AdjustMode5        0x54
#define    AdjustMode6        0x64
#define    AdjustMode7        0x74
#define    AdjustMode8        0x84
#define    AdjustMode9        0x94
#define    AdjustWrongMode      0xf0

/*

typedef struct{
  u8  DMA_AddL;
  u8   DMA_AddM;
  u8  DMA_UpLoad_SizeL;
  u8  DMA_UpLoad_SizeM;
  u8  DMA_Ctrl_Cmd;
  u8   SPI_Ctrl;
  u8   SPI_Master_Read;
  u8  SPI_Master_AddrHsb;
  u8  SPI_Master_AddrMid;
  u8   SPI_Master_AddrLsb;
  u8  SPI_Start;
}DMA_UpLoad;



typedef  struct{

   u8 Scaler_Data[21];  

}Scaler_UpDown;


typedef struct{

  u8 Gcm_Data[13];

}Gcm_Window;


typedef  struct{

  u8 VGA_ADC_PLL_Data[9];

}VGA_Input_ADC_Pll;


typedef struct{
  
  u8 VGA_ADC[40];

}VAG_Input_ADC;




typedef struct{

  u8 VGA_ADC_Reg[40];  //Reg68ex Registers define

}VGA_Input_ADC_Reg;



typedef struct{

  u8 Core_Pll_Data[9];

}Core_Pll;


typedef struct{

  u8  Scalar_Data[21];      //scalar relation registers

}Scalar_UpDown_Reg;



typedef struct{
  
  uchar gcm_param[11];

}AdjustGcmParam;

typedef struct{
  
  u8 gcmTim[20];

}AdjustGcmTiming;

typedef struct{
  
  u8 gcmset[4];

}AdjustGcmSet;

//Adjust Mode Reg Define
typedef  struct{

  uchar   horcntlsb;
  uchar    vercntlsb;
  uchar   cntmsb;
  
}ADJUST_HORVERCNT;
//*/

 typedef struct{
   u8  EdidData[256];      //scalar relation registers
 }Edid_Reg;


 typedef struct{
   u16 Input_Param[8];
 }InputParam;


 union Temp
{
  u8 Temp8[4];
  u16 Temp16[2];
  u32 Temp32;
};

 union Temp2
{
  u8 Temp8[2];
  u16 Temp16;//[2];
};

enum
{
  H_Act,
  V_Act,
  H_Tol,
  V_Tol,
  H_BP,
  Hs_Wid,
  Vs_Wid,
  V_BP
};

extern  u8 KeyPadDetectTime;
extern  u8   Rec_Var[5];
extern   u8  Rec_Count_Num;
extern  u8   CurrentVideoMode;
extern   u8  PreVideoMode;
extern  u8  KeyStatus;
extern  u8   LastKeyStatus;
extern  u8  KeyIndex;
extern  u8  LastKeyIndex;
extern    u16    Count_Number;
extern  u8  PreInputSource;
extern   u8  CurrentInputSource;

extern   u8  PreHdmiClk;
extern   u8  CurrentHdmiClk;

extern   u8  PreVgaHVsync;
extern   u8  CurrentVgaHVsync;
extern   u8  PreColorDeepth;

extern u8 FlagYCbCr2RGB;
extern u8 Rec_Flag;
extern u8 MHL_Input_Flag;
extern u8 HDMI_Input_Flag;
//extern u8 VGA_Input_Flag;  
extern u8 VGA_Mode_Change_Flag;
extern u8 Keypad_Inter_Flag;
extern u8 Hdmi_Mode_Det;
extern u8 Timer_OverFlow;
extern u8 Load_Font_Color_Lib_Num_Flag;

//
//extern VGA_Input_ADC_Pll   code   VGA_ADC_PLL[9];
//extern Gcm_Window    code    Gcm_Rel_Param[14];
//extern Scaler_UpDown    code    Scaler_Mode_Param[15]; 
//extern VAG_Input_ADC     code   VGA_ADC[9];
//extern Core_Pll       code   Core_Pll_Param[4];
//extern u8    code    OSD_Pos_Param[7];
//extern uchar         code   adjusthstep[13];
//extern uchar         code   adjustvstep[13] ;
//extern AdjustGcmParam    code    strAdjustGcmParam[14];
//extern AdjustGcmSet code strAdjustGcmSet[14];
//extern u8    Menu_Param[20];

#endif
