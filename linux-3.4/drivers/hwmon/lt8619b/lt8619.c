/************************************************************
*   Project Name:  lt8619
*   File Name:     lt8619.c
*   Build Date:    2014-10-20
*   Version£º
*   Copyright:     Lontium Semi.
************************************************************/

#include "include.h"

void LT8668exClockDisable(void)
{
  lt8619b_write_reg(BK0_03_REG, 0x00);
  lt8619b_write_reg(BK0_04_REG, 0x1b);
  lt8619b_write_reg(BK0_05_REG, 0x04);//RGB HS/VS detect
  lt8619b_write_reg(BK0_06_REG, 0x00);
  lt8619b_write_reg(BK0_07_REG, 0x00);
  lt8619b_write_reg(BK0_08_REG, 0x30);
  lt8619b_write_reg(BK0_09_REG, 0x3F);
}

void LT8668exClockEnable(void)
{
  lt8619b_write_reg(BK0_03_REG, 0xff);
  lt8619b_write_reg(BK0_04_REG, 0xff);
  lt8619b_write_reg(BK0_05_REG, 0xff);
  lt8619b_write_reg(BK0_06_REG, 0xff);
  lt8619b_write_reg(BK0_07_REG, 0xff);
  lt8619b_write_reg(BK0_08_REG, 0xff);
  lt8619b_write_reg(BK0_09_REG, 0xff);
}

void LT8668exClockConfig(u8 input)
{
  switch(input)
  {
    case HDMIINPUT:
      lt8619b_write_reg(BK0_03_REG, 0xff);
      lt8619b_write_reg(BK0_04_REG, 0xbf);//disable CEC system clock
      lt8619b_write_reg(BK0_05_REG, 0x83);//diable position/phase SOG SYNC clock
      lt8619b_write_reg(BK0_06_REG, 0xff);
      lt8619b_write_reg(BK0_07_REG, 0xff);
      lt8619b_write_reg(BK0_08_REG, 0xff);
      lt8619b_write_reg(BK0_09_REG, 0x3f);//disable analog control pixel clock
      break;
      
    case VGAINPUT:
      lt8619b_write_reg(BK0_03_REG, 0x00);
      lt8619b_write_reg(BK0_04_REG, 0x00);
      lt8619b_write_reg(BK0_05_REG, 0xCC);
      lt8619b_write_reg(BK0_06_REG, 0xFF);
      lt8619b_write_reg(BK0_07_REG, 0xDF);
      lt8619b_write_reg(BK0_08_REG, 0x7D);
      lt8619b_write_reg(BK0_09_REG, 0xFF);
      break;
      
    case MHLINPUT:
      lt8619b_write_reg(BK0_03_REG, 0xff);
      lt8619b_write_reg(BK0_04_REG, 0xff);
      lt8619b_write_reg(BK0_05_REG, 0xff);
      lt8619b_write_reg(BK0_06_REG, 0xff);
      lt8619b_write_reg(BK0_07_REG, 0xff);
      lt8619b_write_reg(BK0_08_REG, 0xff);
      lt8619b_write_reg(BK0_09_REG, 0xff);
      break;  

    default:
      lt8619b_write_reg(BK0_03_REG, 0x00);
      lt8619b_write_reg(BK0_04_REG, 0x02);
      lt8619b_write_reg(BK0_05_REG, 0x04);//RGB HS/VS detect
      lt8619b_write_reg(BK0_06_REG, 0x00);
      lt8619b_write_reg(BK0_07_REG, 0x00);
      lt8619b_write_reg(BK0_08_REG, 0x30);
      lt8619b_write_reg(BK0_09_REG, 0x3F);
      break;        
  }
}


void LT8668exPowerDown(void)
{
  lt8619b_write_reg(BK0_2F_REG, 0xFF);
  lt8619b_write_reg(BK0_30_REG, 0xFF);
  lt8619b_write_reg(BK0_31_REG, 0xFF);
  lt8619b_write_reg(BK0_32_REG, 0xFF);
}

/*
void LT8668exPowerOn(void)
{
  BK0_2F_REG = 0x00;
  BK0_30_REG = 0x00;
  BK0_31_REG = 0x00;
  BK0_32_REG = 0x00;
}
//*/

void LT8668exPowerConfig(u8 input)
{
  switch(input)
  {
    case HDMIINPUT:
      lt8619b_write_reg(BK0_2F_REG, 0xD7);
#ifdef _LVDS_Outout_
      lt8619b_write_reg(BK0_30_REG, 0x07);// Channel B C / D
#endif
#ifdef _RGB_Outout_
      lt8619b_write_reg(BK0_30_REG, 0x3F);
#endif
      lt8619b_write_reg(BK0_31_REG, 0x40);
      lt8619b_write_reg(BK0_32_REG, 0x00);
      break;
      
    case MHLINPUT:
      lt8619b_write_reg(BK0_2F_REG, 0xD7);
      lt8619b_write_reg(BK0_30_REG, 0xFF);
      lt8619b_write_reg(BK0_31_REG, 0xC9);
      lt8619b_write_reg(BK0_32_REG, 0x00);
      break;
      
    default:
      lt8619b_write_reg(BK0_2F_REG, 0xFF);
      lt8619b_write_reg(BK0_30_REG, 0xFF);
      lt8619b_write_reg(BK0_31_REG, 0xFF);
      lt8619b_write_reg(BK0_32_REG, 0xFF);
      break;
  }
}


void LT8668exChipSoftReset(void)
{
  lt8619b_write_reg(BK0_0A_REG, 0xff);
  lt8619b_write_reg(BK0_0B_REG, 0xff);
  lt8619b_write_reg(BK0_0C_REG, 0xff);
  lt8619b_write_reg(BK0_0D_REG, 0xff);
  lt8619b_write_reg(BK0_0E_REG, 0xff);
  lt8619b_write_reg(BK0_0F_REG, 0xff);
  lt8619b_write_reg(BK0_10_REG, 0xff);
  lt8619b_write_reg(BK0_11_REG, 0xff);
}

/*
void LT8668exRxPllSoftReset(void)
{
    BK0_9C_REG = BK0_9C_REG&0xbf;  //RX MHL PLL reset
    BK0_96_REG = BK0_96_REG&0x7f;  //RX PLL reset
    BK0_98_REG = BK0_98_REG&0xbf;  //RX PI reset
    BK0_8C_REG = BK0_8C_REG&0x7f;  //CDR reset
    DelayTime10ms(1);
    BK0_9C_REG = BK0_9C_REG|0x40;  //RX MHL PLL reset       
    BK0_96_REG = BK0_96_REG|0x80;  //RX PLL reset
    BK0_98_REG = BK0_98_REG|0x40;  //RX PI reset
    DelayTime10ms(1);
    BK0_8C_REG = BK0_8C_REG|0x80;  //CDR reset
}
//*/

void LT8668exHdcpSoftReset(void)
{
  unsigned char ret;

  ret = lt8619b_read_reg(BK0_0B_REG);
  lt8619b_write_reg(BK0_0B_REG, ret&0x7f);
  DelayTime10ms(1);
  ret = lt8619b_read_reg(BK0_0B_REG);
  lt8619b_write_reg(BK0_0B_REG, ret&0x80);
}

/*
void LT8668exRxDigitSoftReset(void)
{
    BK0_0A_REG = BK0_0A_REG&0xfe;
    DelayTime10ms(1);
    BK0_0A_REG = BK0_0A_REG|0x01;
}

void LT8668exScalarSoftReset(void)
{
    BK0_0D_REG = 0xf3;
    BK0_0D_REG = 0xff;
}
//*/
void LT8668exRxHdmiClkDet(void)
{
    unsigned char ret0, ret1;
  
//    if( (BK0_A6_REG & 0x80)&&(BK0_B6_REG & 0x01))

    ret0 = lt8619b_read_reg(BK0_A6_REG);
    ret1 = lt8619b_read_reg(BK0_B6_REG);
//printk("%s 0x%02x 0x%02x\n", __func__, ret0, ret1);
    if( (ret0 & 0x80)&&(ret1 & 0x01))
    {
        if(!PreHdmiClk)
        {
            ret0 = lt8619b_read_reg(BK0_10_REG);
            lt8619b_write_reg(BK0_0B_REG, ret0 & 0xDF);
            ret0 = lt8619b_read_reg(BK0_0A_REG);
            lt8619b_write_reg(BK0_0A_REG, ret0 & 0x86);
            DelayTime100ms(10);
            ret0 = lt8619b_read_reg(BK0_10_REG);
            lt8619b_write_reg(BK0_10_REG, ret0 | 0x20);
            lt8619b_write_reg(BK0_0A_REG, 0xFF);

            //DelayTime(50000);//150ms delay before hvsync detect
            DelayTime100ms(15);
        }
        CurrentHdmiClk = 1;
        PreHdmiClk = CurrentHdmiClk;
    }
    else
    {
        if( CurrentHdmiClk )
        {
            LT8668exHdcpSoftReset();
        }
        CurrentHdmiClk = 0;
        PreHdmiClk = CurrentHdmiClk;
    }
}



void LT8668exMhlConfig(void)
{
  lt8619b_write_reg(BK0_81_REG, 0x68);//480p
  lt8619b_write_reg(BK0_82_REG, 0x32);//0x36  
  lt8619b_write_reg(BK0_96_REG, 0xAA);//0xe2
  lt8619b_write_reg(BK0_97_REG, 0x00);//0x05
  lt8619b_write_reg(BK0_98_REG, 0x27);//0x67
  lt8619b_write_reg(BK0_99_REG, 0x22);//0x04 mhl/hdmi
  lt8619b_write_reg(BK0_9B_REG, 0x21);//0x0a mhl/hdmi
  lt8619b_write_reg(BK0_9C_REG, 0x50);//0x40
  lt8619b_write_reg(BK0_9D_REG, 0x14);//0x84
  lt8619b_write_reg(BK0_C9_REG, 0x00);
  lt8619b_write_reg(BK0_D1_REG, 0x00);//0x01 mhl cbus/hdmi  
  lt8619b_write_reg(BK2_2B_REG, 0x14);//hpd high
  lt8619b_write_reg(BK2_BF_REG, 0x00);//bit3:mhl cbus  1k enable
  lt8619b_write_reg(BK2_2D_REG, 0x42);//0x00 mhl/hdmi  
  lt8619b_write_reg(BK2_56_REG, 0x81);//hdcp 
  lt8619b_write_reg(BK2_2E_REG, 0xCF);//0xCF

       //////modify cbus device capability reg/////
  lt8619b_write_reg(BK2_EA_REG, 0x61); //enable MCU access to MHL Device Capability Registers.
  lt8619b_write_reg(BK2_C1_REG, 0x12);//modify MHL version indicated in MHL Device Capability Registers.  This reg needn't be modified in theory, here is for to be compliant with HTC ONE.
  //BK2_C5_REG = 0x3F;  //modify MHL SUPP_ reg in MHL Device Capability Registers to indicate packedpixel mode supported.
  lt8619b_write_reg(BK2_EA_REG, 0x60);//disable MCU access to MHL Device Capability Registers.
  ///////////////////////////////////////

}

/*
void MHL_PixelModeDet(void)
{
  if( (BK2_F1_REG&0x07) == 0x02 )  // 3'b010: PackedPixel clock mode 
                                                     // 3'b011: Normal (24-bit) clock mode 
                                                     // other: Reserved
  {
    MHL_PixelMode = MHL_PACKEDPIXEL_MODE;
    BK0_99_REG = 0x13;  //RX PLL instead of MHL PLL.
    BK2_58_REG = 0xe2;  //select MHLRX_PACKED_MODE value in HDCP block
    BK0_9C_REG = 0x40;//0x40
  }
  else
  {
    MHL_PixelMode = MHL_24BIT_MODE;
    BK0_99_REG = 0x22;  //RX PLL instead of MHL PLL.
    BK2_58_REG = 0xc2;  //select MHLRX_PACKED_MODE value in HDCP block
    BK0_9C_REG = 0x50;//0x40
  }

}

//*/

void LT8668exHdmiConfig(void)
{
  lt8619b_write_reg(BK2_56_REG, 0x95);//hdcp
  lt8619b_write_reg(BK0_81_REG, 0x55);
  lt8619b_write_reg(BK0_82_REG, 0x36);
  lt8619b_write_reg(BK0_99_REG, 0x04);
  lt8619b_write_reg(BK0_9B_REG, 0x0A);
  lt8619b_write_reg(BK0_D1_REG, 0x01);
  lt8619b_write_reg(BK2_2D_REG, 0x00);
}  



void LT8668exAudioConfig(void)
{
  unsigned char ret;

  ret = lt8619b_read_reg(BK0_24_REG);
  lt8619b_write_reg(BK0_24_REG, ret | 0x7F);
  lt8619b_write_reg(BK2_2F_REG, 0x12);
  lt8619b_write_reg(BK1_C4_REG, 0x05);
  lt8619b_write_reg(BK1_C5_REG, 0x00);
}


void LT8668EX_Audio_RST(void)
{
  lt8619b_write_reg(BK2_25_REG, 0x9F);
  DelayTime100ms(1);
  lt8619b_write_reg(BK0_0A_REG, 0xFB);
  DelayTime100ms(3);
  lt8619b_write_reg(BK0_0A_REG, 0xFF);
  DelayTime100ms(1);
}




void LT8668exColorConfig(void)
{
  unsigned char ret;
  u8 tpcolorspace=0;
  u8 tpcolordeepth=0;
  tpcolorspace = lt8619b_read_reg(BK2_04_REG) & 0x60;
  tpcolordeepth = lt8619b_read_reg(BK2_03_REG) & 0xF0;
  if((tpcolorspace == 0x40) ||(tpcolorspace == 0x20))
  {
    if(tpcolorspace == 0x20)
    {
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret | 0x01);
      ret = lt8619b_read_reg(BK2_2B_REG);
      lt8619b_write_reg(BK2_2B_REG, ret | 0x02);
    }
    else
    {
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret & 0xFE);
    }
    if(!FlagYCbCr2RGB)
    {
      ret = lt8619b_read_reg(BK1_99_REG);
      lt8619b_write_reg(BK1_99_REG, ret | 0x10);
      FlagYCbCr2RGB = 1;
    }
  }
  else
  {
    if(FlagYCbCr2RGB)
    {
      ret = lt8619b_read_reg(BK1_99_REG);
      lt8619b_write_reg(BK1_99_REG, ret & 0xEF);
      FlagYCbCr2RGB = 0;
    }
  }
  if(tpcolordeepth !=PreColorDeepth)
  {
    if((tpcolordeepth==0x40)||(tpcolordeepth==0x00))  //8bit
    {
      ret = lt8619b_read_reg(BK2_2B_REG);
      lt8619b_write_reg(BK2_2B_REG, ret & 0xFD);
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret & 0xCF);
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret & 0xFD);
      PreColorDeepth =tpcolordeepth;
      if(tpcolorspace == 0x20)
      {
        ret = lt8619b_read_reg(BK2_2B_REG);
        lt8619b_write_reg(BK2_2B_REG, ret | 0x02);
      }
    }
    else if(tpcolordeepth==0x50) //10bit
    {
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret & 0xCF);
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret | 0x10);
      ret = lt8619b_read_reg(BK2_2B_REG);
      lt8619b_write_reg(BK2_2B_REG, ret | 0x02);
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret & 0xFD);
      PreColorDeepth =tpcolordeepth;  
    }
    else if(tpcolordeepth==0x60) //12bit
    {
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret & 0xCF);
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret | 0x20);
      ret = lt8619b_read_reg(BK2_2B_REG);
      lt8619b_write_reg(BK2_2B_REG, ret | 0x02);
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret & 0xFD);
      PreColorDeepth =tpcolordeepth;
    }    
    else if(tpcolordeepth==0x70) //16bit
    {
      ret = lt8619b_read_reg(BK0_97_REG);
      lt8619b_write_reg(BK0_97_REG, ret & 0xCF);
      ret = lt8619b_read_reg(BK2_2B_REG);
      lt8619b_write_reg(BK2_2B_REG, ret | 0x02);
      ret = lt8619b_read_reg(BK2_31_REG);
      lt8619b_write_reg(BK2_31_REG, ret & 0xFD);
      PreColorDeepth =tpcolordeepth;
    }    
  }
}

void TTL_Output_Control(FunctionalState state)
{
//printk("%s %d\n", __func__, state);
    if( state )
    {
        lt8619b_write_reg(BK0_79_REG, 0xBF);//ttl io output enable
        //BK1_DE_REG = 0xC0;//video ttl enable: 0xc0->enable,through scalar  0x80->enable,bypass rx
//        BK0_7A_REG = 0x00;// L 20150529 0x00;//RGB swap or not- used when test with LT8619 EVB.
        lt8619b_write_reg(BK0_7A_REG, 0x00);
    }
    else
    {
        lt8619b_write_reg(BK0_79_REG, 0x3F);//ttl io output disable
        //BK1_DE_REG = 0x00;//video ttl disable
    }
}

void LT8668ex_Lvds_Set(void)
{
  lt8619b_write_reg(BK1_BB_REG, 0x03);// 1 LVDS Channel //v_pol_s,h_pol_s,bus_wid[1:0],bit_shf,col_dep,dp_de,dp_en
  lt8619b_write_reg(BK1_BC_REG, 0x04);//by_adj_en,dp_lvds_src_sel,SG,dp_bit_swap,dp_rgb_byte_swap,dp_port_swap[2:0]
}

void LT8668ex_Lvds_Close(void)
{
  lt8619b_write_reg(BK1_BB_REG, 0x00);//v_pol_s,h_pol_s,bus_wid[1:0],bit_shf,col_dep,dp_de,dp_en
//  lt8619b_write_reg(BK1_BB_REG, 0x04);   //by_adj_en,dp_lvds_src_sel,SG,dp_bit_swap,dp_rgb_byte_swap,dp_port_swap[2:0]
}




