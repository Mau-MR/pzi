cmake_minimum_required(VERSION 3.20)
project(pzi)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(pzi main.cpp)

# Link your application with OpenCV libraries
target_link_libraries(pzi PRIVATE ${OpenCV_LIBS})
