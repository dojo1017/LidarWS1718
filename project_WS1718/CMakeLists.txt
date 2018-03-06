cmake_minimum_required(VERSION 3.0)
project(project_WS1718)

set(CMAKE_CXX_STANDARD 11)

add_executable(project_WS1718
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
        libs/merlin/MerlinHalfSqhere.h
        )

# Needed libraries:
# sudo apt-get install libi2c-dev
# WiringPi: Von der Website runterladen und kompilieren
# rt und crypt sollten vorinstalliert sein

TARGET_LINK_LIBRARIES(
        project_WS1718
        wiringPi
        rt
        crypt
)
