cmake_minimum_required(VERSION 3.10)
project(MerkleParser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(/path/to/bitcoin/headers)
include_directories(/path/to/json/headers)

find_package(Bitcoin REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(MerkleParser main.cpp)

target_link_libraries(MerkleParser ${Bitcoin_LIBRARIES} ${nlohmann_json_LIBRARIES})
