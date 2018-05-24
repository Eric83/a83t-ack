/*
 * EV76C560 based spi CMOS sensor driver
 *
 * Copyright(C) 2017 Jasper Zhang
 *
 */
#define DEBUG 	1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>


#include "camera.h"


#define EV76C560_NAME  "ev76c560"

static struct v4l2_subdev *glb_sd;
struct spi_device *spidev = NULL;

//for internel driver debug
#define DEV_DBG_EN      1
#if(DEV_DBG_EN == 1)    
#define vfe_dev_dbg(x,arg...) printk("[ev76c560]"x,##arg)
#else
#define vfe_dev_dbg(x,arg...) 
#endif
#define vfe_dev_err(x,arg...) printk("[ev76c560]"x,##arg)
#define vfe_dev_print(x,arg...) printk("[ev76c560]"x,##arg)

#define LOG_ERR_RET(x)  { \
                          int ret;  \
                          ret = x; \
                          if(ret < 0) {\
                            vfe_dev_err("error at %s\n",__func__);  \
                            return ret; \
                          } \
                        }


#define MCLK              (24*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_LOW
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_LOW
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR 0x0800

//define the voltage level of control signal
#define CSI_STBY_ON     1
#define CSI_STBY_OFF    0
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0


/* Analog gain values */
#define EV76C560_MIN_GAIN	0x00
#define EV76C560_MAX_GAIN	0x07
#define EV76C560_DEF_GAIN	0x00
#define EV76C560_GAIN_STEP   	0x1

/* Exposure time values, REF_CLK=24MHz, DATA_CLK=114MHz,
 * and 57MHz CLK_CTRL & 0x70 line_length, step is 15.72us
 */
#define EV76C560_MIN_EXPOSURE		30      /* 16ms */
#define EV76C560_MAX_EXPOSURE		500    /* 500ms */
#define EV76C560_DEF_EXPOSURE		40
#define EV76C560_EXPOSURE_STEP		1


/* capture 2 MP */
#define EV76C560_WIDTH                  1280
#define EV76C560_HEIGHT                 1024

/* EV76C560 has 8/16 registers */
#define EV76C560_8BIT			1
#define EV76C560_16BIT			2

/* terminating token for reg list */
#define EV76C560_TOK_TERM 		0xFF

/* delay token for reg list */
#define EV76C560_TOK_DELAY		100

/* The ID values we are looking for */
#define EV76C560_CHIP_ID			0x0800
/**
 * struct ev76c560_reg - ev76c560 register format
 * @length: length of the register
 * @reg: 8-bit offset to register
 * @val: 8/16bit register value
 *
 * Define a structure for ev76c560 register initialization values
 */
struct ev76c560_reg {
	u8 length;
	u8 reg;
	u16 val;
};


/* list of image formats supported by ev76c560 sensor */
/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 * 
 */

//static struct regval_list sensor_fmt_raw[] = {

  //{REG_TERM,VAL_TERM},
//};


/* Structure to set analog gain */
static struct ev76c560_reg set_analog_gain[] = {
	{EV76C560_16BIT, 0x11, 0x0000},		/* ROI1 */
	/* updating, other ROIs */
	
	{EV76C560_TOK_TERM, 0, 0},	/* End of List */
};

static struct ev76c560_reg set_exposure_time[] = {
	{EV76C560_16BIT, 0x0E, 0x0000},		/* ROI1 */
	/* updating, other ROIs */

	{EV76C560_TOK_TERM, 0, 0},	/* End of List */
};

/* XGA-1024x768 */
static struct ev76c560_reg sensor_xga_regs[] = {
	{EV76C560_16BIT, 0x0F, 0x0000},

	{EV76C560_16BIT, 0x10, 0x0000},
	{EV76C560_16BIT, 0x11, 0x0000},
	{EV76C560_16BIT, 0x12, 0x0086},	/* roi1_0l_1, (1036-768)/2=134 */
	{EV76C560_16BIT, 0x13, 0x0300},	/* roi1_h_1, 768 */
	{EV76C560_16BIT, 0x14, 0x0086},	/* roi1_0c_1, (1292-1024)/2=134 */
	{EV76C560_16BIT, 0x15, 0x0400},	/* roi1_w_1, 1024 */
	{EV76C560_16BIT, 0x16, 0x0000},
	{EV76C560_16BIT, 0x17, 0x0000},
	{EV76C560_16BIT, 0x18, 0x0000},
	{EV76C560_16BIT, 0x19, 0x0000},
	{EV76C560_16BIT, 0x1A, 0x0000},

	/* updating, other ROIs */

	{EV76C560_TOK_TERM, 0, 0},	/* End of List */
};


static struct ev76c560_reg initial_common_regs[] = {
	{EV76C560_16BIT, 0x41, 0x9630},
	{EV76C560_16BIT, 0x45, 0xA7A5},
	{EV76C560_16BIT, 0x46, 0x0200},
	{EV76C560_16BIT, 0x47, 0x0013},
	//{EV76C560_16BIT, 0x48, 0x7D55},
	//{EV76C560_16BIT, 0x49, 0x7B71},
	{EV76C560_16BIT, 0x4A, 0x8EAF},
	{EV76C560_16BIT, 0x4B, 0x0125},
	{EV76C560_16BIT, 0x4C, 0x0A1E},
	{EV76C560_16BIT, 0x4D, 0x061F},
	{EV76C560_16BIT, 0x4E, 0x798D},
	{EV76C560_16BIT, 0x4F, 0x555A},

	{EV76C560_16BIT, 0x50, 0x0A0A},
	{EV76C560_16BIT, 0x51, 0x0207},
	{EV76C560_16BIT, 0x52, 0x017B},
	{EV76C560_16BIT, 0x53, 0x053E},
	//{EV76C560_16BIT, 0x54, 0x5C79},
	{EV76C560_16BIT, 0x55, 0x053C},
	{EV76C560_16BIT, 0x56, 0x3F59},
	{EV76C560_16BIT, 0x57, 0x3F44},
	{EV76C560_16BIT, 0x58, 0x053C},
	//{EV76C560_16BIT, 0x59, 0x5C78},
	{EV76C560_16BIT, 0x5A, 0x073D},
	{EV76C560_16BIT, 0x5B, 0x053E},
	//{EV76C560_16BIT, 0x5C, 0x5C79},
	//{EV76C560_16BIT, 0x5D, 0x0104},
	//{EV76C560_16BIT, 0x5E, 0x0000},
	//{EV76C560_16BIT, 0x5F, 0x1C27},

	//{EV76C560_16BIT, 0x60, 0x0000},
	//{EV76C560_16BIT, 0x61, 0x0105},
	//{EV76C560_16BIT, 0x62, 0x0D22},
	//{EV76C560_16BIT, 0x63, 0x0F1E},
	{EV76C560_16BIT, 0x64, 0x0101},
	{EV76C560_16BIT, 0x7A, 0x0004},

	{EV76C560_TOK_TERM, 0, 0},	/* End of List */
};

static struct ev76c560_reg initial_setup_regs[] = {
	{EV76C560_16BIT, 0x04, 0x80B0},    /* 24.7us @ CLK_CTRL=57MHz */
	{EV76C560_16BIT, 0x05, 0x0000},
	{EV76C560_16BIT, 0x06, 0xD05A},
	{EV76C560_16BIT, 0x07, 0x0A01},
	{EV76C560_16BIT, 0x08, 0xDF22},    /* CLK_REF=24MHz, CLK_PLL=M/(NxP)=114MHz, CLK_CHAIN=DATA_CLK=57MHz, CLK_CTRL=57MHz*/
	{EV76C560_16BIT, 0x09, 0x634B},    /* PLL P=4, N=8, M=152 */
	//{EV76C560_16BIT, 0x0A, 0x02C1},
	{EV76C560_16BIT, 0x0A, 0x02C0},
	{EV76C560_16BIT, 0x0B, 0x0006},
	{EV76C560_16BIT, 0x0C, 0x0000},
	{EV76C560_16BIT, 0x0D, 0x0000},
	{EV76C560_16BIT, 0x0E, 0x0640},    /* 40ms */
	{EV76C560_16BIT, 0x0F, 0x0000},

	{EV76C560_16BIT, 0x10, 0x0000},
	{EV76C560_16BIT, 0x11, 0x0000},
	{EV76C560_16BIT, 0x12, 0x0006},
	//{EV76C560_16BIT, 0x13, 0x0400},
	{EV76C560_16BIT, 0x13, 0x0400},    /* ROI1: height=1024 */
	{EV76C560_16BIT, 0x14, 0x0006},
	//{EV76C560_16BIT, 0x15, 0x0500},
	{EV76C560_16BIT, 0x15, 0x0500},    /* ROI1: width=1280 */
	{EV76C560_16BIT, 0x16, 0x0406},
	{EV76C560_16BIT, 0x17, 0x0000},
	{EV76C560_16BIT, 0x18, 0x0000},
	{EV76C560_16BIT, 0x19, 0x0000},
	{EV76C560_16BIT, 0x1A, 0x0000},
	//{EV76C560_16BIT, 0x1B, 0x0391},
	{EV76C560_16BIT, 0x1B, 0x04F8},
	{EV76C560_16BIT, 0x1C, 0x0000},
	{EV76C560_16BIT, 0x1D, 0x0000},
	//{EV76C560_16BIT, 0x1E, 0x0000},
	{EV76C560_16BIT, 0x1E, 0x0000},
	{EV76C560_16BIT, 0x1F, 0x0006},

	{EV76C560_16BIT, 0x20, 0x0200},
	{EV76C560_16BIT, 0x21, 0x0006},
	{EV76C560_16BIT, 0x22, 0x0280},
	{EV76C560_16BIT, 0x23, 0x0000},
	//{EV76C560_16BIT, 0x24, 0x0391},
	{EV76C560_16BIT, 0x24, 0x04F8},
	{EV76C560_16BIT, 0x25, 0x0000},
	{EV76C560_16BIT, 0x26, 0x0000},
	//{EV76C560_16BIT, 0x27, 0x0000},
	{EV76C560_16BIT, 0x27, 0x0300},
	{EV76C560_16BIT, 0x28, 0x0006},
	{EV76C560_16BIT, 0x29, 0x0001},
	{EV76C560_16BIT, 0x2A, 0x0006},
	{EV76C560_16BIT, 0x2B, 0x0010},
	{EV76C560_16BIT, 0x2C, 0x0000},
	//{EV76C560_16BIT, 0x2D, 0x0391},
	{EV76C560_16BIT, 0x2D, 0x04F8},
	{EV76C560_16BIT, 0x2E, 0x0000},
	{EV76C560_16BIT, 0x2F, 0x0000},

	//{EV76C560_16BIT, 0x30, 0x0000},
	{EV76C560_16BIT, 0x30, 0x0000},
	{EV76C560_16BIT, 0x31, 0x0006},
	{EV76C560_16BIT, 0x32, 0x0001},
	{EV76C560_16BIT, 0x33, 0x0006},
	{EV76C560_16BIT, 0x34, 0x0010},
	{EV76C560_16BIT, 0x35, 0x0000},
	{EV76C560_16BIT, 0x36, 0x8080},
	{EV76C560_16BIT, 0x37, 0x8080},
	{EV76C560_16BIT, 0x38, 0x0080},
	{EV76C560_16BIT, 0x39, 0x1880},
	{EV76C560_16BIT, 0x3A, 0x80AF},
	//{EV76C560_16BIT, 0x3B, 0x0000},
	//{EV76C560_16BIT, 0x3C, 0xA520},
	//{EV76C560_16BIT, 0x3D, 0x1DC0},
	//{EV76C560_16BIT, 0x3E, 0x00C0},
	{EV76C560_16BIT, 0x3F, 0x0001},

	{EV76C560_16BIT, 0x40, 0x0000},
	//{EV76C560_16BIT, 0x41, 0xE93F},
	{EV76C560_16BIT, 0x41, 0x9430},    /* See EOS_AN_022_560.pdf */
	{EV76C560_16BIT, 0x42, 0x001D},
	{EV76C560_16BIT, 0x43, 0x010F},
	{EV76C560_16BIT, 0x44, 0x7C00},
	{EV76C560_16BIT, 0x45, 0xA7A5},
	{EV76C560_16BIT, 0x46, 0x0200},
	{EV76C560_16BIT, 0x47, 0x0013},
	{EV76C560_16BIT, 0x48, 0x7D55},
	{EV76C560_16BIT, 0x49, 0x7B71},
	{EV76C560_16BIT, 0x4A, 0x8EAF},
	{EV76C560_16BIT, 0x4B, 0x0125},
	{EV76C560_16BIT, 0x4C, 0x0A1E},
	{EV76C560_16BIT, 0x4D, 0x061F},
	{EV76C560_16BIT, 0x4E, 0x798D},
	{EV76C560_16BIT, 0x4F, 0x565A},    // 0x555A£¿

	{EV76C560_16BIT, 0x50, 0x0A0A},
	{EV76C560_16BIT, 0x51, 0x0207},
	{EV76C560_16BIT, 0x52, 0x017B},
	{EV76C560_16BIT, 0x53, 0x053E},
	{EV76C560_16BIT, 0x54, 0x5C79},
	{EV76C560_16BIT, 0x55, 0x053C},
	{EV76C560_16BIT, 0x56, 0x3F59},
	{EV76C560_16BIT, 0x57, 0x3F44},
	{EV76C560_16BIT, 0x58, 0x053C},
	{EV76C560_16BIT, 0x59, 0x5C78},
	{EV76C560_16BIT, 0x5A, 0x073D},
	{EV76C560_16BIT, 0x5B, 0x053E},
	{EV76C560_16BIT, 0x5C, 0x5C79},
	{EV76C560_16BIT, 0x5D, 0x0104},
	{EV76C560_16BIT, 0x5E, 0x0000},
	{EV76C560_16BIT, 0x5F, 0x1C27},

	{EV76C560_16BIT, 0x60, 0x0000},
	{EV76C560_16BIT, 0x61, 0x0105},
	{EV76C560_16BIT, 0x62, 0x0D22},
	{EV76C560_16BIT, 0x63, 0x0F1E},
	{EV76C560_16BIT, 0x64, 0x0101},
	{EV76C560_16BIT, 0x65, 0x0517},
	{EV76C560_16BIT, 0x66, 0x0000},
	{EV76C560_16BIT, 0x67, 0x404E},
	{EV76C560_16BIT, 0x68, 0x053F},
	{EV76C560_16BIT, 0x69, 0x506F},
	{EV76C560_16BIT, 0x6A, 0x053D},
	{EV76C560_16BIT, 0x6B, 0x506D},
	{EV76C560_16BIT, 0x6C, 0x066E},
	{EV76C560_16BIT, 0x6D, 0x0000},
	{EV76C560_16BIT, 0x6E, 0x053D},
	{EV76C560_16BIT, 0x6F, 0x073E},

	{EV76C560_16BIT, 0x70, 0x053F},
	{EV76C560_16BIT, 0x71, 0x506F},
	{EV76C560_16BIT, 0x72, 0x0104},
	{EV76C560_16BIT, 0x73, 0x4649},
	{EV76C560_16BIT, 0x74, 0x0000},
	{EV76C560_16BIT, 0x75, 0x0000},
	{EV76C560_16BIT, 0x76, 0x0000},
	{EV76C560_16BIT, 0x77, 0x0000},
	{EV76C560_16BIT, 0x78, 0x0000},
	//{EV76C560_16BIT, 0x79, 0x0000},
	{EV76C560_16BIT, 0x79, 0x0000},
	{EV76C560_16BIT, 0x7A, 0x0004},
	{EV76C560_16BIT, 0x7B, 0x0000},
	{EV76C560_16BIT, 0x7C, 0x0000},
	{EV76C560_16BIT, 0x7D, 0xFFFF},
	//{EV76C560_16BIT, 0x7E, 0x0000},
	{EV76C560_16BIT, 0x7E, 0x0056},
	//{EV76C560_16BIT, 0x7F, 0x0900},

	{EV76C560_TOK_TERM, 0, 0},	/* End of List */
};


static inline struct sensor_info *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sensor_info, sd);
}


static int ev76c560_write_reg(struct spi_device *spi, u8 data_length,
			u8 reg, u16 val)
{
	struct spi_message msg;
	struct spi_transfer wreg_xfer = {
		.len		= 3,
		//.delay_usecs	= 50,
	};
	u8	buffer[4];

	spi_message_init(&msg);

	/* register write */
	if (data_length == EV76C560_8BIT) {
		buffer[0] = reg | 0x80;	/* first bit = 1 */
		buffer[1] =  (u8) (val & 0xff);
	} else {	/* 16Bit */
		buffer[0] = reg | 0x80;	/* first bit = 1 */
		buffer[1] = (u8) (val >> 8);
		buffer[2] = (u8) (val & 0xff);
	}
	wreg_xfer.len = 1 + data_length;
	wreg_xfer.tx_buf = buffer;
	spi_message_add_tail(&wreg_xfer, &msg);

	return spi_sync(spi, &msg);
}


static int ev76c560_read_reg16(struct spi_device *spi, u8 reg, u16 *val)
{
	struct spi_message msg;
	struct spi_transfer addr_xfer = {
		.len		= 1,
	};
	struct spi_transfer val_xfer = {
		.len		= 2,
	};
	u8	buffer[8];
	int ret = 0;

	spi_message_init(&msg);

	/* register read */
	buffer[0] = reg & 0x7f;	/* first bit = 0 */
	addr_xfer.tx_buf = buffer;
	addr_xfer.rx_buf = NULL;
	spi_message_add_tail(&addr_xfer, &msg);

	val_xfer.tx_buf = NULL;
	val_xfer.rx_buf = buffer + 4;
	spi_message_add_tail(&val_xfer, &msg);

	ret = spi_sync(spi, &msg);
	printk("%s: read buff %02x %02x  \n", __func__, buffer[4], buffer[5]);
	*val = (buffer[4] << 8) | buffer[5];

	return ret;
}


/**
 * ev76c560_write_regs - Initializes a list of EV76C560 registers
 * @spidev: spi driver client structure
 * @reglist: list of registers to be written
 *
 * Initializes a list of EV76C560 registers. The list of registers is
 * terminated by EV76C560_TOK_TERM.
 */
static int ev76c560_write_regs(struct spi_device *spidev,
			      const struct ev76c560_reg reglist[])
{
	int err;
	const struct ev76c560_reg *next = reglist;

	for (; next->length != EV76C560_TOK_TERM; next++) {
		if (next->length == EV76C560_TOK_DELAY) {
			mdelay(next->val);
			continue;
		}

		err = ev76c560_write_reg(spidev, next->length,
						next->reg, next->val);
		if (err)
			return err;
	}
	return 0;
}

static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	
	*value = info->exp;
	vfe_dev_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}

static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_time)
{
	struct sensor_info *info = to_state(sd);
	u32 coarse_int_time = 0;
	int err;
	unsigned int sexp_time = exp_time;

	vfe_dev_dbg("%s: exp_time %d.. \n", __func__, sexp_time);
	if ((sexp_time < EV76C560_MIN_EXPOSURE) ||
			(sexp_time > EV76C560_MAX_EXPOSURE)) {
		vfe_dev_dbg("Exposure time %d not within the legal range. use default\n", sexp_time);
		vfe_dev_dbg("Min time %d us Max time %d ms \n",
			EV76C560_MIN_EXPOSURE, EV76C560_MAX_EXPOSURE);
		sexp_time = EV76C560_DEF_EXPOSURE;
		//return -EINVAL;
	}

	/* for line_length = 0xB0 & clk_ctrl = 57MHz, 25 us per step */
	coarse_int_time = (sexp_time * 1000) / 25;

	set_exposure_time[0].val = coarse_int_time;	/* Analog Gain */
	err = ev76c560_write_regs(spidev, set_exposure_time);
	if (err) {
		vfe_dev_err("Error %d setting gain regs.", err);
		return err;
	} else
		info->exp = sexp_time;

	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	
	*value = info->gain;
	vfe_dev_dbg("sensor_get_gain = %d\n", info->gain);

	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, int gain)
{
	struct sensor_info *info = to_state(sd);
	int err = 0;
	int sgain = gain;

	if ((sgain < EV76C560_MIN_GAIN) || (sgain > EV76C560_MAX_GAIN)) {
		vfe_dev_dbg("Gain %d not within the legal range, use default\n", gain);
		sgain = EV76C560_DEF_GAIN;
		//return -EINVAL;
	}
	set_analog_gain[0].val = (sgain & 0x07) << 8;	/* Analog Gain */
	err = ev76c560_write_regs(spidev, set_analog_gain);
	if (err) {
		vfe_dev_err("Error setting gain.%d \n", err);
		return err;
	} else
		info->gain = sgain;

	return 0;
}

static int sensor_s_exp_gain(struct v4l2_subdev *sd, struct sensor_exp_gain *exp_gain)
{
	int exp_val = 0;
	int gain_val = 0;

	exp_val = exp_gain->exp_val/16;
	gain_val = exp_gain->gain_val/16;

	//sensor_s_exp(sd, exp_val);
	//sensor_s_gain(sd, gain_val);

	return 0;
}


/*
 * Stuff that knows about the sensor.
 */
 
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	switch(on) {
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
		usleep_range(10000,12000);
		//vfe_gpio_write(sd, PWDN, CSI_STBY_ON);
		vfe_gpio_write(sd,RESET, CSI_RST_ON);
		vfe_set_mclk(sd, OFF);
		break;

	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000,12000);
		//vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
		vfe_gpio_write(sd,RESET,CSI_RST_OFF);
		usleep_range(40000,50000);
		//msleep(50);
		break;

	case CSI_SUBDEV_PWR_ON:
		vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
		//vfe_gpio_set_status(sd, PWDN, 1);//set the gpio to output
		//vfe_gpio_set_status(sd, RESET, 1);//set the gpio to output
		vfe_gpio_write(sd,PWDN, CSI_STBY_ON);
		vfe_gpio_write(sd,RESET, CSI_RST_ON);

		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		vfe_set_pmu_channel(sd, IOVDD, ON);
		usleep_range(5000, 6000);

		vfe_set_pmu_channel(sd, AVDD, ON);
		usleep_range(5000, 6000);
		vfe_set_pmu_channel(sd, DVDD, ON);
		vfe_set_pmu_channel(sd, AFVDD, ON);
		usleep_range(10000, 12000);

		vfe_gpio_write(sd,RESET,CSI_RST_OFF);
		vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
		usleep_range(40000,50000);
		//msleep(50);
		break;

	case CSI_SUBDEV_PWR_OFF:
		vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");

		vfe_gpio_write(sd, RESET, CSI_RST_ON);
		vfe_gpio_write(sd, PWDN, CSI_STBY_ON);
		usleep_range(10000, 12000);
		vfe_set_pmu_channel(sd, DVDD,OFF);
		usleep_range(5000, 6000);
		vfe_set_pmu_channel(sd, AVDD, OFF);
		usleep_range(5000, 6000);
		vfe_set_pmu_channel(sd, IOVDD, OFF);  
		vfe_set_pmu_channel(sd, AFVDD, OFF);
		vfe_set_mclk(sd, OFF);
		//usleep_range(10000, 12000);
		//vfe_gpio_set_status(sd, RESET, 0);//set the gpio to input
		//vfe_gpio_set_status(sd, PWDN, 0);//set the gpio to input
		break;

	default:
		return -EINVAL;
	}   

	return 0;
}
static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
	vfe_dev_dbg("sensor reset %d!\n", val);
	switch(val) {
	case 0:
		vfe_gpio_write(sd, RESET, CSI_RST_OFF);
		usleep_range(10000,12000);
		break;
	case 1:
		vfe_gpio_write(sd, RESET, CSI_RST_ON);
		usleep_range(10000,12000);
		break;
	default:
		return -EINVAL;
	}
    
	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	u16 chipid = 0;
  
	if (ev76c560_read_reg16(spidev, 0x7F, &chipid))
		return -ENODEV;

	vfe_dev_print("Chip id detected 0x%x \n", chipid);
	if (chipid != EV76C560_CHIP_ID) {
		/* We didn't read the values we expected, so
		 * this must not be an EV76C560?
		 */
		vfe_dev_err("Chip id mismatch 0x%x \n", chipid);
  
  //		  return -ENODEV;
	}
	return 0;

}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	struct sensor_info *info = to_state(sd);
	int ret;

	vfe_dev_dbg("sensor_init\n");

	/*Make sure it is a target sensor*/
	ret = sensor_detect(sd);
	if (ret) {
		vfe_dev_err("chip found is not an target chip.\n");
		return ret;
	}
  
	vfe_get_standby_mode(sd, &info->stby_mode);
	if((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY) \
		&& info->init_first_flag == 0) {
		vfe_dev_print("stby_mode and init_first_flag = 0\n");
		return 0;
	}

	info->focus_status = 0;
	info->low_speed = 0;
	info->width = EV76C560_WIDTH;
	info->height = EV76C560_HEIGHT;
	info->hflip = 0;
	info->vflip = 0;
	info->gain = 0;

	info->tpf.numerator = 1;            
	info->tpf.denominator = 25;    /* 25fps */    

	if (info->init_first_flag) {
		/* first init common regs */
		ret = ev76c560_write_regs(spidev, initial_common_regs);
		if(ret < 0) {
			vfe_dev_err("write initial_common_regs error\n");
			return ret;
		}
	}

	ret = ev76c560_write_regs(spidev, initial_setup_regs);
	if(ret < 0) {
		vfe_dev_err("write initial_setup_regs error\n");
		return ret;
	}

	if(info->stby_mode == 0)
		info->init_first_flag = 0;

	info->preview_first_flag = 1;

	return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	struct sensor_info *info = to_state(sd);
	int ret=0;

	switch(cmd) {
	case GET_CURRENT_WIN_CFG:
		if(info->current_wins != NULL) {
			memcpy(arg, info->current_wins, sizeof(struct sensor_win_size));
			ret = 0;
		} else {
			vfe_dev_err("empty wins!\n");
			ret=-1;
		}
		break;

	case SET_FPS:
		ret=0;
		break;

	case ISP_SET_EXP_GAIN:
		sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	default:
		return -EINVAL;
	}

	return ret;
}


static struct sensor_format_struct {
	__u8 *desc;
	//__u32 pixelformat;
	enum v4l2_mbus_pixelcode mbus_code;
	struct regval_list *regs;
	int regs_size;
	int bpp;   /* Bytes per pixel */
}sensor_formats[] = {
	{
		.desc           = "Raw RGB Bayer",
		.mbus_code      = V4L2_MBUS_FMT_SBGGR10_1X10,
		//.regs 	        = sensor_fmt_raw,
		//.regs_size      = ARRAY_SIZE(sensor_fmt_raw),
		.bpp            = 2,
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */


static struct sensor_win_size sensor_win_sizes[] = {
	/* 1024x768 */
	{
		.width        = XGA_WIDTH,
		.height       = XGA_HEIGHT,
		.hoffset      = 0,
		.voffset      = 0,
		.hts          = 1024,
		.vts          = 768,
		.pclk         = 57*1000*1000,	//PCLK 57MHz
		.fps_fixed    = 1,
		.bin_factor   = 1,
		.intg_min     = EV76C560_MIN_EXPOSURE<<4,
		.intg_max     = EV76C560_MAX_EXPOSURE<<4,
		.gain_min     = 0,
		.gain_max     = 7<<4,
		.regs         = sensor_xga_regs,
		.regs_size    = ARRAY_SIZE(sensor_xga_regs),
		.set_size     = NULL,
	},

	/* 1280x1024 */
    {
      .width	  = EV76C560_WIDTH,
      .height 	  = EV76C560_HEIGHT,
      .hoffset	  = 0,
      .voffset	  = 0,
      .hts        = 1280,
      .vts        = 1024,
      .pclk       = 57*1000*1000,	//PCLK 57MHz
      .fps_fixed  = 1,
      .bin_factor = 1,
      .intg_min   = EV76C560_MIN_EXPOSURE<<4,
      .intg_max   = EV76C560_MAX_EXPOSURE<<4,
      .gain_min   = 0,
      .gain_max   = 7<<4,
      .regs         = NULL,
      .regs_size    = 0,
      .set_size     = NULL,
    },
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_enum_fmt(struct v4l2_subdev *sd, unsigned index,
                 enum v4l2_mbus_pixelcode *code)
{
	if (index >= N_FMTS)
		return -EINVAL;

	*code = sensor_formats[index].mbus_code;
	return 0;
}

static int sensor_enum_size(struct v4l2_subdev *sd,
                            struct v4l2_frmsizeenum *fsize)
{
	if(fsize->index > N_WIN_SIZES-1)
		return -EINVAL;
  
	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = sensor_win_sizes[fsize->index].width;
	fsize->discrete.height = sensor_win_sizes[fsize->index].height;

	return 0;
}


static int sensor_try_fmt_internal(struct v4l2_subdev *sd,
    struct v4l2_mbus_framefmt *fmt,
    struct sensor_format_struct **ret_fmt,
    struct sensor_win_size **ret_wsize)
{
	int index;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	for (index = 0; index < N_FMTS; index++)
		if (sensor_formats[index].mbus_code == fmt->code)
			break;

	if (index >= N_FMTS)
		return -EINVAL;

	if (ret_fmt != NULL)
		*ret_fmt = sensor_formats + index;

	/*
	 * Fields: the sensor devices claim to be progressive.
	 */
	fmt->field = V4L2_FIELD_NONE;

	/*
	 * Round requested image size down to the nearest
	 * we support, but not below the smallest.
	 */
	for (wsize = sensor_win_sizes; wsize < sensor_win_sizes + N_WIN_SIZES; wsize++)
		if (fmt->width >= wsize->width && fmt->height >= wsize->height)
			break;

	if (wsize >= sensor_win_sizes + N_WIN_SIZES)
		wsize--;   /* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;
	/*
	 * Note the size we'll actually handle.
	 */
	fmt->width = wsize->width;
	fmt->height = wsize->height;
	info->current_wins = wsize;

	return 0;
}

static int sensor_try_fmt(struct v4l2_subdev *sd, 
             struct v4l2_mbus_framefmt *fmt)
{
	return sensor_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
           struct v4l2_mbus_config *cfg)
{
	cfg->type = V4L2_MBUS_PARALLEL;
	cfg->flags = V4L2_MBUS_MASTER | VREF_POL | HREF_POL | CLK_POL ;

	return 0;
}


/*
 * Set a format.
 */
static int sensor_s_fmt(struct v4l2_subdev *sd, 
             struct v4l2_mbus_framefmt *fmt)
{
	struct sensor_format_struct *sensor_fmt;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);
	int ret = 0;

	vfe_dev_dbg("sensor_s_fmt\n");

	ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
	if (ret)
		return ret;

	if (wsize->regs) {
		vfe_dev_dbg("set ev76c560 regs for wsize->regs \n");
		ret = ev76c560_write_regs(spidev, wsize->regs);
		if(ret < 0) {
			vfe_dev_err("write wsize->regs error\n");
			return ret;
		}
	}

	if (wsize->set_size)
		LOG_ERR_RET(wsize->set_size(sd))

	/* Set default exposure */
	sensor_s_exp(sd, EV76C560_DEF_EXPOSURE);

	info->fmt = sensor_fmt;
	info->width = wsize->width;
	info->height = wsize->height;

	vfe_dev_print("s_fmt set width = %d, height = %d\n",wsize->width,wsize->height);

	return 0;
}



/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
  
	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->capturemode = info->capture_mode;

	return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_s_parm\n");

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	if (info->tpf.numerator == 0)
		return -EINVAL;

	info->capture_mode = cp->capturemode;

	return 0;
}


static int sensor_queryctrl(struct v4l2_subdev *sd,
    struct v4l2_queryctrl *qc)
{
	/* Fill in min, max, step and default value for these controls. */
	/* see include/linux/videodev2.h for details */
  
	switch (qc->id) {
	case V4L2_CID_GAIN:
		return v4l2_ctrl_query_fill(qc, EV76C560_MIN_GAIN, EV76C560_MAX_GAIN, EV76C560_GAIN_STEP, EV76C560_DEF_GAIN);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, EV76C560_MIN_EXPOSURE, EV76C560_MAX_EXPOSURE, EV76C560_EXPOSURE_STEP, EV76C560_DEF_EXPOSURE);
	}
	return -EINVAL;
}


static int sensor_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_g_gain(sd, &ctrl->value);
	case V4L2_CID_EXPOSURE:
		return sensor_g_exp(sd, &ctrl->value);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	struct v4l2_queryctrl qc;
	int ret;
  
	qc.id = ctrl->id;
	ret = sensor_queryctrl(sd, &qc);
	if (ret < 0)
		return ret;

	if (ctrl->value < qc.minimum || ctrl->value > qc.maximum)
		return -ERANGE;
  
	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->value);
	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->value);
	}
	return -EINVAL;
}



static int sensor_g_chip_ident(struct v4l2_subdev *sd,
    struct v4l2_dbg_chip_ident *chip)
{
	return 0;
}


/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops sensor_core_ops = {
	.g_chip_ident = sensor_g_chip_ident,
	.g_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
	.queryctrl = sensor_queryctrl,
	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.enum_mbus_fmt = sensor_enum_fmt,
	.enum_framesizes = sensor_enum_size,
	.try_mbus_fmt = sensor_try_fmt,
	.s_mbus_fmt = sensor_s_fmt,
	.s_parm = sensor_s_parm,
	.g_parm = sensor_g_parm,
	.g_mbus_config = sensor_g_mbus_config,
};


static const struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
	.video = &sensor_video_ops,
};


/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv = {
	.name = EV76C560_NAME,
	.addr_width = CCI_BITS_16,
	.data_width = CCI_BITS_16,
};


static int __devinit ev76c560_probe(struct spi_device *spi)
{
	struct v4l2_subdev *sd;
	struct sensor_info *info;

	vfe_dev_print("spi device probe in ..\n");
	info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;

	sd = &info->sd;
	glb_sd = sd;
	cci_dev_probe_helper(sd, NULL, &sensor_ops, &cci_drv);
	
	info->fmt = &sensor_formats[0];
	info->af_first_flag = 1;
	info->init_first_flag = 1;
	spidev = spi;
	spi_set_drvdata(spi, sd);

	return 0;
}


static int __devexit ev76c560_remove(struct spi_device *spi)
{
	struct v4l2_subdev * sd;

	sd = cci_dev_remove_helper(NULL, &cci_drv);
	spi_set_drvdata(spi, NULL);
	kfree(to_state(sd));

	return 0;
}


static struct spi_driver ev76c560_driver = {
	.driver = {
		.name	= "ev76c560",
		.owner	= THIS_MODULE,
	},
	.probe		= ev76c560_probe,
	.remove		= __devexit_p(ev76c560_remove),
};


static int __init init_sensor(void)
{
	//cci_dev_init_helper(&sensor_driver);
	return spi_register_driver(&ev76c560_driver);
}
module_init(init_sensor);

static void __exit exit_sensor(void)
{
	//cci_dev_exit_helper(&sensor_driver);
	spi_unregister_driver(&ev76c560_driver);
}
module_exit(exit_sensor);

MODULE_DESCRIPTION("EV76C560 CMOS Sensor Driver");
MODULE_LICENSE("GPL");
