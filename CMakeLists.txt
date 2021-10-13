#
# RasPi4 (BCM2711) RPI NIC driver
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
# Declare RasPi4 (BCM2711) NIC Driver CAmkES Component
#       TODO
#
# Parameters:
#
#   <name>
#     required, component instance name
#
#   LIBS <lib_1> <lib_2> ...
#     additional libraries. At least a TimeServer client library is required
#
function(NIC_RPi4_DeclareCAmkESComponent name)

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NIC_RPi4.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.cpp

            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/bcm54213.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/synchronize64.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/macaddress.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/netdevice.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/spinlock.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/string.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/purecall.cpp
        C_FLAGS
            -Wall
            -Werror
        CXX_FLAGS
            -Wall
            -Werror
            -fno-exceptions
            -fno-rtti
            -fno-threadsafe-statics
        LIBS
            os_core_api
            lib_debug
            circle
            TimeServer_client
    )

    project(circle CXX)

    add_library(${PROJECT_NAME} INTERFACE)

    target_sources(${PROJECT_NAME}
        INTERFACE
    )

    target_include_directories(${PROJECT_NAME}
        INTERFACE
            circle
    )

endfunction()