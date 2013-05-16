/* 
    BMP085
*/


#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/delay.h>

#define BMP085_I2C_ADDRESS			0x77

struct bmp085_meas_data {
	s16 accel_x;
	s16 accel_y;
	s16 accel_z;
	s8 temp;
};


static const unsigned short normal_i2c[] = { BMP085_I2C_ADDRESS, I2C_CLIENT_END };


static s32 bmp085_read_data(struct i2c_client *client, struct bmp085_meas_data *data)
{
	s32 result = i2c_smbus_read_i2c_block_data(client, 0x02, 7, (u8*)data);

	return result;
}


static ssize_t show_data(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	s32 result;
	struct bmp085_meas_data data;

	result = bmp085_read_data(client, &data);
	if (result > 0) {
	  return sprintf(buf, "Oh yeah, file 1: %d\n", data.accel_z>>2);	}
	return result;
}

static ssize_t show_data2(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	s32 result;
	struct bmp085_meas_data data;

	result = bmp085_read_data(client, &data);
	if (result > 0) {
	  return sprintf(buf, "fuck yeah, file 2: %d\n", data.accel_z>>2);	}
	return result;
}

static DEVICE_ATTR(value, S_IRUGO, show_data, NULL);
static DEVICE_ATTR(value2, S_IRUGO, show_data2, NULL);


static struct attribute *bmp085_attributes[] = {
	&dev_attr_value.attr,
	&dev_attr_value2.attr,
	NULL
};

static const struct attribute_group bmp085_attr_group = {
	.attrs = bmp085_attributes,
};


static int bmp085_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	/* Don't know how to identify this chip. Just assume it's there */
	strlcpy(info->type, "bmp085", I2C_NAME_SIZE);
	return 0;
}


static int bmp085_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	int err = 0;
	u8 serial[8];

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &bmp085_attr_group);
	if (err) {
		dev_err(&client->dev, "registering with sysfs failed!\n");
		goto exit;
	}

	dev_info(&client->dev, "probe succeeded!\n");

  exit:
	return err;
}

static int bmp085_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &bmp085_attr_group);
	return 0;
}


static const struct i2c_device_id bmp085_id[] = {
	{ "bmp085", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, bmp085_id);

static struct i2c_driver bmp085_driver = {
	.driver = {
		.owner  = THIS_MODULE,
		.name	= "bmp085",
	},
	.id_table	= bmp085_id,
	.probe		= bmp085_probe,
	.remove		= bmp085_remove,

	.class		= I2C_CLASS_HWMON,
	.detect		= bmp085_detect,
	//.address_list	= normal_i2c,
};

static int __init bmp085_init(void)
{
	return i2c_add_driver(&bmp085_driver);
}

static void __exit bmp085_exit(void)
{
	i2c_del_driver(&bmp085_driver);
}


MODULE_AUTHOR("Gol gol@g0l.ru");
MODULE_DESCRIPTION("Fake driver sample");
MODULE_LICENSE("GPL");

module_init(bmp085_init);
module_exit(bmp085_exit);
