cmake_minimum_required(VERSION 3.10)

# Project name
project(thermal_mlx9064x)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/thermal_mlx9064x.cpp
    src/MLX90640_API.cpp
    src/MLX90641_API.cpp
    src/MLX9064X_I2C_Driver.cpp
)

# USE_MLX90640 또는 USE_MLX90641 정의(USE_MLX90640가 기본)
# add_definitions(-DUSE_MLX90640)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# I2C 라이브러리 링크
# target_link_libraries(${PROJECT_NAME} i2c)

# 필요한 라이브러리가 있을 경우 추가 예시
# target_link_libraries(${PROJECT_NAME} <library_name>)
