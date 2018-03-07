cmake_minimum_required(VERSION 3.0)
project(project_WS1718)

set(CMAKE_CXX_STANDARD 11)
SET(GCC_COMPILE_FLAGS "-std=c++11")
add_definitions(${GCC_COMPILE_FLAGS})

add_executable(main
        # Main project
        main.cpp Lidar.cpp Lidar.h Merlin.cpp Merlin.h Gyro.cpp Gyro.h
        # gyro libs
        libs/gyro/BNO055.cpp
        libs/gyro/BNO055.h
        # lidar libs
        libs/lidar/lidarController.cpp
        libs/lidar/lidarController.h
        libs/lidar/lidarLite.c
        libs/lidar/lidarLite.h
        # merlin libs
        libs/merlin/MerlinHalfSqhere.h)

add_executable(calibrateGyro
        calibrateGyro.cpp
        Gyro.cpp Gyro.h
        libs/gyro/BNO055.cpp libs/gyro/BNO055.h
        libs/gyro/utility/imumaths.h
        libs/gyro/utility/matrix.h
        libs/gyro/utility/quaternion.h
        libs/gyro/utility/vector.h
        )

# Needed libraries:
# sudo apt-get install libi2c-dev
# WiringPi: Von der Website runterladen und kompilieren
# rt und crypt sollten vorinstalliert sein

TARGET_LINK_LIBRARIES(
        main
        wiringPi
        rt
        crypt
)

TARGET_LINK_LIBRARIES(
        calibrateGyro
        wiringPi
        rt
        crypt
)