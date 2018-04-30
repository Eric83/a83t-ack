/*-----------------------------------------------------*\

     ProjectName:
     FileName:
     Version:
     Desciption:
     BuildData:

\*-----------------------------------------------------*/

#include "include.h"


InputParam Input_Resolution_Param[Resolution_Num]=
{
//  H_act  V_act  H_Tol  V_Tol  H_BP   Hs_Wid  Vs_Wid  V_BP
#ifdef HDMI_640x480_60_Mode
  {640,  480,  800,  525,  40,   96,    2,    25},// HDMI_640x768_60
#endif
#ifdef HDMI_720x480_60_Mode
  {720,  480,  858,  525,  60,   62,    6,    30},// HDMI_720x480_60
#endif
#ifdef HDMI_800x600_60_Mode
  {800,  600,  1056,  628,  88,   128,  4,    23},// HDMI_800x600_60
#endif
#ifdef HDMI_848x480_60_Mode
//  {848,  480,  1088,  517,  112,   112,  8,    23},// HDMI_848x480_60
  {854,  480,  1088,  517,  109,   112,  8,    15},// HDMI_848x480_60
#endif
#ifdef HDMI_1024x600_60_Mode
//  {1024,  600,  1344,  635,  160,   20,    3,    23},// HDMI_1024x768_60
//  {1024,  600,  1344,  635,  140,   20,    3,    20},// HDMI_1024x768_60
  {1064,  600,  1344,  750,  140,   23,    20,    70},// HDMI_1024x768_60
#endif
#ifdef HDMI_1024x768_60_Mode
  {1024,  768,  1344,  806,  160,   136,  6,    29},// HDMI_1024x768_60
#endif
#ifdef HDMI_1152x864_75_Mode
  {1152,  864,  1600,  900,  256,  128,  3,    32},// HDMI_1152x864_75
#endif
#ifdef HDMI_1280x720_60_Mode
  {1280,  720,  1650,  750,  220,   40,    5,    20},// HDMI_1280x720_60
#endif
#ifdef HDMI_1280x768_60_68MHz_Mode
  {1280,  768,  1440,  790,  80,   32,    7,    12},// HDMI_1280x768_60
#endif
#ifdef HDMI_1280x768_60_79Mhz_Mode
  {1280,  768,  1664,  798,  192,   128,  7,    20},// HDMI_1280x768_60
#endif
#ifdef HDMI_1280x800_60_71MHz_Mode
  {1280,  800,  1440,  823,  80,   32,    6,  14},// HDMI_1280x800_60
#endif
#ifdef HDMI_1280x800_60_83MHz_Mode
  {1280,  800,  1680,  831,  200,   128,  6,  22},// HDMI_1280x800_60
#endif
#ifdef HDMI_1280x960_60_Mode
  {1280,  960,   1800,  1000,  312,   112,   3,  36},// HDMI_1280x960_60
#endif
#ifdef HDMI_1280x1024_60_Mode
  {1280,  1024,  1688,  1066,  248,   112,  3,  38},// HDMI_1280x1024_60
#endif
#ifdef HDMI_1360x768_60_Mode
  {1360,  768,  1792,  795,  256,   112,  6,  18},// HDMI_1360x768_60
#endif
#ifdef HDMI_1366x768_60_72MHz_Mode
  {1366,  768,  1500,  800,  64,   56,    3,  28},// HDMI_1366x768_60
#endif

#ifdef HDMI_1366x768_60_85MHz_Mode
  {1366,  768,  1792,  798,  213,   143,  3,  24},// HDMI_1366x768_60
#endif
#ifdef HDMI_1400x1050_60_Mode
  {1400,  1050,  1684,  1098,  232,   144,  4,  32},// HDMI_1400x1050_60
#endif
#ifdef HDMI_1440x900_60_88MHz_Mode
  {1440,  900,  1600,  926,  80,   32,    6,  17},// HDMI_1440x900_60
#endif
#ifdef HDMI_1440x900_60_106MHz_Mode
  {1440,  900,  1904,  934,  232,   152,  6,  25},// HDMI_1440x900_60
#endif
#ifdef HDMI_1600x900_60_Mode
  {1600,  900,  1800,  1000,  96,    80,    3,  96},// HDMI_1600x900_60
#endif
#ifdef HDMI_1600x1200_60_Mode
  {1600,  1200,  2160,  1250,  304,  192,   3,  46},// HDMI_1600x1200_60
#endif
#ifdef HDMI_1680x1050_60_Mode
  {1680,  1050,  2240,  1089,  280,   176,  6,  30},// HDMI_1680x1050_60
#endif
#ifdef HDMI_1792x1344_60_Mode
  {1792,  1344,  2448,  1394,  328,   200,  3,  46},// HDMI_1792x1344_60
#endif
#ifdef HDMI_1856x1392_60_Mode
  {1856,  1392,  2528,  1439,  352,   224,  3,  43},// HDMI_1856x1392_60
#endif
#ifdef HDMI_1920x1080_60_Mode
  {1920,  1080,  2200,  1125,  148,  44,    5,  36},// HDMI_1920x1080_60
#endif
#ifdef HDMI_1280x720_50_Mode
  {1280,  720,  1980,  750,  220,   40,    5,  20},// HDMI_1280x720_60
#endif
#ifdef HDMI_1920x1080_50_Mode
  {1920,  1080,  2640,  1125,  148,  44,    5,  36},// HDMI_1920x1080_60
#endif
#ifdef HDMI_720x576_50_Mode
  {720,  576,  864,  625,  68,   64,    5,  39},// HDMI_720x480_60
#endif
#ifdef HDMI_1152x864_60_Mode
  {1152,  864,  1520,  895,  176,  128,  3,  27},// HDMI_1152x864_75
#endif

};


unsigned short Input_Resolution[Resolution_Num][2]=
{
#ifdef HDMI_640x480_60_Mode
  {640,480},
#endif
#ifdef HDMI_720x480_60_Mode
  {720,480},
#endif
#ifdef HDMI_800x480_60_Mode
  {800,480},
#endif
#ifdef HDMI_800x600_60_Mode
  {800,600},
#endif
#ifdef HDMI_848x480_60_Mode
  {848,480},
#endif
#ifdef   HDMI_1024x600_60_Mode
  {1064,600},
#endif
#ifdef   HDMI_1024x768_60_Mode
  {1024,768},
#endif
#ifdef HDMI_1152x864_75_Mode
  {1152,864},
#endif
#ifdef HDMI_1280x720_60_Mode
  {1280,720},
#endif
#ifdef HDMI_1280x768_60_68MHz_Mode
  {1280,768},
#endif
#ifdef HDMI_1280x768_60_79Mhz_Mode
  {1280,768},
#endif
#ifdef   HDMI_1280x800_60_71MHz_Mode
  {1280,800},
#endif
#ifdef   HDMI_1280x800_60_83MHz_Mode
  {1280,800},
#endif

#ifdef HDMI_1280x960_60_Mode
  {1280,960},
#endif
#ifdef HDMI_1280x1024_60_Mode
  {1280,1024},
#endif
#ifdef HDMI_1360x768_60_Mode
  {1360,768},
#endif
#ifdef   HDMI_1366x768_60_72MHz_Mode
  {1366,768},
#endif
#ifdef   HDMI_1366x768_60_85MHz_Mode
  {1366,768},
#endif

#ifdef HDMI_1400x1050_60_Mode
  {1400,1050},
#endif
#ifdef HDMI_1440x900_60_88MHz_Mode
  {1440,900},
#endif
#ifdef HDMI_1440x900_60_106MHz_Mode
  {1440,900},
#endif
#ifdef HDMI_1600x900_60_Mode
  {1600,900},
#endif
#ifdef   HDMI_1600x1200_60_Mode
  {1600,1200},
#endif
#ifdef HDMI_1680x1050_60_Mode
  {1680,1050},
#endif
#ifdef HDMI_1792x1344_60_Mode
  {1792,1344},
#endif
#ifdef HDMI_1856x1392_60_Mode
  {1856,1392},
#endif
#ifdef   HDMI_1920x1080_60_Mode
  {1920,1080},
#endif
#ifdef HDMI_1280x720_50_Mode
  {1280,720},
#endif
#ifdef HDMI_1920x1080_50_Mode
  {1920,1200},
#endif
#ifdef HDMI_720x576_50_Mode
  {720,576},
#endif
#ifdef HDMI_1152x864_60_Mode
  {1152,864},
#endif

};

void Core_Pll_Config(void)
{  
//  float Core_PLL_Temp;
  union Temp  Core_PLL_Ratio ;
  unsigned char ret;

#ifdef _LVDS_Outout_
  lt8619b_write_reg(BK0_C9_REG, 0x02);
#endif
//*
#ifdef _640x480_Ouput_
  if(CurrentVideoMode == HDMI_640x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _800x480_Ouput_
  if(CurrentVideoMode == HDMI_800x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _854x480_Ouput_
  if(CurrentVideoMode == HDMI_848x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _864x480_Ouput_
    if(CurrentVideoMode == HDMI_848x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x768_Ouput_
    if(CurrentVideoMode == HDMI_1024x768_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x600_Ouput_
    if(CurrentVideoMode == HDMI_1024x600_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1280x720_Ouput_
  if(CurrentVideoMode == HDMI_1280x720_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif
#ifdef _1280x800_Ouput_
  if(CurrentVideoMode == HDMI_1280x800_60_71MHz_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif
    {
#ifdef _RGB_Outout_
    lt8619b_write_reg(BK1_DE_REG, 0x80);
#endif
    //BK0_B8_REG = 0x04;  
#ifdef _LVDS_Outout_
    Core_PLL_Ratio.Temp32 = (Panel_Pixel_CLK_Bypass/25)*7;// Panel_Pixel_CLK_Bypass

    lt8619b_write_reg(BK0_BD_REG, (u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF));
    ret = lt8619b_read_reg(BK0_BD_REG);
    lt8619b_write_reg(BK0_BD_REG, ret | 0x80);

    lt8619b_write_reg(BK0_BE_REG, 0x00);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[2];

    Core_PLL_Ratio.Temp32 = (Core_PLL_Ratio.Temp32 % 100)&0x000000FF;//((Core_PLL_Ratio.Temp32%100)/100)*16384;
    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32*16384;
    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32/100;

//    BK0_C0_REG = Core_PLL_Ratio.Temp8[2];//((Core_PLL_Ratio.Temp[1]/256)&0x000000FF);
//    BK0_BF_REG = Core_PLL_Ratio.Temp8[3];//((Core_PLL_Ratio%256)&0x000000FF);
    lt8619b_write_reg(BK0_C0_REG, Core_PLL_Ratio.Temp8[1]);
    lt8619b_write_reg(BK0_BF_REG, Core_PLL_Ratio.Temp8[0]);

    lt8619b_write_reg(BK0_C1_REG, 0x00);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[5];
    lt8619b_write_reg(BK0_C2_REG, 0x14);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[6];
    lt8619b_write_reg(BK0_C3_REG, 0x40);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[7]; 
    lt8619b_write_reg(BK0_C4_REG, 0x30);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[8]; 
#endif
    }
  else //*/
    {
#ifdef _RGB_Outout_
    lt8619b_write_reg(BK1_DE_REG, 0xC0);
    lt8619b_write_reg(BK0_B8_REG, 0x02);
    lt8619b_write_reg(BK0_C9_REG, 0x00);
#endif
    //corepll_tuning_en  

    Core_PLL_Ratio.Temp32 = (Panel_Pixel_CLK/25)*7;
//printk("Core_PLL_Ratio.Temp32 = 0x%x 0x%x\n", Core_PLL_Ratio.Temp32, (u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF));
//    BK0_BD_REG = (u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF);
    lt8619b_write_reg(BK0_BD_REG, (u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF));
    ret = lt8619b_read_reg(BK0_BD_REG);
    lt8619b_write_reg(BK0_BD_REG, ret | 0x80);//0x89;
    
    lt8619b_write_reg(BK0_BE_REG, 0x08);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[2];

    Core_PLL_Ratio.Temp32 = (Core_PLL_Ratio.Temp32 % 100)&0x000000FF;//((Core_PLL_Ratio.Temp32%100)/100)*16384;
    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32*16384;
    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32/100;
    
//    BK0_C0_REG = Core_PLL_Ratio.Temp8[2];//((Core_PLL_Ratio.Temp[1]/256)&0x000000FF);
//    lt8619b_write_reg(BK0_C0_REG, Core_PLL_Ratio.Temp8[2]);
    lt8619b_write_reg(BK0_C0_REG, Core_PLL_Ratio.Temp8[1]);
//    BK0_BF_REG = Core_PLL_Ratio.Temp8[3];//((Core_PLL_Ratio%256)&0x000000FF);
//    lt8619b_write_reg(BK0_BF_REG, Core_PLL_Ratio.Temp8[3]);
    lt8619b_write_reg(BK0_BF_REG, Core_PLL_Ratio.Temp8[0]);

    lt8619b_write_reg(BK0_C1_REG, 0x01);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[5];
    lt8619b_write_reg(BK0_C2_REG, 0x14);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[6];
    lt8619b_write_reg(BK0_C3_REG, 0x45);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[7]; 
    lt8619b_write_reg(BK0_C4_REG, 0x20);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[8]; 

  }

  lt8619b_write_reg(BK0_0F_REG, 0xbf);//A
  lt8619b_write_reg(BK0_10_REG, 0x7F);
  DelayTime(4000);
  lt8619b_write_reg(BK0_10_REG, 0xff);
  lt8619b_write_reg(BK0_0F_REG, 0xff);
}

void Panel_Param_setting(void)
{
  unsigned int f_DIV;
//  union Temp2 H_Scale_Ratio,V_Scale_Ratio,Count_Temp;
  union Temp2 Count_Temp;

  lt8619b_write_reg(BK1_71_REG, 0xFF);
  lt8619b_write_reg(BK1_72_REG, 0x03);
  lt8619b_write_reg(BK1_73_REG, (u8)(Input_Resolution[CurrentVideoMode][0]%256));
  lt8619b_write_reg(BK1_74_REG, 0x01);
  lt8619b_write_reg(BK1_75_REG, ((u8)(Input_Resolution[CurrentVideoMode][0]/256))&0x07);
  lt8619b_write_reg(BK1_76_REG, 0x02);
  lt8619b_write_reg(BK1_77_REG, (u8)(Panel_H_Total%256));
  lt8619b_write_reg(BK1_78_REG, (u8)(Panel_H_BackPorch%256));
  
  Count_Temp.Temp16 = (Panel_H_BackPorch/256)&0x000F;
  Count_Temp.Temp16 <<= 4;
  Count_Temp.Temp16 += (Panel_H_Total/256)&0x000F;
//  lt8619b_write_reg(BK1_79_REG, Count_Temp.Temp8[1]);//(((u8)(Panel_H_BackPorch/256))<<4)&0xF0+((u8)(Panel_H_Total/256))&0x0F;
  lt8619b_write_reg(BK1_79_REG, Count_Temp.Temp8[0]);

  lt8619b_write_reg(BK1_7A_REG, Panel_H_SyncWidth);
  lt8619b_write_reg(BK1_7B_REG, Panel_V_SyncWidth&0x0F);
  lt8619b_write_reg(BK1_7C_REG, (u8)(Panel_H_Active%256));
  lt8619b_write_reg(BK1_7D_REG, (u8)(Panel_V_Active%256));
  
  Count_Temp.Temp16 = (Panel_V_Active/256)&0x000F;
  Count_Temp.Temp16 <<= 4;
  Count_Temp.Temp16 += (Panel_H_Active/256)&0x000F;  
//  lt8619b_write_reg(BK1_7E_REG, Count_Temp.Temp8[1]);//(((u8)(Panel_V_Active/256))<<4)&0xF0+((u8)(Panel_H_Active/256))&0x0F;
  lt8619b_write_reg(BK1_7E_REG, Count_Temp.Temp8[0]);

  lt8619b_write_reg(BK1_7F_REG, 0x00);
  lt8619b_write_reg(BK1_80_REG, 0x00);

//  f_DIV = (((float)Input_Resolution[CurrentVideoMode][0]-1)/(float)(Panel_H_Active-1))*4096;
  f_DIV = ((Input_Resolution[CurrentVideoMode][0]-1)*4096/(Panel_H_Active-1));
  Count_Temp.Temp16 = (u16)f_DIV;
  
#ifdef _854x480_Ouput_  
  if(CurrentVideoMode == HDMI_1920x1080_60_Mode)//&&(BK1_DB_REG&0x0A))
    {
    Count_Temp.Temp16 = 0x23FA; // For 众进思创的笔记本
    }
#endif

//  BK1_81_REG = Count_Temp.Temp8[1];//(u8)(H_Scale_Ratio%256);
//  lt8619b_write_reg(BK1_81_REG, Count_Temp.Temp8[1]);
  lt8619b_write_reg(BK1_81_REG, Count_Temp.Temp8[0]);
//  BK1_82_REG = Count_Temp.Temp8[0];////(u8)(H_Scale_Ratio/256);
//  lt8619b_write_reg(BK1_82_REG,  Count_Temp.Temp8[0]);
  lt8619b_write_reg(BK1_82_REG,  Count_Temp.Temp8[1]);
  
  lt8619b_write_reg(BK1_83_REG, 0x00);
  
//  f_DIV = (((float)Input_Resolution[CurrentVideoMode][1]-1)/(float)(Panel_V_Active-1))*4096;
  f_DIV = ((Input_Resolution[CurrentVideoMode][1]-1)*4096/(Panel_V_Active-1));
  Count_Temp.Temp16 = (u16)f_DIV;
//  lt8619b_write_reg(BK1_84_REG, Count_Temp.Temp8[1]);//(u8)(V_Scale_Ratio%256);
  lt8619b_write_reg(BK1_84_REG, Count_Temp.Temp8[0]);
//  lt8619b_write_reg(BK1_85_REG, Count_Temp.Temp8[0]);//(u8)(V_Scale_Ratio/256);
  lt8619b_write_reg(BK1_85_REG, Count_Temp.Temp8[1]);
}

void Scalar_UpDown_Set(void)
{
//  unsigned char loopx;
  unsigned char ret;

  lt8619b_write_reg(BK1_90_REG, 0x00);//VP processing control: normal mode   //???????????

#ifdef _640x480_Ouput_
  if(CurrentVideoMode == HDMI_640x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _800x480_Ouput_
  if(CurrentVideoMode == HDMI_800x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _854x480_Ouput_
  if(CurrentVideoMode == HDMI_848x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _864x480_Ouput_
  if(CurrentVideoMode == HDMI_848x480_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x768_Ouput_
  if(CurrentVideoMode == HDMI_1024x768_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x600_Ouput_
  if(CurrentVideoMode == HDMI_1024x600_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1280x720_Ouput_
  if(CurrentVideoMode == HDMI_1280x720_60_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif
#ifdef _1280x800_Ouput_
  if(CurrentVideoMode == HDMI_1280x800_60_71MHz_Mode)//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif
    {
    lt8619b_write_reg(BK1_70_REG, 0x00);//bypass between input and output
#if 1
    ret = lt8619b_read_reg(BK0_0D_REG);
    lt8619b_write_reg(BK0_0D_REG, ret & 0xF3);
    DelayTime(500);
    lt8619b_write_reg(BK0_0D_REG, 0xFF);
#endif
    }
  else //*/
  {
    Panel_Param_setting();

    lt8619b_write_reg(BK1_70_REG, 0x82);//scale_en,3'SG,scl_adj_hbnk_en,scl_hbnk_hold,scl_de_mode_en,clear_rw_confc
#if 1
    ret = lt8619b_read_reg(BK0_0D_REG);
    lt8619b_write_reg(BK0_0D_REG, ret & 0xF3);
    DelayTime(500);
    lt8619b_write_reg(BK0_0D_REG, 0xFF);
#endif
    DelayTime(12000);//17ms=1s/60
    lt8619b_write_reg(BK1_70_REG, 0x8A);
  }
}

void Pre_Post_Pattern(void)
{
  lt8619b_write_reg(BK1_33_REG, 0x60);//Pattern mode select
  lt8619b_write_reg(BK1_34_REG, 0x13);//Pattern mode select
  lt8619b_write_reg(BK1_35_REG, 0xf0);//Pattern mode select
  
  if(CurrentInputSource == HDMIINPUT)
  {
    lt8619b_write_reg(BK1_36_REG, 0x01);//hdmi 
  }
  else if(CurrentInputSource == MHLINPUT)  
  {
    lt8619b_write_reg(BK1_36_REG, 0x01);//mhl
  }    
#ifdef _LVDS_Outout_
  lt8619b_write_reg(BK1_36_REG, 0x01);
#endif
  lt8619b_write_reg(BK1_90_REG, 0x01);//VP processing control
}

//void Gcm_Window_Set(void)
//{
//  ///*DelayTime(5000);
//  //if(Reg68ex_Sync_Ctrl0 & 0x01)
//  //  BK1_00_REG |= 0x08;
//  //else
//  //  BK1_00_REG &= 0xf7;
//  //BK1_00_REG  |= 0x04;*
//  if(CurrentVideoMode == VESA_640x480_60_Mode)
//    BK1_00_REG = 0x84;//??
//  else
//    BK1_00_REG = 0x8c;
//  
//  BK1_17_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[0];  
//  BK1_18_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[1];  
//  BK1_19_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[2];  
//  BK1_1A_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[3];  
//  BK1_1B_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[4];
//  BK1_1C_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[5];
//  BK1_1D_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[6];
//  BK1_1E_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[7];
//  BK1_1F_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[8];
//  BK1_20_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[9];
//  BK1_21_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[10];
//  BK1_22_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[11];
//  BK1_23_REG = Gcm_Rel_Param[CurrentVideoMode].Gcm_Data[12];
//
//  #if 0
//  BK0_0C_REG &=0xef;//???
//  DelayTime(500);
//  BK0_0C_REG = 0xff; 
//  #endif
//}

void Gcm_Pattern_Set(void)
{
  union Temp2 Input_Temp;
  
  if(CurrentVideoMode == HDMI_1024x768_60_Mode)//|| HDMI_640x480_60_Mode CurrentVideoMode == HDMI_640x480_60_Mode )
    lt8619b_write_reg(BK1_00_REG, 0x84);//??
  else
    lt8619b_write_reg(BK1_00_REG, 0x8c);
  
  lt8619b_write_reg(BK1_17_REG, 0xff);
  lt8619b_write_reg(BK1_18_REG, 0x00);
  
  Input_Temp.Temp16 = Input_Resolution_Param[CurrentVideoMode].Input_Param[Hs_Wid]+Input_Resolution_Param[CurrentVideoMode].Input_Param[H_BP];
  
//  lt8619b_write_reg(BK1_19_REG, Input_Temp.Temp8[1]);
  lt8619b_write_reg(BK1_19_REG, Input_Temp.Temp8[0]);
  lt8619b_write_reg(BK1_1A_REG, (u8)(Input_Resolution_Param[CurrentVideoMode].Input_Param[H_Act]%256));
  
  Input_Temp.Temp16 &= 0x0F00;
  Input_Temp.Temp16 >>= 8;
  Input_Temp.Temp16 += (((Input_Resolution_Param[CurrentVideoMode].Input_Param[H_Act]/256)&0x000F)<<4);
//  lt8619b_write_reg(BK1_1B_REG,  Input_Temp.Temp8[1]);
  lt8619b_write_reg(BK1_1B_REG,  Input_Temp.Temp8[0]);

  Input_Temp.Temp16 = Input_Resolution_Param[CurrentVideoMode].Input_Param[Vs_Wid]+Input_Resolution_Param[CurrentVideoMode].Input_Param[V_BP];
//  lt8619b_write_reg(BK1_1C_REG, Input_Temp.Temp8[1]&0x7F);
  lt8619b_write_reg(BK1_1C_REG, Input_Temp.Temp8[0]&0x7F);

  lt8619b_write_reg(BK1_1D_REG, (u8)(Input_Resolution_Param[CurrentVideoMode].Input_Param[V_Act]%256));
  lt8619b_write_reg(BK1_1E_REG, (u8)(Input_Resolution_Param[CurrentVideoMode].Input_Param[H_Tol]%256));
  
  Input_Temp.Temp16 = (Input_Resolution_Param[CurrentVideoMode].Input_Param[H_Tol]/256)&0x000F;
  Input_Temp.Temp16 <<= 4;
  Input_Temp.Temp16 += (Input_Resolution_Param[CurrentVideoMode].Input_Param[V_Act]/256)&0x07;
//  lt8619b_write_reg(BK1_1F_REG, Input_Temp.Temp8[1]);
  lt8619b_write_reg(BK1_1F_REG, Input_Temp.Temp8[0]);
  
  lt8619b_write_reg(BK1_20_REG, (u8)(Input_Resolution_Param[CurrentVideoMode].Input_Param[V_Tol]%256));
  lt8619b_write_reg(BK1_21_REG, (u8)((Input_Resolution_Param[CurrentVideoMode].Input_Param[V_Tol]/256)&0x0007));
  
  lt8619b_write_reg(BK1_22_REG, (u8)(Input_Resolution_Param[CurrentVideoMode].Input_Param[Hs_Wid]%256));
  Input_Temp.Temp16 = (Input_Resolution_Param[CurrentVideoMode].Input_Param[Hs_Wid]/256)&0x000F;
  Input_Temp.Temp16 <<= 4;
  Input_Temp.Temp16 += Input_Resolution_Param[CurrentVideoMode].Input_Param[Vs_Wid];
//  lt8619b_write_reg(BK1_23_REG, Input_Temp.Temp8[1]);
  lt8619b_write_reg(BK1_23_REG, Input_Temp.Temp8[0]);

  #if 0
  BK0_0C_REG &=0xef;//???
  DelayTime(500);
  BK0_0C_REG = 0xff; 
  #endif
}

void HDMI_Pol_Adjust(void)
{
  unsigned char ret;
  DelayTime(50000);
  if(lt8619b_read_reg(BK2_00_REG) & 0x04)//vid_vspol  0x8200[2] = 1 
  {
    ret = lt8619b_read_reg(BK2_3F_REG);
    lt8619b_write_reg(BK2_3F_REG, ret & 0xfe);//clear vs_pol_adj 0 0x823f[0] = 0
  }
  else
  {
    ret = lt8619b_read_reg(BK2_3F_REG);
    lt8619b_write_reg(BK2_3F_REG, ret | 0x01);//set vs_pol_adj 0x823f[0] = 1 
  }
     
  if(lt8619b_read_reg(BK2_00_REG) & 0x08)//vid_hspol 0x8200[3]= 1
  {
    ret = lt8619b_read_reg(BK2_3F_REG);
    lt8619b_write_reg(BK2_3F_REG, ret & 0xfd);//clear hs_pol_adj 0x823f[1] = 0
  }
  else
  {
    ret = lt8619b_read_reg(BK2_3F_REG);
    lt8619b_write_reg(BK2_3F_REG, ret | 0x02);//set hs_pol_adj 0x823f[1] = 1
  }
}

void Core_Pll_Config_Nosync(void)
{  
  union Temp  Core_PLL_Ratio ;
  unsigned char ret;
  lt8619b_write_reg(BK1_DE_REG, 0xC0);
#ifdef _RGB_Outout_
  lt8619b_write_reg(BK0_B8_REG, 0x02);
  lt8619b_write_reg(BK0_C9_REG, 0x00);
#endif
#ifdef _LVDS_Outout_
  lt8619b_write_reg(BK0_C9_REG, 0x02);
#endif
  //corepll_tuning_en  

  //    Core_PLL_Temp = (3650/25)*7;
  //    Core_PLL_Ratio.Temp32 = Core_PLL_Temp;//(Panel_Pixel_CLK/25)*7;
#ifdef _854x480_Ouput_    
  lt8619b_write_reg(BK0_BD_REG, 0x09);//(u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF);
  lt8619b_write_reg(BK0_BD_REG, 0x89);

  lt8619b_write_reg(BK0_BE_REG, 0x08);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[2];

  //    Core_PLL_Ratio.Temp32 = (Core_PLL_Ratio.Temp32 % 100)&0x000000FF;//((Core_PLL_Ratio.Temp32%100)/100)*16384;
  //    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32*16384;
  //    Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32/100;

  lt8619b_write_reg(BK0_C0_REG, 0x88);//Core_PLL_Ratio.Temp8[2];//((Core_PLL_Ratio.Temp[1]/256)&0x000000FF);
  lt8619b_write_reg(BK0_BF_REG, 0x4C);//Core_PLL_Ratio.Temp8[3];//((Core_PLL_Ratio%256)&0x000000FF);
#else

//#ifdef _1280x720_Ouput_
  Core_PLL_Ratio.Temp32 = (Panel_Pixel_CLK/25)*7;

  lt8619b_write_reg(BK0_BD_REG, (u8)((Core_PLL_Ratio.Temp32/100)&0x000000FF));
    ret = lt8619b_read_reg(BK0_BD_REG);
    lt8619b_write_reg(BK0_BD_REG, ret | 0x80);//0x89;

  lt8619b_write_reg(BK0_BE_REG, 0x08);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[2];

  Core_PLL_Ratio.Temp32 = (Core_PLL_Ratio.Temp32 % 100)&0x000000FF;//((Core_PLL_Ratio.Temp32%100)/100)*16384;
  Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32*16384;
  Core_PLL_Ratio.Temp32 = Core_PLL_Ratio.Temp32/100;

//  lt8619b_write_reg(BK0_C0_REG, Core_PLL_Ratio.Temp8[2]);//((Core_PLL_Ratio.Temp[1]/256)&0x000000FF);
  lt8619b_write_reg(BK0_C0_REG, Core_PLL_Ratio.Temp8[1]);
//  lt8619b_write_reg(BK0_BF_REG, Core_PLL_Ratio.Temp8[3]);//((Core_PLL_Ratio%256)&0x000000FF);
  lt8619b_write_reg(BK0_BF_REG, Core_PLL_Ratio.Temp8[0]);

#endif

  lt8619b_write_reg(BK0_C1_REG, 0x01);// Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[5];
  lt8619b_write_reg(BK0_C2_REG, 0x14);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[6];
  lt8619b_write_reg(BK0_C3_REG, 0x45);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[7]; 
  lt8619b_write_reg(BK0_C4_REG, 0x20);//Core_Pll_Param[CurrentVideoMode].Core_Pll_Data[8]; 

  lt8619b_write_reg(BK0_0F_REG, 0xbf);
  lt8619b_write_reg(BK0_10_REG, 0x7F);
  DelayTime(4000);
  lt8619b_write_reg(BK0_10_REG, 0xff);
  lt8619b_write_reg(BK0_0F_REG, 0xff);
}


void Pattern_Output_Config(void)
{
#ifdef _640x480_Ouput_
    CurrentVideoMode = HDMI_640x480_60_Mode ;//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0 //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _800x480_Ouput_
    CurrentVideoMode = HDMI_800x480_60_Mode ;//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0 //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _854x480_Ouput_
  CurrentVideoMode = HDMI_848x480_60_Mode; //|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _864x480_Ouput_
  CurrentVideoMode = HDMI_848x480_60_Mode; //|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x768_Ouput_
  CurrentVideoMode = HDMI_1024x768_60_Mode; //|| CurrentVideoMode == MHL_1280x720_60_Mode) //0 //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1024x600_Ouput_
  CurrentVideoMode = HDMI_1024x600_60_Mode; //|| CurrentVideoMode == MHL_1280x720_60_Mode) //0 //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1280x720_Ouput_
  CurrentVideoMode = HDMI_1280x720_60_Mode; //|| CurrentVideoMode == MHL_1280x720_60_Mode) //0 //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

#ifdef _1280x800_Ouput_
  CurrentVideoMode = HDMI_1280x800_60_71MHz_Mode;//|| CurrentVideoMode == MHL_1280x720_60_Mode) //0  //CurrentVideoMode == HDMI_800x480_60_Mode
#endif

  Core_Pll_Config_Nosync();
  Gcm_Pattern_Set();
  Pre_Post_Pattern();

  CurrentVideoMode = VIDEO_NOSYNC;
}

void RxHdmiHpdHigh(void)
{
  lt8619b_write_reg(BK0_D1_REG, 0x01);
  lt8619b_write_reg(BK2_2B_REG, 0x14);//high
  lt8619b_write_reg(BK2_BF_REG, 0x08);
}

void RxHdmiHpdLow(void)
{
  lt8619b_write_reg(BK0_D1_REG, 0x01);//hdp mode, no cbus
  lt8619b_write_reg(BK2_2B_REG, 0x10);//hpd low
  lt8619b_write_reg(BK2_BF_REG, 0x08);//cbus 1k disable  
}



