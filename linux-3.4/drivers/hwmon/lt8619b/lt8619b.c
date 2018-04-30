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
#include <mach/gpio-pt.h>


extern int lt8619b_i2c_init();
//extern void lt8619b_i2c_pre_init();

static struct class *lt8619b_class;
struct device *lt8619b_dev;

struct i2c_client *lt8619b_client = NULL;

struct lt8619b_data {
  struct i2c_client *client;
};

static int lt8619b_init_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t len)
{
  int ret;
  do{
    ret = lt8619b_i2c_init();
    if(ret == 0)
      break;
    printk("lt8619b_init_set retry\n");
    gpio_direction_output(GPIO_LT8619B_RESET, 0);
    msleep(50);
    gpio_direction_output(GPIO_LT8619B_RESET, 1);
    msleep(3000);
  }while(ret == -1);

  return strnlen(buf, PAGE_SIZE);
}
static DEVICE_ATTR(lt8619b_init, 0666, NULL, lt8619b_init_set);

static const struct i2c_device_id lt8619b_ids[] = {
  { "lt8619b", 0 },
};
MODULE_DEVICE_TABLE(i2c, lt8619b_ids);

static int lt8619b_detect(struct i2c_client *client, struct i2c_board_info *info)
{
  struct i2c_adapter *adapter = client->adapter;
  int ret = 0;

  if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
    return -ENODEV;
  if(0 == adapter->nr){
    printk("%s\n", __func__);
//{
//  int i;
//  for(i=1; i<0x80; i++)
//  {
//    client->addr = i;
//    ret = i2c_smbus_write_byte_data(client, 0xFF, 0x80);
//    if(ret >=0)
//      printk("found add 0x%02x\n", i);
//    msleep(100);
//  }
//}

//    gpio_direction_output(GPIO_LT8619B_RESET, 1);
//    msleep(500);
    ret = i2c_smbus_write_byte_data(client, 0xFF, 0x80);
    if(ret < 0)
    {
      printk("addr:0x%x,write 0xEE :0x%x\n",client->addr,ret);
      return -ENODEV;
    }
    ret = i2c_smbus_write_byte_data(client, 0xEE, 0x01);
    if(ret < 0)
    {
      printk("addr:0x%x,write 0xEE :0x%x\n",client->addr,ret);
      return -ENODEV;
    }
    else
      strlcpy(info->type, "lt8619b", I2C_NAME_SIZE);
    printk("%s ok\n", __func__);
    return 0;

  }else{
    return -ENODEV;
  }
}

static int lt8619b_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
  struct lt8619b_data *lt8619b;
  int err;

  printk("%s\n", __func__);

  if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
    err = -EPFNOSUPPORT;
    goto err_out;
  }

  lt8619b = kzalloc(sizeof(struct lt8619b_data), GFP_KERNEL);
  if (!lt8619b) {
    err = -ENOMEM;
    goto err_out;
  }

  lt8619b->client = client;

  i2c_set_clientdata(client, lt8619b);

//  err = gpio_request(GPIOH(22), "lt8619b_WP");
//  if (err < 0) {
//    printk("failed to request GPIO PH22, error %d\n",  err);
//  }
//  gpio_direction_output(GPIOH(22), 1);

//  msleep(30000);
  lt8619b_client = client;

  lt8619b_class = class_create(THIS_MODULE, "lt8619b");
  lt8619b_dev = device_create(lt8619b_class, NULL, 0, NULL, "lt8619b");
  device_create_file(lt8619b_dev, &dev_attr_lt8619b_init);
 
//  lt8619b_i2c_pre_init();

  printk("%s success\n", __func__);
  return 0;

err_out:
  dev_dbg(&client->dev, "probe error %d\n", err);
  return err;
}

static int __devexit lt8619b_remove(struct i2c_client *client)
{
  struct lt8619b_data *lt8619b;

  lt8619b = i2c_get_clientdata(client);
  i2c_unregister_device(lt8619b->client);

  kfree(lt8619b);
  return 0;
}

static const unsigned short normal_i2c[] = {0x29, I2C_CLIENT_END}; //0x52
static struct i2c_driver lt8619b_driver = {
  .driver = {
    .name = "lt8619b",
    .owner = THIS_MODULE,
  },
  .probe = lt8619b_probe,
  .remove = __devexit_p(lt8619b_remove),
  .id_table = lt8619b_ids,
  .address_list = normal_i2c,
  .class = I2C_CLASS_HWMON,
  .detect = lt8619b_detect,
};

static int __init lt8619b_init(void)
{
  return i2c_add_driver(&lt8619b_driver);
}
module_init(lt8619b_init);

static void __exit lt8619b_exit(void)
{
  i2c_del_driver(&lt8619b_driver);
}
module_exit(lt8619b_exit);

MODULE_DESCRIPTION("Driver for LT8619B");
MODULE_AUTHOR("Apollo5520@126.com");
MODULE_LICENSE("GPL");
