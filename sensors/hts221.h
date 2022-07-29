#ifndef HTS221
#define HTS221


//
// hts221_init initialises the I2C bus and sensor so humidity and temperature can be read.
//
int hts221_init();

//
// hts221_read_humidity returns the humidity in %.
//
double hts221_read_humidity();

//
// hts221_read_humidity returns the temperature in °C.
//
double hts221_read_temperature();



#endif /* HTS221 */
