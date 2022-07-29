#ifndef LPS25H
#define LPS25H


//
// lps25h_init initialises the I2C bus and sensor so pressure and temperature can be read.
//
int lps25h_init();

//
// lps25h_read_pressure returns the pressure in hPa.
//
double lps25h_read_pressure();

//
// lps25h_read_temperature returns the temperature in °C.
//
double lps25h_read_temperature();



#endif /* LPS25H */
