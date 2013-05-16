#include <stdio.h>
#include "bmp085.h"

int main(void)
{
	struct bmp085_data data;
	open_bmp085(0x00, &data);

	int temp = get_temperature(&data);
	int pressure = get_pressure(&data);

	printf("p: %fhPa\tt: %fC\n", pressure/100.0, temp/10.0);

	close_bmp085(&data);
}
