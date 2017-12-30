cmake_minimum_required(VERSION 2.8)

project(i2spass)
set(DEVICES
    "devices/hd44780.cpp"
    "i2cbase.cpp"
    "main.cpp"
    )

add_executable(${PROJECT_NAME} ${DEVICES})
