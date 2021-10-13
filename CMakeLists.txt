#
# RPi4 (BCM2711) NIC driver
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.18)

#-------------------------------------------------------------------------------
# Declare RPi4 (BCM2711) NIC Driver CAmkES Component
#
# Parameters:
#
#   <name>
#     required, component instance name
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
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/macaddress.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/netdevice.cpp
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
            -DRASPPI=4
            -DAARCH=64
            -DNIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS=16
        LIBS
            os_core_api
            lib_debug
            TimeServer_client
    )

endfunction()