// config
#define VENDOR_ID 0x16c0
#define PRODUCT_ID 0x05df

// main
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hid.h"

void usage(void)
{
	printf("Usage: tiny85_hid_pwm <duty> [<retry = 3>]\r\n  duty: 0-255\r\n  retry: 1-100\r\n");
	exit(1);
}

int try_set(int value)
{
	int st = 1;
	if (rawhid_open(1, VENDOR_ID, PRODUCT_ID, -1, -1))
	{
		unsigned char txbuf[] = { value };
		if (rawhid_send(0, txbuf, 1, 100) == 1)
		{
			st = 0;
		}
		rawhid_close(0);
	}
	return st;
}

int main(int argc, char* argv[])
{
	// parse argument
	if (argc < 2)
	{
		usage();
	}

	int retry = 3;
	if (argc == 3)
	{
		retry = atoi(argv[2]);
		if (retry < 1 || retry > 100) {
			usage();
		}
	}
	
	int value = atoi(argv[1]);
	if (value < 0 || value > 255)
	{
		usage();
	}

	// try set value
	int i;
	for (i = 0; i < retry; i++)
	{
		if (try_set(value) == 0)
		{
			printf("Success, set value = %d\r\n", value);
			return 0;
		}
		usleep(100000);
	}

	printf("Failed.\r\n");
	return 1;
}

