/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <circleos.h>

#include "lib_debug/Debug.h"

#include "environment.h"

#include "TimeServer.h"

#include <camkes.h>
#include <camkes/dma.h>
#include <camkes/io.h>

static mailbox_t mbox;

static const if_OS_Timer_t timer =
    IF_OS_TIMER_ASSIGN(
        timeServer_rpc,
        timeServer_notify);

/* Environment functions -----------------------------------------------------*/

void*
dma_alloc(
    unsigned nSize,
    unsigned alignement)
{
    // we are setting cached to false to allocate non-cached DMA memory for the
    // NIC driver
    return camkes_dma_alloc(nSize, alignement, false);
}

void
dma_free(
    void* pBlock,
    unsigned alignement)
{
    camkes_dma_free(pBlock, alignement);
}

uintptr_t
dma_getPhysicalAddr(void* ptr)
{
    return camkes_dma_get_paddr(ptr);
}

void
LogWrite(
    const char* pSource,
    unsigned Severity,
    const char* pMessage,
    ...)
{
    va_list args;
    va_start (args, pMessage);

    switch (Severity)
    {
    case CIRCLE_LOG_ERROR:
        printf("\n ERROR %s: - ", pSource);
        break;
    case CIRCLE_LOG_WARNING:
        printf("\n WARNING %s: - ", pSource);
        break;
    case CIRCLE_LOG_NOTICE:
        printf("\n NOTICE %s: - ", pSource);
        break;
    case CIRCLE_LOG_DEBUG:
        printf("\n DEBUG %s: - ", pSource);
        break;
    }

    vprintf(pMessage, args);
    printf("\n");
    va_end(args);
}

void
MsDelay(unsigned nMilliSeconds)
{
    usDelay(1000 * nMilliSeconds);
}

void
usDelay(unsigned nMicroSeconds)
{
    nsDelay(1000 * nMicroSeconds);
}

void
nsDelay(unsigned nNanoSeconds)
{
    OS_Error_t err;

    if ((err = TimeServer_sleep(&timer, TimeServer_PRECISION_NSEC,
                                nNanoSeconds)) != OS_SUCCESS)
    {
        Debug_LOG_ERROR("TimeServer_sleep() failed with %d", err);
    }
}

unsigned
StartKernelTimer(
    unsigned nHzDelay,
    TKernelTimerHandler* pHandler,
    void* pParam,
    void* pContext)
{
    Debug_LOG_WARNING("Not implemented!");
    return 0;
}

void
CancelKernelTimer(unsigned hTimer)
{
    Debug_LOG_WARNING("Not implemented!");
}

/*
 * Not necessary => used for configuring the usb interrupt which is in our
 * environment done by camkes
 */
void
ConnectInterrupt(
    unsigned nIRQ,
    TInterruptHandler* pHandler,
    void* pParam)
{
    Debug_LOG_WARNING("Not implemented!");
}

void
DisconnectInterrupt(unsigned nIRQ)
{
    Debug_LOG_WARNING("Not implemented!");
}

int
SetPowerStateOn(unsigned nDeviceId)
{
    return mailbox_set_power_state_on(&mbox, nDeviceId) ? 1 : 0;
}

int
GetMACAddress(unsigned char Buffer[6])
{
    return mailbox_get_mac_address(&mbox, Buffer) ? 1 : 0;
}

unsigned
GetClockTicks(void)
{
    /*
     * This function overwrites the GetClockTicks() function provided in
     * timer.cpp in the circle library. The implementation reads from the
     * System Timer peripheral. This implementation can be extracted to this
     * environment file but additional modifications would need to be done:
     *
     *  - create SystemTimer instance in CAmkES
     *  - refer to SystemTimer base address defined in CAmkES in bcm2835.h of
     *    the circle library
     *  - reference additional header files of the circle library in this
     *    file
     *  - read from the CLO register of the System Timer peripheral
     *
     * The purpose of this environment file is to create an interface to
     * TRENTOS functionality for the circle library. If we implement the
     * GetClockTicks() as described above, we would have dependencies back to
     * the circle library.
     *
     * Fortunately, TRENTOS provides already a TimeServer component that has
     * an RPC TimeServer_getTime() which will call the SystemTimer anyways.
     * That way, we can do without defining the System Timer in CAmkES,
     * remove dependencies to the circle library and leave the circle library
     * itself in a more clean state.
     */
    uint64_t result = 0;
    // Since the SystemTimer runs at 1MHz, the clock ticks have usec precision.
    TimeServer_getTime(&timer, TimeServer_PRECISION_USEC, &result);
    return (unsigned) result;
}

/* Public functions ----------------------------------------------------------*/

int
mbox_init(ps_io_ops_t* io_ops)
{
    int ret = mailbox_init(io_ops, &mbox);
    if (ret < 0)
    {
        Debug_LOG_ERROR("Failed to initialize mailbox - error code: %d", ret);
        return ret;
    }
    return 0;
}

}