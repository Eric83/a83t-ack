/*++

 Copyright (c) 2012-2022 ChipOne Technology (Beijing) Co., Ltd. All Rights Reserved.
 This PROPRIETARY SOFTWARE is the property of ChipOne Technology (Beijing) Co., Ltd.
 and may contains trade secrets and/or other confidential information of ChipOne
 Technology (Beijing) Co., Ltd. This file shall not be disclosed to any third party,
 in whole or in part, without prior written consent of ChipOne.
 THIS PROPRIETARY SOFTWARE & ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
 WITH ALL FAULTS, & WITHOUT WARRANTY OF ANY KIND. CHIPONE DISCLAIMS ALL EXPRESS OR
 IMPLIED WARRANTIES.

 File Name:    icn85xx.c
 Abstract:
               input driver.
 Author:       Zhimin Tian
 Date :        08,14,2013
 Version:      1.0
 History :
     2012,10,30, V0.1 first version
 --*/

#include "icn85xx.h"

#if COMPILE_FW_WITH_DRIVER
#include "icn85xx_fw.h"
#endif

#define CTP_IRQ_NUM            (config_info.int_number)
#define CTP_IRQ_MODE            (IRQF_TRIGGER_FALLING)

#define SCREEN_MAX_X             (screen_max_x)
#define SCREEN_MAX_Y             (screen_max_y)

extern int found_tp;


//static struct regulator *tp_regulator = NULL;
static struct i2c_client *this_client;
short log_basedata[COL_NUM][ROW_NUM] = {{0,0}};
short log_rawdata[COL_NUM][ROW_NUM] = {{0,0}};
short log_diffdata[COL_NUM][ROW_NUM] = {{0,0}};
//system config
static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int exchange_x_y_flag = 0;
static u32 int_handle = 0;
static __u32 twi_id = 0;
//static bool is_suspend = false;

static struct ctp_config_info config_info = {
	.input_type = CTP_TYPE,
	.name = NULL,
	.int_number = 0,
};

/* Addresses to scan */
static const unsigned short normal_i2c[2] = {ICN85XX_NORMAL_IIC_ADDR, I2C_CLIENT_END};

unsigned int log_on_off = 0;

//static char firmware[128] = {"/misc/modules/ICN8505.BIN"};
//static char firmware[128] = {"/mnt/sdcard/ICN8505.bin"};

//if file system not ready,you can use inner array
static char firmware[128] = "icn85xx_firmware";

static u32 debug_mask = 1;
enum{	
	DEBUG_INIT = 1U << 0,	
	DEBUG_SUSPEND = 1U << 1,
	DEBUG_INT_INFO = 1U << 2,
	DEBUG_X_Y_INFO = 1U << 3,
	DEBUG_KEY_INFO = 1U << 4,
	DEBUG_WAKEUP_INFO = 1U << 5,
	DEBUG_OTHERS_INFO = 1U << 6,
};
#define dprintk(level_mask,fmt,arg...)    if(unlikely(debug_mask & level_mask)) \
        printk("***CTP***"fmt, ## arg)
module_param_named(debug_mask,debug_mask,int,S_IRUGO | S_IWUSR | S_IWGRP);

#if SUPPORT_SYSFS
static enum hrtimer_restart chipone_timer_func(struct hrtimer *timer);

static ssize_t icn85xx_show_update(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t icn85xx_store_update(struct device* cd, struct device_attribute *attr, const char* buf, size_t len);
static ssize_t icn85xx_show_process(struct device* cd,struct device_attribute *attr, char* buf);
static ssize_t icn85xx_store_process(struct device* cd, struct device_attribute *attr,const char* buf, size_t len);

static DEVICE_ATTR(update, S_IRUGO | S_IWUSR, icn85xx_show_update, icn85xx_store_update);
static DEVICE_ATTR(process, S_IRUGO | S_IWUSR, icn85xx_show_process, icn85xx_store_process);

static ssize_t icn85xx_show_process(struct device* cd,struct device_attribute *attr, char* buf)
{
    ssize_t ret = 0;
    sprintf(buf, "icn85xx process\n");
    ret = strlen(buf) + 1;
    return ret;
}

static ssize_t icn85xx_store_process(struct device* cd, struct device_attribute *attr,
               const char* buf, size_t len)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    unsigned long on_off = simple_strtoul(buf, NULL, 10);
    log_on_off = on_off;
    memset(&log_basedata[0][0], 0, COL_NUM*ROW_NUM*2);
    if(on_off == 0)
    {
        icn85xx_ts->work_mode = 0;
    }
    else if((on_off == 1) || (on_off == 2) || (on_off == 3))
    {
        if((icn85xx_ts->work_mode == 0) && (icn85xx_ts->use_irq == 1))
        {
            hrtimer_init(&icn85xx_ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
            icn85xx_ts->timer.function = chipone_timer_func;
            hrtimer_start(&icn85xx_ts->timer, ktime_set(CTP_START_TIMER/1000, (CTP_START_TIMER%1000)*1000000), HRTIMER_MODE_REL);
        }
        icn85xx_ts->work_mode = on_off;
    }
    else if(on_off == 10)
    {
        icn85xx_ts->work_mode = 4;
        mdelay(10);
        printk("update baseline\n");
        icn85xx_write_reg(4, 0x30);
        icn85xx_ts->work_mode = 0;
    }
    else
    {
        icn85xx_ts->work_mode = 0;
    }


    return len;
}

static ssize_t icn85xx_show_update(struct device* cd,
                     struct device_attribute *attr, char* buf)
{
    ssize_t ret = 0;
    sprintf(buf, firmware);
    ret = strlen(buf) + 1;
    printk("firmware: %s, ret: %d\n", firmware, ret);

    return ret;
}

static ssize_t icn85xx_store_update(struct device* cd, struct device_attribute *attr, const char* buf, size_t len)
{
    printk("len: %d, update: %s\n", len, buf);
    memset(firmware, 0, 128);
    memcpy(firmware, buf, len-1);
    if(R_OK == icn85xx_fw_update(firmware))
    {
        printk("update ok\n");
    }
    else
    {
        printk("update error\n");
    }
    return len;
}

static int icn85xx_create_sysfs(struct i2c_client *client)
{
    int err;
    struct device *dev = &(client->dev);
    icn85xx_trace("%s: \n",__func__);
    err = device_create_file(dev, &dev_attr_update);
    err = device_create_file(dev, &dev_attr_process);
    return err;
}

static void icn85xx_remove_sysfs(struct i2c_client *client)
{
    struct device *dev = &(client->dev);
    icn85xx_trace("%s: \n",__func__);
    device_remove_file(dev, &dev_attr_update);
    device_remove_file(dev, &dev_attr_process);
}
#endif

#if SUPPORT_PROC_FS

static pack_head cmd_head;
static struct proc_dir_entry *icn85xx_proc_entry;
static int  DATA_LENGTH = 0;


STRUCT_PANEL_PARA_H g_structPanelPara;

static int icn85xx_tool_write(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
    int ret = 0;
    int i;
    unsigned short addr;
    char retvalue;
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    proc_info("%s \n",__func__);
    if(down_interruptible(&icn85xx_ts->sem))
    {
        return -1;
    }
    ret = copy_from_user(&cmd_head, buff, CMD_HEAD_LENGTH);
    if(ret)
    {
        proc_error("copy_from_user failed.\n");
        goto write_out;
    }
    else
    {
        ret = CMD_HEAD_LENGTH;
    }

    proc_info("wr  :0x%02x.\n", cmd_head.wr);
    proc_info("flag:0x%02x.\n", cmd_head.flag);
    proc_info("circle  :%d.\n", (int)cmd_head.circle);
    proc_info("times   :%d.\n", (int)cmd_head.times);
    proc_info("retry   :%d.\n", (int)cmd_head.retry);
    proc_info("data len:%d.\n", (int)cmd_head.data_len);
    proc_info("addr len:%d.\n", (int)cmd_head.addr_len);
    proc_info("addr:0x%02x%02x.\n", cmd_head.addr[0], cmd_head.addr[1]);
    proc_info("len:%d.\n", (int)len);
    proc_info("data:0x%02x%02x.\n", buff[CMD_HEAD_LENGTH], buff[CMD_HEAD_LENGTH+1]);
    if (1 == cmd_head.wr)  // write para
    {
        proc_info("cmd_head_.wr == 1  \n");
		ret = copy_from_user(&cmd_head.data[0], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
        if(ret)
        {
            proc_error("copy_from_user failed.\n");
            goto write_out;
        }
        //need copy to g_structPanelPara

        memcpy(&g_structPanelPara, &cmd_head.data[0], cmd_head.data_len);
        //write para to tp
        for(i=0; i<cmd_head.data_len; )
        {
        	int size = ((i+64) > cmd_head.data_len)?(cmd_head.data_len-i):64;
		    ret = icn85xx_i2c_txdata(0x8000+i, &cmd_head.data[i], size);
		    if (ret < 0) {
		        proc_error("write para failed!\n");
		        goto write_out;
		    }	
		    i = i + 64;	    
		}
		
        ret = cmd_head.data_len + CMD_HEAD_LENGTH;
        icn85xx_ts->work_mode = 5; //reinit
        goto write_out;

    }
    else if(3 == cmd_head.wr)   //set update file
    {
        proc_info("cmd_head_.wr == 3  \n");
		ret = copy_from_user(&cmd_head.data[0], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
        if(ret)
        {
            proc_error("copy_from_user failed.\n");
            goto write_out;
        }
        ret = cmd_head.data_len + CMD_HEAD_LENGTH;
        memset(firmware, 0, 128);
        memcpy(firmware, &cmd_head.data[0], cmd_head.data_len);
        proc_info("firmware : %s\n", firmware);
    }
    else if(5 == cmd_head.wr)  //start update
    {
        proc_info("cmd_head_.wr == 5 \n");
		icn85xx_update_status(1);
         ret = kernel_thread(icn85xx_fw_update,firmware,CLONE_KERNEL);
        icn85xx_trace("the kernel_thread result is:%d\n", ret);
    }
    else if(11 == cmd_head.wr) //write hostcomm
    { 
        ret = copy_from_user(&cmd_head.data[0], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
        if(ret)
        {
            proc_error("copy_from_user failed.\n");
            goto write_out;
        }   	    	
        addr = (cmd_head.addr[1]<<8) | cmd_head.addr[0];
        icn85xx_write_reg(addr, cmd_head.data[0]);        
    }
    else if(13 == cmd_head.wr) //adc enable
    {
       proc_info("cmd_head_.wr == 13  \n");
	    icn85xx_ts->work_mode = 4;
        mdelay(10);
        //set col
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8ColNum), 1);
        //u8RXOrder[0] = u8RXOrder[cmd_head.addr[0]];
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8RXOrder[0]), g_structPanelPara.u8RXOrder[cmd_head.addr[0]]);
        //set row
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8RowNum), 1);
        //u8TXOrder[0] = u8TXOrder[cmd_head.addr[1]];
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8TXOrder[0]), g_structPanelPara.u8TXOrder[cmd_head.addr[1]]);
        //scan mode
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8ScanMode), 0);
        //bit
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u16BitFreq), 0xD0);
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u16BitFreq)+1, 0x07);
        //freq
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u16FreqCycleNum[0]), 0x64);
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u16FreqCycleNum[0])+1, 0x00);
        //pga
        icn85xx_write_reg(0x8000+STRUCT_OFFSET(STRUCT_PANEL_PARA_H, u8PgaGain), 0x0);

        //config mode
        icn85xx_write_reg(0, 0x2);

        mdelay(1);
        icn85xx_read_reg(2, &retvalue);
        printk("retvalue0: %d\n", retvalue);
        while(retvalue != 1)
        {
            printk("retvalue: %d\n", retvalue);
            mdelay(1);
            icn85xx_read_reg(2, &retvalue);
        }

        if(icn85xx_goto_progmode() != 0)
        {
            printk("icn85xx_goto_progmode() != 0 error\n");
            goto write_out;
        }

        icn85xx_prog_write_reg(0x040870, 1);

    }

write_out:
    up(&icn85xx_ts->sem);
	proc_info("icn85xx_tool_write write_out  \n");
    return len;

}

static int icn85xx_tool_read( char *page, char **start, off_t off, int count, int *eof, void *data )
{
    int i, j;
    int ret = 0;
    char row, column, retvalue;
    unsigned short addr;
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    if(down_interruptible(&icn85xx_ts->sem))
    {
        return -1;
    }
    proc_info("%s: count:%d, off:%d, cmd_head.data_len: %d\n",__func__, count,(int)off,(int)cmd_head.data_len);
    if (cmd_head.wr % 2)
    {
        ret = 0;
		proc_info("cmd_head_.wr == 1111111  \n");
        goto read_out;
    }
    else if (0 == cmd_head.wr)   //read para
    {
        //read para
		proc_info("cmd_head_.wr == 0  \n");
        ret = icn85xx_i2c_rxdata(0x8000, &page[0], cmd_head.data_len);
        if (ret < 0) {
            icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
        }
        memcpy(&g_structPanelPara, &page[0], sizeof(g_structPanelPara));
        goto read_out;

    }
    else if(2 == cmd_head.wr)  //get update status
    {
        proc_info("cmd_head_.wr == 2  \n");
		page[0] = icn85xx_get_status();
        proc_info("status: %d\n", page[0]);
    }
    else if(4 == cmd_head.wr)  //read rawdata
    {
        //icn85xx_read_reg(0x8004, &row);
        //icn85xx_read_reg(0x8005, &column);
		proc_info("cmd_head_.wr == 4  \n");
        row = cmd_head.addr[1];
        column = cmd_head.addr[0];
        //scan tp rawdata
        icn85xx_write_reg(4, 0x20);
        mdelay(1);
        for(i=0; i<1000; i++)
        {
            mdelay(1);
            icn85xx_read_reg(2, &retvalue);
            if(retvalue == 1)
                break;
        }

        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x2000 + i*(COL_NUM)*2,(char *) &log_rawdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            //icn85xx_rawdatadump(&log_rawdata[i][0], column, COL_NUM);
            memcpy(&page[column*2*i], &log_rawdata[i][0], column*2);

        }

        //finish scan tp rawdata
        icn85xx_write_reg(2, 0x0);
        icn85xx_write_reg(4, 0x21);
        goto read_out;
    }
    else if(6 == cmd_head.wr)  //read diffdata
    {
        proc_info("cmd_head_.wr == 6   \n");
        row = cmd_head.addr[1];
        column = cmd_head.addr[0];

        //scan tp rawdata
        icn85xx_write_reg(4, 0x20);
        mdelay(1);

        for(i=0; i<1000; i++)
        {
            mdelay(1);
            icn85xx_read_reg(2, &retvalue);
            if(retvalue == 1)
                break;
        }

        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x3000 + (i+1)*(COL_NUM+2)*2 + 2,(char *) &log_diffdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            //icn85xx_rawdatadump(&log_diffdata[i][0], column, COL_NUM);
            memcpy(&page[column*2*i], &log_diffdata[i][0], column*2);
        }
        //finish scan tp rawdata
        icn85xx_write_reg(2, 0x0);
        icn85xx_write_reg(4, 0x21);

        goto read_out;
    }
    else if(8 == cmd_head.wr)  //change TxVol, read diff
    {
        proc_info("cmd_head_.wr == 8  \n");
		row = cmd_head.addr[1];
        column = cmd_head.addr[0];
        //scan tp rawdata
        icn85xx_write_reg(4, 0x20);
        mdelay(1);
        for(i=0; i<1000; i++)
        {
            mdelay(1);
            icn85xx_read_reg(2, &retvalue);
            if(retvalue == 1)
                break;
        }

        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x2000 + i*(COL_NUM)*2,(char *) &log_rawdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }

        }

        //finish scan tp rawdata
        icn85xx_write_reg(2, 0x0);
        icn85xx_write_reg(4, 0x21);

        icn85xx_write_reg(4, 0x12);

        //scan tp rawdata
        icn85xx_write_reg(4, 0x20);
        mdelay(1);
        for(i=0; i<1000; i++)
        {
            mdelay(1);
            icn85xx_read_reg(2, &retvalue);
            if(retvalue == 1)
                break;
        }

        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x2000 + i*(COL_NUM)*2,(char *) &log_diffdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            for(j=0; j<column; j++)
            {
                *(short *)&page[2*(column*i +j)] = log_rawdata[i][j] - log_diffdata[i][j];
            }

        }

        //finish scan tp rawdata
        icn85xx_write_reg(2, 0x0);
        icn85xx_write_reg(4, 0x21);

        icn85xx_write_reg(4, 0x10);

        goto read_out;
    }
    else if(10 == cmd_head.wr)  //read adc data
    {
        proc_info("cmd_head_.wr == 10  \n");
	    if(cmd_head.flag == 0)
        {
            icn85xx_prog_write_reg(0x040874, 0);
        }
	   //icn85xx_prog_read_page(2500*cmd_head.flag,&page[0],cmd_head.data_len);
       icn85xx_prog_i2c_rxdata(2500*cmd_head.flag ,&page[0], cmd_head.data_len);
        //icn85xx_rawdatadump(&page[0], 1024, 16);
        if(cmd_head.flag == 9)
        {
            //reload code
            if(icn85xx_ts->ictype == ICN85XX_WITHOUT_FLASH)
            {
                if(R_OK == icn85xx_fw_update(firmware))
                {
                    icn85xx_ts->code_loaded_flag = 1;
                    icn85xx_trace("ICN85XX_WITHOUT_FLASH, reload code ok\n");
                }
                else
                {
                    icn85xx_ts->code_loaded_flag = 0;
                    icn85xx_trace("ICN85XX_WITHOUT_FLASH, reload code error\n");
                }
            }
            else
            {
                icn85xx_bootfrom_flash();
                msleep(50);
            }
            icn85xx_ts->work_mode = 0;
        }
    }
    else if(12 == cmd_head.wr) //read hostcomm
    {
        proc_info("cmd_head_.wr == 12  \n");
		addr = (cmd_head.addr[1]<<8) | cmd_head.addr[0];
        icn85xx_read_reg(addr, &retvalue);
        page[0] = retvalue;
    }
    else if(14 == cmd_head.wr) //read adc status
    {
        proc_info("cmd_head_.wr == 14  \n");
	    icn85xx_prog_read_reg(0x4085E, &retvalue);
        page[0] = retvalue;
        printk("0x4085E: 0x%x\n", retvalue);
    }
read_out:
    up(&icn85xx_ts->sem);
    proc_info("%s out: %d, cmd_head.data_len: %d\n\n",__func__, count, cmd_head.data_len);
    return cmd_head.data_len;
}

void init_proc_node(void)
{
    int i;
    memset(&cmd_head, 0, sizeof(cmd_head));
    cmd_head.data = NULL;

    i = 5;
    while ((!cmd_head.data) && i)
    {
        cmd_head.data = kzalloc(i * DATA_LENGTH_UINT, GFP_KERNEL);
        if (NULL != cmd_head.data)
        {
            break;
        }
        i--;
    }
    if (i)
    {
        //DATA_LENGTH = i * DATA_LENGTH_UINT + GTP_ADDR_LENGTH;
        DATA_LENGTH = i * DATA_LENGTH_UINT;
        icn85xx_trace("alloc memory size:%d.\n", DATA_LENGTH);
    }
    else
    {
        proc_error("alloc for memory failed.\n");
        return ;
    }

    icn85xx_proc_entry = create_proc_entry(ICN85XX_ENTRY_NAME, 0666, NULL);
    if (icn85xx_proc_entry == NULL)
    {
        proc_error("Couldn't create proc entry!\n");
        return ;
    }
    else
    {
        icn85xx_trace("Create proc entry success!\n");
        icn85xx_proc_entry->write_proc = icn85xx_tool_write;
        icn85xx_proc_entry->read_proc = icn85xx_tool_read;
    }

    return ;
}

void uninit_proc_node(void)
{
    kfree(cmd_head.data);
    cmd_head.data = NULL;
    remove_proc_entry(ICN85XX_ENTRY_NAME, NULL);
}

#endif


#if TOUCH_VIRTUAL_KEYS
static ssize_t virtual_keys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf,
     __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":100:1030:50:60"
     ":" __stringify(EV_KEY) ":" __stringify(KEY_HOME) ":280:1030:50:60"
     ":" __stringify(EV_KEY) ":" __stringify(KEY_BACK) ":470:1030:50:60"
     ":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":900:1030:50:60"
     "\n");
}

static struct kobj_attribute virtual_keys_attr = {
    .attr = {
        .name = "virtualkeys.chipone-ts",
        .mode = S_IRUGO,
    },
    .show = &virtual_keys_show,
};

static struct attribute *properties_attrs[] = {
    &virtual_keys_attr.attr,
    NULL
};

static struct attribute_group properties_attr_group = {
    .attrs = properties_attrs,
};

static void icn85xx_ts_virtual_keys_init(void)
{
    int ret;
    struct kobject *properties_kobj;
    properties_kobj = kobject_create_and_add("board_properties", NULL);
    if (properties_kobj)
        ret = sysfs_create_group(properties_kobj,
                     &properties_attr_group);
    if (!properties_kobj || ret)
        pr_err("failed to create board_properties\n");
}
void icn85xx_ts_virtual_keys_remove(void)
{
    kobject_put(properties_kobj);
}
#endif

/**
 * ctp_print_info - sysconfig print function
 * return value:
 *
 */
static void ctp_print_info(struct ctp_config_info info,int debug_level)
{
        if(debug_level == DEBUG_INIT)
        {
                printk("info.ctp_used:%d\n",info.ctp_used);
                printk("info.twi_id:%d\n",info.twi_id);
                printk("info.screen_max_x:%d\n",info.screen_max_x);
                printk("info.screen_max_y:%d\n",info.screen_max_y);
                printk("info.revert_x_flag:%d\n",info.revert_x_flag);
                printk("info.revert_y_flag:%d\n",info.revert_y_flag);
                printk("info.exchange_x_y_flag:%d\n",info.exchange_x_y_flag);
                printk("info.irq_gpio_number:%d\n",info.irq_gpio.gpio);
                printk("info.wakeup_gpio_number:%d\n",info.wakeup_gpio.gpio);
        }
}

/**
 * ctp_wakeup - function
 *
 */
static int ctp_wakeup(int status,int ms)
{
	dprintk(DEBUG_INIT,"***CTP*** %s:status:%d,ms = %d\n",__func__,status,ms);

	if (status == 0) {

		if(ms == 0) {
			__gpio_set_value(config_info.wakeup_gpio.gpio, 0);
		}else {
			__gpio_set_value(config_info.wakeup_gpio.gpio, 0);
			msleep(ms);
			__gpio_set_value(config_info.wakeup_gpio.gpio, 1);
		}
	}
	if (status == 1) {
		if(ms == 0) {
			__gpio_set_value(config_info.wakeup_gpio.gpio, 1);
		}else {
			__gpio_set_value(config_info.wakeup_gpio.gpio, 1);
			msleep(ms);
			__gpio_set_value(config_info.wakeup_gpio.gpio, 0);
		}
	}
	msleep(5);

	return 0;
}

/* ---------------------------------------------------------------------
*
*   Chipone panel related driver
*
*
----------------------------------------------------------------------*/
/***********************************************************************************************
Name    :   icn85xx_ts_wakeup
Input   :   void
Output  :   ret
function    : this function is used to wakeup tp
***********************************************************************************************/
void icn85xx_ts_wakeup(void)
{

}

/***********************************************************************************************
Name    :   icn85xx_ts_reset
Input   :   void
Output  :   ret
function    : this function is used to reset tp, you should not delete it
***********************************************************************************************/
void icn85xx_ts_reset(void)
{
    //set reset func
  	ctp_wakeup(0,50);
	  msleep(100);

}

/***********************************************************************************************
Name    :   icn85xx_irq_disable
Input   :   void
Output  :   ret
function    : this function is used to disable irq
***********************************************************************************************/
void icn85xx_irq_disable(void)
{
    unsigned long irqflags;
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);

    if (!icn85xx_ts->irq_is_disable)
    {
        icn85xx_ts->irq_is_disable = 1;
		    input_set_int_enable(&(config_info.input_type), 0);
		//sw_gpio_eint_set_enable(CTP_IRQ_NUM,0);
    }
}

/***********************************************************************************************
Name    :   icn85xx_irq_enable
Input   :   void
Output  :   ret
function    : this function is used to enable irq
***********************************************************************************************/
void icn85xx_irq_enable(void)
{
    unsigned long irqflags = 0;
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);

    if (icn85xx_ts->irq_is_disable)
    {
        icn85xx_ts->irq_is_disable = 0;
      	input_set_int_enable(&(config_info.input_type), 1);
    }
}

/***********************************************************************************************
Name    :   icn85xx_prog_i2c_rxdata
Input   :   addr
            *rxdata
            length
Output  :   ret
function    : read data from icn85xx, prog mode
***********************************************************************************************/
int icn85xx_prog_i2c_rxdata(unsigned int addr, char *rxdata, int length)
{
    int ret = -1;
    int retries = 0;
#if 0
    struct i2c_msg msgs[] = {
        {
            .addr   = ICN85XX_PROG_IIC_ADDR,//this_client->addr,
            .flags  = I2C_M_RD,
            .len    = length,
            .buf    = rxdata,
#if SUPPORT_ROCKCHIP
            .scl_rate = ICN85XX_I2C_SCL,
#endif
        },
    };

    icn85xx_prog_i2c_txdata(addr, NULL, 0);
    while(retries < IIC_RETRY_NUM)
    {
        ret = i2c_transfer(this_client->adapter, msgs, 1);
        if(ret == 1)break;
        retries++;
    }
    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c read error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }
#else
    unsigned char tmp_buf[3];
    struct i2c_msg msgs[] = {
        {
            .addr   = ICN85XX_PROG_IIC_ADDR,//this_client->addr,
            .flags  = 0,
            .len    = 3,
            .buf    = tmp_buf,
        },
        {
            .addr   = ICN85XX_PROG_IIC_ADDR,//this_client->addr,
            .flags  = I2C_M_RD,
            .len    = length,
            .buf    = rxdata,
        },
    };

    tmp_buf[0] = (unsigned char)(addr>>16);
    tmp_buf[1] = (unsigned char)(addr>>8);
    tmp_buf[2] = (unsigned char)(addr);


    while(retries < IIC_RETRY_NUM)
    {
        ret = i2c_transfer(this_client->adapter, msgs, 2);
        if(ret == 2)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c read error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }
#endif
    return ret;
}
/***********************************************************************************************
Name    :   icn85xx_prog_i2c_txdata
Input   :   addr
            *rxdata
            length
Output  :   ret
function    : send data to icn85xx , prog mode
***********************************************************************************************/
int icn85xx_prog_i2c_txdata(unsigned int addr, char *txdata, int length)
{
    int ret = -1;
    char tmp_buf[128];
    int retries = 0;
    struct i2c_msg msg[] = {
        {
            .addr   = ICN85XX_PROG_IIC_ADDR,//this_client->addr,
            .flags  = 0,
            .len    = length + 3,
            .buf    = tmp_buf,
        },
    };

    if (length > 125)
    {
        icn85xx_error("%s too big datalen = %d!\n", __func__, length);
        return -1;
    }

    tmp_buf[0] = (unsigned char)(addr>>16);
    tmp_buf[1] = (unsigned char)(addr>>8);
    tmp_buf[2] = (unsigned char)(addr);


    if (length != 0 && txdata != NULL)
    {
        memcpy(&tmp_buf[3], txdata, length);
    }

    while(retries < IIC_RETRY_NUM)
    {
        ret = i2c_transfer(this_client->adapter, msg, 1);
        if(ret == 1)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c write error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }
    return ret;
}
/***********************************************************************************************
Name    :   icn85xx_prog_write_reg
Input   :   addr -- address
            para -- parameter
Output  :
function    :   write register of icn85xx, prog mode
***********************************************************************************************/
int icn85xx_prog_write_reg(unsigned int addr, char para)
{
    char buf[3];
    int ret = -1;

    buf[0] = para;
    ret = icn85xx_prog_i2c_txdata(addr, buf, 1);
    if (ret < 0) {
        icn85xx_error("%s write reg failed! %#x ret: %d\n", __func__, buf[0], ret);
        return -1;
    }

    return ret;
}


/***********************************************************************************************
Name    :   icn85xx_prog_read_reg
Input   :   addr
            pdata
Output  :
function    :   read register of icn85xx, prog mode
***********************************************************************************************/
int icn85xx_prog_read_reg(unsigned int addr, char *pdata)
{
    int ret = -1;
    ret = icn85xx_prog_i2c_rxdata(addr, pdata, 1);
    return ret;
}
/***********************************************************************************************
Name    :   icn85xx_prog_read_page
Input   :   Addr
            Buffer
Output  :
function    :   read large register of icn85xx, in prog mode
***********************************************************************************************/
int icn85xx_prog_read_page(unsigned int Addr,unsigned char *Buffer, unsigned int Length)
{
    int ret =0;
	unsigned int StartAddr = Addr;
    while(Length){
		if(Length > MAX_LENGTH_PER_TRANSFER){
        	ret = icn85xx_prog_i2c_rxdata(StartAddr, Buffer, MAX_LENGTH_PER_TRANSFER);
			Length -= MAX_LENGTH_PER_TRANSFER;
			Buffer += MAX_LENGTH_PER_TRANSFER;
			StartAddr += MAX_LENGTH_PER_TRANSFER;
		}
		else{
		    ret = icn85xx_prog_i2c_rxdata(StartAddr, Buffer, Length);
			Length = 0;
			Buffer += Length;
			StartAddr += Length;
			break;
		}
        icn85xx_error("\n icn85xx_prog_read_page StartAddr:0x%x, length: %d\n",StartAddr,Length);
    }
    if (ret < 0) {
        icn85xx_error("\n icn85xx_prog_read_page failed! StartAddr:  0x%x, ret: %d\n", StartAddr, ret);
        return ret;
    }
	else{
          printk("\n icn85xx_prog_read_page, StartAddr 0x%x, Length: %d\n", StartAddr, Length);
          return ret;
	  }
}
/***********************************************************************************************
Name    :   icn85xx_i2c_rxdata
Input   :   addr
            *rxdata
            length
Output  :   ret
function    : read data from icn85xx, normal mode
***********************************************************************************************/
int icn85xx_i2c_rxdata(unsigned short addr, char *rxdata, int length)
{
    int ret = -1;
    int retries = 0;
    unsigned char tmp_buf[2];

    struct i2c_msg msgs[] = {
        {
            .addr   = this_client->addr,
            .flags  = 0,
            .len    = 2,
            .buf    = tmp_buf,
        },
        {
            .addr   = this_client->addr,
            .flags  = I2C_M_RD,
            .len    = length,
            .buf    = rxdata,
        },
    };
 #if 0
    tmp_buf[0] = (unsigned char)(addr>>8);
    tmp_buf[1] = (unsigned char)(addr);


    while(retries < IIC_RETRY_NUM)
    {
        ret = i2c_transfer(this_client->adapter, msgs, 2);
	//	ret = i2c_master_send(this_client, tmp_buf, 2);
	//	ret = i2c_master_recv(this_client, rxdata, length);
        if(ret == 2)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c read error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }
#else

    tmp_buf[0] = (unsigned char)(addr>>8);
    tmp_buf[1] = (unsigned char)(addr);

    while(retries < IIC_RETRY_NUM)
    {
      //  ret = i2c_transfer(this_client->adapter, msgs, 2);
		ret = i2c_master_send(this_client, tmp_buf, 2);
	    if (ret < 0)
			return ret;
		ret = i2c_master_recv(this_client, rxdata, length);
		if (ret < 0)
			return ret;
		if(ret == length)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c read error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }
#endif
    return ret;
}
/***********************************************************************************************
Name    :   icn85xx_i2c_txdata
Input   :   addr
            *rxdata
            length
Output  :   ret
function    : send data to icn85xx , normal mode
***********************************************************************************************/
int icn85xx_i2c_txdata(unsigned short addr, char *txdata, int length)
{
    int ret = -1;
    unsigned char tmp_buf[128];
    int retries = 0;

    struct i2c_msg msg[] = {
        {
            .addr   = this_client->addr,
            .flags  = 0,
            .len    = length + 2,
            .buf    = tmp_buf,
        },
    };

    if (length > 125)
    {
        icn85xx_error("%s too big datalen = %d!\n", __func__, length);
        return -1;
    }

    tmp_buf[0] = (unsigned char)(addr>>8);
    tmp_buf[1] = (unsigned char)(addr);

    if (length != 0 && txdata != NULL)
    {
        memcpy(&tmp_buf[2], txdata, length);
    }

    while(retries < IIC_RETRY_NUM)
    {
        ret = i2c_transfer(this_client->adapter, msg, 1);
        if(ret == 1)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        icn85xx_error("%s i2c write error: %d\n", __func__, ret);
//        icn85xx_ts_reset();
    }

    return ret;
}

/***********************************************************************************************
Name    :   icn85xx_write_reg
Input   :   addr -- address
            para -- parameter
Output  :
function    :   write register of icn85xx, normal mode
***********************************************************************************************/
int icn85xx_write_reg(unsigned short addr, char para)
{
    char buf[3];
    int ret = -1;

    buf[0] = para;
    ret = icn85xx_i2c_txdata(addr, buf, 1);
    if (ret < 0) {
        icn85xx_error("write reg failed! %#x ret: %d\n", buf[0], ret);
        return -1;
    }

    return ret;
}


/***********************************************************************************************
Name    :   icn85xx_read_reg
Input   :   addr
            pdata
Output  :
function    :   read register of icn85xx, normal mode
***********************************************************************************************/
int icn85xx_read_reg(unsigned short addr, char *pdata)
{
    int ret = -1;
    ret = icn85xx_i2c_rxdata(addr, pdata, 1);
    printk("addr: 0x%x: 0x%x\n", addr, *pdata);
    return ret;
}


/***********************************************************************************************
Name    :   icn85xx_log
Input   :   0: rawdata, 1: diff data
Output  :   err type
function    :   calibrate param
***********************************************************************************************/
static void icn85xx_log(char diff)
{
    char row = 0;
    char column = 0;
    int i, j, ret;
    char retvalue = 0;

    icn85xx_read_reg(0x8004, &row);
    icn85xx_read_reg(0x8005, &column);

    //scan tp rawdata
    icn85xx_write_reg(4, 0x20);
    mdelay(1);
    for(i=0; i<1000; i++)
    {
        mdelay(1);
        icn85xx_read_reg(2, &retvalue);
        if(retvalue == 1)
            break;
    }
    if(diff == 0)
    {
        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x2000 + i*COL_NUM*2, (char *)&log_rawdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            icn85xx_rawdatadump(&log_rawdata[i][0], column, COL_NUM);

        }
    }
    if(diff == 1)
    {
        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x3000 + (i+1)*(COL_NUM+2)*2 + 2, (char *)&log_diffdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            icn85xx_rawdatadump(&log_diffdata[i][0], column, COL_NUM);

        }
    }
    else if(diff == 2)
    {
        for(i=0; i<row; i++)
        {
            ret = icn85xx_i2c_rxdata(0x2000 + i*COL_NUM*2, (char *)&log_rawdata[i][0], column*2);
            if (ret < 0) {
                icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
            }
            if((log_basedata[0][0] != 0) || (log_basedata[0][1] != 0))
            {
                for(j=0; j<column; j++)
                {
                    log_rawdata[i][j] = log_basedata[i][j] - log_rawdata[i][j];
                }
            }
            icn85xx_rawdatadump(&log_rawdata[i][0], column, COL_NUM);

        }
        if((log_basedata[0][0] == 0) && (log_basedata[0][1] == 0))
        {
            memcpy(&log_basedata[0][0], &log_rawdata[0][0], COL_NUM*ROW_NUM*2);
        }


    }

    //finish scan tp rawdata
    icn85xx_write_reg(2, 0x0);
    icn85xx_write_reg(4, 0x21);
}


/***********************************************************************************************
Name    :   icn85xx_iic_test
Input   :   void
Output  :
function    : 0 success,
***********************************************************************************************/
static int icn85xx_iic_test(void)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    int  ret = -1;
    char value = 0;
    int  retry = 0;
    int  flashid;
    icn85xx_ts->ictype = 0;
    icn85xx_trace("====%s begin=====.  \n", __func__);

    while(retry++ < 3)
    {
		ret = icn85xx_read_reg(0xa, &value);
        if(ret > 0)
        {
        	if(value == 0x85)
        	{
                icn85xx_ts->ictype = ICN85XX_WITH_FLASH;
            	return ret;
        	}
        }

        icn85xx_error("iic test error! retry = %d\n", retry);
        msleep(3);
    }

    // add junfuzhang 20131211
    // force ic enter progmode
    icn85xx_goto_progmode();
    msleep(10);

    retry = 0;
    while(retry++ < 3)
    {
	    ret = icn85xx_prog_i2c_rxdata(0x040002, &value, 1);
	    icn85xx_trace("icn85xx_check_progmod: 0x%x\n", value);
	    if(ret > 0)
	    {
	    	if(value == 0x85)
	    	{
                flashid = icn85xx_read_flashid();
                if((MD25D40_ID1 == flashid) || (MD25D40_ID2 == flashid)
                    ||(MD25D20_ID1 == flashid) || (MD25D20_ID2 == flashid)
                    ||(GD25Q10_ID == flashid) || (MX25L512E_ID == flashid)
                    || (MD25D05_ID == flashid)|| (MD25D10_ID == flashid))
                {
                    icn85xx_ts->ictype = ICN85XX_WITH_FLASH;
                }
                else
                {
                    icn85xx_ts->ictype = ICN85XX_WITHOUT_FLASH;
                }
	        	return ret;
	    	}
	    }
        icn85xx_error("iic2 test error! %d\n", retry);
        msleep(3);
    }

    return ret;
}
/***********************************************************************************************
Name    :   icn85xx_ts_release
Input   :   void
Output  :
function    : touch release
***********************************************************************************************/
static void icn85xx_ts_release(void)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    icn85xx_info("==icn85xx_ts_release ==\n");
    input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
    input_report_key(icn85xx_ts->input_dev, BTN_TOUCH, 0);
    input_sync(icn85xx_ts->input_dev);
}

/***********************************************************************************************
Name    :   icn85xx_report_value_A
Input   :   void
Output  :
function    : reprot touch ponit
***********************************************************************************************/
static void icn85xx_report_value_A(void)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    char buf[POINT_NUM*POINT_SIZE+3]={0};
    int ret = -1;
    int i;
    icn85xx_info("==icn85xx_report_value_A ==\n");
#if TOUCH_VIRTUAL_KEYS
    unsigned char button;
    static unsigned char button_last;
#endif

    ret = icn85xx_i2c_rxdata(0x1000, buf, POINT_NUM*POINT_SIZE+2);
    if (ret < 0) {
        icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
        return ;
    }
#if TOUCH_VIRTUAL_KEYS
    button = buf[0];
    icn85xx_info("%s: button=%d\n",__func__, button);

    if((button_last != 0) && (button == 0))
    {
        icn85xx_ts_release();
        button_last = button;
        return ;
    }
    if(button != 0)
    {
        switch(button)
        {
            case ICN_VIRTUAL_BUTTON_HOME:
                icn85xx_info("ICN_VIRTUAL_BUTTON_HOME down\n");
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 200);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_X, 280);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, 1030);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
                input_mt_sync(icn85xx_ts->input_dev);
                input_sync(icn85xx_ts->input_dev);
                break;
            case ICN_VIRTUAL_BUTTON_BACK:
                icn85xx_info("ICN_VIRTUAL_BUTTON_BACK down\n");
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 200);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_X, 470);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, 1030);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
                input_mt_sync(icn85xx_ts->input_dev);
                input_sync(icn85xx_ts->input_dev);
                break;
            case ICN_VIRTUAL_BUTTON_MENU:
                icn85xx_info("ICN_VIRTUAL_BUTTON_MENU down\n");
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 200);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_X, 100);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, 1030);
                input_report_abs(icn85xx_ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
                input_mt_sync(icn85xx_ts->input_dev);
                input_sync(icn85xx_ts->input_dev);
                break;
            default:
                icn85xx_info("other gesture\n");
                break;
        }
        button_last = button;
        return ;
    }
#endif

    icn85xx_ts->point_num = buf[1];
    if (icn85xx_ts->point_num == 0) {
        icn85xx_ts_release();
        return ;
    }
    for(i=0;i<icn85xx_ts->point_num;i++){
        if(buf[8 + POINT_SIZE*i]  != 4)
        {
          break ;
        }
        else
        {

        }
    }

    if(i == icn85xx_ts->point_num) {
        icn85xx_ts_release();
        return ;
    }
	
    for(i=0; i<icn85xx_ts->point_num; i++)
    {
        icn85xx_ts->point_info[i].u8ID = buf[2 + POINT_SIZE*i];
        icn85xx_ts->point_info[i].u16PosX = (buf[4 + POINT_SIZE*i]<<8) + buf[3 + POINT_SIZE*i];
        icn85xx_ts->point_info[i].u16PosY = (buf[6 + POINT_SIZE*i]<<8) + buf[5 + POINT_SIZE*i];
        icn85xx_ts->point_info[i].u8Pressure = 200;//buf[7 + POINT_SIZE*i];
        icn85xx_ts->point_info[i].u8EventId = buf[8 + POINT_SIZE*i];

        if(1 == icn85xx_ts->exchange_x_y_flag)
        {
            swap_ab(icn85xx_ts->point_info[i].u16PosX, icn85xx_ts->point_info[i].u16PosY);
        }
        if(1 == icn85xx_ts->revert_x_flag)
        {
            icn85xx_ts->point_info[i].u16PosX = icn85xx_ts->screen_max_x- icn85xx_ts->point_info[i].u16PosX;
        }
        if(1 == icn85xx_ts->revert_y_flag)
        {
            icn85xx_ts->point_info[i].u16PosY = icn85xx_ts->screen_max_y- icn85xx_ts->point_info[i].u16PosY;
        }

        icn85xx_info("u8ID %d\n", icn85xx_ts->point_info[i].u8ID);
        icn85xx_info("u16PosX %d\n", icn85xx_ts->point_info[i].u16PosX);
        icn85xx_info("u16PosY %d\n", icn85xx_ts->point_info[i].u16PosY);
        icn85xx_info("u8Pressure %d\n", icn85xx_ts->point_info[i].u8Pressure);
        icn85xx_info("u8EventId %d\n", icn85xx_ts->point_info[i].u8EventId);

//        {
//          icn85xx_ts->point_info[i].u16PosX = icn85xx_ts->point_info[i].u16PosX * 1024 / 800;
//          icn85xx_ts->point_info[i].u16PosY = icn85xx_ts->point_info[i].u16PosY * 600 / 480;
//        }
//printk("id=%d x=%4d y=%4d\n", icn85xx_ts->point_info[i].u8ID, icn85xx_ts->point_info[i].u16PosX, icn85xx_ts->point_info[i].u16PosY);


        input_report_abs(icn85xx_ts->input_dev, ABS_MT_TRACKING_ID, icn85xx_ts->point_info[i].u8ID);
        input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, icn85xx_ts->point_info[i].u8Pressure);
        input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_X, icn85xx_ts->point_info[i].u16PosX);
        input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, icn85xx_ts->point_info[i].u16PosY);
        input_report_abs(icn85xx_ts->input_dev, ABS_MT_WIDTH_MAJOR, 1);
        input_mt_sync(icn85xx_ts->input_dev);
        //icn85xx_point_info("point: %d ===x = %d,y = %d, press = %d ====\n",i, icn85xx_ts->point_info[i].u16PosX,icn85xx_ts->point_info[i].u16PosY, icn85xx_ts->point_info[i].u8Pressure);
    }

    input_report_key(icn85xx_ts->input_dev, BTN_TOUCH, icn85xx_ts->point_num);
    input_sync(icn85xx_ts->input_dev);

}
/***********************************************************************************************
Name    :   icn85xx_report_value_B
Input   :   void
Output  :
function    : reprot touch ponit
***********************************************************************************************/
#if CTP_REPORT_PROTOCOL
static void icn85xx_report_value_B(void)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    char buf[POINT_NUM*POINT_SIZE+4]={0};
//	char buf_check = 0,i,j;
    static unsigned char finger_last[POINT_NUM + 1]={0};
    unsigned char  finger_current[POINT_NUM + 1] = {0};
    unsigned int position = 0;
    int temp = 0;
    int ret = -1;
   icn85xx_trace("==icn85xx_report_value_B ==\n");

    ret = icn85xx_i2c_rxdata(0x1000, buf, POINT_NUM*POINT_SIZE+2);
    if (ret < 0) {
         icn85xx_error("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
          return ;
      }
    icn85xx_ts->point_num = buf[1];
    if (icn85xx_ts->point_num > POINT_NUM)
     {
       return ;
     }

    if(icn85xx_ts->point_num > 0)
    {
        for(position = 0; position<icn85xx_ts->point_num; position++)
        {
            temp = buf[2 + POINT_SIZE*position] + 1;
            finger_current[temp] = 1;
            icn85xx_ts->point_info[temp].u8ID = buf[2 + POINT_SIZE*position];
            icn85xx_ts->point_info[temp].u16PosX = (buf[4 + POINT_SIZE*position]<<8) + buf[3 + POINT_SIZE*position];
            icn85xx_ts->point_info[temp].u16PosY = (buf[6 + POINT_SIZE*position]<<8) + buf[5 + POINT_SIZE*position];
            icn85xx_ts->point_info[temp].u8Pressure = buf[7 + POINT_SIZE*position];
            icn85xx_ts->point_info[temp].u8EventId = buf[8 + POINT_SIZE*position];

            if(icn85xx_ts->point_info[temp].u8EventId == 4)
                finger_current[temp] = 0;

            if(1 == icn85xx_ts->exchange_x_y_flag)
            {
                swap_ab(icn85xx_ts->point_info[temp].u16PosX, icn85xx_ts->point_info[temp].u16PosY);
            }
            if(1 == icn85xx_ts->revert_x_flag)
            {
                icn85xx_ts->point_info[temp].u16PosX = icn85xx_ts->screen_max_x- icn85xx_ts->point_info[temp].u16PosX;
            }
            if(1 == icn85xx_ts->revert_y_flag)
            {
                icn85xx_ts->point_info[temp].u16PosY = icn85xx_ts->screen_max_y- icn85xx_ts->point_info[temp].u16PosY;
            }
            //icn85xx_info("temp %d\n", temp);
            icn85xx_info("u8ID %d\n", icn85xx_ts->point_info[temp].u8ID);
            icn85xx_info("u16PosX %d\n", icn85xx_ts->point_info[temp].u16PosX);
            icn85xx_info("u16PosY %d\n", icn85xx_ts->point_info[temp].u16PosY);
           icn85xx_info("u8Pressure %d\n", icn85xx_ts->point_info[temp].u8Pressure);
           // icn85xx_info("u8EventId %d\n", icn85xx_ts->point_info[temp].u8EventId);
            //icn85xx_info("u8Pressure %d\n", icn85xx_ts->point_info[temp].u8Pressure*16);
        }
    }
    else
    {
        for(position = 1; position < POINT_NUM+1; position++)
        {
            finger_current[position] = 0;
        }
        icn85xx_info("no touch\n");
    }

    for(position = 1; position < POINT_NUM + 1; position++)
    {
        if((finger_current[position] == 0) && (finger_last[position] != 0))
        {
            input_mt_slot(icn85xx_ts->input_dev, position-1);
            input_mt_report_slot_state(icn85xx_ts->input_dev, MT_TOOL_FINGER, false);
           // input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
            //input_report_abs(icn85xx_ts->input_dev, ABS_MT_PRESSURE, icn85xx_ts->point_info[position].u8Pressure);
          //  input_report_abs(icn85xx_ts->input_dev, ABS_MT_PRESSURE, 0);
	     input_mt_sync(icn85xx_ts->input_dev);
            icn85xx_point_info("one touch up: %d\n", position);
        }
        else if(finger_current[position])
        {
            input_mt_slot(icn85xx_ts->input_dev, position-1);
            input_mt_report_slot_state(icn85xx_ts->input_dev, MT_TOOL_FINGER, true);
            input_report_abs(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 1);
            //input_report_abs(icn85xx_ts->input_dev, ABS_MT_PRESSURE, icn85xx_ts->point_info[position].u8Pressure);
            input_report_abs(icn85xx_ts->input_dev, ABS_MT_PRESSURE, 200);
            input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_X, icn85xx_ts->point_info[position].u16PosX);
            input_report_abs(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, icn85xx_ts->point_info[position].u16PosY);
	     input_mt_sync(icn85xx_ts->input_dev);
         //   icn85xx_trace("===position: %d, x = %d,y = %d, press = %d ====\n", position, icn85xx_ts->point_info[position].u16PosX,icn85xx_ts->point_info[position].u16PosY, icn85xx_ts->point_info[position].u8Pressure);

        }

    }
    input_report_key(icn85xx_ts->input_dev, BTN_TOUCH, icn85xx_ts->point_num);
    input_sync(icn85xx_ts->input_dev);

    for(position = 1; position < POINT_NUM + 1; position++)
    {
        finger_last[position] = finger_current[position];
    }

}
#endif

/***********************************************************************************************
Name    :   icn85xx_ts_pen_irq_work
Input   :   void
Output  :
function    : work_struct
***********************************************************************************************/
static void icn85xx_ts_pen_irq_work(struct work_struct *work)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
#if SUPPORT_PROC_FS
    if(down_interruptible(&icn85xx_ts->sem))
    {
        return ;
    }
#endif

    if(icn85xx_ts->work_mode == 0)
    {
#if CTP_REPORT_PROTOCOL
        icn85xx_report_value_B();
#else
       icn85xx_report_value_A();
#endif


        if(icn85xx_ts->use_irq)
        {
            icn85xx_irq_enable();
        }
        if(log_on_off == 4)
        {
            printk("normal raw data\n");
            icn85xx_log(0);   //raw data
        }
        else if(log_on_off == 5)
        {
            printk("normal diff data\n");
            icn85xx_log(1);   //diff data
        }
        else if(log_on_off == 6)
        {
            printk("normal raw2diff\n");
            icn85xx_log(2);   //diff data
        }
    }
    else if(icn85xx_ts->work_mode == 1)
    {
        printk("raw data\n");
        icn85xx_log(0);   //raw data
    }
    else if(icn85xx_ts->work_mode == 2)
    {
        printk("diff data\n");
        icn85xx_log(1);   //diff data
    }
    else if(icn85xx_ts->work_mode == 3)
    {
        printk("raw2diff data\n");
        icn85xx_log(2);   //diff data
    }
    else if(icn85xx_ts->work_mode == 4)  //idle
    {
        ;
    }
    else if(icn85xx_ts->work_mode == 5)//write para, reinit
    {
        printk("reinit tp\n");
        icn85xx_write_reg(0, 1);
        mdelay(100);
        icn85xx_write_reg(0, 0);
        icn85xx_ts->work_mode = 0;
    }

#if SUPPORT_PROC_FS
    up(&icn85xx_ts->sem);
#endif


}
/***********************************************************************************************
Name    :   chipone_timer_func
Input   :   void
Output  :
function    : Timer interrupt service routine.
***********************************************************************************************/
static enum hrtimer_restart chipone_timer_func(struct hrtimer *timer)
{
    struct icn85xx_ts_data *icn85xx_ts = container_of(timer, struct icn85xx_ts_data, timer);
    queue_work(icn85xx_ts->ts_workqueue, &icn85xx_ts->pen_event_work);
    //icn85xx_info("chipone_timer_func\n");
    if(icn85xx_ts->use_irq == 1)
    {
        if((icn85xx_ts->work_mode == 1) || (icn85xx_ts->work_mode == 2) || (icn85xx_ts->work_mode == 3))
        {
            hrtimer_start(&icn85xx_ts->timer, ktime_set(CTP_POLL_TIMER/1000, (CTP_POLL_TIMER%1000)*1000000), HRTIMER_MODE_REL);
        }
    }
    else
    {
        hrtimer_start(&icn85xx_ts->timer, ktime_set(CTP_POLL_TIMER/1000, (CTP_POLL_TIMER%1000)*1000000), HRTIMER_MODE_REL);
    }
    return HRTIMER_NORESTART;
}
/***********************************************************************************************
Name    :   icn85xx_ts_interrupt
Input   :   void
Output  :
function    : interrupt service routine
***********************************************************************************************/
irqreturn_t icn85xx_ts_interrupt(int irq, void *dev_id)
{
    struct icn85xx_ts_data *icn85xx_ts = (struct icn83xx_ts_data *)dev_id;

  icn85xx_info("==========------icn85xx_ts TS Interrupt-----============\n");
    if(icn85xx_ts->use_irq)
      icn85xx_irq_disable();
   if (!work_pending(&icn85xx_ts->pen_event_work))
       {
        icn85xx_info("Enter work\n");
        queue_work(icn85xx_ts->ts_workqueue, &icn85xx_ts->pen_event_work);
        }
   return 0;
}


#ifdef CONFIG_HAS_EARLYSUSPEND
/***********************************************************************************************
Name    :   icn85xx_ts_suspend
Input   :   void
Output  :
function    : tp enter sleep mode
***********************************************************************************************/
static void icn85xx_ts_suspend(struct early_suspend *handler)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
//	unsigned char i = 0;
    icn85xx_trace("icn85xx_ts_suspend\n");
    if (icn85xx_ts->use_irq)
    {
        icn85xx_irq_disable();
    }
    else
    {
        hrtimer_cancel(&icn85xx_ts->timer);
    }
    //}

    //reset flag if ic is flashless when power off
    if(icn85xx_ts->ictype == ICN85XX_WITHOUT_FLASH)
    {
        //icn85xx_ts->code_loaded_flag = 1;
    }

#if SUSPEND_POWER_OFF
    //power off
    //todo
    regulator_disable(tp_regulator);
#else
//    icn85xx_write_reg(ICN85XX_REG_PMODE, PMODE_HIBERNATE);
#endif

}

/***********************************************************************************************
Name    :   icn85xx_ts_resume
Input   :   void
Output  :
function    : wakeup tp or reset tp
***********************************************************************************************/
static void icn85xx_ts_resume(struct early_suspend *handler)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(this_client);
    int i;
    int ret = -1;
    int retry = 3;
    int need_update_fw = false;
    unsigned char value;
    icn85xx_trace("==icn85xx_ts_resume== \n");
//report touch release
#if CTP_REPORT_PROTOCOL
    for(i = 0; i < POINT_NUM; i++)
    {
        input_mt_slot(icn85xx_ts->input_dev, i);
        input_mt_report_slot_state(icn85xx_ts->input_dev, MT_TOOL_FINGER, false);
    }
#else
    icn85xx_ts_release();
#endif

#if SUSPEND_POWER_OFF
    //power on tp
    //todo
	 regulator_enable(tp_regulator);

#else
    //icn85xx_ts_wakeup();
    //icn85xx_ts_reset();
//    icn85xx_write_reg(ICN85XX_REG_PMODE, 0xff);
#endif
   // msleep(300);

    //reload code
    if(icn85xx_ts->ictype == ICN85XX_WITHOUT_FLASH) {
	    while (retry-- && !need_update_fw) {
		    icn85xx_ts_reset();
		    icn85xx_bootfrom_sram();
		    msleep(50);
		    ret = icn85xx_read_reg(0xa, &value);
		    if (ret > 0) { 
                            need_update_fw = false;
                            break; 
                    }
	    }
     if (retry <= 0) need_update_fw = true;

	    if (need_update_fw) {
		    if(R_OK == icn85xx_fw_update(firmware))
		    {
			    icn85xx_ts->code_loaded_flag = 1;
			    icn85xx_trace("ICN85XX_WITHOUT_FLASH, reload code ok\n"); 
		    }
		    else
		    {
			    icn85xx_ts->code_loaded_flag = 0;
			    icn85xx_trace("ICN85XX_WITHOUT_FLASH, reload code error\n"); 
		    }
	    }   
    }
    else {
//	    icn85xx_ts_reset();
    }
    if (icn85xx_ts->use_irq)
    {
        icn85xx_irq_enable();
    }
    else
    {
        hrtimer_start(&icn85xx_ts->timer, ktime_set(CTP_START_TIMER/1000, (CTP_START_TIMER%1000)*1000000), HRTIMER_MODE_REL);
    }

}
#endif

/***********************************************************************************************
Name    :   icn85xx_request_io_port
Input   :   void
Output  :
function    : 0 success,
***********************************************************************************************/
static void icn85xx_request_io_port(struct icn85xx_ts_data *icn85xx_ts)
{

    icn85xx_ts->screen_max_x = screen_max_x;
    icn85xx_ts->screen_max_y = screen_max_y;
    icn85xx_ts->revert_x_flag = revert_x_flag;
    icn85xx_ts->revert_y_flag = revert_y_flag;
    icn85xx_ts->exchange_x_y_flag = exchange_x_y_flag;
    icn85xx_ts->irq = CTP_IRQ_NUM;


    return ;

}

/***********************************************************************************************
Name    :   icn85xx_request_input_dev
Input   :   void
Output  :
function    : 0 success,
***********************************************************************************************/
static int icn85xx_request_input_dev(struct icn85xx_ts_data *icn85xx_ts)
{
    int ret = -1;
    struct input_dev *input_dev;

    input_dev = input_allocate_device();
    if (!input_dev) {
        icn85xx_error("failed to allocate input device\n");
        return -ENOMEM;
    }
    icn85xx_ts->input_dev = input_dev;

    icn85xx_ts->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
#if CTP_REPORT_PROTOCOL
    __set_bit(INPUT_PROP_DIRECT, icn85xx_ts->input_dev->propbit);
    input_mt_init_slots(icn85xx_ts->input_dev, 255);
#else
    set_bit(ABS_MT_TOUCH_MAJOR, icn85xx_ts->input_dev->absbit);
    set_bit(ABS_MT_POSITION_X, icn85xx_ts->input_dev->absbit);
    set_bit(ABS_MT_POSITION_Y, icn85xx_ts->input_dev->absbit);
    set_bit(ABS_MT_WIDTH_MAJOR, icn85xx_ts->input_dev->absbit);
    set_bit(ABS_MT_TRACKING_ID, icn85xx_ts->input_dev->absbit);
#endif
    input_set_abs_params(icn85xx_ts->input_dev, ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
    input_set_abs_params(icn85xx_ts->input_dev, ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
    input_set_abs_params(icn85xx_ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(icn85xx_ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
    input_set_abs_params(icn85xx_ts->input_dev, ABS_MT_TRACKING_ID, 0, 255, 0, 0);

#if TOUCH_VIRTUAL_KEYS
    __set_bit(KEY_MENU,  input_dev->keybit);
    __set_bit(KEY_BACK,  input_dev->keybit);
    __set_bit(KEY_HOME,  input_dev->keybit);
    __set_bit(KEY_SEARCH,  input_dev->keybit);
#endif
    __set_bit(BTN_TOUCH,  input_dev->keybit);

    input_dev->name = CTP_NAME;
    ret = input_register_device(input_dev);
    if (ret) {
        icn85xx_error("Register %s input device failed\n", input_dev->name);
        input_free_device(input_dev);
        return -ENODEV;
    }

#ifdef CONFIG_HAS_EARLYSUSPEND
    icn85xx_trace("==register_early_suspend =\n");
    icn85xx_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
    icn85xx_ts->early_suspend.suspend = icn85xx_ts_suspend;
    icn85xx_ts->early_suspend.resume  = icn85xx_ts_resume;
    register_early_suspend(&icn85xx_ts->early_suspend);
#endif

    return 0;
}

static int icn85xx_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct icn85xx_ts_data *icn85xx_ts;
    short fwVersion = 0;
    short curVersion = 0;
    int err = 0,ret =0;
    int retry;

    icn85xx_trace("====%s begin=====.\n", __func__);
    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
    {
        icn85xx_error("I2C check functionality failed.\n");
        ret =  -ENODEV;
	    goto err_check_functionality_failed;
    }

    icn85xx_ts = kzalloc(sizeof(*icn85xx_ts), GFP_KERNEL);
    if (!icn85xx_ts)
    {
        icn85xx_error("Alloc icn85xx_ts memory failed.\n");
         ret = -ENOMEM;
	    goto err_alloc_data_failed;
    }
    memset(icn85xx_ts, 0, sizeof(*icn85xx_ts));

    this_client = client;
    this_client->addr = client->addr;
    i2c_set_clientdata(client, icn85xx_ts);

    icn85xx_ts->work_mode = 0;

   	icn85xx_ts_reset();
    err = icn85xx_iic_test();
    if (err <= 0)
    {
        icn85xx_error("icn85xx_iic_test  failed.\n");
         ret = -ENODEV;
            goto err_i2c_failed;

    }
    else
    {
        icn85xx_trace("iic communication ok: 0x%x\n", icn85xx_ts->ictype);
    }
    #if SUPPORT_WITHOUT_FLASH
	    icn85xx_ts->ictype = ICN85XX_WITHOUT_FLASH;
    #endif
    err= icn85xx_request_input_dev(icn85xx_ts);
    if (err < 0)
    {
        icn85xx_error("request input dev failed\n");
        kfree(icn85xx_ts);
        return err;
    }

    icn85xx_request_io_port(icn85xx_ts);
    if(icn85xx_ts->ictype == ICN85XX_WITHOUT_FLASH)
    {
        #if COMPILE_FW_WITH_DRIVER
            icn85xx_set_fw(sizeof(icn85xx_fw), &icn85xx_fw[0]);
        #endif
        if(R_OK == icn85xx_fw_update(firmware))
        {
            icn85xx_ts->code_loaded_flag = 1;
           icn85xx_trace("ICN85XX_WITHOUT_FLASH, update ok\n");

        }
        else
        {
            icn85xx_ts->code_loaded_flag = 0;
            icn85xx_trace("ICN85XX_WITHOUT_FLASH, update error\n");
            return -1;
        }


    }
    else if(icn85xx_ts->ictype == ICN85XX_WITH_FLASH)
    {
        #if COMPILE_FW_WITH_DRIVER
            icn85xx_set_fw(sizeof(icn85xx_fw), &icn85xx_fw[0]);
        #endif

        fwVersion = icn85xx_read_fw_Ver(firmware);
        curVersion = icn85xx_readVersion();
        icn85xx_trace("fwVersion : 0x%x\n", fwVersion);
        icn85xx_trace("current version: 0x%x\n", curVersion);


#if FORCE_UPDATA_FW
        retry = 5;
        while(retry > 0)
        {
		    if(icn85xx_goto_progmode() != 0)
		    {
				printk("icn85xx_goto_progmode() != 0 error\n");
		        return -1;
		    }
        	icn85xx_read_flashid();
           /// printk("begin to update\n");
            if(R_OK == icn85xx_fw_update(firmware))
            {
                break;
            }
            retry--;
            icn85xx_error("icn85xx_fw_update failed.\n");
        }

#else
        if(fwVersion > curVersion)
        {
            retry = 5;
            while(retry > 0)
            {
                icn85xx_read_flashid();
				if(R_OK == icn85xx_fw_update(firmware))
                {
                    break;
                }
                retry--;
                icn85xx_error("icn85xx_fw_update failed.\n");
            }
        }
#endif
    }

#if TOUCH_VIRTUAL_KEYS
    icn85xx_ts_virtual_keys_init();
#endif

#if SUPPORT_SYSFS
    icn85xx_create_sysfs(client);
#endif

#if SUPPORT_PROC_FS
    sema_init(&icn85xx_ts->sem, 1);
   init_proc_node();
#endif

    INIT_WORK(&icn85xx_ts->pen_event_work, icn85xx_ts_pen_irq_work);
    icn85xx_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	flush_workqueue(icn85xx_ts->ts_workqueue);
	
    if (!icn85xx_ts->ts_workqueue) {
	//if (!icn85xx_wq) {
        icn85xx_error("create_singlethread_workqueue failed.\n");
        kfree(icn85xx_ts);
        return -ESRCH;
    }
	
   config_info.dev = &(icn85xx_ts->input_dev->dev);
   int_handle  = input_request_int(&(config_info.input_type) ,icn85xx_ts_interrupt, CTP_IRQ_MODE, icn85xx_ts);
	  if (int_handle) 
    {
        icn85xx_error("request irq error, use timer\n");
        input_free_int(&(config_info.input_type), icn85xx_ts);
        hrtimer_init(&icn85xx_ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        icn85xx_ts->timer.function = chipone_timer_func;
        hrtimer_start(&icn85xx_ts->timer, ktime_set(CTP_START_TIMER/1000, (CTP_START_TIMER%1000)*1000000), HRTIMER_MODE_REL);
        icn85xx_ts->use_irq = 0;
    }
    else 
    {
    	icn85xx_ts->use_irq = 1;
    }

	icn85xx_trace("icn85xx_ts_probe: request irq OK!!!!! \n"); 
    if(icn85xx_ts->use_irq)
        icn85xx_irq_enable();
    icn85xx_trace("==%s over =\n", __func__);
    found_tp = 1;

    return 0;
exit_irq_request_failed:
    cancel_work_sync(&icn85xx_ts->pen_event_work);
    destroy_workqueue(icn85xx_ts->ts_workqueue);
err_input_register_device_failed:
	
err_i2c_failed:
	 kfree(icn85xx_ts);
err_alloc_data_failed:
err_check_functionality_failed:
	return ret;
}

static int __devexit icn85xx_ts_remove(struct i2c_client *client)
{
    struct icn85xx_ts_data *icn85xx_ts = i2c_get_clientdata(client);
    icn85xx_trace("==icn85xx_ts_remove=\n");
    if(icn85xx_ts->use_irq)
    {
      icn85xx_irq_disable();
      input_free_int(&(config_info.input_type), icn85xx_ts);
    }
#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&icn85xx_ts->early_suspend);
#endif

#if SUPPORT_PROC_FS
    uninit_proc_node();
#endif

#if SUPPORT_SYSFS
    icn85xx_remove_sysfs(client);
#endif
    input_unregister_device(icn85xx_ts->input_dev);
    input_free_device(icn85xx_ts->input_dev);
    cancel_work_sync(&icn85xx_ts->pen_event_work);
    destroy_workqueue(icn85xx_ts->ts_workqueue);
    //icn85xx_free_io_port(icn85xx_ts);
    kfree(icn85xx_ts);
    i2c_set_clientdata(client, NULL);
    return 0;
}

static const struct i2c_device_id icn85xx_ts_id[] = {
    { CTP_NAME, 0 },
    {}
};
MODULE_DEVICE_TABLE(i2c, icn85xx_ts_id);

static struct i2c_driver icn85xx_ts_driver = {
    .class      = I2C_CLASS_HWMON,
    .probe      = icn85xx_ts_probe,
    .remove     = __devexit_p(icn85xx_ts_remove),
#ifndef CONFIG_HAS_EARLYSUSPEND
    .suspend    = icn85xx_ts_suspend,
    .resume     = icn85xx_ts_resume,
#endif
    .id_table   = icn85xx_ts_id,
    .driver = {
        .name   = CTP_NAME,
        .owner  = THIS_MODULE,
    },
      .address_list   = normal_i2c,
};

static int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;


    printk("****************  ctp detect ***************:<1>\n");

    if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
        return -ENODEV;
	if(twi_id == adapter->nr){
      printk("inc85xx ctp_detect found_tp = %d\n", found_tp);
    	if(found_tp == 1)
    	  return -ENODEV;
    	strlcpy(info->type, CTP_NAME, I2C_NAME_SIZE);
      printk("inc85xx ctp_detect OK\n");
			found_tp = 1;	
    	return 0;
	}else{
		return -ENODEV;
	}
}

static int ctp_get_system_config(void)
{   
        ctp_print_info(config_info,DEBUG_INIT);
        twi_id = config_info.twi_id; 
        screen_max_x = config_info.screen_max_x;
        screen_max_y = config_info.screen_max_y;
        revert_x_flag = config_info.revert_x_flag;
        revert_y_flag = config_info.revert_y_flag;
        exchange_x_y_flag = config_info.exchange_x_y_flag;


      if((screen_max_x == 0) || (screen_max_y == 0)){
                printk("%s:read config error!\n",__func__);
                return 0;
        }
        return 1;
}

static int __init icn85xx_ts_init(void)
{
	if(found_tp == 1)
	  return -1;
int ret = -1;
    icn85xx_trace("===========================%s=====================\n", __func__);

	if (input_fetch_sysconfig_para(&(config_info.input_type))) {
		printk("%s-cris: ctp_fetch_sysconfig_para err.\n", __func__);
		return 0;
	} else {
		ret = input_init_platform_resource(&(config_info.input_type));
		if (0 != ret) {
			printk("%s-cris: ctp_ops.init_platform_resource err. \n", __func__);
		}
	}

	if(config_info.ctp_used == 0){
        printk("*** if use ctp,please put the sys_config.fex ctp_used set to 1. \n");
        return 0;
	}

    if(!ctp_get_system_config()){
        printk("*** ctp_used set to 0 !\n");
        printk("%s:read config fail!\n",__func__);
        return ret;
    }

    ctp_wakeup(0, CTP_WAKEUP_LOW_PERIOD);
    icn85xx_ts_driver.detect = ctp_detect;

    ret = i2c_add_driver(&icn85xx_ts_driver);
    return ret;

}

static void __exit icn85xx_ts_exit(void)
{
    icn85xx_trace("==icn85xx_ts_exit==\n");
    i2c_del_driver(&icn85xx_ts_driver);
}

late_initcall(icn85xx_ts_init);
module_exit(icn85xx_ts_exit);

MODULE_AUTHOR("<zmtian@chiponeic.com>");
MODULE_DESCRIPTION("Chipone icn85xx TouchScreen driver");
MODULE_LICENSE("GPL");
