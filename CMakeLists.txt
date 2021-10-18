#
# RasPi4 (BCM2711) RPI NIC driver
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

function(NIC_RPi3_DeclareCAmkESComponent name)

    cmake_parse_arguments(
        PARSE_ARGV
        1
        NIC_RPi3
        "" # Option arguments
        "" # Single arguments
        "CXX_FLAGS" # Multiple arguments
    )

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NIC_RPi4.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.cpp

            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/purecall.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/string.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/macaddress.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/netdevice.cpp

            # required for RPi3
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcidevice.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciregister.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbhostcontroller.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbstandardhub.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbfunction.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbdevice.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbconfigparser.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbstring.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbendpoint.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbdevicefactory.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/lan7800.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbrequest.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcixferstagedata.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschednsplit.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschednper.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschedper.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcirootport.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcixactqueue.cpp

            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/numberpool.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/device.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/classallocator.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/devicenameservice.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/ptrlist.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/koptions.cpp

        C_FLAGS
            -Wall
            -Werror
        CXX_FLAGS
            -Wall
            -Werror
            -fno-exceptions
            -fno-rtti
            -fno-threadsafe-statics
            ${NIC_RPi3_CXX_FLAGS}
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


function(NIC_RPi4_DeclareCAmkESComponent name)

    cmake_parse_arguments(
        PARSE_ARGV
        1
        NIC_RPi4
        "" # Option arguments
        "" # Single arguments
        "CXX_FLAGS" # Multiple arguments
    )

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NIC_RPi4.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.cpp

            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/purecall.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/string.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/macaddress.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/netdevice.cpp

            # required for RPi4
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/bcm54213.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/synchronize64.cpp
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/spinlock.cpp
        C_FLAGS
            -Wall
            -Werror
        CXX_FLAGS
            -Wall
            -Werror
            -fno-exceptions
            -fno-rtti
            -fno-threadsafe-statics
            ${NIC_RPi4_CXX_FLAGS}
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

#-------------------------------------------------------------------------------
# declare CAmkES components for all NICs of a platform
function(DeclareCAmkESComponents_for_NIC)

    set(dir_plat "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/plat/${PLATFORM}")

    if(NOT IS_DIRECTORY "${dir_plat}")
        message(FATAL_ERROR "unsupported platform: '${PLATFORM}'")
    endif()

    CAmkESAddCPPInclude("${dir_plat}")
    include("${dir_plat}/plat_nic.cmake")

endfunction()