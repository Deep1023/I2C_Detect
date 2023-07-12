/*LPC2129*/
/*I2C_detect_driver.c*/
/*dipdmp99@gmail.com*/

#include "header.h"

int main()
{
	uart0_init();
	i2c_init();
	i2c_detect();
}
