cmake_minimum_required(VERSION 3.15)
project(cpp_logic CXX)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY CXX_EXTENSIONS ON)
set_property(GLOBAL PROPERTY LINKER_LANGUAGE CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}")

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Threads (pthread)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

add_executable(cpp_logic
    main.cpp
    void_thread_pool/void_thread_pool.cpp
    sim_src/core/c_clock.cpp
    sim_src/core/c_comp.cpp
    sim_src/core/c_device.cpp
    sim_src/core/c_gate.cpp
    sim_src/core/c_m_engine.cpp
    sim_src/core/c_probe.cpp
    sim_src/core/c_sim.cpp
    sim_src/devices/game_of_life/game_of_life.cpp
    sim_src/devices/devices.cpp
    sim_src/devices/n_bit_sr_latch.cpp
    sim_src/devices/quad_sr_latch.cpp
    sim_src/devices/sr_latch.cpp
    sim_src/magic_devices/simple_ram.cpp
    sim_src/magic_devices/simple_rom.cpp
    sim_src/utils/strnatcmp.cpp
    sim_src/utils/utils.cpp
)

target_include_directories(cpp_logic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src/devices
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src/magic_devices
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src/simple_terminal_client
    ${CMAKE_CURRENT_SOURCE_DIR}/sim_src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/void_thread_pool
)