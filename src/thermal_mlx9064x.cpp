#include "thermal_mlx9064x.hpp"
#include "MLX9064X_I2C_Driver.h"



ThermalMLX9064x::ThermalMLX9064x(){
    printf("Create Class ThermalMLX9064x!\n");

    if((i2cFile = open(i2cDevice, O_RDWR)) < 0){
        std::cerr << "Failed to open the i2c bus" << std::endl;
        exit(1);
    }
    if(ioctl(i2cFile, I2C_SLAVE, address) < 0){
        std::cerr << "Failed to acquire bus access and/or talk to slave" << std::endl;
        exit(1);
    }

    printf("Successfully created ThermalMLX9064x\n");
}


bool ThermalMLX9064x::initialize(){
    printf("Start initialize\n");
    //uint16_t eeMLX90640[832];
    printf("Before MLX90640_DumpEE\n");
    int status = MLX90640_DumpEE(address, eeData);
    if(status !=0){
        std::cerr << "Failed to load system parameter!\n" << std::endl;
        return false;
    }
    printf("Before MLX90640_ExtractParameters\n");
    status = MLX90640_ExtractParameters(eeData, &mlx9064x);
    if(status !=0){
        std::cerr << "Parameter extraction failed!" << std::endl;
        return false;
    }
    printf("Before MLX90640_SetRefreshRate\n");
    int error;
    error = MLX90640_SetRefreshRate(address, 0b011);
    printf("initialize was finished! error code(%d)\n", error);
    
    return true;
}

void ThermalMLX9064x::readAndPrintData(){
    
    int status = MLX90640_GetFrameData(address, frameData);
    //float vdd = MLX90640_GetVdd(MLX9064xFrame, &mlx9064x);
    float Ta = MLX90640_GetTa(frameData, &mlx9064x);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(frameData, &mlx9064x, emissivity, tr, toData);

    printSensorData(toData);

}

void ThermalMLX9064x::printSensorData(float* data){
    int rows, cols;
    int array_size;
    std::cout << std::fixed << std::setprecision(2);

    rows = 24;
    cols = 32;
    array_size = 768;
    
    system("clear"); // 화면 클리어

    for(int row = 0; row < rows; ++row){
        for(int col = 0; col<cols; ++col){
            int index = row * cols + col;
            std::cout << data[index];

            if(col < cols -1){
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    
}


