#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>
#include <linux/mod_devicetable.h>
#include <linux/log2.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <linux/of.h>
#include <linux/i2c.h>
#include <linux/gpio.h>

unsigned char lt8619b_read_reg(unsigned short reg);
void lt8619b_write_reg(unsigned short reg, unsigned char value);

extern void LT8668exClockDisable(void);

#include "variable.h"
#include "reg_lt8619.h"
#include "lt8619.h"
#include "Scaler.h"


#endif
