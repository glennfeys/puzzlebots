#include "RTIMU_c_wrapper.h"

IMUSettings* createIMUSettings() {
    RTIMUSettings* cpp_settings;

    IMUSettings* settings = (IMUSettings*)malloc(sizeof(IMUSettings));
    cpp_settings = new RTIMUSettings();
    settings->obj = cpp_settings;

    return settings;
}

void freeIMUSettings(IMUSettings* settings) {
    if (settings == NULL)
        return;
    delete static_cast<RTIMUSettings*>(settings->obj);
    free(settings);
}

IMU* createIMU(IMUSettings* settings) {
    RTIMU* cpp_imu;

    IMU* imu = (IMU*)malloc(sizeof(IMU));
    cpp_imu = RTIMU::createIMU(static_cast<RTIMUSettings*>(settings->obj));
    imu->obj = cpp_imu;

    return imu;
}

void freeIMU(IMU* imu) {
    if (imu == NULL)
        return;
    delete static_cast<RTIMU*>(imu->obj);
    free(imu);
}

IMUData* getData(IMU* imu) {
    IMUData* data;
    const RTIMU_DATA cpp_data = (static_cast<RTIMU*>(imu->obj))->getIMUData();

    data = (IMUData*)malloc(sizeof(IMUData));
    data->x = cpp_data.fusionPose.x();
    data->y = cpp_data.fusionPose.y();
    data->z = cpp_data.fusionPose.z();
    
    return data;
}

int IMURead(IMU* imu) { return ((RTIMU*)imu->obj)->IMURead(); }

void initIMU(IMU* imu) { ((RTIMU*)imu->obj)->IMUInit(); }

void enableGyro(IMU* imu, int bl) { (static_cast<RTIMU*>(imu->obj))->setGyroEnable(bl); }

void enableAccel(IMU* imu, int bl) { (static_cast<RTIMU*>(imu->obj))->setAccelEnable(bl); }

void enableCompass(IMU* imu, int bl) { (static_cast<RTIMU*>(imu->obj))->setCompassEnable(bl); }

uint64_t currentSinceEpoch() {
    return RTMath::currentUSecsSinceEpoch();
}

void setSlerp(IMU* imu, RTFLOAT power) { (static_cast<RTIMU*>(imu->obj))->setSlerpPower(power); }

