
cmake_minimum_required(VERSION 3.17)

NIC_RPi4_DeclareCAmkESComponent(
    NIC_RPi4
    CXX_FLAGS
		-DRASPPI=4
        -DAARCH=64
        -DNIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS=16
)