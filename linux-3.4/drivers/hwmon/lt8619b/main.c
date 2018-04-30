/************************************************************
*   Project Name:  lt8619
*   File Name:     main.c
*   Build Date:    2014-10-20
*   Version：
*   Copyright:     Lontium Semi.
************************************************************/

#include "include.h"

#define I2C_RETRY_TIME  10

u8  Rec_Var[5];
u8  Rec_Count_Num = 0;
u16    Count_Number = 0;

u8   CurrentVideoMode = 0;
u8   PreVideoMode = 0x0F;
u8  PreInputSource = 0x00;
u8  CurrentInputSource = 0x00;
u8  PreHdmiClk = 0x00;
u8  CurrentHdmiClk = 0x00;

u8  PreColorDeepth=0;

u8  FlagYCbCr2RGB=0;
u8  MHL_Input_Flag = 0;
u8  HDMI_Input_Flag = 0;
u8  Rec_Flag = 0;
u8  Hdmi_Mode_Det = 1;
MHLPixelMode MHL_PixelMode;

extern struct i2c_client *lt8619b_client;

//void MCU_Init()
//{
//  IE |= 0x82;
//  TMOD = 0x01;
//  EIE |= 0x04;
//
//  TMOD |= 0x20;
//  SCON0 = 0x50;
//  PCON |= 0x80;
//
//  TH1 = 0xFB;
//  TL1 = 0xFB;
//
//  IE |= 0x90;
//  TI_0 = 0;
//  RI_0 = 0;
//  TR1 = 1;
//}

void Load_HdcpKey(void)
{
  u8 flag_load_key_done = 0, i = 0;

  lt8619b_write_reg((BK2_56_REG), 0x11);
  lt8619b_write_reg(BK2_40_REG, 0xc7);
  while(i <= 10) //wait load hdcp key done.
  {
    i++;
    DelayTime10ms(5);
//        flag_load_key_done = BK2_68_REG;
    flag_load_key_done = lt8619b_read_reg(BK2_68_REG);
    if(flag_load_key_done & 0x04)
    {
        break;
    }
  }
//  BK2_40_REG = 0xc6;
  lt8619b_write_reg(BK2_40_REG, 0xc6);
}

void SystemInit(void)
{
  CurrentVideoMode = VIDEO_NOSYNC;
  PreVideoMode = 0x80;// VIDEO_NOSYNC;
  CurrentInputSource = HDMIINPUT;//NOINPUT;
  PreInputSource =  HDMIINPUT;//0x08;
  PreHdmiClk = 0;
  CurrentHdmiClk = 0;            

  HDMI_Input_Flag = 0;
  MHL_Input_Flag = 0;
}

//void GpioInit(void)
//{
//
////Pin63 -- GPIO15 -- OutPut
//  BK0_2B_REG = BK0_2B_REG&0x7f;
//  BK0_2C_REG = BK0_2C_REG&0xef;
//  BK0_25_REG = BK0_25_REG|0x80;
//  BK0_21_REG = BK0_21_REG|0x80;
//  
////Pin73 -- GPIO13 -- OutPut
//  BK0_25_REG = BK0_25_REG|0x20;
//  BK0_21_REG = BK0_21_REG|0x20;  
//
//
///*
////Pin63 -- GPIO15 -- CD_SENSE
//  BK0_2B_REG = BK0_2B_REG&0x7f;
//  BK0_2C_REG = BK0_2C_REG&0xef;
//  BK0_25_REG = BK0_25_REG&0x7f;
//
////Pin73 -- GPIO13 -- CBUS POWER CTL CIRCUIT
//  BK0_25_REG = BK0_25_REG|0x20;
//  BK0_21_REG = BK0_21_REG&0xdf;  //Set CBUS CTL low when power on.
////*/
//
///*
////Pin81 -- GPIO12/PWM_EN -- CD_SENSE
//    BK0_2B_REG = BK0_2B_REG&0xef;
//    BK0_2C_REG = BK0_2C_REG&0xdf;
//    BK0_25_REG = BK0_25_REG&0xef;
//
////Pin82 -- GPIO15 -- CHARGE_EN
//    BK0_2B_REG = BK0_2B_REG&0x7f;
//    BK0_2C_REG = BK0_2C_REG&0xef;
//    BK0_25_REG = BK0_25_REG|0x80;
//    BK0_21_REG = BK0_21_REG|0x80;   //Set CHARGE_EN high to disable  5VtoVBUS.
//
////Pin83 -- GPIO14 -- CBUS POWER CTL CIRCUIT
//    BK0_25_REG = BK0_25_REG|0x40;
//    BK0_21_REG = BK0_21_REG&0xbf;   //Set CBUS CTL low when power on.
//
////Pin95 -- GPIO13 -- HDMI 5V DETECT
//    BK0_25_REG = BK0_25_REG&0xdf;
////*/
//
//}

void lt8619b_write_reg(unsigned short reg, unsigned char value)
{
  int ret0, ret1;
  unsigned char page;
  unsigned char page_reg;
  int i=0;

//page = lt8619b_read_reg(reg);
//printk("write 0x%04x=0x%02x read=0x%02x", reg, value);
//if(page != value)
//printk(" diff");
//printk("\n");
  page = reg >> 8;
  page_reg = reg & 0xFF;
  do
  {
    ret0 = i2c_smbus_write_byte_data(lt8619b_client, 0xFF, page);
    if(ret0 < 0)
    {
      i++;
printk("lt8619b_write_reg page=%02x\n", page);
      msleep(50);
      continue;
    }
    ret1 = i2c_smbus_write_byte_data(lt8619b_client, page_reg, value);
    if(ret1 < 0)
    {
      i++;
printk("lt8619b_write_reg %02x=%02x\n", page_reg, value);
      msleep(50);
      continue;
    }
    else 
      break;
  }while(i<I2C_RETRY_TIME);

  if(i >= I2C_RETRY_TIME)
    printk("lt8619b_write_reg 0x%04x 0x%02x error\n", reg, value);
}

unsigned char lt8619b_read_reg(unsigned short reg)
{
  unsigned char ret;
  unsigned char page;
  unsigned char page_reg;
  int i=0;
  page = reg >> 8;
  page_reg = reg & 0xFF;
  do
  {
    ret = i2c_smbus_write_byte_data(lt8619b_client, 0xFF, page);
    if(ret < 0)
    {
      i++;
      msleep(50);
      continue;
    }
    ret = i2c_smbus_read_byte_data(lt8619b_client, page_reg);
    if(ret < 0)
    {
      i++;
      msleep(50);
      continue;
    }
    else
      break;
  }while(i<I2C_RETRY_TIME);
  if(i >= I2C_RETRY_TIME)
    printk("lt8619b_read_reg 0x%04x error\n", reg);
  return ret;
}
void dump()
{
  printk("0_BD=0x%02x\n", lt8619b_read_reg(BK0_BD_REG));
  printk("0_BE=0x%02x\n", lt8619b_read_reg(BK0_BE_REG));
  printk("0_BF=0x%02x\n", lt8619b_read_reg(BK0_BF_REG));
  printk("0_C0=0x%02x\n", lt8619b_read_reg(BK0_C0_REG));
  printk("0_C1=0x%02x\n", lt8619b_read_reg(BK0_C1_REG));
  printk("0_C2=0x%02x\n", lt8619b_read_reg(BK0_C2_REG));
  printk("0_C3=0x%02x\n", lt8619b_read_reg(BK0_C3_REG));
  printk("0_C4=0x%02x\n", lt8619b_read_reg(BK0_C4_REG));
  printk("1_D9=0x%02x\n", lt8619b_read_reg(BK1_D9_REG));
  printk("1_DA=0x%02x\n", lt8619b_read_reg(BK1_DA_REG));
  printk("1_DB=0x%02x\n", lt8619b_read_reg(BK1_DB_REG));
  printk("1_DC=0x%02x\n", lt8619b_read_reg(BK1_DC_REG));
  printk("1_DD=0x%02x\n", lt8619b_read_reg(BK1_DD_REG));
  printk("1_70=0x%02x\n", lt8619b_read_reg(BK1_70_REG));
  printk("1_71=0x%02x\n", lt8619b_read_reg(BK1_71_REG));
  printk("1_72=0x%02x\n", lt8619b_read_reg(BK1_72_REG));
  printk("1_73=0x%02x\n", lt8619b_read_reg(BK1_73_REG));
  printk("1_74=0x%02x\n", lt8619b_read_reg(BK1_74_REG));
  printk("1_75=0x%02x\n", lt8619b_read_reg(BK1_75_REG));
  printk("1_76=0x%02x\n", lt8619b_read_reg(BK1_76_REG));
  printk("1_77=0x%02x\n", lt8619b_read_reg(BK1_77_REG));
  printk("1_78=0x%02x\n", lt8619b_read_reg(BK1_78_REG));
  printk("1_79=0x%02x\n", lt8619b_read_reg(BK1_79_REG));
  printk("1_7A=0x%02x\n", lt8619b_read_reg(BK1_7A_REG));
  printk("1_7B=0x%02x\n", lt8619b_read_reg(BK1_7B_REG));
  printk("1_7C=0x%02x\n", lt8619b_read_reg(BK1_7C_REG));
  printk("1_7D=0x%02x\n", lt8619b_read_reg(BK1_7D_REG));
  printk("1_7E=0x%02x\n", lt8619b_read_reg(BK1_7E_REG));
  printk("1_7F=0x%02x\n", lt8619b_read_reg(BK1_7F_REG));
  printk("1_80=0x%02x\n", lt8619b_read_reg(BK1_80_REG));
  printk("1_81=0x%02x\n", lt8619b_read_reg(BK1_81_REG));
  printk("1_82=0x%02x\n", lt8619b_read_reg(BK1_82_REG));
  printk("1_83=0x%02x\n", lt8619b_read_reg(BK1_83_REG));
  printk("1_84=0x%02x\n", lt8619b_read_reg(BK1_84_REG));
  printk("1_85=0x%02x\n", lt8619b_read_reg(BK1_85_REG));
}

int lt8619b_i2c_init()
{
  unsigned char ret0;
  int i=0;
//  bit TestPattern_Status = 0;
//  MCU_Init();
  if(lt8619b_client == NULL)
    return 0;

  DelayTime100ms(3);

  lt8619b_write_reg(BK0_EE_REG, 0x01);
  DelayTime100ms(1);
//#if 1
//  LT8668exClockDisable();
//  LT8668exChipSoftReset();
//  LT8668exPowerDown();
//#endif 
  SystemInit();
//  BK0_5A_REG = 0x02;// for LT8619B
  lt8619b_write_reg(BK0_5A_REG, 0x02);
#if 0
  LT8668exClockDisable();
  LT8668exChipSoftReset();
  LT8668exPowerDown();
#endif 
//  LT8668exHdmiEdidSet();

//#ifdef _RGB_Outout_
//  lt8619b_write_reg(BK0_79_REG, 0x3F);//0xBF;//ttl output enable
//  lt8619b_write_reg(BK0_7A_REG, 0x00);
//#endif
#ifdef _LVDS_Outout_
//  LT8668ex_Lvds_Set();
//  lt8619b_write_reg(BK1_BB_REG, 0x02);// 1 LVDS Channel //v_pol_s,h_pol_s,bus_wid[1:0],bit_shf,col_dep,dp_de,dp_en
//  lt8619b_write_reg(BK1_BC_REG, 0x02);
    LT8668ex_Lvds_Close();
#endif

//  BK1_DE_REG = 0xC0;
  Load_HdcpKey();
  DelayTime100ms(1);

  lt8619b_write_reg(BK0_5A_REG, 0x02);// for LT8619B
#ifdef _LVDS_Outout_
  lt8619b_write_reg(BK1_90_REG, 0x00);
#endif
  ret0 = lt8619b_read_reg(BK2_57_REG);
  lt8619b_write_reg(BK2_57_REG, ret0 & 0xFE);
  lt8619b_write_reg(BK1_36_REG, 0x01);
  
//  Pattern_Output_Config();
  LT8668exClockConfig(HDMIINPUT);
  LT8668exPowerConfig(HDMIINPUT);
  LT8668exHdmiConfig();
  //    HPD_CBUS_Ctl(CurrentInputSource); //yc
  RxHdmiHpdHigh();
  DelayTime100ms(2);
//  LT8668exAudioConfig();
////  while(1)
  {
    //Get_Input_Source();
    //if(CurrentInputSource == HDMIINPUT)        //HDMI input 
    {
      /*
      if(PreInputSource != CurrentInputSource)
        {
        PreInputSource = CurrentInputSource;
        TTL_Output_Control(DISABLE);
        //    VBUS_Charge_Enable(DISABLE);
        LT8668exClockConfig(CurrentInputSource);
        LT8668exPowerConfig(CurrentInputSource);
        LT8668exHdmiConfig();
        //    HPD_CBUS_Ctl(CurrentInputSource); //yc
        RxHdmiHpdHigh();
        DelayTime100ms(2);
      //  BK0_8C_REG = 0x01;
      //  BK0_8C_REG = 0x81;
         }//*/

      LT8668exRxHdmiClkDet();
      ret0 = 0xFF;
      while(i<=5)
      {
        ret0 = Get_Hdmi_Input_Video_Mode();
        if(ret0 == 0x07)
          break;
        i++;
        msleep(1000);
      }
      if(i>=5)
      {
        printk("Get_Hdmi_Input_Video_Mode error 0x%02x\n", ret0);
        return -1;
      }
      lt8619b_write_reg(BK1_F3_REG, CurrentVideoMode);

      LT8668exColorConfig();
      
      if(PreVideoMode != CurrentVideoMode)
      {
        PreVideoMode = CurrentVideoMode;
        if(CurrentVideoMode != VIDEO_NOSYNC)
        {
          lt8619b_write_reg(BK1_70_REG, 0x00);
#ifdef _RGB_Outout_
          TTL_Output_Control(DISABLE);
#endif
      //      BK1_90_REG = 0x00;
          Core_Pll_Config();
          HDMI_Pol_Adjust();
          DelayTime10ms(1);
          Scalar_UpDown_Set();
          DelayTime10ms(3);  
      //      BK1_90_REG = 0x00;
      //      BK0_5A_REG = 0x02;
//            DelayTime10ms(3);
#ifdef _RGB_Outout_
          TTL_Output_Control(ENABLE);
#endif
#ifdef _LVDS_Outout_
          lt8619b_write_reg(BK0_8C_REG, 0x01);
          DelayTime10ms(1);
          lt8619b_write_reg(BK0_8C_REG, 0x81);

          lt8619b_write_reg(BK0_0E_REG, 0xf3);
          DelayTime10ms(1);
          lt8619b_write_reg(BK0_0E_REG, 0xff);
          LT8668ex_Lvds_Set();
//          dump();
#endif
//          LT8668exAudioConfig();
//          LT8668EX_Audio_RST();
        }
        else
        {
          // 无信号的时候蓝屏输出
      //RxHdmiHpdLow();
      //  LT8668exClockConfig(HDMIINPUT);
      //  LT8668exPowerConfig(HDMIINPUT);// L 20150528
      //  Core_Pll_Config_Nosync();
      //  DelayTime10ms(2);
          Pattern_Output_Config();
          DelayTime10ms(2);
            
#ifdef _RGB_Outout_
          TTL_Output_Control(ENABLE); 
#endif
#ifdef _LVDS_Outout_
          LT8668ex_Lvds_Set();
#endif
        }
      }
    }  /*  
    else if(CurrentInputSource == MHLINPUT)        //MHL input 
        {
        if(PreInputSource != CurrentInputSource)
          {
          PreInputSource = CurrentInputSource;
          //VBUS_Charge_Enable(ENABLE);
          LT8668exClockConfig(CurrentInputSource);
          LT8668exPowerConfig(CurrentInputSource);
          //HPD_CBUS_Ctl(CurrentInputSource);
          LT8668exMhlConfig();

          BK0_8C_REG = 0x01;
          BK0_8C_REG = 0x81;
          }

        if( BK2_E4_REG&0x20 )
          {
          BK2_E3_REG = 0x20;
          BK2_E3_REG = 0x00;

          BK0_10_REG &= 0xDF;
          BK0_0A_REG &= 0x86;
          DelayTime(1000);
          BK0_10_REG |= 0x20;
          BK0_0A_REG = 0xFF;
          }

        //MHL_PixelModeDet();
        Get_Mhl_Input_Video_Mode();
        LT8668exColorConfig();
        
        if(PreVideoMode != CurrentVideoMode)
          {
          PreVideoMode = CurrentVideoMode;
          if(CurrentVideoMode != VIDEO_NOSYNC)
            {
            BK1_70_REG = 0x00;
            BK1_90_REG = 0x00;
            Core_Pll_Config();
            HDMI_Pol_Adjust(); 
            Scalar_UpDown_Set();
            TTL_Output_Control(ENABLE);
            LT8668exAudioConfig();
            BK1_90_REG = 0x00;
            }
          else
            {
            
            PreVideoMode = VIDEO_NOSYNC;
            //      RxHdmiHpdLow();

        //    LT8668exClockConfig(MHLINPUT);
        //    LT8668exPowerConfig(MHLINPUT);// L 20150528

        //    Core_Pll_Config_Nosync();
        //    DelayTime10ms(2);

            Pattern_Output_Config();

            DelayTime10ms(2);
            BK1_90_REG = 0x01;
            TTL_Output_Control(ENABLE); 

            }

          }  
      
      }//*/
    /*
    else  //None input detected.
    {
      if(PreInputSource != CurrentInputSource)
      {
                PreInputSource = CurrentInputSource;
                PreVideoMode = VIDEO_NOSYNC;
          //      RxHdmiHpdLow();
        
        LT8668exClockConfig(0x02);
        LT8668exPowerConfig(0x02);// L 20150528
        
        Core_Pll_Config_Nosync();
        DelayTime10ms(2);
        
        Pattern_Output_Config();
        
        DelayTime10ms(2);
        TTL_Output_Control(ENABLE); 

      }
    }*/
    //Uart_Read_Write();        //uart communication
  }
  return 0;
}

//void lt8619b_i2c_pre_init()
//{
//  unsigned char ret0;
////  bit TestPattern_Status = 0;
////  MCU_Init();
//  if(lt8619b_client == NULL)
//    return;
////  DelayTime100ms(3);
//    lt8619b_write_reg(BK0_EE_REG, 0x01);
//    DelayTime100ms(1);
//  SystemInit();
////  GpioInit();
//  //      LedDriverConfig();
////  BK0_5A_REG = 0x02;// for LT8619B
//  lt8619b_write_reg(BK0_5A_REG, 0x02);
//#if 0
//  LT8668exClockDisable();
//  LT8668exChipSoftReset();
//  LT8668exPowerDown();
//#endif 
//  LT8668exHdmiEdidSet();
//
////#ifdef _RGB_Outout_
////  lt8619b_write_reg(BK0_79_REG, 0x3F);//0xBF;//ttl output enable
////  lt8619b_write_reg(BK0_7A_REG, 0x00);
////#endif
//#ifdef _LVDS_Outout_
////  LT8668ex_Lvds_Set();
//  lt8619b_write_reg(BK1_BB_REG, 0x02);// 1 LVDS Channel //v_pol_s,h_pol_s,bus_wid[1:0],bit_shf,col_dep,dp_de,dp_en
//  lt8619b_write_reg(BK1_BC_REG, 0x02);
//#endif
//
////  BK1_DE_REG = 0xC0;
//  Load_HdcpKey();
//  DelayTime100ms(1);
//
//  lt8619b_write_reg(BK0_5A_REG, 0x02);// for LT8619B
//#ifdef _LVDS_Outout_
//  lt8619b_write_reg(BK1_90_REG, 0x00);
//#endif
//  ret0 = lt8619b_read_reg(BK2_57_REG);
//  lt8619b_write_reg(BK2_57_REG, ret0 & 0xFE);
//  lt8619b_write_reg(BK1_36_REG, 0x01);
//  
////  Pattern_Output_Config();
//  LT8668exClockConfig(HDMIINPUT);
//  LT8668exPowerConfig(HDMIINPUT);
//  LT8668exHdmiConfig();
//  //    HPD_CBUS_Ctl(CurrentInputSource); //yc
//  RxHdmiHpdHigh();
//  DelayTime100ms(2);
//  LT8668exAudioConfig();
//////  while(1)
//  {
//    //Get_Input_Source();
//    //if(CurrentInputSource == HDMIINPUT)        //HDMI input 
//      {
//      /*
//      if(PreInputSource != CurrentInputSource)
//        {
//        PreInputSource = CurrentInputSource;
//        TTL_Output_Control(DISABLE);
//        //    VBUS_Charge_Enable(DISABLE);
//        LT8668exClockConfig(CurrentInputSource);
//        LT8668exPowerConfig(CurrentInputSource);
//        LT8668exHdmiConfig();
//        //    HPD_CBUS_Ctl(CurrentInputSource); //yc
//        RxHdmiHpdHigh();
//        DelayTime100ms(2);
//      //  BK0_8C_REG = 0x01;
//      //  BK0_8C_REG = 0x81;
//         }//*/
//
////      LT8668exRxHdmiClkDet();
////      Get_Hdmi_Input_Video_Mode();
//      CurrentHdmiClk = 0;
//      PreHdmiClk = CurrentHdmiClk;
//      CurrentVideoMode = VIDEO_NOSYNC;
//      lt8619b_write_reg(BK1_F3_REG, CurrentVideoMode);
//
//      LT8668exColorConfig();
//      
//      if(PreVideoMode != CurrentVideoMode)
//        {
//          PreVideoMode = CurrentVideoMode;
//          if(CurrentVideoMode != VIDEO_NOSYNC)
//          {
//            lt8619b_write_reg(BK1_70_REG, 0x00);
//#ifdef _RGB_Outout_
//            TTL_Output_Control(DISABLE);
//#endif
//      //      BK1_90_REG = 0x00;
//          
//            Core_Pll_Config();
//            HDMI_Pol_Adjust();
//            DelayTime10ms(1);
//            Scalar_UpDown_Set();
//            DelayTime10ms(3);  
//
//      //      BK1_90_REG = 0x00;
//      //      BK0_5A_REG = 0x02;
////            DelayTime10ms(3);
//#ifdef _RGB_Outout_
//            TTL_Output_Control(ENABLE);
//#endif
//#ifdef _LVDS_Outout_
//            lt8619b_write_reg(BK0_8C_REG, 0x01);
//            DelayTime10ms(1);
//            lt8619b_write_reg(BK0_8C_REG, 0x81);
//
//            lt8619b_write_reg(BK0_0E_REG, 0xf3);
//            DelayTime10ms(1);
//            lt8619b_write_reg(BK0_0E_REG, 0xff);
//            LT8668ex_Lvds_Set();
//#endif
////            LT8668exAudioConfig();
////            LT8668EX_Audio_RST();
////
//            }
//          else
//            {
//              // 无信号的时候蓝屏输出
//          //RxHdmiHpdLow();
//          
//        
//          //  LT8668exClockConfig(HDMIINPUT);
//          //  LT8668exPowerConfig(HDMIINPUT);// L 20150528
//
//          //  Core_Pll_Config_Nosync();
//          //  DelayTime10ms(2);
//
//            Pattern_Output_Config();
//            DelayTime10ms(2);
//            
//#ifdef _RGB_Outout_
//            TTL_Output_Control(ENABLE); 
//#endif
//#ifdef _LVDS_Outout_
//            LT8668ex_Lvds_Set();
//#endif
//            }
////          
//        }
//
//      }  /*  
//    else if(CurrentInputSource == MHLINPUT)        //MHL input 
//        {
//        if(PreInputSource != CurrentInputSource)
//          {
//          PreInputSource = CurrentInputSource;
//          //VBUS_Charge_Enable(ENABLE);
//          LT8668exClockConfig(CurrentInputSource);
//          LT8668exPowerConfig(CurrentInputSource);
//          //HPD_CBUS_Ctl(CurrentInputSource);
//          LT8668exMhlConfig();
//
//          BK0_8C_REG = 0x01;
//          BK0_8C_REG = 0x81;
//          }
//
//        if( BK2_E4_REG&0x20 )
//          {
//          BK2_E3_REG = 0x20;
//          BK2_E3_REG = 0x00;
//
//          BK0_10_REG &= 0xDF;
//          BK0_0A_REG &= 0x86;
//          DelayTime(1000);
//          BK0_10_REG |= 0x20;
//          BK0_0A_REG = 0xFF;
//          }
//
//        //MHL_PixelModeDet();
//        Get_Mhl_Input_Video_Mode();
//        LT8668exColorConfig();
//        
//        if(PreVideoMode != CurrentVideoMode)
//          {
//          PreVideoMode = CurrentVideoMode;
//          if(CurrentVideoMode != VIDEO_NOSYNC)
//            {
//            BK1_70_REG = 0x00;
//            BK1_90_REG = 0x00;
//            Core_Pll_Config();
//            HDMI_Pol_Adjust(); 
//            Scalar_UpDown_Set();
//            TTL_Output_Control(ENABLE);
//            LT8668exAudioConfig();
//            BK1_90_REG = 0x00;
//            }
//          else
//            {
//            
//            PreVideoMode = VIDEO_NOSYNC;
//            //      RxHdmiHpdLow();
//
//        //    LT8668exClockConfig(MHLINPUT);
//        //    LT8668exPowerConfig(MHLINPUT);// L 20150528
//
//        //    Core_Pll_Config_Nosync();
//        //    DelayTime10ms(2);
//
//            Pattern_Output_Config();
//
//            DelayTime10ms(2);
//            BK1_90_REG = 0x01;
//            TTL_Output_Control(ENABLE); 
//
//            }
//
//          }  
//      
//      }//*/
//    /*
//    else  //None input detected.
//    {
//      if(PreInputSource != CurrentInputSource)
//      {
//                PreInputSource = CurrentInputSource;
//                PreVideoMode = VIDEO_NOSYNC;
//          //      RxHdmiHpdLow();
//        
//        LT8668exClockConfig(0x02);
//        LT8668exPowerConfig(0x02);// L 20150528
//        
//        Core_Pll_Config_Nosync();
//        DelayTime10ms(2);
//        
//        Pattern_Output_Config();
//        
//        DelayTime10ms(2);
//        TTL_Output_Control(ENABLE); 
//
//      }
//    }*/
//    //Uart_Read_Write();        //uart communication
//  }
//}