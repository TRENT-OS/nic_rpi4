/**
* Copyright (C) 2021, HENSOLDT Cyber GmbH
*/
extern "C" {
#include "OS_Error.h"
#include "OS_Dataport.h"
#include "lib_debug/Debug.h"
#include "network/OS_NetworkStack.h"
#include <sel4/sel4.h>
#include <circleos.h>
#include <camkes.h>
}

#include <string.h>
#include <stdlib.h>
#include "environment.h"
#include <circle/sysconfig.h>

#if RASPPI <= 3
#include <circle/netdevice.h>
#include <circle/usb/usb.h>
#include <circle/usb/usbhcidevice.h>
#else
#include <circle/bcm54213.h>
#endif

// HENSOLDT
#include <stdlib.h>

void*
operator new(size_t n)
{
    void* const p = dma_alloc(DMA_PAGE_SIZE,DMA_ALIGNEMENT);

    return p;
}

void*
operator new[](size_t n)
{
    void* const p = dma_alloc(DMA_PAGE_SIZE,DMA_ALIGNEMENT);

    return p;
}

void
operator delete(void* p, size_t sz)
{
    dma_free(p,DMA_ALIGNEMENT);
}

void
operator delete[](void* p)
{
    dma_free(p, DMA_ALIGNEMENT);
}

//----------

#if RASPPI <= 3
	CUSBHCIDevice		*m_USBHCI;
	CDWHCIDevice 		*pDWHCI;
#else
	CBcm54213Device		*m_Bcm54213;
#endif
CNetDevice *pEth0;

// If we pass the define from a system configuration header. CAmkES generation
// crashes when parsing this file. As a workaround we hardcode the value here
#define NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS 16

static volatile boolean init_ok = false;

extern "C" void post_init(void)
{
	ps_io_ops_t io_ops;
	int ret = camkes_io_ops(&io_ops);
    if (0 != ret)
    {
        Debug_LOG_ERROR("camkes_io_ops() failed - error code: %d", ret);
        return;
    }

    //mailbox initialization
    ret = mbox_init(&io_ops);
    if (0 != ret)
    {
		Debug_LOG_ERROR("Mailbox initialization failed!");
        return;
    }

#if RASPPI <= 3
	// m_USBHCI = new CUSBHCIDevice();
#else
	m_Bcm54213 = new CBcm54213Device();
#endif
    init_ok = true;
}

extern "C" int run(void)
{
#if RASPPI <= 3
    if (!m_USBHCI->Initialize())
	{
		Debug_LOG_ERROR("Cannot initialize USBHCI");
	}
#else
    if (!m_Bcm54213->Initialize())
	{
		Debug_LOG_ERROR("Cannot initialize BCM54213");
	}
#endif

	Debug_LOG_INFO("[EthDrv '%s'] starting", get_instance_name());

	unsigned nTimeout = 0;
	pEth0 = CNetDevice::GetNetDevice (0);
	if (pEth0 == 0)
	{
		Debug_LOG_ERROR("Net device not found");
	}

	while (!pEth0->IsLinkUp())
	{
		MsDelay (100);

		if (++nTimeout < 40)
		{
			continue;
		}
		nTimeout = 0;

		Debug_LOG_WARNING("Link is down");
	}

	Debug_LOG_DEBUG("Link is up");

	if(!nic_driver_init_done_post())
	{
		Debug_LOG_DEBUG("Post failed.");
	}

	uint8_t* Buffer = (uint8_t*)dma_alloc(DMA_PAGE_SIZE, DMA_ALIGNEMENT);
	size_t receivedLength = 0;

	unsigned int count = 0;
	while(true)
	{
		if(pEth0->ReceiveFrame(Buffer,(u32 *)&receivedLength))
		{
            OS_NetworkStack_RxBuffer_t* buf_ptr = (OS_NetworkStack_RxBuffer_t*)nic_to_port;

            if (receivedLength > sizeof(buf_ptr->data) )
            {
                Debug_LOG_WARNING(
                    "The max length of the data is %u, but the length of the "
                    "read data is %zu",
                    (unsigned)sizeof(buf_ptr->data),
                    receivedLength);
                // throw away current frame and read the next one
                continue;
            }

			// if the slot to be used in the ringbuffer isn't empty we
			// wait here in a loop
			// TODO: Implement it in an event driven fashion
			while(buf_ptr[count].len != 0) {
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

extern "C" OS_Error_t
nic_rpc_rx_data(
	size_t* pLen,
	size_t* framesRemaining)
{
    return OS_ERROR_NOT_IMPLEMENTED;
}

extern "C" OS_Error_t nic_rpc_tx_data(
	size_t* len)
{
	if (!pEth0->SendFrame(nic_from_port,*len))
	{
        Debug_LOG_ERROR("Bcm54213SendFrame failed");
		return OS_ERROR_ABORTED;
	}

	return OS_SUCCESS;
}

extern "C" OS_Error_t nic_rpc_get_mac_address(void)
{
	if(!nic_driver_init_done_wait())
	{
		Debug_LOG_DEBUG("Wait failed.");
	}

    GetMACAddress((uint8_t *)nic_to_port);

	return OS_SUCCESS;
}

#if RASPPI <= 3
/* USB interrupt handler -----------------------------------------------------------------*/
void usbBaseIrq_handle(void) {
	pDWHCI->InterruptHandler();

	int error = usbBaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}

#else

/* Genet interrupt handler A-----------------------------------------------------------------*/
extern "C" void genetA_BaseIrq_handle(void) {
	m_Bcm54213->InterruptHandler0();

	int error = genetA_BaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}

/* Genet interrupt handler B-----------------------------------------------------------------*/
extern "C" void genetB_BaseIrq_handle(void) {
	m_Bcm54213->InterruptHandler1();

	int error = genetB_BaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}
#endif