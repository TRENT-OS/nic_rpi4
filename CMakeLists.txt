#
# RPi4 (BCM2711) NIC driver
#
# Copyright (C) 2021, HENSOLDT Cyber GmbH
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.18)

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

            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/purecall.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/string.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/macaddress.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/netdevice.cpp

            # required for RPi3
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcidevice.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciregister.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcixferstagedata.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbconfigparser.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbdevice.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbdevicefactory.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbendpoint.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbrequest.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbstandardhub.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbfunction.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/smsc951x.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/lan7800.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbmassdevice.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschednper.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschedper.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbkeyboard.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhcirootport.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbmouse.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/dwhciframeschednsplit.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbgamepad.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbstring.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/usb/usbmidi.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/devicenameservice.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/util.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/input/keymap.cpp
            # ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/circle/lib/synchronize.cpp

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
            TimeServer_client
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
            ${NIC_RPi4_CXX_FLAGS}
        LIBS
            os_core_api
            lib_debug
            TimeServer_client
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
    include("${dir_plat}/plat_eth.cmake")

endfunction()
