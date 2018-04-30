/************************************************************
*   Project Name:  lt8619
*   File Name:     misc.c
*   Build Date:    2014-10-20
*   Version：
*   Copyright:     Lontium Semi.
************************************************************/

#include "include.h"



void DelayTime(uint i)  // 1.7us    
{
   while(i --);
}


void DelayTime1ms(uchar time)
{
  u8 loopx;
  for(loopx=0; loopx<time; loopx++)
  {
//    DelayTime(565);
    msleep(1);
  }
}


void DelayTime10ms(uchar time)
{
  u8 loopx;
  for(loopx=0; loopx<time; loopx++)
  {
//    DelayTime(5682);
    msleep(10);
  }
}

void DelayTime100ms(uchar time)
{
  u8 loopx;
  for(loopx=0; loopx<time; loopx++)
  {
//    DelayTime(56820);
    msleep(100);
  }
}
/*
void DelayTime1000ms(uchar time)
{
  uchar loopx,loopy;
  for(loopx=0; loopx<time; loopx++)
  {
    for(loopy=0; loopy<10; loopy++)
    {
      DelayTime100ms(1);
    }
  }
}
//*/

void HPD_CBUS_Ctl(u8 input)
{
  switch(input)
  {
    case HDMIINPUT:

      break;
      
    case VGAINPUT:

      break;
      
    case MHLINPUT:

      break;  

    default:
          
      break;        
  }
}


/*
void HDMI_Input_Mode_Conif()
{
  IE |= 0x80;
  EIE |= 0x02;

  BK1_ED_REG &= 0xfe;     //clear 0x81ed[0],to not clear the interrupt flag
  BK1_E7_REG &= 0xfe;    //clear 0x81e7[0],to open interrupt mask
}
//*/

void HdmiMhl_Source_Input_Judge(void)
{
  
//  if(BK0_23_REG & 0x80)  //CD_SENSE
  if(lt8619b_read_reg(BK0_23_REG) & 0x80)  //CD_SENSE
  {
    MHL_Input_Flag = 1;
  }
  else
  {
  /*
    if(PreInputSource==0x03) //0x03 - MHLINPUT
    {
        VBUS_Charge_Enable(DISABLE);
      DelayTime100ms(1);
    }//*/
  //  if(BK0_23_REG & 0x20)  // don't detect HDMI 5V
    {
      HDMI_Input_Flag = 1;
    }
    /*
    else
    {
      HDMI_Input_Flag = 0;
    }//*/
  }
}



void Get_Mhl_Input_Video_Mode(void)
{
  uint Hor_Count = 0;
  uint Ver_Count = 0;
  
  Hor_Count = lt8619b_read_reg(BK1_DB_REG) & 0x0f;//81db
  Hor_Count <<= 8;
  Hor_Count += lt8619b_read_reg(BK1_D9_REG);//81d9
  
  Ver_Count = lt8619b_read_reg(BK1_DB_REG) & 0xf0;
  Ver_Count <<= 4;
  Ver_Count += lt8619b_read_reg(BK1_DA_REG);//81da

 if(Ver_Count >= 500 && Ver_Count <= 536)  
  {
/*     if(Hor_Count >= 780 && Hor_Count <= 820)//hdmi 640*480  v:525   h:800
     {
      CurrentVideoMode = HDMI_640x480_60_Mode;
     }
    else//*/
      if(Hor_Count >= 840 && Hor_Count <= 870)//hdmi 720*480  v:525   h:858
     {
      CurrentVideoMode = MHL_720x480_60_Mode;
     }
/*    else
      if(Hor_Count >= 1070 && Hor_Count <= 1100)//hdmi 848*480  v:517   h:1088
     {
      CurrentVideoMode = HDMI_848x480_60_Mode;
     }//*/
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }/*//
  else 

   if(Ver_Count >= 610 && Ver_Count <= 640)  //hdmi 800*600  v:628   h:1056
  {
     if(Hor_Count >= 1040 && Hor_Count <= 1070)
     {
      CurrentVideoMode = HDMI_800x600_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }
  else 
    if(Ver_Count >= 785 && Ver_Count <= 814)  
  {
     if(Hor_Count >= 1320 && Hor_Count <= 1360)//hdmi 1024*768  v:806   h:1344
     {
      CurrentVideoMode = HDMI_1024x768_60_Mode;
     }
    else
    if(Hor_Count >= 1650 && Hor_Count <= 1680)//hdmi 1280*768  v:798   h:1664
     {
      CurrentVideoMode = HDMI_1280x768_60_Mode;
     }
    else
    if(Hor_Count >= 1780 && Hor_Count <= 1810)//hdmi 1360*768  v:798   h:1792
     {
      CurrentVideoMode = HDMI_1360x768_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }//*/
  else 
    if(Ver_Count >= 0x2d0 && Ver_Count <= 0x2ff)  //hdmi 1280x720  v:750   h:1650
      {
      if(Hor_Count >= 0x652 && Hor_Count <= 0x692)
        {
        CurrentVideoMode = MHL_1280x720_60_Mode;
        }
      else
        {
        CurrentVideoMode = VIDEO_NOSYNC;
        }
      }/*//
  else 
    if(Ver_Count >= 815 && Ver_Count <= 840)  //hdmi 1280x800  v:823   h:1440
  {
     if(Hor_Count >= 1430 && Hor_Count <= 1450)
     {
      CurrentVideoMode = HDMI_1280x800_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }  
  else 
    if(Ver_Count >= 990 && Ver_Count <= 1010)  //hdmi 1280x960  v:1000   h:1800
  {
     if(Hor_Count >= 1790 && Hor_Count <= 1810)
     {
      CurrentVideoMode = HDMI_1280x960_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }  
  else 
    if(Ver_Count >= 1050 && Ver_Count <= 1080)  //hdmi 1280x1024  v:1066   h:1688
  {
     if(Hor_Count >= 1675 && Hor_Count <= 1700)
     {
      CurrentVideoMode = HDMI_1280x1024_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }  
  else 
    if(Ver_Count >= 920 && Ver_Count <= 950)  //hdmi 1440x900  v:934   h:1904
  {
     if(Hor_Count >= 1890 && Hor_Count <= 1920)
     {
      CurrentVideoMode = HDMI_1440x900_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }//*/
  /*
  else 
    if(Ver_Count >= 990 && Ver_Count <= 1010)  //hdmi 1600x900  v:1000   h:1800
  {
     if(Hor_Count >= 1790 && Hor_Count <= 1810)
     {
      CurrentVideoMode = HDMI_1600x900_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }  //*/
/*  else 
    if(Ver_Count >= 1240 && Ver_Count <= 1060)  //hdmi 1600x1200  v:1250   h:2160
  {
     if(Hor_Count >= 2150 && Hor_Count <= 2170)
     {
      CurrentVideoMode = HDMI_1600x1200_60_Mode;
     }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }
  }
  else
    if(Ver_Count >= 0x455 && Ver_Count <= 0x480)  //hdmi 1920x1080 v:0x466 h:0x898
  {
      if(Hor_Count >= 0x878 && Hor_Count <= 0x8b8)
      {
      CurrentVideoMode = HDMI_1920x1080_60_Mode;
      }
    else
    {
      CurrentVideoMode = VIDEO_NOSYNC;
    }    
  }//*/
  else
    {
    CurrentVideoMode = VIDEO_NOSYNC;
    }
  
//    BK1_F3_REG = CurrentVideoMode;
    lt8619b_write_reg(BK1_F3_REG, CurrentVideoMode);
}


//get the hdmi input video mode     before get the hdmi input vic config rx relation registers
unsigned char Get_Hdmi_Input_Video_Mode(void)
{
  uint Hor_Count = 0;
  uint Ver_Count = 0;
  uint DeCount=0;

  if(!CurrentHdmiClk )//rx hdmi no clk
    CurrentVideoMode = VIDEO_NOSYNC;
  else   //hdmi clk
  {
    Hor_Count = lt8619b_read_reg(BK1_DB_REG) & 0x0F;//81db
    Hor_Count <<= 8;
    Hor_Count += lt8619b_read_reg(BK1_D9_REG);//81d9
    
    Ver_Count = lt8619b_read_reg(BK1_DB_REG) & 0xF0;
    Ver_Count <<= 4;
    Ver_Count += lt8619b_read_reg(BK1_DA_REG);//81da

    DeCount = lt8619b_read_reg(BK1_DD_REG) & 0x0F;
    DeCount <<=8;
    DeCount += lt8619b_read_reg(BK1_DC_REG);

printk("Hor_Count=%4d Ver_Count=%4d DeCount=%4d\n", Hor_Count, Ver_Count, DeCount);
    if(Ver_Count >= 0x1f0 && Ver_Count <= 0x220)    //hdmi 720x480  v:0x20e 
    {
#ifdef HDMI_640x480_60_Mode
      if(Hor_Count >= 0x310 && Hor_Count <= 0x330) // 0x320
        CurrentVideoMode = HDMI_640x480_60_Mode;  
      else 
#endif
#ifdef HDMI_720x480_60_Mode
      if(Hor_Count >= 0x340 && Hor_Count <= 0x370) // h:0x35a
        CurrentVideoMode = HDMI_720x480_60_Mode;
      else
#endif
#ifdef HDMI_800x480_60_Mode
      if(Hor_Count >= 0x410 && Hor_Count < 0x430) // h:0x35a
        CurrentVideoMode = HDMI_800x480_60_Mode;
      else
#endif

#ifdef HDMI_848x480_60_Mode
      if(Hor_Count >= 0x430 && Hor_Count <= 0x450) // h:0x440
        CurrentVideoMode = HDMI_848x480_60_Mode;
      else
#endif
      CurrentVideoMode = VIDEO_NOSYNC;
    }
    else if(Ver_Count >= 0x255 && Ver_Count <= 0x295)  //hdmi 800x600  v:0x275  h:0x420
    {
#ifdef HDMI_800x600_60_Mode
      if(Hor_Count >= 0x400 && Hor_Count <= 0x440) // 0x420
        CurrentVideoMode = HDMI_800x600_60_Mode;
      else
#endif

#ifdef HDMI_1024x600_60_Mode
      if(Hor_Count >= 0x530 && Hor_Count <= 0x550) // 0x420
        CurrentVideoMode = HDMI_1024x600_60_Mode;
      else
#endif

#ifdef HDMI_720x576_50_Mode
      if(Hor_Count >= 0x350 && Hor_Count <= 0x370) // 0x420
        CurrentVideoMode = HDMI_720x576_50_Mode;
      else
#endif
      CurrentVideoMode = VIDEO_NOSYNC;
    }
    else if(Ver_Count >= 780 && Ver_Count <= 814)  
    {
#ifdef HDMI_1024x768_60_Mode
      if(Hor_Count >= 1320 && Hor_Count <= 1360)//hdmi 1024x768
        CurrentVideoMode = HDMI_1024x768_60_Mode;  
      else 
#endif
#ifdef HDMI_1280x768_60_68MHz_Mode
      if(Hor_Count >= 1420 && Hor_Count <= 1460)//hdmi 1280*768  v:798  h:1664
        CurrentVideoMode = HDMI_1280x768_60_68MHz_Mode;
      else
#endif
#ifdef HDMI_1280x768_60_79Mhz_Mode
      if(Hor_Count >= 1650 && Hor_Count <= 1680)//hdmi 1280*768  v:798  h:1664
        CurrentVideoMode = HDMI_1280x768_60_79Mhz_Mode;
      else
#endif

#ifdef HDMI_1366x768_60_72MHz_Mode
      if(Hor_Count >= 1480 && Hor_Count <= 1520)//hdmi 1366*768  v:800  h:1500
        CurrentVideoMode = HDMI_1366x768_60_72MHz_Mode;
      else
#endif

      if(Hor_Count >= 1772 && Hor_Count <= 1812)
      {
#ifdef HDMI_1360x768_60_Mode
        if(Ver_Count>=790 && Ver_Count<797)
          CurrentVideoMode = HDMI_1360x768_60_Mode;
        else
#endif  
#ifdef HDMI_1366x768_60_85MHz_Mode
        if(Ver_Count>=797 && Ver_Count<800)
          CurrentVideoMode = HDMI_1366x768_60_85MHz_Mode;
        else
#endif
        CurrentVideoMode = VIDEO_NOSYNC;
      }
    }
    else if(Ver_Count >= 884 && Ver_Count <= 916)
    {
#ifdef HDMI_1152x864_75_Mode
      if(Hor_Count >= 0x630 && Hor_Count <= 0x650)
        CurrentVideoMode = HDMI_1152x864_75_Mode;
      else
#endif
#ifdef HDMI_1152x864_60_Mode
      if(Hor_Count >= 0x5e0 && Hor_Count <= 0x600)
        CurrentVideoMode = HDMI_1152x864_60_Mode;
      else
#endif
      CurrentVideoMode = VIDEO_NOSYNC;
    }
    else if(Ver_Count >= 0x2d0 && Ver_Count <= 0x2ff)  //hdmi 1280x720 v:0x2ef  h:0x672
    {
#ifdef HDMI_1280x720_60_Mode
      if(Hor_Count >= 0x652 && Hor_Count <= 0x692)
        CurrentVideoMode = HDMI_1280x720_60_Mode;
      else
#endif
#ifdef HDMI_1280x720_50_Mode
      if(Hor_Count >= 0x7ac && Hor_Count <= 0x7cc)
        CurrentVideoMode = HDMI_1280x720_50_Mode;
      else
#endif
        CurrentVideoMode = VIDEO_NOSYNC;
    }
    else if(Ver_Count >= 815 && Ver_Count <= 840)  //hdmi 1280x800 v:823  h:1440
    {
#ifdef HDMI_1280x800_60_83MHz_Mode
      if(Hor_Count >= 1660 && Hor_Count <= 1690)
        CurrentVideoMode = HDMI_1280x800_60_83MHz_Mode;
      else
#endif
#ifdef HDMI_1280x800_60_71MHz_Mode
      if(Hor_Count >= 1420 && Hor_Count <= 1460)
        CurrentVideoMode = HDMI_1280x800_60_71MHz_Mode;
      else
#endif
      CurrentVideoMode = VIDEO_NOSYNC;
    }  
    else if(Ver_Count >= 990 && Ver_Count <= 1010)  
    {
      if(Hor_Count >= 1790 && Hor_Count <= 1810)
      {
#ifdef HDMI_1600x900_60_Mode
        if(DeCount >= 890 && DeCount <= 910) //hdmi 1600x900 v:1000 h:1800
          CurrentVideoMode = HDMI_1600x900_60_Mode;
        else
#endif
#ifdef HDMI_1280x960_60_Mode
        if(DeCount >= 950 && DeCount <= 970)  //hdmi 1280x960 v:1000  h:1800
          CurrentVideoMode = HDMI_1280x960_60_Mode;
        else
#endif
        CurrentVideoMode = VIDEO_NOSYNC;
      }
      else
        CurrentVideoMode = VIDEO_NOSYNC;
    }  
#ifdef HDMI_1280x1024_60_Mode
    else if(Ver_Count >= 0x41A && Ver_Count <= 0x43A)  //hdmi 1280x1024  v:0x42A  h:0x698
    {
      if(Hor_Count >= 0x688 && Hor_Count <= 0x6A8)
        CurrentVideoMode = HDMI_1280x1024_60_Mode;
      else
        CurrentVideoMode = VIDEO_NOSYNC;
    }
#endif
    else if(Ver_Count > 1073 && Ver_Count < 1105)
    {
#ifdef HDMI_1400x1050_60_Mode
      if(Hor_Count> 1848&& Hor_Count<1880) 
        CurrentVideoMode = HDMI_1400x1050_60_Mode;
      else 
#endif
#ifdef HDMI_1680x1050_60_Mode
      if(Hor_Count> 2126 && Hor_Count< 2256) 
        CurrentVideoMode = HDMI_1680x1050_60_Mode;
      else
#endif
      CurrentVideoMode = VIDEO_NOSYNC;
    }
    else if(Ver_Count >= 920 && Ver_Count <= 950)  //hdmi 1440x900 v:934  h:1904
    {
#ifdef HDMI_1440x900_60_88MHz_Mode
      if(Hor_Count >= 1580 && Hor_Count <= 1620)
        CurrentVideoMode = HDMI_1440x900_60_88MHz_Mode;
      else
#endif

#ifdef HDMI_1440x900_60_106MHz_Mode
      if(Hor_Count >= 1890 && Hor_Count <= 1920)
        CurrentVideoMode = HDMI_1440x900_60_106MHz_Mode;
      else
#endif
        CurrentVideoMode = VIDEO_NOSYNC;
    } 

#ifdef HDMI_1600x1200_60_Mode
    else if(Ver_Count >= 1240 && Ver_Count <= 1260)  //hdmi 1600x1200  v:1250  h:2160
    {
      //  if(Hor_Count >= 2150 && Hor_Count <= 2170)
      if(DeCount >= 1190 && DeCount <= 1210)
        CurrentVideoMode = HDMI_1600x1200_60_Mode;
      else
        CurrentVideoMode = VIDEO_NOSYNC;
    }
#endif
#ifdef HDMI_1792x1344_60_Mode
    else if(Ver_Count > 1378 && Ver_Count < 1410)
    {
      if(Hor_Count>2232 && Hor_Count<2264) // 1600x1200
        CurrentVideoMode = HDMI_1792x1344_60_Mode;
      else
        CurrentVideoMode = VIDEO_NOSYNC;
    }
#endif
#ifdef HDMI_1856x1392_60_Mode
    else if(Ver_Count > 1420 && Ver_Count < 1455)
    {
      if(Hor_Count>2512 && Hor_Count<2544) // 1600x1200
        CurrentVideoMode = HDMI_1856x1392_60_Mode;
      else
        CurrentVideoMode = VIDEO_NOSYNC;
    }
#endif
    else if(Ver_Count >= 0x455 && Ver_Count <= 0x480)  //hdmi 1920x1080 v:0x466 h:0x898
    {
#ifdef HDMI_1920x1080_60_Mode
      if(Hor_Count >= 0xa00 && Hor_Count <= 0xa20)// for 众进思创
        CurrentVideoMode = HDMI_1920x1080_60_Mode;
      else if(Hor_Count >= 0x878 && Hor_Count <= 0x8b8)
        CurrentVideoMode = HDMI_1920x1080_60_Mode;
      else
#endif
#ifdef HDMI_1920x1080_50_Mode
      if(Hor_Count >= 0xa40 && Hor_Count <= 0xa60)
        CurrentVideoMode = HDMI_1920x1080_50_Mode;
      else
#endif
        CurrentVideoMode = VIDEO_NOSYNC;
    }
    else
      CurrentVideoMode = VIDEO_NOSYNC;
  }
#ifdef _RGB_Outout_
    if(PreVideoMode != CurrentVideoMode)
      TTL_Output_Control(DISABLE);
#endif
printk("%s CurrentVideoMode=0x%02x\n", __func__, CurrentVideoMode);
//  BK1_F3_REG = CurrentVideoMode;  //for test
//  CurrentVideoMode = HDMI_1280x720_60_Mode;
//  lt8619b_write_reg(BK1_F3_REG, CurrentVideoMode);
  return CurrentVideoMode;
}

void Get_Input_Source()
{
  HdmiMhl_Source_Input_Judge();

  if(MHL_Input_Flag)
  {
    CurrentInputSource = MHLINPUT;//0x03
    HDMI_Input_Flag = 0;
    MHL_Input_Flag = 0;
  
//    BK1_36_REG = 0x01;
    lt8619b_write_reg(BK1_36_REG, 0x01);
  }
  else if(HDMI_Input_Flag)        //just have hdmi source input
  {
    CurrentInputSource = HDMIINPUT;//0x02
    HDMI_Input_Flag = 0;
    MHL_Input_Flag = 0;

//    BK1_36_REG = 0x01;
    lt8619b_write_reg(BK1_36_REG, 0x01);
  }
  else
  {
    CurrentInputSource = NOINPUT;
    HDMI_Input_Flag = 0;
    MHL_Input_Flag = 0;
  }
    
//  BK1_F2_REG =CurrentInputSource;    //for test
  lt8619b_write_reg(BK1_F2_REG, CurrentInputSource);
}

  

/*

void VBUS_Charge_Enable(FunctionalState status)
{

    if( status )
    {
        BK0_21_REG = BK0_21_REG&0x7f;   //Set CHARGE_EN low to enable  5VtoVBUS.
    }
    else
    {
        BK0_21_REG = BK0_21_REG|0x80;   //Set CHARGE_EN high to disable  5VtoVBUS.
    }   
  
}
//*/



