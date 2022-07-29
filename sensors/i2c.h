#ifndef I2C
#define I2C

#include <stdint.h>


//
// i2c_init_adapter initialises the I2C bus to communicate with the
// chip at `addr` so bytes can be read and written.
//
// returns the file descriptor on success and -1 on failure.
//
int i2c_init_adapter(int addr);

//
// i2c_write_byte_data writes the byte `data` to register with address
// `reg` from i2c device file `file`.
//
// returns 0 on success and -1 on failure.
// 
int i2c_write_byte_data(int file, uint8_t reg, uint8_t data);

//
// i2c_read_byte_data reads the register with address `reg` from i2c
// device file `file`.
//
// returns the contents of the register on success and -1 on failure.
//
int i2c_read_byte_data(int file, uint8_t reg);



#endif /* I2C */
