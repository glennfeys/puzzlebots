#ifndef SYSPROG_RTIMU_C_WRAPPER_H
#define SYSPROG_RTIMU_C_WRAPPER_H

#ifdef __cplusplus
#ifdef RPI
#include "../../libraries/RTIMULib/RTIMULib.h"
#endif
extern "C" {
#endif

typedef struct IMU {
    void* obj;
} IMU;

typedef struct IMUSettings {
    void* obj;
} IMUSettings;

typedef struct IMUData {
    float x;
    float y;
    float z;
} IMUData;

IMUSettings* createIMUSettings();
void freeIMUSettings(IMUSettings* settings);
IMU* createIMU(IMUSettings* settings);
void freeIMU(IMU* imu);
IMUData* getData(IMU* imu);

int IMURead(IMU* imu);
void initIMU(IMU* imu);
void enableGyro(IMU* imu, int bl);
void enableAccel(IMU* imu, int bl);
void enableCompass(IMU* imu, int bl);
uint64_t currentSinceEpoch();
void setSlerp(IMU* imu, float power);

#ifdef __cplusplus
}
#endif

#endif // SYSPROG_RTIMU_C_WRAPPER_H
