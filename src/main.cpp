#include <iostream>
#include "thermal_mlx9064x.hpp"
#include <unistd.h>

int main() {
    printf("Start Program...\n");
    ThermalMLX9064x sensor;
    if(!sensor.initialize()){

        std::cerr << "Failed to Initialize sensor!" << std::endl;
    }

    // 센서 처리 로직 추가 가능
    printf("Start reading data...\n");
    while(true){
        sensor.readAndPrintData();
        usleep(300000); // delay for 1 sencond
    }

    return 0;
}
