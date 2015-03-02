#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include "sensor.h"

double calculerTemp(char o1, char o2)
{
	double returnTemp = 0.0;
	
	returnTemp = ((o1 << 8) | o2);

	returnTemp = (returnTemp / 65536 *165)-40;

	return returnTemp;
}

double calculerHumid(char o1, char o2)
{
	double returnHumid = 0.0;
	
	returnHumid = ((o1 << 8) | o2);

	returnHumid = returnHumid/65536*100;

	return returnHumid;
}

int main(void)
{
	int fd = open("/dev/i2c-0", O_RDWR);
	char buf[10];

	if(fd <0)
	{	
		printf("Erreur lors de l'ouverture !\n");
		exit(1);
	}	

	if(ioctl(fd, I2C_SLAVE, HAL_TEMP_HDC1000_I2C_ADDRESS) <0)
	{
		printf("Erreur lors de l'ioctl !\n");
		exit(1);
	}

	buf[0] = CONFIGURATION_REGISTER;
	buf[1] = TEMPERATURE_AND_HUMIDITY | TEMPERATURE_14_BITS_RESOLUTION | HUMIDITY_14_BITS_RESOLUTION;
	buf[2] = 0x00;

	if(write(fd, buf,  3) != 3)	// premiere etape
	{
		printf("Ecriture pas OK premiere etape\n");
	}		

	buf[0] = 0x00;	// deuxieme etape

	if(write(fd, buf,  1) != 1)	
	{
		printf("Ecriture pas OK deuxieme etape\n");
	}

	sleep(1);	// Etape 3

	// Etape 4

	if(read(fd, buf,  4) != 4)	
	{
		printf("Lecture pas OK temp\n");
	}
	else
	{	
		printf("Temperature : %lf\n", calculerTemp(buf[0], buf[1]));
		printf("Humidite : %lf\n", calculerHumid(buf[2], buf[3]));
	}


	close(fd);

	

	return 0;
}