cmake_minimum_required(VERSION 3.16)

project(TensorTest
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_STATIC_LIBS ON)

find_package (Boost REQUIRED COMPONENTS
    program_options
    filesystem)

include_directories(${Boost_INCLUDE_DIR})

add_executable(analyzer)

target_sources(analyzer PRIVATE
    main.cpp
    analyzer.cpp
    parser.cpp
    tokenizer.cpp
    dependenciestree.cpp
)

target_link_libraries(analyzer LINK_PUBLIC ${Boost_LIBRARIES})