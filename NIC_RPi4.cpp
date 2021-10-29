/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

extern "C"
{
#include "OS_Error.h"
#include "OS_Dataport.h"
#include "lib_debug/Debug.h"
#include "network/OS_NetworkStackTypes.h"
#include <sel4/sel4.h>
#include <circleos.h>
#include <camkes.h>
}

#include <string.h>
#include <stdlib.h>
#include <circle/bcm54213.h>
#include "environment.h"

void*
operator new(size_t n)
{
    void* const p = dma_alloc(DMA_PAGE_SIZE, DMA_ALIGNEMENT);

    return p;
}

void*
operator new[](size_t n)
{
    void* const p = dma_alloc(DMA_PAGE_SIZE, DMA_ALIGNEMENT);

    return p;
}

void
operator delete(
    void* p,
    size_t sz)
{
    dma_free(p, DMA_ALIGNEMENT);
}

void
operator delete[](void* p)
{
    dma_free(p, DMA_ALIGNEMENT);
}

static CBcm54213Device* bcm54213Device;
static volatile boolean initOk = false;

extern "C"
{

void
post_init(void)
{
    ps_io_ops_t io_ops;
    int ret = camkes_io_ops(&io_ops);
    if (0 != ret)
    {
        Debug_LOG_ERROR("camkes_io_ops() failed - error code: %d", ret);
        return;
    }

    // mailbox initialization
    ret = mbox_init(&io_ops);
    if (0 != ret)
    {
        Debug_LOG_ERROR("Mailbox initialization failed!");
        return;
    }

    bcm54213Device = new CBcm54213Device();
    initOk = true;
}

int
run(void)
{
    if (!initOk)
    {
        Debug_LOG_ERROR("NIC driver not (correctly) intitialized");
        return OS_ERROR_NOT_INITIALIZED;
    }

    if (!bcm54213Device->Initialize())
    {
        Debug_LOG_ERROR("Cannot initialize BCM54213");
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_INFO("[EthDrv '%s'] starting", get_instance_name());

    unsigned timeout = 0;
    while (!bcm54213Device->IsLinkUp())
    {
        MsDelay (100);

        if (++timeout < 40)
        {
            continue;
        }
        timeout = 0;

        Debug_LOG_WARNING("Link is down");
    }

    Debug_LOG_DEBUG("Link is up");

    if (!nic_driver_init_done_post())
    {
        Debug_LOG_DEBUG("Post failed.");
    }

    uint8_t* Buffer = (uint8_t*)dma_alloc(DMA_PAGE_SIZE, DMA_ALIGNEMENT);
    size_t receivedLength = 0;

    unsigned int count = 0;
    while (true)
    {
        if (bcm54213Device->ReceiveFrame(Buffer, (u32*)&receivedLength))
        {
            OS_NetworkStack_RxBuffer_t* buf_ptr = (OS_NetworkStack_RxBuffer_t*)nic_to_port;

            if (receivedLength > sizeof(buf_ptr->data) )
            {
                Debug_LOG_WARNING(
                    "The max length of the data is %u, but the length of the "
                    "read data is %ld",
                    (unsigned)sizeof(buf_ptr->data),
                    receivedLength);
                // throw away current frame and read the next one
                continue;
            }

            // if the slot to be used in the ringbuffer isn't empty we
            // wait here in a loop
            // TODO: Implement it in an event driven fashion
            while (buf_ptr[count].len != 0)
            {
                seL4_Yield();
            }
            memcpy(buf_ptr[count].data, Buffer, receivedLength);
            buf_ptr[count].len = receivedLength;
            count = (count + 1) % NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS;
            nic_event_hasData_emit();
        }
    }

    dma_free(Buffer, DMA_ALIGNEMENT);

    return OS_SUCCESS;
}


/* nic_rpc interface ----------------------------------------------------------------*/

OS_Error_t
nic_rpc_rx_data(
    size_t* pLen,
    size_t* framesRemaining)
{
    if (!initOk)
    {
        Debug_LOG_ERROR("NIC driver not (correctly) intitialized");
        return OS_ERROR_NOT_INITIALIZED;
    }

    return OS_ERROR_NOT_IMPLEMENTED;
}

OS_Error_t
nic_rpc_tx_data(
    size_t* len)
{
    if (!initOk)
    {
        Debug_LOG_ERROR("NIC driver not (correctly) intitialized");
        return OS_ERROR_NOT_INITIALIZED;
    }

    if (!bcm54213Device->SendFrame(nic_from_port, *len))
    {
        Debug_LOG_ERROR("Bcm54213SendFrame failed");
        return OS_ERROR_ABORTED;
    }

    return OS_SUCCESS;
}

OS_Error_t
nic_rpc_get_mac_address(void)
{
    if (!initOk)
    {
        Debug_LOG_ERROR("NIC driver not (correctly) intitialized");
        return OS_ERROR_NOT_INITIALIZED;
    }

    if (!nic_driver_init_done_wait())
    {
        Debug_LOG_DEBUG("Wait failed.");
    }

    GetMACAddress((uint8_t*)nic_to_port);

    return OS_SUCCESS;
}

/* Genet interrupt handler A-----------------------------------------------------------------*/
void
genetA_BaseIrq_handle(void)
{
    bcm54213Device->InterruptHandler0();

    int error = genetA_BaseIrq_acknowledge();

    if (error != 0)
    {
        Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
    }
}

/* Genet interrupt handler B-----------------------------------------------------------------*/
void
genetB_BaseIrq_handle(void)
{
    bcm54213Device->InterruptHandler1();

    int error = genetB_BaseIrq_acknowledge();

    if (error != 0)
    {
        Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
    }
}

}