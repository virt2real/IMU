#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "i2c-dev.h"
#include "bmp085.h"


int msleep(unsigned int msecs)
{
	usleep(msecs*1000);
}

s32 read_calibration_values(struct bmp085_data *data)
{
	u8 tmp[BMP085_CALIBRATION_DATA_LENGTH];
	s32 status = i2c_smbus_read_i2c_block_data(data->client, BMP085_CALIBRATION_DATA_START,
						   BMP085_CALIBRATION_DATA_LENGTH, tmp);

	struct bmp085_calibration_data *cali = &data->calibration;
	cali->AC1 =  (tmp[0] <<8) | tmp[1];
	cali->AC2 =  (tmp[2] <<8) | tmp[3];
	cali->AC3 =  (tmp[4] <<8) | tmp[5];
	cali->AC4 =  (tmp[6] <<8) | tmp[7];
	cali->AC5 =  (tmp[8] <<8) | tmp[9];
	cali->AC6 = (tmp[10] <<8) | tmp[11];

	/*parameters B1,B2*/
	cali->B1 =  (tmp[12] <<8) | tmp[13];
	cali->B2 =  (tmp[14] <<8) | tmp[15];

	/*parameters MB,MC,MD*/
	cali->MB =  (tmp[16] <<8) | tmp[17];
	cali->MC =  (tmp[18] <<8) | tmp[19];
	cali->MD =  (tmp[20] <<8) | tmp[21];
	return status;
}


s32 update_raw_temperature(struct bmp085_data *data)
{
	u16 temperature = 0x00;
	u8 tmp[2];
	s32 status = i2c_smbus_write_byte_data(data->client, 0xF4, 0x2E);
	if (status != 0) {
		printf("bmp085: Error while requesting temperature measurement: %d\n", status);
		return status;
	}
	msleep(5);

	status = i2c_smbus_read_i2c_block_data(data->client, 0xF6, sizeof(tmp), tmp);
	if (status != sizeof(tmp)) {
		printf("bmp085: Error while requesting temperature measurement (II): %d\n", status);
		return status;
	}
	data->raw_temperature = (tmp[0] << 8) + tmp[1];
	data->next_temp_measurement = time(NULL)+1;
//	printf("temperature: %u\n", temperature);
	return status;
}

s32 update_raw_pressure(struct bmp085_data *data)
{
	u8 tmp[3];
	s32 status = 0;

	status = i2c_smbus_write_byte_data(data->client, 0xF4, 0x34 + (data->oversampling_setting<<6));
	if (status != 0) {
		printf("bmp085: Error while requesting pressure measurement.\n");
		return status;
	}
	msleep(2+(3 << data->oversampling_setting<<1)); /* wait for the end of conversion */

	status = i2c_smbus_read_i2c_block_data(data->client, 0xF6, sizeof(tmp), tmp);
	if (status != sizeof(tmp)) {
		printf("bmp085: Error while requesting pressure measurement (II): %d\n", status);
		return status;
	}
	/* swap positions to correct the MSB/LSB positions */
	data->raw_pressure = (tmp[0] << 16) | (tmp[1] << 8) | tmp[2];
	data->raw_pressure = data->raw_pressure >> (8-data->oversampling_setting);
	return status;
}


s32 get_temperature(struct bmp085_data *data)
{
	if (data->next_temp_measurement+1 < time(NULL)) {
		update_raw_temperature(data);
	}

	struct bmp085_calibration_data *cali = &data->calibration;
	long x1 = ((data->raw_temperature - cali->AC6) * cali->AC5) >> 15;
	long x2 = (cali->MC << 11) / (x1 + cali->MD);
	data->b6 = x1 + x2 - 4000;
//	printf("got raw temp: %ld; AC5: %d, AC6: %d, MC: %d, MD: %d\n", data->raw_temperature, cali->AC5, cali->AC6, cali->MC, cali->MD);
	return ((x1+x2+8) >> 4);
}


s32 get_pressure(struct bmp085_data *data)
{
	struct bmp085_calibration_data *cali = &data->calibration;
	long x1, x2, x3, b3;
	unsigned long b4, b7;
	long p;

 	if (data->next_temp_measurement+1 < time(NULL)) {
 		get_temperature(data);
 	}
 	update_raw_pressure(data);

//	printf("raw_pressure: %ud, B6: %ld\n", data->raw_pressure, data->b6);

	x1 = (data->b6 * data->b6) >> 12;
	x1 *= cali->B2;
	x1 >>= 11;

	x2 = cali->AC2 * data->b6;
	x2 >>= 11;

	x3 = x1 + x2;

	b3 = (((((long)cali->AC1) * 4 + x3) << data->oversampling_setting) + 2) >> 2;

//	printf("x1: %ld, x2: %ld, x3: %ld, b3: %ld\n", x1, x2, x3, b3);
	x1 = (cali->AC3 * data->b6) >> 13;
	x2 = (cali->B1 * ((data->b6 * data->b6) >> 12)) >> 16;
	x3 = (x1 + x2 + 2) >> 2;
	b4 = (cali->AC4 * (unsigned long)(x3 + 32768)) >> 15;

//	printf("x1: %ld, x2: %ld, x3: %ld, b4: %ld\n", x1, x2, x3, b4);
	b7 = ((unsigned long)data->raw_pressure - b3) * (50000 >> data->oversampling_setting);
	p = ((b7 < 0x80000000) ? ((b7 << 1) / b4) : ((b7 / b4) * 2));

	x1 = p >> 8;
	x1 *= x1;
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p += (x1 + x2 + 3791) >> 4;
	return p;
}


void set_oversampling(struct bmp085_data *data, unsigned char oversampling)
{
	data->oversampling_setting = oversampling;
}


unsigned char get_oversampling(struct bmp085_data *data)
{
	return data->oversampling_setting;
}


int open_bmp085(int adapter_nr, struct bmp085_data *data)
{
	char filename[20];

	data->oversampling_setting = 0x03;

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	data->client = open(filename, O_RDWR);
	if (data->client < 0) {
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return(-1);
	}

	if (ioctl(data->client, I2C_SLAVE, BMP085_I2C_ADDRESS) < 0) {
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return(-1);
	}

	read_calibration_values(data);
}


void close_bmp085(struct bmp085_data *data)
{
	close(data->client);
}
