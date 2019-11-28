cmake_minimum_required(VERSION 2.8.11)
project(cuda_jpeg_encode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#find_package(CUDA)
find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


# find_package( OpenCV REQUIRED )
# include( $ENV{OpenCV3_DIR}/OpenCVConfig.cmake )
include(/data/dinghao/MyLibs/opencv_cu100_jp_turbo/share/OpenCV/OpenCVConfig.cmake)

message( STATUS "OpenCV library status:" )
message( STATUS "    version: ${OpenCV_VERSION}" )
message( STATUS "    libraries: ${OpenCV_LIBS}" )
message( STATUS "    include path: ${OpenCV_INCLUDE_DIRS}" )

include_directories( ${OpenCV_INCLUDE_DIRS} )

#find_package( gpujpeg REQUIRED )
include_directories( /usr/local/include/libgpujpeg/ )
include_directories( ./include )

# cpp example
cuda_add_executable( encode src/encode.cpp )
target_link_libraries( encode ${OpenCV_LIBS} gpujpeg)

cuda_add_executable( decode src/decode.cpp )
target_link_libraries( decode ${OpenCV_LIBS} gpujpeg)

# python binding
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pybind11 )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pybind11/include )

cuda_add_library(GPUJPEG_Encoder SHARED src/encode_pybind.cpp )
target_link_libraries(GPUJPEG_Encoder ${OpenCV_LIBS} gpujpeg)
set_target_properties(GPUJPEG_Encoder PROPERTIES PREFIX "")

cuda_add_library(GPUJPEG_Decoder SHARED src/decode_pybind.cpp )
target_link_libraries(GPUJPEG_Decoder ${OpenCV_LIBS} gpujpeg)
set_target_properties(GPUJPEG_Decoder PROPERTIES PREFIX "")