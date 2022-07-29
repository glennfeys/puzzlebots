cmake_minimum_required(VERSION 3.0)
project(PuzzleBots LANGUAGES C CXX)

#
#
# Compiler setup
#
#

# according to https://gcc.gnu.org/projects/cxx-status.html we need at least 8 for full C++17 support
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
    message(FATAL_ERROR "You need at least gcc 8  (found gcc version ${CMAKE_CXX_COMPILER_VERSION})")
endif()

set(SYSTEM_TYPE x64)
set(SDL_FOLDER_LOCATION ${CMAKE_SOURCE_DIR}/libraries/SDL-1.2.15)
set(GLMC_FOLDER_LOCATION ${CMAKE_SOURCE_DIR}/libraries/glmc)

# Note: consider -Werror instead of -Wall to make all warnings fatal
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -std=gnu11 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -fms-extensions -fdiagnostics-color ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wno-unused-function -Wno-unused-variable -Wno-comment -fms-extensions -fdiagnostics-color -pthread")

# Enable the address sanitizer on Mac and Linux.
# Turn this off by setting the `NO_ASAN` environment variable to `1`
# Cache the environment variable
if (NOT "$ENV{NO_ASAN}" STREQUAL "")
    if ("$ENV{NO_ASAN}" STREQUAL "1")
        set(NO_ASAN "$ENV{NO_ASAN}" CACHE INTERNAL "Copied from environment variable")
    elseif ("$ENV{NO_ASAN}" STREQUAL "0")
        unset (NO_ASAN CACHE)
    endif()
endif()
if(NOT WIN32 AND NOT NO_ASAN)
    if(EXISTS "/etc/arch-release")
        message(STATUS "Note: Compiling with dynamic asan address sanitization and leak checking (\"Arch, as a rule, doesn't include static libs\")")
        set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}      -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS}    -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    else()
        message(STATUS "Note: Compiling with asan address sanitization and leak checking")
        set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}      -fsanitize=address -static-libasan -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS}    -fsanitize=address -static-libasan -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -static-libasan -fno-omit-frame-pointer")
    endif()
else()
    message("ASAN disabled: To enable, run: NO_ASAN=0 cmake .")
endif()

# Detect if compiling on raspberry PI.
if ( EXISTS /opt/vc/include/bcm_host.h )
    message(STATUS "Note: Compiling on Raspberry Pi")
    add_definitions( -DRPI -DIS_RPI )
    set(RPI True)
else()
    message(STATUS "Note: Compiling on NON Raspberry Pi")
endif()


#
# Build lists with certain groups of source files.
# We will use these lists later.
#

file(GLOB es_SRC
        "game/es/*.h"
        "game/es/*.c"
        )

file(GLOB es_system_SRC
        "game/es/system/*.h"
        "game/es/system/*.c"
        )

file(GLOB botai_c_SRC
        "game/bot_automaton/*.h"
        "game/bot_automaton/*.c"
        )

file(GLOB botai_cpp_SRC
        "game/bot_automaton/*.h"
        "game/bot_automaton/*.cpp"
        "game/bot_asm/*.h"
        "game/bot_asm/*.cpp"
        )

file(GLOB fsm_c_SRC
        "game/fsm/*.h"
        "game/fsm/*.c"
        )

file(GLOB tests_cpp_SRC
        "game/bot_asm/tests/*.h"
        "game/bot_asm/tests/*.cpp"
        )

file(GLOB graphics_SRC
        "game/graphics/*.h"
        "game/graphics/*.c"
        )

file(GLOB level_SRC
        "game/level/*.h"
        "game/level/*.c"
        )

file(GLOB imu_SRC
        "game/imu/*.h"
        "game/imu/*.cpp"
        )

file(GLOB led_SRC
        "game/led/*.h"
        "game/led/*.c"
        )

file(GLOB util_C_SRC
        "util/util.h"
        "util/util.c"
        "util/sleep.h"
        "util/sleep.c"
        "util/rgb_triple.h"
        "util/benchmarking_utils.h"
        "util/benchmarking_utils.c"
        )

file(GLOB test_C_SRC
        "util/min_unit.h"
        "util/min_unit.c"
        )

file(GLOB RTIMULib_SRC
        "libraries/RTIMULib/*.h"
        "libraries/RTIMULib/*.cpp"
        )

file(GLOB RTIMULib_drivers_SRC
        "libraries/RTIMULib/IMUDrivers/*.h"
        "libraries/RTIMULib/IMUDrivers/*.cpp"
        )

file(GLOB glmc_lib_SRC
        "libraries/glmc/include/*.h"
        "libraries/glmc/src/*.c"
        )

if (RPI)
    #nothing to do, it is added by wildcard already
else()
    #remove systems that should not be added

    #this doesn't work: #list(REMOVE_ITEM es_system_SRC "game/es/system/eyeimu_system.c" "game/es/system/eyeimu_system.h")

    string (REPLACE "game/es/system/eyeimu_system.c" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "game/es/system/eyeimu_system.h" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "game/es/system/led_system.c" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "game/es/system/led_system.h" "" es_system_SRC "${es_system_SRC}")
endif()


#
# Define which files are used to create which executable
#


set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${level_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_system_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${util_C_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${graphics_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${glmc_lib_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${led_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${sensor_SRC})

set(GAME_PART2_SRC_FILES ${GAME_PART2_SRC_FILES} ${botai_cpp_SRC} ${fsm_c_SRC})

set(GAME_PART2_TEST_FILES ${GAME_PART2_TEST_FILES} ${tests_cpp_SRC})


if (RPI)
    set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${imu_SRC})
    set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${RTIMULib_SRC} ${RTIMULib_drivers_SRC})
endif()


add_executable(puzzle_bots_part1 puzzle_bots_part1_main.c ${GAME_PART1_SRC_FILES} ${botai_c_SRC} ${fsm_c_SRC})

add_executable(puzzle_bots_part2 puzzle_bots_part2_main.cpp ${GAME_PART1_SRC_FILES} ${GAME_PART2_SRC_FILES})
target_compile_definitions(puzzle_bots_part2 PUBLIC PUZZLE_BOTS_PT2)
add_executable(puzzle_bots_part2_test puzzle_bots_part2_test_main.cpp ${GAME_PART1_SRC_FILES} ${GAME_PART2_SRC_FILES} ${GAME_PART2_TEST_FILES})

add_executable(puzzle_bots_part1_replay_benchmark puzzle_bots_part1_replay_benchmark_main.c ${GAME_PART1_SRC_FILES} ${botai_c_SRC} ${fsm_c_SRC})
add_executable(puzzle_bots_part1_record_benchmark puzzle_bots_part1_main.c ${GAME_PART1_SRC_FILES} ${botai_c_SRC} ${fsm_c_SRC})

set(TEST_SOURCE_FILES puzzle_bots_part1_test_main.c ${test_C_SRC} ${GAME_PART1_SRC_FILES} ${botai_c_SRC} ${fsm_c_SRC})
add_executable(puzzle_bots_part1_test ${TEST_SOURCE_FILES})


set(GLMC_DEMO_SOURCE_FILES glmc_demo_main.c)
add_executable(glmc_demo ${GLMC_DEMO_SOURCE_FILES} ${glmc_lib_SRC})

# Sensor executable only on Raspberry Pi
if (RPI)
    file(GLOB pi_sensors_SRC
            "sensors/*.h"
            "sensors/*.c"
            )
    set(PI_SENSORS_SRC_FILES ${pi_sensors_SRC})

    add_executable(pi_sensors pi_sensors_main.c ${PI_SENSORS_SRC_FILES})
endif()


#
# Now find the required dependencies (libraries) and use them when linking the executables
#
find_package(OpenGL REQUIRED)
find_package(SDL_image REQUIRED)

if(WIN32)
    #custom SDL1.2 Library in libraries/ folder for Windows
    # This is needed because the default lib present on the system redirects stdout or stderr to a file!
    set(SDL_INCLUDE_DIR ${SDL_FOLDER_LOCATION}/include)
    set(SDL_LIBRARY ;${SDL_FOLDER_LOCATION}/lib/x64/libSDL.dll.a)
else()
    find_package(SDL REQUIRED)
    if(NOT SDL_FOUND)
        #on debian based systems, if this fails, run: apt-get install libsdl1.2-dev
        message( FATAL ERROR "SDL not found!")
    endif(NOT SDL_FOUND)
endif(WIN32)

if(NOT SDLIMAGE_FOUND)
    #on debian based systems, if this fails, run: apt-get install libsdl-image1.2-dev
    message( FATAL ERROR "SDL image not found!")
endif(NOT SDLIMAGE_FOUND)


#on debian based systems, if this fails, run: apt-get install libglew-dev
Find_Package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR} )
target_link_libraries (puzzle_bots_part1 ${GLEW_LIBRARY} )
target_link_libraries (puzzle_bots_part2 ${GLEW_LIBRARY} )
target_link_libraries (puzzle_bots_part1_test ${GLEW_LIBRARY} )
target_link_libraries (puzzle_bots_part2_test ${GLEW_LIBRARY} )
target_link_libraries (puzzle_bots_part1_replay_benchmark ${GLEW_LIBRARY} )
target_link_libraries (puzzle_bots_part1_record_benchmark ${GLEW_LIBRARY} )

if ( RPI )
    include_directories( /opt/vc/include/ /opt/vc/include/interface/vcos/pthreads /opt/vc/include/interface/vmcs_host/linux /opt/vc/src/hello_pi/libs/ilclient /opt/vc/src/hello_pi/libs/vgfont )
    find_library (BCM_HOST_LIBRARY bcm_host HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (VCOS_LIBRARY vcos HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (VCHIQ_ARM_LIBRARY vchiq_arm HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )

    target_link_libraries (puzzle_bots_part1 ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part1 ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part1 pthread rt dl )

    target_link_libraries (puzzle_bots_part2 ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part2 ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part2 pthread rt dl )

    target_link_libraries (puzzle_bots_part1_test ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part1_test ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part1_test pthread rt dl )

    target_link_libraries (puzzle_bots_part2_test ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part2_test ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part2_test pthread rt dl )

    target_link_libraries (puzzle_bots_part1_replay_benchmark ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part1_replay_benchmark ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part1_replay_benchmark pthread rt dl )

    target_link_libraries (puzzle_bots_part1_record_benchmark ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (puzzle_bots_part1_record_benchmark ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (puzzle_bots_part1_record_benchmark pthread rt dl )
endif()

set(GLMC_INCLUDE_DIR ${GLMC_FOLDER_LOCATION}/include)
if (NOT EXISTS ${GLMC_INCLUDE_DIR}/glmc.h )
    message( FATAL_ERROR "GLMC header not found at ${GLMC_INCLUDE_DIR}/glmc.h!")
endif()

set(GLMC_SOURCE_DIR ${GLMC_FOLDER_LOCATION}/src)
if (NOT EXISTS ${GLMC_SOURCE_DIR}/glmc_model.c )
    message( FATAL_ERROR "GLMC source not found at ${GLMC_SOURCE_DIR}/glmc_model.c!")
endif()


include_directories(${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIR} ${GLMC_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} )
target_link_libraries (puzzle_bots_part1 ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries (puzzle_bots_part2 ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries (puzzle_bots_part2_test ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries (puzzle_bots_part1_test ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})


target_link_libraries (puzzle_bots_part1_replay_benchmark ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries (puzzle_bots_part1_record_benchmark ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries (glmc_demo)

if(NOT WIN32)
    target_link_libraries (puzzle_bots_part1 m)
    target_link_libraries (puzzle_bots_part2 m)
    target_link_libraries (puzzle_bots_part1_test m)
    target_link_libraries (puzzle_bots_part2_test m)
    target_link_libraries (puzzle_bots_part1_replay_benchmark m)
    target_link_libraries (puzzle_bots_part1_record_benchmark m)
    target_link_libraries (glmc_demo m)
endif()


find_package (Boost COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(puzzle_bots_part2_test    ${Boost_FILESYSTEM_LIBRARY}
                                                ${Boost_SYSTEM_LIBRARY} 
                                                ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# target_compile_definitions (puzzle_bots_part1_replay_benchmark PRIVATE BENCHMARK=1)
target_compile_definitions (puzzle_bots_part1_record_benchmark PRIVATE BENCHMARK=1)



