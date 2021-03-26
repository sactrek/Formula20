#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
 
#define I2C_BUS	"/dev/i2c-3" // I2C bus device

int main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-3";
	if((file = open(I2C_BUS, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MMA8452Q I2C address is 0x1C(28)
	if (ioctl(file, I2C_SLAVE, 0x1C) < 0){
		printf("Error failed to set I2C address [%s].\n", 0x1C);
		exit(1);
	}

	// Select mode register(0x2A)
	// Standby mode(0x00)
	char config[2] = {0};
	config[0] = 0x2A;
	config[1] = 0x00;
	write(file, config, 2);
 
	// Select mode register(0x2A)
	// Active mode(0x01)
	config[0] = 0x2A;
	config[1] = 0x01;
	write(file, config, 2);
 
	// Select configuration register(0x0E)
	// Set range to +/- 2g(0x00)
	config[0] = 0x0E;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(0.5);
 
	// Read 7 bytes of data(0x00)
	// staus, xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[7] = {0};
	if(read(file, data, 7) != 7)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data to 12-bits
		int xAccl = ((data[1] * 256) + data[2]) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}
 
		int yAccl = ((data[3] * 256) + data[4]) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}
 
		int zAccl = ((data[5] * 256) + data[6]) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}
 
		// Output data to screen
		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);

		/*xAccl= (int16_t)(data[1] | data[2] << 8);
		yAccl= (int16_t)(data[3] | data[4] << 8);
		zAccl= (int16_t)(data[5] | data[6] << 8);

		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);*/
/*
		short Xout_14_bit = ((short) (data[1]<<8 | data[2])) >> 2;           // Compute 14-bit X-axis output value
		if(Xout_14_bit > 2047)
		{
			Xout_14_bit -= 4096;
		}
     	short Yout_14_bit = ((short) (data[3]<<8 | data[4])) >> 2;           // Compute 14-bit Y-axis output value
		if(Yout_14_bit > 2047)
		{
			Yout_14_bit -= 4096;
		}
 
     	short Zout_14_bit = ((short) (data[5]<<8 | data[6])) >> 2;           // Compute 14-bit Z-axis output value             
		if(Zout_14_bit > 2047)
		{
			Zout_14_bit -= 4096;
		}
		printf("Acceleration in X-Axis : %d \n", Xout_14_bit);
		printf("Acceleration in Y-Axis : %d \n", Yout_14_bit);
		printf("Acceleration in Z-Axis : %d \n", Zout_14_bit);		
*/
	}

}
