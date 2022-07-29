#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "i2c.h"

int i2c_init_adapter(int addr){
	//Open the local i2c interface
	const char* filename = "/dev/i2c-1";
	int file = open(filename, O_RDWR); 
	//if file does not exist
	if (file < 0) { 
		return -1; //give error
	}
	//manipulates the underlying device parameters
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		return -1; // give error
	}
	//return file descriptor
    return file;
}

int i2c_write_byte_data(int file, uint8_t reg, uint8_t data){
	//buffer with first the register and second the data
	uint8_t buf[2] = {reg, data};
	//write both things of the buffer
	if (write(file, buf, 2) != 2) {
		return -1; //give error
	}
  return 0; //successful program termination
}

int i2c_read_byte_data(int file, uint8_t reg){
	//buffer with the register
	uint8_t buf[1] = {reg};
	//write register to file to select that you want to read from that register.
	if (write(file, buf, 1) != 1) {
		return -1; // give error
	}
	//read the data of the file and put it in the buffer
	if (read(file, buf, 1) != 1) {
		return -1; //give error
	}
	//return the data in the buffer
    return buf[0];
}
