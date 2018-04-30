/************************************************************
*   Project Name:  lt8619
*   File Name:     lt8619.h
*   Build Date:    2014-10-20
*   Version£º
*   Copyright:     Lontium Semi.
************************************************************/

#ifndef __LT8619_H__
#define __LT8619_H__

extern   u8  Rec_Var[5];
extern   u8  Rec_Count_Num;
extern   u8 CurrentVideoMode;
extern   u8 PreVideoMode;
extern   u16 Count_Number;
extern   u8  PreInputSource;
extern   u8  CurrentInputSource;

extern   u8  PreHdmiClk;
extern   u8  CurrentHdmiClk;
extern   u8  PreColorDeepth;

extern   u8  FlagYCbCr2RGB;
extern   u8  MHL_Input_Flag;
extern   u8  HDMI_Input_Flag;
extern   u8  Rec_Flag;
extern   u8  Hdmi_Mode_Det;
extern   MHLPixelMode MHL_PixelMode;

extern   void LT8668exHdmiEdidSet(void);
extern   void LT8668exVgaEdidSet(void);

extern   void LT8668exRxHdmiClkDet(void);
extern   void LT8668exClockDisable(void);
extern   void LT8668exClockEnable(void);
extern   void LT8668exClockConfig(u8 input);
extern   void LT8668exChipSoftReset(void);
extern   void LT8668exPowerDown(void);
//extern   void LT8668exPowerOn(void);
extern   void LT8668exPowerConfig(u8 input);
extern   void LT8668exMhlConfig(void);



extern   void LT8668exHdmiConfig(void);
extern   void LT8668exAudioConfig(void);
extern   void LT8668exColorConfig(void);


extern   void TTL_Output_Control(FunctionalState state);
extern   void Pattern_Output_Config(void);
extern   void LT8668exRxDigitSoftReset(void);
//extern   void MHL_PixelModeDet(void);

extern void Core_Pll_Config_Nosync(void);
//extern  void LedDriverConfig(void);


extern   void DelayTime(uint i);
extern   void DelayTime1ms(uchar time);
extern   void DelayTime10ms(uchar time);
extern   void DelayTime100ms(uchar time);
extern   void DelayTime1000ms(uchar time);
extern   void HPD_CBUS_Ctl(u8 input);
//extern   void VBUS_Charge_Enable(FunctionalState status);
//extern   void HDMI_Input_Mode_Conif();
extern   void HdmiMhl_Source_Input_Judge(void);
extern   void Get_Mhl_Input_Video_Mode(void);
extern   unsigned char Get_Hdmi_Input_Video_Mode(void);
extern   void Get_Input_Source(void);

extern   void Core_Pll_Config(void);
//extern   void Gcm_Window_Set(void);
extern   void Gcm_Pattern_Set(void);
extern   void Scalar_UpDown_Set(void);
//extern   void GpioPadOutputDisable(void);
//extern   void GpioPadOutputEnable(void);
extern   void Pre_Post_Pattern(void);
extern   void HDMI_Pol_Adjust(void);
extern   void RxHdmiHpdHigh(void);
extern   void RxHdmiHpdLow(void);

extern void LT8668EX_Audio_RST(void);

extern void LT8668ex_Lvds_Set(void);
extern void LT8668ex_Lvds_Close(void);

#endif


