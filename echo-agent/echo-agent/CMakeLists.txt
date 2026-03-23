cmake_minimum_required(VERSION 3.5)
project(echo_agent CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon REQUIRED)

add_executable(${PROJECT_NAME}
    main.cc
    src/watchman/Watchman.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
