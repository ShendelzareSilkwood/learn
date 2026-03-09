cmake_minimum_required(VERSION 4.2.1)

#set(CMAKE_C_COMPILER "D:/mingw64_posix/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "D:/mingw64_posix/bin/g++.exe")
project(camera_calibration)

set(CMAKE_C_COMPILER "D:/Qt/Tools/MinGW1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/Qt/Tools/MinGW1310_64/bin/g++.exe")


link_libraries(ws2_32)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
file(GLOB HEADERS "*.h")
set(INC_LIST ${HEADERS})

set(Qt6_DIR D:/Qt/6.10.1/mingw_64/lib/cmake/Qt6)
set(OpenCV_DIR D:/Github/opencv/build/install/x64/mingw/lib)
find_package(Qt6 COMPONENTS Widgets charts REQUIRED)
find_package(OpenCV REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTUIC ON)
set(CMAKE_AUTORCC ON)


#message(STATUS "Source files: ${SRC_LIST}")
#message(STATUS "Header files: ${INC_LIST}")
add_executable(camera_calibration ${SRC_LIST} ${INC_LIST} camlib.ui dataPlot.ui)
set_target_properties(camera_calibration PROPERTIES
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(camera_calibration PUBLIC Qt6::Widgets Qt6::Gui Qt6::Charts Qt6::Core)
target_link_libraries(camera_calibration PUBLIC ${OpenCV_LIBS})
