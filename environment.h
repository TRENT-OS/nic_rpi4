/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

extern "C"
{
#include <platsupport/mach/mailbox_util.h>

int mbox_init(ps_io_ops_t* io_ops);
}