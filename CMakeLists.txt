cmake_minimum_required(VERSION 3.7)
project(poc_bluetooth)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system REQUIRED)

find_package(cppzmq REQUIRED)

include_directories(${cppzmq_INCLUDE_DIR})

add_executable(Publisher controlPublisher.cpp)
add_executable(Subscriber controlSubscriber.cpp)

target_link_libraries(Publisher
        pthread
        ${Boost_SYSTEM_LIBRARY}
        ${cppzmq_LIBRARY})

target_link_libraries(Subscriber ${cppzmq_LIBRARY})
