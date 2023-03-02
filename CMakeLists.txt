cmake_minimum_required(VERSION 3.24)
project(minio_aws_sdk_sample)

set(CMAKE_CXX_STANDARD 17)

find_package(AWSSDK CONFIG COMPONENTS core dynamodb kinesis s3 REQUIRED)
find_package(OpenCV 4.5.5 REQUIRED)

aux_source_directory(./src SRC_LIST)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ./include)
target_include_directories(${PROJECT_NAME} PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LIBRARIES} ${OpenCV_LIBS})
