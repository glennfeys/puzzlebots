#include <stdio.h>

#include "i2c.h"
#include "lps25h.h"

#define ADDRESS          0x5c

/*

Registers for LPS25H. See: http://www.st.com/content/ccc/resource/technical/document/datasheet/58/d2/33/a4/42/89/42/0b/DM00066332.pdf/files/DM00066332.pdf/jcr:content/translations/en.DM00066332.pdf
""
*/

#define LPS25H_CTRL_REG1        0x20

#define LPS25H_STATUS_REG              0x27
#define LPS25H_BM_TEMPERATURE_READY    0x1
#define LPS25H_BM_PRESSURE_READY       0x2
/*
The pressure value is composed by LPS25H_PRESS_OUT_H/_L/_XL and is represented as 2’s
complement.

Pressure output data: Pout(hPa) = LPS25H_PRESS_OUT / 4096
*/
#define LPS25H_PRESS_OUT_H         0x2A
#define LPS25H_PRESS_OUT_L         0x29
#define LPS25H_PRESS_OUT_XL        0x28
/*
The Temperature data are expressed as LPS25H_TEMP_OUT_H & LPS25H_TEMP_OUT_L as 2’s complement
numbers.

Temperature output data: T(°C) = 42.5 + (LPS25H_TEMP_OUT / 480)
*/
#define LPS25H_TEMP_L_REG          0x2B
#define LPS25H_TEMP_H_REG          0x2C

static int _lps25h_adapter;
static double _lps25h_temperature = 0.0;
static double _lps25h_pressure    = 0.0;

int lps25h_init(){
    // Specify the address to communicate with
    _lps25h_adapter = i2c_init_adapter(ADDRESS);

    uint8_t reg = LPS25H_CTRL_REG1;

    int result = i2c_write_byte_data(_lps25h_adapter, reg, 0x90);
    if (result == -1){
        printf("write failed!\n");
        return -1;
    }

    printf("Waiting for Pressure and Temperature sensors to become ready.");
    reg = LPS25H_STATUS_REG;
    while(1){
        int value = i2c_read_byte_data(_lps25h_adapter, reg);
        if (value == -1){
            printf("Read FAILED!\n");
            return -1;
        } else if(value & (LPS25H_BM_PRESSURE_READY | LPS25H_BM_TEMPERATURE_READY)){
            break;
        }
        printf(".");
    }
    printf("\nSensors are ready!\n");

    return 0;
}

double lps25h_read_pressure(){
    if (!(i2c_read_byte_data(_lps25h_adapter, LPS25H_STATUS_REG) & LPS25H_BM_PRESSURE_READY)){
        return _lps25h_pressure;
    }

    int press_h = i2c_read_byte_data(_lps25h_adapter, LPS25H_PRESS_OUT_H);
    int press_l = i2c_read_byte_data(_lps25h_adapter, LPS25H_PRESS_OUT_L);
    int press_xl = i2c_read_byte_data(_lps25h_adapter, LPS25H_PRESS_OUT_XL);
    if (press_h == -1 || press_l == -1 || press_xl == -1){
        printf("Read FAILED!\n");
        return -1;
    }
    //printf("Read succeeded! Got back: %02X %02X %02X\n", press_h, press_l, press_xl);
    press_h = press_h << 16;
    press_l = press_l << 8;
    press_xl = press_xl << 0;
    //printf("Got: %i %i %i\n", press_h, press_l, press_xl);
    double pressure = press_h | press_l | press_xl;
    //printf("Pressure: %f\n", pressure);
    _lps25h_pressure = pressure / 4096;
    return _lps25h_pressure;
}


double lps25h_read_temperature(){
    if (!(i2c_read_byte_data(_lps25h_adapter, LPS25H_STATUS_REG) & LPS25H_BM_TEMPERATURE_READY)){
        return _lps25h_temperature;
    }

    int temp_h = i2c_read_byte_data(_lps25h_adapter, LPS25H_TEMP_H_REG);
    int temp_l = i2c_read_byte_data(_lps25h_adapter, LPS25H_TEMP_L_REG);
    if (temp_h == -1 || temp_l == -1){
        printf("Read FAILED!\n");
        return -1;
    }
    //printf("Read succeeded! Got back: %02X %02X %02X\n", press_h, press_l, press_xl);
    temp_h = temp_h << 8;
    temp_l = temp_l << 0;
    int16_t t_out = temp_h | temp_l;
    //printf("TTT: %i\n", t_out);
    //printf("TTT: %f\n", t_out / 480.0);
    //printf("TTT: %f\n", 42.5 + (t_out / 480.0));
    _lps25h_temperature = 42.5 + (t_out / 480.0);
    return _lps25h_temperature;
}
