cmake_minimum_required(VERSION 2.8.11)
project(cuda_jpeg_encode)

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
link_directories( /usr/local/lib )

#add_executable( ${PROJECT_NAME} demo.cpp )
cuda_add_executable( ${PROJECT_NAME} demo.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} gpujpeg)
