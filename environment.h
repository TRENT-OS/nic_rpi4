/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 */

#pragma once

extern "C"
{
#include <platsupport/mach/mailbox_util.h>

int mbox_init(ps_io_ops_t* io_ops);
}