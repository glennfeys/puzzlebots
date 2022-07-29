#include <stdio.h>


#include "i2c.h"
#include "lps25h.h"



#define HTS221_ADDRESS          0x5F

/*

Registers for HTS221. See: http://www.st.com/content/ccc/resource/technical/document/datasheet/58/d2/33/a4/42/89/42/0b/DM00066332.pdf/files/DM00066332.pdf/jcr:content/translations/en.DM00066332.pdf
""
*/

#define HTS221_CTRL_REG1               0x20
#define HTS221_BM_POWER_UP           0x80
#define HTS221_BM_BDU_SET            0x4
#define HTS221_BM_ODR0_SET           0x1   // setting sensor reading period 1Hz

#define HTS221_AV_CONF          0x10

#define HTS221_STATUS_REG              0x27
#define HTS221_BM_TEMPERATURE_READY  0x1
#define HTS221_BM_HUMIDITY_READY     0x2


/*
The Temperature and Humidity data are expressed as *_OUT_H & *_OUT_L as 2’s complement
numbers.

The relative humidity and temperature values must be computed by linear interpolation of
current registers with calibration registers, according to Table 19 and scaling as described in
Section 8.
*/
#define HTS221_HUMIDITY_OUT_L         0x28
#define HTS221_HUMIDITY_OUT_H         0x29

#define HTS221_TEMP_OUT_L             0x2A
#define HTS221_TEMP_OUT_H             0x2B


// Calibration registers
#define HTS221_H0_RH_X2               0x30
#define HTS221_H1_RH_X2               0x31
#define HTS221_T0_DEGC_X8_LSB         0x32
#define HTS221_T1_DEGC_X8_LSB         0x33
#define HTS221_T1_T0_MSB              0x35
#define HTS221_BM_T0_degC_X8_MSB  0x03
#define HTS221_BM_T1_degC_X8_MSB  0x0C
#define HTS221_T0_OUT_L               0x3C
#define HTS221_T0_OUT_H               0x3D
#define HTS221_T1_OUT_L               0x3E
#define HTS221_T1_OUT_H               0x3F
#define HTS221_H0_T0_OUT_L            0x36
#define HTS221_H0_T0_OUT_H            0x37
#define HTS221_H1_T0_OUT_L            0x3A
#define HTS221_H1_T0_OUT_H            0x3B

// File descriptor of i2c interface
static int _hts221_adapter;

// Calibration coeficients
static uint8_t _hts221_H0_rH, _hts221_H1_rH;
static uint16_t _hts221_T0_degC_x8, _hts221_T1_degC_x8; //this is actually an unsigned 10-bit integer
static int16_t _hts221_H0_T0_OUT, _hts221_H1_T0_OUT, _hts221_T0_OUT, _hts221_T1_OUT;

// last known values
static double _hts221_temperature = 0.0;
static double _hts221_humidity    = 0.0;

int hts221_init(){
    // Specify the address to communicate with
    _hts221_adapter = i2c_init_adapter(HTS221_ADDRESS);

    int result = i2c_write_byte_data(_hts221_adapter, HTS221_CTRL_REG1, 0x87);
    if (result == -1){
        printf("write failed!\n");
        return -1;
    }

    result = i2c_write_byte_data(_hts221_adapter, HTS221_AV_CONF, 0x1b);
    if (result == -1){
        printf("write failed!\n");
        return -1;
    }

    printf("Waiting for Temperature and Humidity sensors to become ready.");
    while(1){
        int value = i2c_read_byte_data(_hts221_adapter, HTS221_STATUS_REG);
        if (value == -1){
            printf("Read FAILED!\n");
            return -1;
        } else if(value & (HTS221_BM_HUMIDITY_READY | HTS221_BM_TEMPERATURE_READY)){
            break;
        }
        printf(".");
    }
    printf("\nSensors are ready!\n");
    // printf("------------------init -----------------\n");

    // Get the calibration values
    _hts221_T0_OUT = i2c_read_byte_data(_hts221_adapter, HTS221_T0_OUT_L);
    _hts221_T0_OUT |= i2c_read_byte_data(_hts221_adapter, HTS221_T0_OUT_H) << 8;
    _hts221_T1_OUT = i2c_read_byte_data(_hts221_adapter, HTS221_T1_OUT_H) << 8 | i2c_read_byte_data(_hts221_adapter, HTS221_T1_OUT_L);
    _hts221_H0_T0_OUT = i2c_read_byte_data(_hts221_adapter, HTS221_H0_T0_OUT_H) << 8 | i2c_read_byte_data(_hts221_adapter, HTS221_H0_T0_OUT_L);
    _hts221_H1_T0_OUT = i2c_read_byte_data(_hts221_adapter, HTS221_H1_T0_OUT_H) << 8 | i2c_read_byte_data(_hts221_adapter, HTS221_H1_T0_OUT_L);


    _hts221_T0_degC_x8 = (i2c_read_byte_data(_hts221_adapter, HTS221_T1_T0_MSB) & HTS221_BM_T0_degC_X8_MSB) << 8 | i2c_read_byte_data(_hts221_adapter, HTS221_T0_DEGC_X8_LSB);
    _hts221_T1_degC_x8 = ((i2c_read_byte_data(_hts221_adapter, HTS221_T1_T0_MSB) & HTS221_BM_T1_degC_X8_MSB) >> 2) << 8 | i2c_read_byte_data(_hts221_adapter, HTS221_T1_DEGC_X8_LSB);

    _hts221_H0_rH = i2c_read_byte_data(_hts221_adapter, HTS221_H0_RH_X2)>>1;
    _hts221_H1_rH = i2c_read_byte_data(_hts221_adapter, HTS221_H1_RH_X2)>>1;


    return 0;
}



double hts221_read_humidity(){
    int humidity_h = i2c_read_byte_data(_hts221_adapter, HTS221_HUMIDITY_OUT_H);
    int humidity_l = i2c_read_byte_data(_hts221_adapter, HTS221_HUMIDITY_OUT_L);
    if (humidity_h == -1 || humidity_l == -1){
        printf("Read FAILED!\n");
        return -1;
    }

    int16_t h_out = humidity_h;
    h_out = h_out << 8;
    h_out |= humidity_l;

    //https://en.wikipedia.org/wiki/Linear_interpolation
    //y = y0 + (y1 - y0)*((x-x0)/(x1-x0))
    _hts221_humidity = _hts221_H0_rH + ((_hts221_H1_rH - _hts221_H0_rH)) * ((double)(h_out - _hts221_H0_T0_OUT)/(_hts221_H1_T0_OUT - _hts221_H0_T0_OUT));
    return _hts221_humidity;
}


double hts221_read_temperature(){
    if (!(i2c_read_byte_data(_hts221_adapter, HTS221_STATUS_REG) & HTS221_BM_TEMPERATURE_READY)){
        return _hts221_temperature;
    }

    int temp_h = i2c_read_byte_data(_hts221_adapter, HTS221_TEMP_OUT_H);
    int temp_l = i2c_read_byte_data(_hts221_adapter, HTS221_TEMP_OUT_L);
    if (temp_h == -1 || temp_l == -1){
        printf("Read FAILED!\n");
        return -1;
    }
    temp_h = temp_h << 8;
    temp_l = temp_l << 0;
    int16_t t_out = temp_h | temp_l;

    //https://en.wikipedia.org/wiki/Linear_interpolation
    //y = y0 + (y1 - y0)*((x-x0)/(x1-x0))
    _hts221_temperature = (_hts221_T0_degC_x8 / 8.0) + ((_hts221_T1_degC_x8 - _hts221_T0_degC_x8)/8.0) * ((float)(t_out - _hts221_T0_OUT) / (_hts221_T1_OUT - _hts221_T0_OUT));
    return _hts221_temperature;
}
