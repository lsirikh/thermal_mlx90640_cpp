#ifndef THERMAL_MLX9064X_HPP
#define THERMAL_MLX9064X_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iomanip> // std::setprecision 사용을 위한 헤더 추가
#include <cstdlib>  // system 함수 사용을 위한 헤더
#include "MLX90640_API.h"
#define TA_SHIFT 8

class ThermalMLX9064x {
public:
    ThermalMLX9064x(); // ctor
    bool initialize(); // initialize thermal camera
    void readAndPrintData(); // Read and Write camera data

private:
    int i2cFile;
    const char* i2cDevice = "/dev/i2c-1";
    const uint8_t address = 0x33; // thermal camera I2C address
    paramsMLX90640 mlx9064x; // common parameter
    float toData[768]; // 온도 데이터 배열
    uint16_t eeData[832]; // EEPROM 데이터 배열을 멤버 변수로 선언
    uint16_t frameData[834]; // 프레임 데이터 배열
    void printSensorData(float* data); // function for printing data

};

#endif // THERMAL_MLX9064X_HPP
