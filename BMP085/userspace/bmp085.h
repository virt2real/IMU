#include <sys/types.h>

#ifndef BMP085_H
#define BMP085_H

#define BMP085_I2C_ADDRESS		0x77

#define BMP085_CALIBRATION_DATA_START	0xAA
#define BMP085_CALIBRATION_DATA_LENGTH	22

#define BMP085_CHIP_ID_REG		0xD0
#define BMP085_VERSION_REG		0xD1

#define BMP085_CHIP_ID			0x55 /* 85 */


typedef int16_t s16;
typedef u_int16_t u16;
typedef int32_t s32;
typedef u_int8_t u8;


struct bmp085_calibration_data {
	s16 AC1, AC2, AC3;
	u16 AC4, AC5, AC6;
	s16 B1, B2;
	s16 MB, MC, MD;
};


/* Each client has this additional data */
struct bmp085_data {
	int client;
	unsigned char version;
	struct bmp085_calibration_data calibration;
	unsigned long raw_temperature;
	unsigned long raw_pressure;
	unsigned char oversampling_setting;
	unsigned long next_temp_measurement;
	long b6; // calculated temperature correction coefficient
};

int open_bmp085(int adapter_nr, struct bmp085_data *data);
void close_bmp085(struct bmp085_data *data);
void set_oversampling(struct bmp085_data *data, unsigned char oversampling);
unsigned char get_oversampling(struct bmp085_data *data);
s32 get_temperature(struct bmp085_data *data);
s32 get_pressure(struct bmp085_data *data);


#endif
